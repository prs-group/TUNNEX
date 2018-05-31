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

#ifndef PLOTCONTROL_H
#define PLOTCONTROL_H

#include <QDialog>
#include <QVector>
#include <QPoint>

class QCustomPlot;

namespace WKB {
namespace widgets {
namespace Ui {
class PlotControl;
}

class PlotControl: public QDialog {
	Q_OBJECT

public:
	/**
	 * Reimplement standard Constructor from QDialog
	 * @param parent The parent widget (passed to QDialog superclass)
	 */
	explicit PlotControl(QWidget *parent = 0);
	/**
	 * Additional Constructor with the plotWidget
	 * @param parent The parent widget (passed to QDialog superclass)
	 * @param plot Controlled plot widget
	 */
	explicit PlotControl(QWidget *parent, QCustomPlot* plot);
	/**
	 * Basic Deconstructor
	 */
	~PlotControl();
	/**
	 * Sets data to x and y
	 * Initilized floatingPointQSliders and sets the default values
	 * for the text fields with data given.
	 * emits yMaxchanged yMinChanged xMaxChanged and xMinChanged after
	 * everything is initilized
	 * @param x The x values of the plot
	 * @param y The y values of the plot
	 */
	void setData(const QVector<double> &x, const QVector<double> &y);
	/**
	 * Same as setData(const QVector<double>&, const QVector<double>&)
	 * @param xy The xy data as a vector of points
	 */
	void setData(QVector<QPointF> xy);
	/**
	 * Sets the plot widget
	 * The plot widget is saved as a pointer copy
	 * and is never to be deleted by this class
	 * @param plot The plot widget this class should control
	 */
	void setPlot(QCustomPlot* plot);

public slots:
	/**
	 * Is invoked when the minimum Value of y is changed
	 * Will be connected to floatingPointQSlider::valueChanged()
	 * @param v The changed value
	 */
	void yMinChanged(double v);
	/// Same as yMinChanged()
	void yMaxChanged(double v);
	/// Same as yMinChanged()
	void xMinChanged(double v);
	/// Same as yMinChanged()
	void xMaxChanged(double v);
	/**
	 * Is invoked when the y Min changes in the text field
	 */
	void yMinEntered();
	/// Same as yMinEntered
	void yMaxEntered();
	/// Same as yMinEntered
	void xMinEntered();
	/// Same as yMinEntered
	void xMaxEntered();

private:
	Ui::PlotControl *_ui;
	qreal _yMinValue, _yMaxValue, _xMinValue, _xMaxValue;
	QCustomPlot *_plot;
    
    void SetupUiConnections();

};
}
}

#endif // PLOTCONTROL_H
