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

#ifndef WKBModel_hpp
#define WKBModel_hpp

#include <QObject>
#include <QSharedPointer>
#include <functional>

// Forward declaration qt
template <class T>
class QVector;


namespace WKB{
namespace model {
// Forward declaration WKB::model
class AbstractInterpol;
struct parameters {
	double startingMaterialEnergy;
	double startingMaterialZPVE;
	double reactionMode;
	int symmetryNumber;

	parameters() {
		startingMaterialEnergy = 0.0;
		startingMaterialZPVE = 0.0;
		reactionMode = 5;
		symmetryNumber = 1;
	}
};
class WKBModel: public QObject {

Q_OBJECT

	// The result
	QVector<double>* _halfLives, *_attemptEnergies;
	// This is a copy of the interpolated data
	QSharedPointer<AbstractInterpol> _correctedIRC;
	// The actual parameters
	parameters _p;

	// A vector for attempt energies
	QVector<QPointF> *_turningPoints;

	// A vector for the energy functions to interpolate
	QVector<std::function<double(double)>>* _energyFunctionsInH;

	bool _defaultEnergy, _defaultZPVE;

	void resetEnergyFunctions() Q_DECL_NOEXCEPT;
	void populateEnergyFunctions() Q_DECL_NOEXCEPT;

public:
	WKBModel(QObject *parent = Q_NULLPTR) Q_DECL_NOEXCEPT;
	//! Compute the attempt energies as following:
	//! \f$ \sum_i^4 E=(1/2 + i) \cdot \epsilon_{attempt} \f$
	//! Converted to hatree
	void computeAttemptEnergies() Q_DECL_NOEXCEPT;
	//! Computes the turning points. Returns false if there is an error
	bool getTurningPoints() Q_DECL_NOEXCEPT;
	//! Turning points can be reseted
	void resetTurningPoints() Q_DECL_NOEXCEPT;
	//! This function validates the starting material
	//! The energy + ZPVE must be within 2% of the value of the IRP
	bool validatestartingMaterialEnergy() Q_DECL_NOEXCEPT;
	//! Sets all the half-lives to -1
	void resetHalfLives() Q_DECL_NOEXCEPT;

	//! All the necessary steps to compute the tunneling half-lives
	//! Also resets the half lives to -1 if not possible
	void computeOrResetHalfLives() Q_DECL_NOEXCEPT;

	//! Default Destructor
	~WKBModel() Q_DECL_NOEXCEPT;

public slots:
	void correctedInterpolChanged(QSharedPointer<AbstractInterpol> interp)
			Q_DECL_NOEXCEPT;
	void startingMaterialEnergyChanged(const QVariant &p) Q_DECL_NOEXCEPT;
	void startingMaterialZPVEChanged(const QVariant &p) Q_DECL_NOEXCEPT;
	void reactiveModeChanged(const QVariant &p) Q_DECL_NOEXCEPT;
	void symmetryNumberChanged(const QVariant &p) Q_DECL_NOEXCEPT;


	signals:
	void halfLiveChanged(const QVector<double> &halfLive);
};
}
}


#endif /* WKBModel_h */
