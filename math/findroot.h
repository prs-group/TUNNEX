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
#ifndef FINDROOT_H
#define FINDROOT_H
#include <functional>
#include "math_global.h"

template <class T,class D> class MATH_LIB_DECL FindRoot
{
public:
    explicit FindRoot();
    bool zbrac(std::function<T(D)> func, double &x1, double &x2) throw();

    
    double rtbis(std::function<T(D)> func, const double x1, const double x2, const double xacc) throw();
    double rtflsp(std::function<T(D)> func, const double x1, const double x2, const double xacc) throw();

};

#endif // FINDROOT_H

