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
#include "rat_interp.h"
#include <QObject>
#include <cmath>

// Windows compiler does not support using in c++11
#ifdef Q_OS_WIN
typedef  QVector<double> dvec;
typedef QVector<double>::const_iterator dvecit;
#else
using dvec = QVector<qreal>;
using dvecit = QVector<qreal>::const_iterator;
#endif

Rat_interp::Rat_interp(QVector<qreal> &xv, QVector<qreal> &yv, int m): AbstractInterpol(xv, yv,m), dy(0.0) {

}

double Rat_interp::rawinterp(int jl, double x){
    const double TINY=1.0e-99;
    int m,i,ns=0;
    qreal y,w,t,hh,h,dd;
    dvecit xa = &this->xx[jl], ya = &this->yy[jl];
    dvec c(this->mm), d(this->mm);
    hh = std::fabs(x-xa[0]);
    for (i = 0; i < this->mm; i++){
        h = std::fabs(x-xa[i]);
        if (h == 0.0){
            this->dy=0.0;
            return ya[i];
        } else if( h < hh){
            ns = i;
            hh = h;
        }
        c[i] = ya[i];
        d[i] = ya[i]+TINY;
    }
    y = ya[ns--];
    for (m = 1; m < this->mm; m++){
        for(i = 0; i < this->mm-m; i++){
            w = c[i+1]-d[i];
            h=xa[i+m]-x;
            t=(xa[i]-x)*d[i]/h;
            dd = t-c[i+1];
            if (dd == 0.0)
                throw(QObject::tr("Error in routine ratint"));
            dd=w/dd;
            d[i] = c[i+1]*dd;
            c[i] = t*dd;
        }
        y += (this->dy=(2*(ns+1) < (this->mm-m) ? c[ns+1] : d[ns--]));
    }
    return y;
}
