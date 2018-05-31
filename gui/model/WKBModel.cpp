/* This file is part of TUNNEX.

 TUNNEX is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 laster any later version.

 TUNNEX is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with TUNNEX.  If not, see <http://www.gnu.org/licenses/>.

 TUNNEX 1.0.0 
 Copyright (c) Henrik Quanz
*/

#include <math.h>
#include <QVector>
#include <QPoint>
#include <QVariant>
#include <QDebug>
#include <QFile>
#include "model/WKBModel.h"
#include "model/AbstractInterpol.h"
#include "model/AbstractXYDataFileModel.h"
#include "gui/definitions.h"
#include "math/findroot.h"
#include "math/adapt.h"

struct myExept: std::exception {
    myExept(const char *c) throw ();
    const char* msg;
    virtual const char* what() const throw ();
};

myExept::myExept(const char *c) throw () :
msg(c) {

}

const char* myExept::what() const throw () {
    return msg;
}

using namespace WKB::model;

WKBModel::WKBModel(QObject* parent) Q_DECL_NOEXCEPT : QObject(parent),
_halfLives(new QVector<double>()),_correctedIRC(Q_NULLPTR),
_defaultEnergy(true),_defaultZPVE(true),
_attemptEnergies(new QVector<double>(4)),
_turningPoints(new QVector<QPointF>()),
_energyFunctionsInH(new QVector<std::function<double(double)>>())
{
}

WKBModel::~WKBModel() Q_DECL_NOEXCEPT{
    _correctedIRC.reset();
    delete _halfLives;
	delete _attemptEnergies;
}

void WKBModel::resetHalfLives() Q_DECL_NOEXCEPT {
	// First set the half-lives to 0
	_halfLives->clear();
	_halfLives->insert(_halfLives->begin(), 4, -1.0);
    emit(halfLiveChanged(*_halfLives));
}

void WKBModel::computeOrResetHalfLives() Q_DECL_NOEXCEPT{
    // First set the half-lives to 0
	resetHalfLives();
    // Checks if we can make a computation
    if (_correctedIRC.isNull())
        return;
    // No zero or negative reaction Mode
	if (_p.reactionMode == 0.0 || _p.reactionMode < 0)
        return;

    // The computed starting material ZPVE+Energy should be in the
    // range of 10% of the educt
    // energy +(zpve-reactionMode) entered by
    // the user

    if(!validatestartingMaterialEnergy())
    	return;

    // Ok everything setup correctly! Compute turning points
	computeAttemptEnergies();
	resetTurningPoints();
	resetEnergyFunctions();
	populateEnergyFunctions();
	if (!getTurningPoints())
    	return;

	auto attempt = _attemptEnergies->begin();
	auto tP = _turningPoints->begin();
	auto energyFunctionInH = _energyFunctionsInH->begin();
    // For saving the half live
    QVector<double> halfLives;

    double startingMaterialMinEnergy=_correctedIRC->interpolate(_correctedIRC->xmin());
    if(!_defaultEnergy)
		startingMaterialMinEnergy = _p.startingMaterialEnergy + _p.startingMaterialZPVE;

	

	for (; tP != _turningPoints->end(); attempt++, tP++, energyFunctionInH++) {
        if(tP->x() == 0.0){
			halfLives.push_back(-1);
            continue;
        }

        double TSEnergy=_correctedIRC->interpolate(0);

		auto func = [energyFunctionInH] (double x) {
			double y=(*energyFunctionInH)(x);
			// This is ok, because of numerical imprecision and
			// it does not contribute to the integral
			if(y < 0.0) {
				return 0.0;
			}
			return (sqrt(y));
			};

        Adapt<double> aInt(0.001);
        double intRes=aInt.integrate(func, tP->x(), tP->y());
		intRes *= (sqrt(2 * REFELO));
        // Compute Transmission Probabilty
        double transP=1/(1+exp(2*intRes));
        // Compute theoretical half live

		double hL = log(2) / (_p.reactionMode * 100 * SPEEDOFLIGHT * transP);
		hL /= _p.symmetryNumber;
#ifdef TUNNELDETAIL
        qDebug() << "*************************************";
        qDebug() << "Tunneling Parameters: ";
        qDebug() << "Attempt Energy: " << *attempt*HTOKCAL << " kcal/mol";
        qDebug() << "Turning Points: " << tP->x() << " and " << tP->y();
        qDebug() << "Penetration Integral: " << intRes;
        qDebug() << "Transmission Probability: " << transP;
        qDebug() << "Half Live: " << hL << " s\n";
        qDebug() << "*************************************";
#endif

        halfLives.push_back(hL);

    }
    *_halfLives=halfLives;

    emit(halfLiveChanged(*_halfLives));
}

