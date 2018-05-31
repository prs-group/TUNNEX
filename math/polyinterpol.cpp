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
#include "polyinterpol.h"
#include<QObject>
#include <cmath>


PolyInterpol::PolyInterpol(QVector<double> &xv, QVector<double> &yv, int m): AbstractInterpol(xv,yv,m), dy(0.)
{
}

double PolyInterpol::rawinterp(int jl, double x){
    int i=0,m=0,ns=0;
    double y, den, dif, dift, ho,hp,w;
    QVector<double>::const_iterator xa, ya = &this->yy[jl];
    for(QVector<double>::Iterator pos = this->xx.begin(); pos != this->xx.end(); pos++){
        if(*pos == this->xx.at(jl))
            xa = pos;
    }

    QVector<double> c(this->mm), d(this->mm);
    dif = std::abs(x-xa[0]);
    for(i=0;i< this->mm;i++){
        if((dift=std::fabs((double) x-xa[i])) < dif){
            ns=i;
            dif=dift;
        }
        c[i]=ya[i];
        d[i]=ya[i];
    }
    y=ya[ns--];
    for(m=1; m < this->mm; m++){
        for(i=0; i < this->mm-m; i++){
            ho = xa[i] - x;
            hp = xa[i+m]-x;
            w=c[i+1] -d[i];
            if((den=ho-hp) == 0.0)
                throw(QObject::tr("Poly_interp error"));
            den = w/den;
            d[i]=hp*den;
            c[i]=ho*den;
        }
        y += (this->dy = (2*(ns+1) < (this->mm-m) ? c[ns+1] : d[ns--]));
    }
    return y;
}
