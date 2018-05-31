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

#include <QtAlgorithms>
#include <QString>
#include <QDialog>
#include <cmath>
#include "controller/plotcontrol.h"
#include "view/ui_plotcontrol.h"
#include "QCustomPlot/qcustomplot.h"

using namespace WKB::widgets;

PlotControl::PlotControl(QWidget *parent) :
		QDialog(parent), _ui(new Ui::PlotControl),
_yMinValue(0.0), _yMaxValue(0.0),
_xMinValue(0.0), _xMaxValue(0.0),
_plot(Q_NULLPTR) {
	_ui->setupUi(this);
    SetupUiConnections();

}

PlotControl::PlotControl(QWidget *parent, QCustomPlot* plot) :
		QDialog(parent), _ui(new Ui::PlotControl),
    _yMinValue(0.0), _yMaxValue(0.0),
    _xMinValue(0.0), _xMaxValue(0.0),
    _plot(plot) {

	_ui->setupUi(this);
    SetupUiConnections();

	emit(yMinChanged(_ui->xMinSlider->getSliderPosition()));
	emit(yMaxChanged(_ui->xMaxSlider->getSliderPosition()));
	emit(xMinChanged(_ui->yMinSlider->getSliderPosition()));
	emit(xMaxChanged(_ui->yMaxSlider->getSliderPosition()));
}

PlotControl::~PlotControl() {
	delete _ui;
}

void PlotControl::SetupUiConnections(){
    
    // Connect the valueChanged signal of sliders to class specific slots
    connect(_ui->yMinSlider, &FloatingPointQSlider::valueChanged,
            this,&PlotControl::yMinChanged);
    connect(_ui->yMaxSlider, &FloatingPointQSlider::valueChanged,
            this,&PlotControl::yMaxChanged);
    connect(_ui->xMinSlider, &FloatingPointQSlider::valueChanged,
            this,&PlotControl::xMinChanged);
    connect(_ui->xMaxSlider, &FloatingPointQSlider::valueChanged,
            this,&PlotControl::xMaxChanged);
    
    // Connect line edit signals returnPressed to class specific slots
    connect(_ui->yMinEdit,&QLineEdit::returnPressed,
            this,&PlotControl::yMinEntered);
    connect(_ui->yMaxEdit,&QLineEdit::returnPressed,
            this,&PlotControl::yMaxEntered);
    connect(_ui->xMinEdit,&QLineEdit::returnPressed,
            this,&PlotControl::xMinEntered);
    connect(_ui->xMaxEdit,&QLineEdit::returnPressed,
            this,&PlotControl::xMaxEntered);
}

void PlotControl::yMinChanged(double v) {

	QString* temp = new QString;
	temp->setNum(v);
	_ui->yMinEdit->setText(*temp);
	this->_yMinValue = v;
	if (this->_plot != NULL) {
		this->_plot->yAxis->setRangeLower(v);
		this->_plot->replot();
	}
	delete temp;
}

void PlotControl::yMaxChanged(double v) {
	QString* temp = new QString;
	temp->setNum(v);
	_ui->yMaxEdit->setText(*temp);
	this->_yMaxValue = v;
	if (this->_plot != NULL) {
		this->_plot->yAxis->setRangeUpper(v);
		this->_plot->replot();
	}
	delete temp;
}

void PlotControl::xMinChanged(double v) {
	QString* temp = new QString;
	temp->setNum(v);
	_ui->xMinEdit->setText(*temp);
	this->_xMinValue = v;
	if (this->_plot != NULL) {
		this->_plot->xAxis->setRangeLower(v);
		this->_plot->replot();
	}
	delete temp;
}

void PlotControl::xMaxChanged(double v) {
	QString* temp = new QString;
	temp->setNum(v);
	_ui->xMaxEdit->setText(*temp);
	this->_yMaxValue = v;
	if (this->_plot != NULL) {
		this->_plot->xAxis->setRangeUpper(v);
		this->_plot->replot();
	}
	delete temp;
}

void PlotControl::yMinEntered() {
	QString *temp = new QString(_ui->yMinEdit->text());
	double value = temp->toDouble();
	if (value == this->_yMinValue)
		return;

	if (value > this->_yMinValue) {
	}
	_ui->yMinSlider->setSliderPosition(value);
}

void PlotControl::yMaxEntered() {
	QString *temp = new QString(_ui->yMaxEdit->text());
	double value = temp->toDouble();
	if (value == this->_yMaxValue)
		return;
	_ui->yMaxSlider->setSliderPosition(value);
}

void PlotControl::xMinEntered() {
	QString *temp = new QString(_ui->xMinEdit->text());
	double value = temp->toDouble();
	if (value == this->_xMinValue)
		return;
	_ui->xMinSlider->setSliderPosition(value);
}

void PlotControl::xMaxEntered() {
	QString *temp = new QString(_ui->xMaxEdit->text());
	double value = temp->toDouble();
	if (value == this->_xMaxValue)
		return;
	_ui->xMaxSlider->setSliderPosition(value);
	delete temp;
}