void WKBModel::correctedInterpolChanged(QSharedPointer<AbstractInterpol> interp)
		Q_DECL_NOEXCEPT {

    _correctedIRC.reset();
    this->_correctedIRC=interp;
    computeOrResetHalfLives();
}

void WKBModel::startingMaterialEnergyChanged(const QVariant &p) Q_DECL_NOEXCEPT{
	if (p.toDouble() == 0.0) {
		_defaultEnergy = true;
		_p.startingMaterialEnergy = 0.0;
	} else {
		_p.startingMaterialEnergy = p.toDouble();
		_defaultEnergy = false;
	}
    computeOrResetHalfLives();
}

void WKBModel::startingMaterialZPVEChanged(const QVariant &p) Q_DECL_NOEXCEPT{
	if (p.toDouble() == 0.0) {
		_defaultZPVE = true;
		_p.startingMaterialZPVE = 0.0;
	} else {
		_p.startingMaterialZPVE = p.toDouble();
    _defaultZPVE=false;
	}
    computeOrResetHalfLives();
}

void WKBModel::reactiveModeChanged(const QVariant &p) Q_DECL_NOEXCEPT{
	_p.reactionMode = p.toDouble();
    computeOrResetHalfLives();
}

void WKBModel::symmetryNumberChanged(const QVariant &p) Q_DECL_NOEXCEPT {
	_p.symmetryNumber = p.toInt();
	computeOrResetHalfLives();
}

bool WKBModel::validatestartingMaterialEnergy() Q_DECL_NOEXCEPT{
	// Overwrite if user didn't input anything
	if(_defaultEnergy)
		return true;
    // The starting material is always located at the lowest x
    // value
    double ircstartingMaterialEnergy=_correctedIRC->interpolate(_correctedIRC->xmin());
    // Because the IRC should be corrected with projected
    // ZPVE data we need to reduce the starting material ZPVE
    // by the attempt energy
	double enteredstartingMaterialEnergy = _p.startingMaterialEnergy
			+ (_p.startingMaterialZPVE - _p.reactionMode * 0.5 * WNTOH);

    if ((fabs(enteredstartingMaterialEnergy-ircstartingMaterialEnergy)
    		> fabs(ircstartingMaterialEnergy*0.02))
        ) {
		qInfo() << "Entered starting material energy not within"
        << " the range of 2% of the computed IRC"
        << " energy: " << enteredstartingMaterialEnergy;
        qInfo() << "Entered Parameters:\n"
        << "Starting Material Energy: "
				<< _p.startingMaterialEnergy << "\n"
				<< "Starting Material ZPVE: "
				<< _p.startingMaterialZPVE << "\n" << "startingMaterial Mode: "
				<< _p.reactionMode << "\n"
				<< "IRC starting Material Energy: " << ircstartingMaterialEnergy;
        return false;
    }
	return true;
}

