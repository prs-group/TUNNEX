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

#ifndef CUSTOMSLIDER_H
#define CUSTOMSLIDER_H

#include <QSlider>

namespace WKB {
    namespace impldetail{
        class FloatingPointQSliderPrivate;
    }
    
    namespace widgets {
        
        
        class FloatingPointQSlider: public QSlider {
            Q_OBJECT
        public:
			//! Standard Constructor
			/** \details For details see <a href="http://doc.qt.io/qt-5/qslider.html#QSlider">
			Qt Documentation </a>
			*/
            explicit FloatingPointQSlider(QWidget *parent = 0);
			//! This constructor initializes the object with min and max values
			 /**
			  *
			  * @param min Minimum slider value
			  * @param max Maximum slider value
			  * @param parent parent widget
			  */
            FloatingPointQSlider(const double min, const double max, QWidget *parent = 0);
            /**
             * Getter method for the factor
             * @details the factor determines the conversion of floating point to
             * integer
             * @return The current factor of this slider
             */
            const double getFactor() const;
            /**
             * Getter method for the current value
             * The current value is the position of the slider
             * @return The current value
             */
            const double getValue() const;
            /**
             * This Setter sets the slider positions
             * @details Finds the correct floating point to integer conversion
             * factor and then sets the integeger min and max values of the slider
             * Does nothing if diff = 0 and min > max
             * @param diff Desired number of ticks
             * @param min minimum value of the slider
             * @param max maximium value of the slider
             * @param factor desired factor ( if = 0 automatically determined)
             */
            void setValue(const double diff, const double min, const double max,
                          const double factor = 0);
            /**
             * Sets silder to position which corresponds to value
             * @param value  slider position
             */
            void setSliderPosition(double value);
            const double getSliderPosition() const;
            
        signals:
		/**
		 * Is emitted when silder is changed
		 * @param The changed value
		 */
            void valueChanged(double value);
        public slots:
		/**
		 * Sets the silder to value
		 * @param value The value to set the silder to
		 */
            void customValueChanged(int value);

        private:
            Q_DECLARE_PRIVATE(WKB::impldetail::FloatingPointQSlider)
            
            WKB::impldetail::FloatingPointQSliderPrivate* d_ptr;
            
        };
    } // widgets
} // WKB

#endif // CUSTOMSLIDER_H
