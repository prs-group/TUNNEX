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
#ifndef ABSTRACTINTERPOL_H
#define ABSTRACTINTERPOL_H

#include "math_global.h"
#include <QVector>

class MATH_LIB_DECL AbstractInterpol
{
protected:
    int mm, jsav, cor, dj;
    const int n;
    QVector<double> xx, yy;
    double virtual rawinterp(int jlo, double x) = 0;
    int locate(const double x);
    int hunt (const double x);

public:
    AbstractInterpol(QVector<double> &x, const QVector<double> y, int m);
    virtual ~AbstractInterpol(){};
    double interp(double x);




};

#endif // ABSTRACTINTERPOL_H