bool WKBModel::getTurningPoints()
		Q_DECL_NOEXCEPT {


	// Prefill turning points with invalid values
	for (auto &turningPoint : *_turningPoints)
		turningPoint = QPointF(0.0, 0.0);
		

	int i = 0;
	auto attemptEnergy = _attemptEnergies->begin();
	auto reactionCoordinateVSRelEnergyFunctionInH =
			_energyFunctionsInH->begin();

	for (;
			attemptEnergy != _attemptEnergies->end()
					|| reactionCoordinateVSRelEnergyFunctionInH
							!= _energyFunctionsInH->end();
			attemptEnergy++, reactionCoordinateVSRelEnergyFunctionInH++) {
        try {
            FindRoot<double, double> root;
			double barrierHeightInKcal =
					(*reactionCoordinateVSRelEnergyFunctionInH)(0) * HTOKCAL;
			qDebug() << "Barrier height in kcal/mol at v=" <<
			i++ << ": " << barrierHeightInKcal;

            double xmin = _correctedIRC->xmin();
            double xmax = _correctedIRC->xmax();
			double xt11 = xmin, leftTS = -0.1, rightTS = 0.1,
			xt22 = xmax,
			turn1 = 0.0, turn2 = 0.0;
			if (root.zbrac(*reactionCoordinateVSRelEnergyFunctionInH,
							xt11,
					leftTS)) {
				turn1 = root.rtbis(*reactionCoordinateVSRelEnergyFunctionInH,
					xt11, leftTS, 0.0001);
			} else {
                qWarning() << __FILE__ ":" <<__LINE__ <<
                		" Did not find root turn1";
            }

			if (root.zbrac(*reactionCoordinateVSRelEnergyFunctionInH, rightTS,
					xt22)) {
				turn2 = root.rtbis(*reactionCoordinateVSRelEnergyFunctionInH,
						rightTS, xt22, 0.0001);
            } else{
                qWarning() << __FILE__ ":" <<__LINE__ <<
                		" Did not find root turn2";
            }

            QPointF tP(turn1,turn2);
			_turningPoints->append(tP);
        } catch (std::exception &e) {
            qWarning() << e.what();
            return false;
        } catch (...) {
            qWarning() << "An error occured";
            return false;
        }
    }

    return true;

}

void WKBModel::computeAttemptEnergies() Q_DECL_NOEXCEPT {
    // The classical turning points
    for(int i=0; i < 4;i++)
		(*_attemptEnergies)[i] = (i + 0.5) * _p.reactionMode * WNTOH;

}

void WKBModel::resetTurningPoints() Q_DECL_NOEXCEPT {
	delete _turningPoints;
	_turningPoints = new QVector<QPointF>();
}

void WKBModel::resetEnergyFunctions() Q_DECL_NOEXCEPT {
	delete _energyFunctionsInH;
	_energyFunctionsInH = new QVector<std::function<double(double)>>();
}

void WKBModel::populateEnergyFunctions() Q_DECL_NOEXCEPT {
	/* The energy of the starting material is
	 * equal to the electronic energy and the
	 * ZPVE - attempt energy
	 */
	double startingMaterialAttemptEnergyInH = 0.0;
	if (!_defaultEnergy) {
		startingMaterialAttemptEnergyInH = _p.startingMaterialEnergy;
	} else {
		startingMaterialAttemptEnergyInH = _correctedIRC->interpolate(_correctedIRC->xmin());
	}

	if (!_defaultZPVE) {
		startingMaterialAttemptEnergyInH += (_p.startingMaterialZPVE
				- _attemptEnergies->at(0));
	}

	for (auto &attemptEnergy : *_attemptEnergies) {
	double referenceEnergyInH = attemptEnergy + startingMaterialAttemptEnergyInH;
		auto reactionCoordinateVSRelEnergyFunctionInH =
				[=](const double &reactionCoordinate) -> double {
					double currentEnergy=_correctedIRC->interpolate(
							reactionCoordinate);
					return ((currentEnergy-referenceEnergyInH));
				};
		_energyFunctionsInH->append(reactionCoordinateVSRelEnergyFunctionInH);
	}
}
