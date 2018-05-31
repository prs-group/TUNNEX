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
#include "abstractinterpol.h"
#include <algorithm>
#include <math.h>
#include <QString>
#include <QObject>


AbstractInterpol::AbstractInterpol(QVector<double> &x, const QVector<double> y, int m): mm(m), jsav(0), cor(0),dj(0), n(x.size()), xx(x), yy(y){
    dj = std::min(1,(int) pow((double) this->n,0.25));
}

double AbstractInterpol::interp(double x){
    int jlo = this->cor ? this->hunt(x) : this->locate(x);
    return rawinterp(jlo,x);
}

int AbstractInterpol::locate(const double x){
    int ju,jm,jl;
    if (this->n < 2 || this->mm < 2 || this->mm > this->n)
        throw(QObject::tr("locate size error"));

    bool ascnd = (this->xx[this->n-1] >= this->xx[0]);
    jl=0;
    ju=this->n-1;
    while(ju-jl > 1){
        jm = (ju+jl) >> 1;
        if(x >= this->xx[jm] == ascnd)
            jl=jm;
        else
            ju=jm;
    }
    this->cor = abs(jl-this->jsav) > this->dj ? 0 : 1;
    jsav = jl;
    return std::max(0, std::min(this->n-this->mm,jl-((this->mm-2)>>1)));
}

int AbstractInterpol::hunt(const double x){
    int jl=this->jsav, jm=1, ju, inc=1;
    if(this->n < 2 || this->mm < 2 || this->mm > this->n)
        throw(QObject::tr("hunt size error"));
    bool ascnd=(this->xx[this->n-1] >= this->xx[0]);
    if(jl < 0 || jl > this->n-1){
        jl=0;
        ju=this->n-1;
    }
    else {
        if( x >= this->xx[jl] == ascnd){
            for(;;){
                ju = jl + inc;
                if (ju >= this->n -1){
                    ju = this->n-1;
                    break;
                }
                else if (x < this->xx[ju] == ascnd)
                    break;

                else{
                    jl = ju;
                    inc += inc;
                }
            }
        }
        else {
            ju = jl;
            for(;;){
                jl = jl - inc;
                if(jl <= 0){
                    jl = 0;
                    break;
                }
                else if(x >= this->xx[jl] == ascnd)
                    break;
                else{
                    ju = jl;
                    inc += inc;
                }
            }
        }
    }
    while( ju-jl > 1){
        jm = (ju+jl) >> 1;
        if (x >= this->xx[jm] == ascnd)
            jl = jm;
        else
            ju=jm;
    }
    this->cor = abs(jl-this->jsav) > dj ? 0 : 1;
    this->jsav = jl;
    return std::max(0,std::min(this->n-this->mm,jl-((this->mm-2)>>1)));
}
