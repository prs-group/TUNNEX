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
#ifndef ADEPT_H
#define ADEPT_H
#include "math_global.h"
#include <functional>
template <class T> class Adapt {
public:
    double TOL,toler;
    static const double alpha,beta,x1,x2,x3,x[12];
    bool terminate,out_of_tolerance;
    Adapt(double tol);

    double integrate(std::function<T(T)> func, const double a, const double b);
    double adaptlob(std::function<T(T)> func, const double a, const double b, const double fa, const double fb, const double is);
};

#endif // ADEPT_H

