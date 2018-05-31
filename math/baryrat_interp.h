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
#ifndef BARYRAT_INTERP_H
#define BARYRAT_INTERP_H

#include "abstractinterpol.h"

class MATH_LIB_DECL BaryRat_interp : public AbstractInterpol
{
private:
    QVector<double> w;
    int d;

public:
    BaryRat_interp(QVector<double> &xv, QVector<double> &yv, int dd);
    double rawinterp(int jl, double x);
    double interp(double x);
};

#endif // BARYRAT_INTERP_H
