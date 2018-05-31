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

#include "controller/plotedit.h"
#include "controller/plotedit_p.h"

using namespace WKB::widgets;
using namespace WKB::model;

using namespace WKB::impldetail;

PlotEditPrivate::PlotEditPrivate(PlotEdit *q) :
		q_ptr(q) {
	init();
}

PlotEditPrivate::~PlotEditPrivate() {
	delete _buttonLayout;
	delete _plot;
	delete _sigmap;
	delete _layout;
	if (!_data)
		delete _data;
}

void PlotEditPrivate::init() {
	Q_Q (widgets::PlotEdit);
	_layout = new QBoxLayout(QBoxLayout::TopToBottom, q);
	_buttonLayout = new QHBoxLayout();
	_plot = new QCustomPlot;
	_data = new WKB::model::TSVData;
	_changedData = false;
	QPushButton *ok = new QPushButton(tr("Ok"));
	QPushButton *cancel = new QPushButton(tr("Cancel"));
	QPushButton *apply = new QPushButton(tr("Apply"));
	cancel->setDefault(true);
	_buttonLayout->addWidget(ok);
	_buttonLayout->addWidget(cancel);
	_buttonLayout->addWidget(apply);
	_plot->setInteractions(QCP::iRangeZoom | QCP::iSelectPlottables);
	_plot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
	_sigmap = new QSignalMapper(this);
	_sigmap->setMapping(ok, buttons::ok);
	_sigmap->setMapping(cancel, buttons::cancel);
	_sigmap->setMapping(apply, buttons::apply);
	_layout->addWidget(_plot);
	_layout->addLayout(_buttonLayout);
	connect(ok, SIGNAL(clicked()), _sigmap, SLOT(map()));
	connect(cancel, SIGNAL(clicked()), _sigmap, SLOT(map()));
	connect(apply, SIGNAL(clicked()), _sigmap, SLOT(map()));
	connect(_sigmap, SIGNAL(mapped(int)), this, SLOT(buttonsClicked(int)));
	connect(this, SIGNAL(plotDataChanged()), this, SLOT(dataChanged()));
connect(_plot,SIGNAL(mouseMove(QMouseEvent*)) ,this, SLOT(plottableClickEvent(QMouseEvent*)));
}

void PlotEditPrivate::keyPressEvent(QKeyEvent *event) {
switch (event->key()) {
case Qt::Key_Delete: {
	QList<QCPGraph*> selection = _plot->selectedGraphs();
	if (!selection.isEmpty()) {
		for (auto *s : selection) {
			_plot->removeGraph(s);
			emit (plotDataChanged());}
			_plot->replot();
		}
	break;
}
default:
	break;
}
}

void PlotEditPrivate::plottableClickEvent(QMouseEvent *event) {
if (event->buttons() == Qt::LeftButton) {
	QList<QCPGraph*> selection = _plot->selectedGraphs();
	if (!selection.isEmpty()) {
		switch (selection.count()) {
		case 1: {
			// x and y relative to widget
			this->_plot->setInteraction(QCP::iRangeDrag, false);
			//int x = event->x();
			int y = event->y();
			//QCPAxis* xAxis = plot->axisRect()->rangeDragAxis(Qt::Horizontal);
			QCPAxis* yAxis = _plot->axisRect()->rangeDragAxis(Qt::Vertical);
			//double xd=xAxis->pixelToCoord((double) x);
			double yd = yAxis->pixelToCoord((double) y);
			QVector<double> xv, yv;
			const QCPDataMap* data = selection.first()->data();
			xv.append(data->begin().key());
			yv.append(yd);

			selection.first()->setData(xv, yv);
			_plot->replot();
			this->_plot->setInteraction(QCP::iRangeDrag, true);
			emit (plotDataChanged());break
;		}
		default: {
			selection.first()->setSelected(false);
			break;
		}
	}
}
}
}

void PlotEditPrivate::dataChanged() {
	XYdata dData;
	for (int i = 0; i < _plot->graphCount(); i++) {
		QCPGraph* graph = _plot->graph(i);
		const QCPDataMap* data = graph->data();

		QList<double> keys = data->keys();

		for (auto &k : keys)
			dData.push_back((QPointF(k, data->value(k).value)));

	}
	this->_data->setData(dData);
	this->_changedData = true;
#ifdef DEBUG
	qDebug() << dData.first().count() << dData.last().count();
#endif
}

void PlotEditPrivate::setData(WKB::model::AbstractXYDataFileModel *data) {

	if (!data)
		return;

	if (this->_data)
		delete this->_data;

	if (!data->isEmpty()) {
		this->_data = new WKB::model::TSVData(data->getData());
		replot();
	}

}

void PlotEditPrivate::replot() {

	XYdata dData = _data->getData();

	// Check for valid data
	if (dData.isEmpty())
		return;
	// create graph and assign data to it
	// Set every point in data to plottable
	for (int i = 0; i < dData.size(); i++) {
		QVector<double> x(1, dData.at(i).x()), y(1, dData.at(i).y());
		_plot->addGraph();
		_plot->graph(i)->setData(x, y);
		_plot->graph(i)->setLineStyle(QCPGraph::lsNone);
		_plot->graph(i)->setScatterStyle(QCPScatterStyle::ssCross);
	}
	// give the axes some labels:
	_plot->xAxis->setLabel("Reaction Coordinate");
	_plot->yAxis->setLabel("Energy");
	// set axes ranges, so we see all data
	_plot->xAxis->setRange(_data->xmin(), _data->xmax());
	_plot->yAxis->setRange(_data->ymin(), _data->ymax());
	_plot->replot();
}

WKB::model::AbstractXYDataFileModel* PlotEditPrivate::getData() {
	return this->_data;
}

void PlotEditPrivate::buttonsClicked(int b) {
	Q_Q (widgets::PlotEdit);
	switch (b) {
	case buttons::ok: {
		if (this->_changedData)
			emit(q->dataChanged(this->_data));
		emit(q->aboutToClose(QDialog::Accepted));
		break;

	}
	case buttons::cancel: {
		emit(q->aboutToClose(QDialog::Rejected));
		break;
	}
	case buttons::apply: {
		if (this->_changedData)
			emit(q->dataChanged(this->_data));
		break;

	}
	default:
		break;
	}
	return;
}

//Plotedit Implementation

PlotEdit::PlotEdit(QWidget *parent) :
        QDialog(parent), d_ptr(new PlotEditPrivate(this)) {
	//this->setMinimumWidth(500);
	//this->setMinimumHeight(300);
}

PlotEdit::PlotEdit(PlotEditPrivate &d, QWidget* parent) :
		QDialog(parent), d_ptr(&d) {
}

PlotEdit::~PlotEdit() {
	delete d_ptr;
}

void PlotEdit::setData(AbstractXYDataFileModel *data) {
	Q_D(impldetail::PlotEdit);
	d->setData(data);
}

void PlotEdit::aboutToClose(int code) {
	this->done(code);
	emit(finished(0));
}

AbstractXYDataFileModel* PlotEdit::getData() {
	Q_D(impldetail::PlotEdit);
	return d->getData();
}

