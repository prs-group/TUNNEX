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

 TUNNEX 1.0.0 
 Copyright (c) Henrik Quanz
*/

#ifndef interpolation_hpp
#define interpolation_hpp

#include "model/AbstractInterpol.h"

namespace alglib{
class spline1dinterpolant;
}

namespace WKB {
    namespace model {

        class spline1dALG : public AbstractInterpol{
        private:
            QSharedPointer<alglib::spline1dinterpolant> s;

            void buildSpline() Q_DECL_NOEXCEPT;

            
        protected:
            spline1dALG(const spline1dALG &other) Q_DECL_NOEXCEPT;
        public:
            spline1dALG() Q_DECL_NOEXCEPT;

            spline1dALG(QSharedPointer<AbstractXYDataFileModel> data) Q_DECL_NOEXCEPT;
            virtual ~spline1dALG() Q_DECL_NOEXCEPT;
            
            double interpolate(const double &x) const Q_DECL_NOEXCEPT Q_DECL_OVERRIDE;
            double integrate(double lower,double upper) const Q_DECL_NOEXCEPT Q_DECL_OVERRIDE;

            virtual void setData(QSharedPointer<AbstractXYDataFileModel> m) Q_DECL_NOEXCEPT Q_DECL_OVERRIDE;
            
            AbstractInterpol* copy() const Q_DECL_NOEXCEPT Q_DECL_OVERRIDE;
        };
    }
}



#endif /* interpolation_hpp */
