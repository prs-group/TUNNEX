# This file is part of TUNNEX.

# TUNNEX is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# laster any later version.

# TUNNEX is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with TUNNEX.  If not, see <http://www.gnu.org/licenses/>.

# TUNNEX 1.0.0 
# Copyright (c) Henrik Quanz

#--------------------------------------------------------------------------------
# This is the list of source files that need to be compiled
#--------------------------------------------------------------------------------
file(GLOB SRC "[a-zA-Z]*.cpp")
set(model_SRC "${SRC}" PARENT_SCOPE)

#--------------------------------------------------------------------------------
# another list, this time it includes all header files that should be treated with moc
#--------------------------------------------------------------------------------
file(GLOB HEADER "[a-zA-Z]*.h")
set(model_HEADER "${HEADER}" PARENT_SCOPE)
