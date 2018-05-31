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

#include "controller/floatingPointQSlider.h"
#include <QDebug>
#include <cmath>

using namespace WKB::widgets;
using namespace WKB::impldetail;

namespace WKB {
    namespace impldetail{
        class FloatingPointQSliderPrivate {
        public:
            explicit FloatingPointQSliderPrivate(FloatingPointQSlider *s);
            FloatingPointQSliderPrivate(FloatingPointQSlider* s,const double, const double);
            
            const double getFactor() const;
            const double getValue() const;
            void setValue(const double diff, const double min, const double max, const double factor = 0);
            
            void setSliderPosition(double);
            const double getSliderPosition() const;
            double customValueChanged(int);
            
            bool _sliderPositionSet;
            
        private:

            Q_DECLARE_PUBLIC(WKB::widgets::FloatingPointQSlider)
            WKB::widgets::FloatingPointQSlider *q_ptr;
            double _factor, _value, _diff;
        };
    }
}


FloatingPointQSliderPrivate::FloatingPointQSliderPrivate(FloatingPointQSlider* s) :
_sliderPositionSet(false),
q_ptr(s),
_factor(1), _value(0.0), _diff(0.0) {
}

FloatingPointQSliderPrivate::FloatingPointQSliderPrivate(FloatingPointQSlider *s,const double value, const double diff) :
_sliderPositionSet(false),
q_ptr(s),
_factor(10), _value(value),  _diff(diff) {
}

const double FloatingPointQSliderPrivate::getFactor() const {
    return this->_factor;
}

const double FloatingPointQSliderPrivate::getValue() const {
    return this->_value;
}

const double FloatingPointQSliderPrivate::getSliderPosition() const {
    const FloatingPointQSlider *q=q_func();
    return static_cast<double>(q->sliderPosition()) / this->_factor;
}

void FloatingPointQSliderPrivate::setValue(const double diff,
                                           const double min, const double max, const double factor) {
    Q_Q(FloatingPointQSlider);
    if (factor != 0) {
        this->_factor = factor;
        q->setRange(min, max);
    }
    if (diff == 0)
        return;
    if (min > max)
        return;
    
    else {
        this->_factor = 1;
        bool factorFound = false;
        this->_diff = diff;
        if (this->_diff > 10000) { // We need to make the difference smaller
            while (!factorFound) {
                if (this->_diff * this->_factor < 10)
                    factorFound = true;
                else
                    this->_factor *= 0.1;
            }
        } else
            while (!factorFound) {
                if (this->_diff * this->_factor > 10)
                    factorFound = true;
                else
                    this->_factor *= 10;
            }
    }
    q->setRange(static_cast<int>(min * this->_factor),
                static_cast<int>(max * this->_factor));
}

//Slots

double FloatingPointQSliderPrivate::customValueChanged(int v) {
    if (!this->_sliderPositionSet) {
        double value = this->getValue()
        + (static_cast<double>(v) / this->getFactor());
        this->_sliderPositionSet = false;
        return value;
    }
    this->_sliderPositionSet = false;
    return 0;
    
}

void FloatingPointQSliderPrivate::setSliderPosition(
                                                    double value) {
    Q_Q(FloatingPointQSlider);
    int ivalue = 0;
    ivalue = static_cast<int>((value - this->getValue()) * this->getFactor());
    QAbstractSlider* abstract = dynamic_cast<QAbstractSlider*>(q);
    this->_sliderPositionSet = true;
    abstract->setSliderPosition(ivalue);
    q->customValueChanged(ivalue);
}

// floatingPointQSlider implementation

FloatingPointQSlider::FloatingPointQSlider(QWidget *parent) :
QSlider(parent), d_ptr(new FloatingPointQSliderPrivate(this)) {
    connect(this,SIGNAL(valueChanged(int)), this,SLOT(customValueChanged(int)));
}

FloatingPointQSlider::FloatingPointQSlider(const double value,
                                           const double diff, QWidget *parent) :
QSlider(parent), d_ptr(new FloatingPointQSliderPrivate(this,value, diff)) {
    connect(this,SIGNAL(valueChanged(int)), this,SLOT(customValueChanged(int)));
}

const double FloatingPointQSlider::getFactor() const {
    //Q_D(floatingPointQSlider);
    const FloatingPointQSliderPrivate *d=d_func();
  
    return d->getFactor();
}

const double FloatingPointQSlider::getValue() const {
    const FloatingPointQSliderPrivate *d=d_func();
    return d->getValue();
}

void FloatingPointQSlider::setValue(const double diff, const double min,
                                    const double max, const double factor) {
    Q_D(FloatingPointQSlider);
    d->setValue(diff, min, max, factor);
}

void FloatingPointQSlider::setSliderPosition(double v) {
    Q_D(FloatingPointQSlider);
    d->setSliderPosition(v);
}

const double FloatingPointQSlider::getSliderPosition() const{
    
    const FloatingPointQSliderPrivate *d=d_func();

    return d->getSliderPosition();
}

void FloatingPointQSlider::customValueChanged(int value) {
    Q_D(FloatingPointQSlider);
    if (!d->_sliderPositionSet)
        emit(valueChanged(d->customValueChanged(value)));
    
    d->_sliderPositionSet = false;
}

