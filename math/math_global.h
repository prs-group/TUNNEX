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
#ifndef INTERPOLATION_GLOBAL_H
#define INTERPOLATION_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(COMPILE_MATH_LIB)
    #define MATH_LIB_DECL Q_DECL_EXPORT
#elif defined(USE_MATH_LIB)
    #define MATH_LIB_DECL Q_DECL_IMPORT
#else
    #define MATH_LIB_DECL
#endif

#endif // INTERPOLATION_GLOBAL_H
