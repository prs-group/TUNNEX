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
*/
#include "splineinterpol.h"
#include <QObject>

SplineInterpol::SplineInterpol(QVector<double> &xv, QVector<double> &yv, double yp1, double ypn):
    AbstractInterpol(xv,yv,2), y2(xv.size()){
    this->sety2(xv,yv,yp1,ypn);
}

void SplineInterpol::sety2(const QVector<double> xv, const QVector<double> yv, double yp1, double ypn){
    int i,k;
    double p,qn,sig,un;
    int n = this->y2.size();
    QVector<double> u(n-1);
    if(yp1 > 0.99e99)
        this->y2[0] = u[0]=0.0;
    else{
        this->y2[0] = -0.5;
        u[0] = (3.0/(xv[1]-xv[0]))*((yv[1]-yv[0])/(xv[1]-xv[0])-yp1);
    }
    for (i = 1; i < n-1; i++){
        sig = (xv[i]-xv[i-1])/(xv[i+1]-xv[i-1]);
        p=sig*this->y2[i-1]+2.0;
        this->y2[i] = (sig-1.0)/p;
        u[i] = (yv[i+1]-yv[i])/(xv[i+1]-xv[i]) - (yv[i]-yv[i-1])/(xv[i]-xv[i-1]);
        u[i] = (6.0*u[i]/(xv[i+1]-xv[i-1])-sig*u[i-1])/p;
    }
    if (ypn > 0.99e99)
        qn = un = 0.0;
    else{
        qn = 0.5;
        un = (3.0/(xv[n-1]-xv[n-2]))*(ypn-(yv[n-1]-yv[n-2])/(xv[n-1]-xv[n-2]));
    }
    this->y2[n-1] = (un-qn*u[n-2]) / (qn*this->y2[n-2]+1.0);
    for(k=n-2; k >= 0; k--)
        this->y2[k] = this->y2[k]*this->y2[k+1]+u[k];
}

double SplineInterpol::rawinterp(int jl, double x){
    int klo=jl, khi=jl+1;
    double y,h,b,a;
    h=this->xx[khi]-this->xx[klo];
    if(h == 0.0)
        throw(QObject::tr("Bad input to routine splint"));

    a=(this->xx[khi]-x)/h;
    b=(x-this->xx[klo])/h;
    y=a*this->yy[klo]+b*this->yy[khi]+((a*a*a-a)*this->y2[klo]
                                       + (b*b*b-b)*y2[khi])*(h*h)/6.0;
    return y;
}
