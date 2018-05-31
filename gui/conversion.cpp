/* This file is part of TUNNEX.

 TUNNEX is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 laster any later version.

 Foobar is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with TUNNEX.  If not, see <http://www.gnu.org/licenses/>.

 TUNNEX 1.0.0 
 Copyright (c) Henrik Quanz
*/

#include "conversion.h"

namespace myTime {
extern const double timeConvTable[5][5] = { { 1, 0.01666666666, 0.00027777777,
		0.00001157407, 3.1709792e-8 }, { 60, 1, 0.01666666666, 0.00069444444,
		0.00000190258 }, { 3600, 60, 1, 0.04166666666, 0.00011415525 }, { 86400,
		1440, 24, 1, 0.00273972602 }, { 31536000, 86400, 3600, 365, 1 } };
}

    