void PlotControl::setData(const QVector<double> &x, const QVector<double> &y) {
	QVector<double>* xsort, *ysort;
	QString xmin, xmax, ymin, ymax;

	double ydiff = 0, xdiff = 0;

	xsort = new QVector<double>(x);
	ysort = new QVector<double>(y);
	std::sort(xsort->begin(), xsort->end(), std::less<double>());
	std::sort(ysort->begin(), ysort->end(), std::less<double>());

	//Compute diff
	xdiff = fabs(xsort->last() - xsort->first());
	ydiff = fabs(ysort->last() - ysort->first());

	qDebug() << "ysort: ";
	for (const auto &e : *ysort)
		qDebug() << e;
	qDebug() << "xsort: ";
	for (const auto &e : *xsort)
		qDebug() << e;

#define debugP(var) #var << var << " "
	qDebug() << debugP(xdiff) << debugP(ydiff);

	_ui->xMaxSlider->setValue(xdiff, xsort->last() - (xdiff / 2),
			xsort->last() + (xdiff / 2));
	_ui->xMinSlider->setValue(xdiff, xsort->first() - (xdiff / 2),
			xsort->first() + (xdiff / 2));
	_ui->yMaxSlider->setValue(ydiff, ysort->last() - (ydiff / 2),
			ysort->last() + (ydiff / 2));
	_ui->yMinSlider->setValue(ydiff, ysort->first() - (ydiff / 2),
			ysort->first() + (ydiff / 2));
	_ui->xMinSlider->setSliderPosition(xsort->first());
	_ui->xMaxSlider->setSliderPosition(xsort->last());
	_ui->yMinSlider->setSliderPosition(ysort->first());
	_ui->yMaxSlider->setSliderPosition(ysort->last());
	xmin.setNum(_ui->xMinSlider->getSliderPosition());
	xmax.setNum(_ui->xMaxSlider->getSliderPosition());
	ymin.setNum(_ui->yMinSlider->getSliderPosition());
	ymax.setNum(_ui->yMaxSlider->getSliderPosition());
	_ui->yMinEdit->setText(ymin);
	_ui->yMaxEdit->setText(ymax);
	_ui->xMinEdit->setText(xmin);
	_ui->xMaxEdit->setText(xmax);

	emit(this->xMinChanged(_ui->xMinSlider->getSliderPosition()));
	emit(this->xMaxChanged(_ui->xMaxSlider->getSliderPosition()));
	emit(this->yMinChanged(_ui->yMinSlider->getSliderPosition()));
	emit(this->yMaxChanged(_ui->yMaxSlider->getSliderPosition()));

	delete xsort;
	delete ysort;
}

void PlotControl::setData(QVector<QPointF> xy) {

	qreal ydiff = 0, xdiff = 0;
	QString xmin, xmax, ymin, ymax;
	auto sortX = [](const QPointF p1,const QPointF p2) -> bool {
		return p1.x() < p2.x();
	};
	auto sortY = [](const QPointF p1, const QPointF p2) -> bool {
		return p1.y() < p2.y();
	};

	// First sort for x
	std::sort(xy.begin(), xy.end(), sortX);
	_xMinValue = xy.first().x();
	_xMaxValue = xy.last().x();

	// THen sort y
	std::sort(xy.begin(), xy.end(), sortY);

	_yMinValue = xy.first().y();
	_yMaxValue = xy.last().y();

	//Compute diff
	xdiff = fabs(_xMaxValue - _xMinValue);
	ydiff = fabs(_yMaxValue - _yMinValue);

	_ui->xMaxSlider->setValue(xdiff, _xMaxValue - (xdiff / 2),
			_xMaxValue + (xdiff / 2));
	_ui->xMinSlider->setValue(xdiff, _xMinValue - (xdiff / 2),
			_xMinValue + (xdiff / 2));
	_ui->yMaxSlider->setValue(ydiff, _yMaxValue - (ydiff / 2),
			_yMaxValue + (ydiff / 2));
	_ui->yMinSlider->setValue(ydiff, _yMinValue - (ydiff / 2),
			_yMinValue + (ydiff / 2));
	_ui->xMinSlider->setSliderPosition(_xMinValue);
	_ui->xMaxSlider->setSliderPosition(_xMaxValue);
	_ui->yMinSlider->setSliderPosition(_yMinValue);
	_ui->yMaxSlider->setSliderPosition(_yMaxValue);
	xmin.setNum(_ui->xMinSlider->getSliderPosition());
	xmax.setNum(_ui->xMaxSlider->getSliderPosition());
	ymin.setNum(_ui->yMinSlider->getSliderPosition());
	ymax.setNum(_ui->yMaxSlider->getSliderPosition());
	_ui->yMinEdit->setText(ymin);
	_ui->yMaxEdit->setText(ymax);
	_ui->xMinEdit->setText(xmin);
	_ui->xMaxEdit->setText(xmax);

	emit(this->xMinChanged(_ui->xMinSlider->getSliderPosition()));
	emit(this->xMaxChanged(_ui->xMaxSlider->getSliderPosition()));
	emit(this->yMinChanged(_ui->yMinSlider->getSliderPosition()));
	emit(this->yMaxChanged(_ui->yMaxSlider->getSliderPosition()));

}

void PlotControl::setPlot(QCustomPlot * plot) {
	if (plot != this->_plot)
		this->_plot = plot;
}
