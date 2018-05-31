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
#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define REFELO 1822.8886259874
#define WNTOH 1.f/219474.f
#define HTOKCAL 2625499.62/4.19/1000
#define SPEEDOFLIGHT 299792458
#define DISPLAYPREC 3
#define FLOATTOLARANCE 1e-9
#define debugP(var) #var << var << " "
#define printContainer(container) qDebug() << #container << ":\n" ;\
    for(const auto &e : container) \
        qDebug() << e;


#endif // DEFINITIONS_H
