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

#include <QVector2D>
#include "model/interpolation.h"
#include "model/AbstractXYDataFileModel.h"
#include "ALGLIB/interpolation.h"
#include "Math/adapt.h"

using namespace WKB::model;
using namespace alglib;

// ALGLIB should not throw an exception, except something goes
// horribly wrong. This is why I declare everything with noexept
// This means a serious bug in data handling so the program crashes

spline1dALG::spline1dALG() Q_DECL_NOEXCEPT: s(Q_NULLPTR){
    
}


spline1dALG::spline1dALG(const spline1dALG &other) Q_DECL_NOEXCEPT: AbstractInterpol(other){
    buildSpline();
    this->recomputeInterpol();
}

spline1dALG::spline1dALG(QSharedPointer<AbstractXYDataFileModel> data) Q_DECL_NOEXCEPT: AbstractInterpol(data){
    buildSpline();
    this->recomputeInterpol();
}

spline1dALG::~spline1dALG() Q_DECL_NOEXCEPT {
    s.reset();
}

void spline1dALG::buildSpline() Q_DECL_NOEXCEPT{
    if(getData() == Q_NULLPTR || getData()->isEmpty())
        return;

    s.reset(new alglib::spline1dinterpolant);
    real_1d_array ry;
    real_1d_array rx;
    

    auto xV=this->_data->getMonotonicXValues();
    auto yV=this->_data->getMonotonicYValues();
//auto xV=getData()->getMonotonicXValues();
//    auto yV=getData()->getMonotonicYValues();
    
    ry.setcontent(yV->size(), yV->data());
    rx.setcontent(xV->size(), xV->data());
    
    // build spline
    try {
    	spline1dbuildmonotone(rx, ry, *s);
    } catch (alglib::ap_error &e){
    	qWarning() << "A horrible error within"  << __FILE__ << "line: "
    			<< __LINE__ << "! \n Could not create spline interpolation. Error: " <<
				QString::fromStdString(e.msg);
    }
    
}


double spline1dALG::interpolate(const double &x) const Q_DECL_NOEXCEPT{
    if(getData() == Q_NULLPTR || getData()->isEmpty())
        return 0.0f;
    return spline1dcalc(*s, x);
}



double spline1dALG::integrate(double lower, double upper) const Q_DECL_NOEXCEPT {
    if(s == Q_NULLPTR)
        return 0.0f;
    Adapt<double> a(0.001);
    auto func=[this](double x){
        return this->interpolate(x);
    };
    
    return a.integrate(func, lower, upper);

}


void spline1dALG::setData(QSharedPointer<AbstractXYDataFileModel> m) Q_DECL_NOEXCEPT{
    AbstractInterpol::setData(m);
    buildSpline();
    recomputeInterpol();
}

AbstractInterpol* spline1dALG::copy() const Q_DECL_NOEXCEPT {

    spline1dALG* inter=new spline1dALG(*this);
    return inter;
}
