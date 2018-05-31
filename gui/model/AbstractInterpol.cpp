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

#include <QVector>
#include <cmath>
#include "model/AbstractInterpol.h"
#include "model/tsvdata.h"

using namespace WKB::model;

AbstractInterpol::AbstractInterpol(QObject *parent):QObject(parent), _data(Q_NULLPTR),
_interpData(Q_NULLPTR){
    
}

AbstractInterpol::AbstractInterpol(QSharedPointer<AbstractXYDataFileModel> data,QObject *parent): QObject(parent), _data(data) {

}

AbstractInterpol::AbstractInterpol(const AbstractInterpol  &other): QObject(other.parent()){
    if(other._data != Q_NULLPTR)
        this->_data=other._data;
    if(other._interpData != Q_NULLPTR)
        this->_interpData=other._interpData;
    
}

AbstractInterpol::~AbstractInterpol(){
    _data.reset();
    _interpData.reset();
}


void AbstractInterpol::setData(QSharedPointer<AbstractXYDataFileModel> m){
    _data=m;
    recomputeInterpol();
}

void AbstractInterpol::recomputeInterpol(){

    // First checks pointer
    if( _data == Q_NULLPTR || _data->isEmpty())
        return;
    
    // Now check if we either have to create a new object
    if( _interpData == Q_NULLPTR)
        _interpData.reset(new TSVData);

    

    /* Now we have to compute the step size
     between the data points. I aim for
     2 data points between existing points */
    
    dVector xValues=_data->getXValues();
    qSort(xValues->begin(), xValues->end());
    XYdata d;
    for(auto n=xValues->begin(); n != xValues->end()-1; n++){
		double diff = std::fabs(*n - *(n + 1));
        diff/=4.0;
        for(double i=*n; i < *(n+1); i+=diff){
            QPointF p;
            p.setX(i);
            p.setY(this->interpolate(i));
            d.append(p);
        }
        
    }

    _interpData->setData(d);

    
}

double AbstractInterpol::xmin() const{
    if(_data != Q_NULLPTR)
        return _data->xmin();
    
    return 0.0f;
}

double AbstractInterpol::xmax() const{
    if(_data != Q_NULLPTR)
        return _data->xmax();
    
    return 0.0f;
}




