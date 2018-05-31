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

#ifndef PLOTEDIT_P_H
#define PLOTEDIT_P_H
#include <QObject>
#include <QtCore>
#include <QHBoxLayout>
#include <QSignalMapper>
#include <QtCore/qpointer.h>

#include "QCustomPlot/qcustomplot.h"
#include "model/AbstractXYDataFileModel.h"
#include "model/tsvdata.h"

enum buttons {
	ok, cancel, apply,
};

namespace WKB {
namespace widgets {
class PlotEdit;

}

namespace model {
class AbstractXYDataFileModel;
}
}

namespace WKB {
namespace impldetail {
class PlotEditPrivate: public QObject {
	Q_OBJECT
	Q_DECLARE_PUBLIC(WKB::widgets::PlotEdit)
public:
	PlotEditPrivate(WKB::widgets::PlotEdit* q);
	~PlotEditPrivate();
	void setData(WKB::model::AbstractXYDataFileModel* data);
	WKB::model::AbstractXYDataFileModel* getData();

public slots:
	void dataChanged();
	void plottableClickEvent(QMouseEvent* event);
	void buttonsClicked(int b);
protected:
	void keyPressEvent(QKeyEvent* event);
	void replot();

	signals:
	void plotDataChanged();

private:
	QBoxLayout* _layout;
	QHBoxLayout* _buttonLayout;
	QCustomPlot* _plot;
	WKB::model::AbstractXYDataFileModel* _data;
	QSignalMapper* _sigmap;
	bool _changedData;
	WKB::widgets::PlotEdit* q_ptr;
	void init();

};
}
}

#endif // PLOTEDIT_P_H
