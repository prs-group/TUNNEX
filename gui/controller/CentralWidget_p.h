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

#ifndef CentralWidget_p_h
#define CentralWidget_p_h

#include "controller/CentralWidget.h"
#include <QtCore>
#include <QJsonObject>

#include "model/AbstractXYDataFileModel.h"
#include "controller/plotcontrol.h"
#include "controller/paramwidget.h"
#include "model/paramlistmodel.h"

#include "view/ui_WKBWindowSession.h"

#include "controller/resultwidget.h"
#include "controller/plotedit.h"
#include "model/interpolation.h"
#include "math/findroot.h"
#include "math/adapt.h"
#include "gui/definitions.h"
#include <complex>
#include <math.h>
#include <algorithm>
#include <QStateMachine>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QDialog>
#include <QLineEdit>

#define AXIS_ADD_RANGE 0.05

using namespace alglib;
using namespace WKB::model;
using namespace QCP;

namespace WKB {

namespace impldetail {
//! Private implementation of Central Widget

//! The private implementation of the Central Widgets is
//! implemented using the pimple idom so here is the actual
//! code

class CentralWidgetPrivate: public QObject {

private:

	Q_DECLARE_PUBLIC (WKB::widgets::CentralWidget);
	//! Instance of Central Widget
	WKB::widgets::CentralWidget *q_ptr;

	//! Saves original position of projected IRC
	/*!
	 * This is done for proper resize of total widget
	 */
	QPoint* _irc_pro_org_pos;
	//! @{
	//!  The widgets internal logic
	/*!
	 *  The state machine saves the current state of application
	 *  Currently two states are inserted: The input state and
	 *  the actual wkb computation with the combined IRC
	 */


	//! This is needed in order to store the startingMaterial information
	double startingMaterialEnergy=0.0;
	double startingMaterialZPVE=0.0;

	bool toKcal=true;
	bool zoomedIn=false;

	enum graphLayer {data=0, inter=1, attempt0=2,attempt1=3,attempt2=4,attempt3=5};



	/*!
	 * @brief Statemachine manages transition
	 * @detail The states manged by this machine
	 * includes the zoomed in state and zoomed out state
	 */
	QStateMachine zoomIrcCorrecedMachine;

	// Need to know if IRC is enlarged for switch
	bool enlarged=false;

	void replotIRC(AbstractXYDataFileModel *&data) Q_DECL_NOEXCEPT;
	void replotIRCp(AbstractXYDataFileModel *&data) Q_DECL_NOEXCEPT;
	void replotIRCcor(AbstractXYDataFileModel *&data) Q_DECL_NOEXCEPT;

	void interIRC(AbstractInterpol* inter) Q_DECL_NOEXCEPT;
	void interIRCp(AbstractInterpol* inter) Q_DECL_NOEXCEPT;
	void interIRCcorr(AbstractInterpol* inter) Q_DECL_NOEXCEPT;

	void plotGraph(QCustomPlot * plot, AbstractXYDataFileModel* data,
			graphLayer l) Q_DECL_NOEXCEPT;
	void replotAll() Q_DECL_NOEXCEPT;

	void enlargeIRCCor() Q_DECL_NOEXCEPT;
	void shrinkIRCCor() Q_DECL_NOEXCEPT;

public:
	//! Constructor
	//!
	//! This Constructor evaluates the private method setupConnections
	//! and setupStateMachine
	//! @param s the instance of public visible class
	CentralWidgetPrivate(widgets::CentralWidget *s) Q_DECL_NOEXCEPT;

	enum plot {IRC,IRCp,IRCcorr};

	void dataChanged(plot i, AbstractXYDataFileModel* data) Q_DECL_NOEXCEPT {
		switch(i) {
			case plot::IRC:
			replotIRC(data);
			break;
			case plot::IRCp:
			replotIRCp(data);
			break;
			case plot::IRCcorr:
			replotIRCcor(data);
			break;
			default:
			break;

		}
	}

	void interpolChanged(plot i, AbstractInterpol* inter) Q_DECL_NOEXCEPT {
		switch (i) {
			case plot::IRC:
			interIRC(inter);
			break;
			case plot::IRCp:
			interIRCp(inter);
			break;
			case plot::IRCcorr:
			interIRCcorr(inter);
			break;

			default:
			break;
		}
	}

	void reactiveModeChanged(const QVariant &v) Q_DECL_NOEXCEPT;
	void doubleClickedIRC() Q_DECL_NOEXCEPT;
	void resize(QResizeEvent* event) Q_DECL_NOEXCEPT;

	// Function is invoked when Context Menu is requested by the plot widget
	void showContextMenu(const QPoint &pos, QCustomPlot *plot);

	// Action slots for customMenu
	void resetAxisToStandard(QCustomPlot *plot);
	void exportGraph(QCustomPlot *plot);

};

class ImageSizeDialog: public QDialog {

public:
	// We need two text fields for width and height
	QLineEdit *_widthEdit, *_heightEdit;
	QLabel *_widthLabel, *_heightLabel;
	QPushButton* _ok, *_cancel;

	ImageSizeDialog(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f =
			Qt::WindowFlags()) :
			QDialog(parent, f), _widthEdit(new QLineEdit), _heightEdit(
					new QLineEdit), _widthLabel(new QLabel(tr("Width: "))), _heightLabel(
					new QLabel(tr("Height: "))), _ok(new QPushButton(tr("Ok"))), _cancel(
					new QPushButton(tr("Cancel"))) {

		QGridLayout* layout = new QGridLayout;
		QHBoxLayout* buttonLayout = new QHBoxLayout;
		layout->addWidget(_widthLabel, 0, 0);
		layout->addWidget(_widthEdit, 0, 1);
		layout->addWidget(_heightLabel, 1, 0);
		layout->addWidget(_heightEdit, 1, 1);
		buttonLayout->addWidget(_ok);
		buttonLayout->addWidget(_cancel);
		buttonLayout->setSizeConstraint(QLayout::SetFixedSize);
		layout->addItem(
				new QSpacerItem(20, 20, QSizePolicy::Expanding,
						QSizePolicy::Expanding), 2, 0);
		layout->addLayout(buttonLayout, 2, 1);
		setModal(true);
		this->setLayout(layout);

		// Resizing
		_ok->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		_ok->setDisabled(true);
		_cancel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		_widthLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		_heightLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		this->setFixedSize(this->minimumSizeHint());

		// Line Edit
		_heightEdit->setInputMask("D999");
		_widthEdit->setInputMask("D999");

		connect(_ok, &QPushButton::clicked, this, &ImageSizeDialog::accept);

		connect(_cancel, &QPushButton::clicked, this, &ImageSizeDialog::reject);
		connect(_widthEdit, &QLineEdit::textChanged, this,
				&ImageSizeDialog::textHasChanged);
		connect(_heightEdit, &QLineEdit::textChanged, this,
				&ImageSizeDialog::textHasChanged);
#ifdef Q_OS_MACX
		this->setWindowFlags(Qt::Sheet);
#endif
	}
	;

	void textHasChanged(const QString &text) {
		if (_heightEdit->text().isEmpty() || _widthEdit->text().isEmpty())
			_ok->setDisabled(true);
		else
			_ok->setDisabled(false);
	}
	;

};

//*****************************************************************
// Implementation
//*****************************************************************
CentralWidgetPrivate::CentralWidgetPrivate(widgets::CentralWidget *s) Q_DECL_NOEXCEPT :
q_ptr(s)
{
	Q_Q(widgets::CentralWidget);

	q->_ui=new widgets::Ui::CentralWidget();

	q->_ui->setupUi(q);



	// Interactions
	q->_ui->irc->setInteraction(iRangeZoom);
	q->_ui->irc->setInteraction(iRangeDrag);

	q->_ui->irc_pro->setInteraction(iRangeZoom);
	q->_ui->irc_pro->setInteraction(iRangeDrag);
	q->_ui->irc_pro->setInteraction(iSelectPlottables);

	q->_ui->irc_cor->setInteraction(iRangeZoom);
	q->_ui->irc_cor->setInteraction(iRangeDrag);

	connect(q->_ui->irc_cor,&QCustomPlot::mouseDoubleClick,
			this,[this](QMouseEvent* event) {
				if(enlarged)
				this->shrinkIRCCor();
				else
				this->enlargeIRCCor();

				event->accept();
			});

	// Setup connection for customMenu
	connect(q->_ui->irc, &QCustomPlot::customContextMenuRequested,
			this, [this,q](const QPoint &point) {
				this->showContextMenu(point,q->_ui->irc);
			});

	connect(q->_ui->irc_pro, &QCustomPlot::customContextMenuRequested,
			this, [this,q](const QPoint &point) {
				this->showContextMenu(point,q->_ui->irc_pro);
			});

	connect(q->_ui->irc_cor, &QCustomPlot::customContextMenuRequested,
			this, [this,q](const QPoint &point) {
				this->showContextMenu(point,q->_ui->irc_cor);
			});



		}

void CentralWidgetPrivate::replotIRC(AbstractXYDataFileModel *&data)
		Q_DECL_NOEXCEPT {
	Q_Q(widgets::CentralWidget);
	plotGraph(q->_ui->irc, data, graphLayer::data);
}
void CentralWidgetPrivate::replotIRCp(AbstractXYDataFileModel *&data)
		Q_DECL_NOEXCEPT {
	Q_Q(widgets::CentralWidget);
	plotGraph(q->_ui->irc_pro, data, graphLayer::data);
}
void CentralWidgetPrivate::replotIRCcor(AbstractXYDataFileModel *&data)
		Q_DECL_NOEXCEPT {
	Q_Q(widgets::CentralWidget);
	plotGraph(q->_ui->irc_cor, data, graphLayer::data);
}

void CentralWidgetPrivate::interIRC(AbstractInterpol* inter) Q_DECL_NOEXCEPT {
	Q_Q(widgets::CentralWidget);
	if (inter == Q_NULLPTR)
		return;
	plotGraph(q->_ui->irc, inter->getInterpData(), graphLayer::inter);
}

void CentralWidgetPrivate::interIRCp(AbstractInterpol* inter) Q_DECL_NOEXCEPT {
	Q_Q(widgets::CentralWidget);
	if (inter == Q_NULLPTR)
		return;
	plotGraph(q->_ui->irc_pro, inter->getInterpData(), graphLayer::inter);
}

void CentralWidgetPrivate::interIRCcorr(AbstractInterpol* inter)
		Q_DECL_NOEXCEPT {
	Q_Q(widgets::CentralWidget);
	if (inter == Q_NULLPTR)
		return;

	plotGraph(q->_ui->irc_cor, inter->getInterpData(), graphLayer::inter);
}

void CentralWidgetPrivate::plotGraph(QCustomPlot * plot,
		AbstractXYDataFileModel* data, graphLayer l) Q_DECL_NOEXCEPT {
	// Delete old data when no data is given
	while (plot->graphCount() <= 2)
		plot->addGraph();

	if (data == Q_NULLPTR) {
		plot->clearGraphs();
		plot->replot();
		return;
	}

	if (data->isEmpty()) {
		plot->clearGraphs();
		plot->replot();
		return;
	}




	XYdata dData = data->getData();
	dVector x, y;


	// Everything right replot by clearing Graphs
	plot->graph(l)->clearData();
	plot->replot();

	// Fill it with data
	x = data->getXValues();
	y = data->getYValues();

	double xMin = data->xmin(), xMax = data->xmax(), yMin = data->ymin(), yMax =
			data->ymax();

	XYdata d = data->getData();

	qSort(d.begin(), d.end(), [](QPointF &v1,QPointF &v2) {
		return v1.x() < v2.x();
	});
	double startingMaterialEnergy = d.begin()->y();

	auto convertToRel = [&startingMaterialEnergy](double &v) {
		v=(v-startingMaterialEnergy)*HTOKCAL;
	};

	// Convert to kcal with relative Energy
	if (toKcal) {
		for (auto &i : *y)
			convertToRel(i);
	}



	plot->graph(l)->addData(*x, *y);
	// To Do: right labeling of axis

	plot->xAxis->setLabel(
			QString("Intrinsic Reaction Coordinate [") + QChar(0x221A)
					+ QString("(amu)") + QString(" bohr ]"));
	if (toKcal)

		plot->yAxis->setLabel(
				QString("E [kcal mol") + QChar(0x207B) + QChar(0x00B9)
						+ QString("]"));

	else
		plot->yAxis->setLabel("E [Hartree]");


	if (toKcal) {
		convertToRel(yMin);
		convertToRel(yMax);
	}

	switch (l) {
	case graphLayer::data:
		plot->graph(l)->setLineStyle(QCPGraph::lsNone);
		plot->graph(l)->setScatterStyle(QCPScatterStyle::ssPlus);
		xMin += xMin * AXIS_ADD_RANGE;
		xMax += xMax * AXIS_ADD_RANGE;
		yMin += yMin * AXIS_ADD_RANGE;
		yMax += yMax * AXIS_ADD_RANGE;
		plot->xAxis->setRange(xMin, xMax);
		plot->yAxis->setRange(yMin, yMax);
		plot->graph(l)->setSelectable(true);
		break;

	case graphLayer::inter: {
		plot->graph(l)->setLineStyle(QCPGraph::lsLine);
		plot->graph(l)->setScatterStyle(QCPScatterStyle::ssNone);
		plot->graph(l)->setSelectable(false);
		break;
	}

	default:
		break;
	}

	plot->replot();
}


void CentralWidgetPrivate::reactiveModeChanged(const QVariant &mode)
		Q_DECL_NOEXCEPT {

	Q_Q(widgets::CentralWidget);
	while (true) {
		if (q->_ui->irc_cor->graphCount() > 6)
			break;
		q->_ui->irc_cor->addGraph();
	}
	auto graph = [q](graphLayer l) -> QCPGraph* {
		return q->_ui->irc_cor->graph(l);
	};

	auto xAxis = [q]() -> QCPAxis* {
		return q->_ui->irc_cor->xAxis;
	};
	// The energy in mode is relative to startingMaterial Energy

	// Good thing that the graph stores everything sorted
	// From the lowest to the largest x value

	QCPGraph* dataGraph = q->_ui->irc_cor->graph(graphLayer::data);
	double startingMaterialEnergy = 0.0;
	if (!dataGraph->data()->isEmpty()) {
		startingMaterialEnergy = dataGraph->data()->first().value;
	}

	double xMin = xAxis()->range().lower;
	double xMax = xAxis()->range().upper;
	// The line should't go through the whole graph
	// So take 10 % off;
	xMin += xMin * 0.1;
	xMax -= xMax * 0.1;
	QVector<double> attemptEnergies;
	for (double i = 0.5; i <= 3.5; i += 1)
		attemptEnergies.append(mode.toDouble() * i);

	for (auto &i : attemptEnergies) {
		i *= WNTOH;
		if (toKcal)
			i *= HTOKCAL;
		i += startingMaterialEnergy;
	}



	QVector<double> xValues( { xMin, xMax }), yValues0( { attemptEnergies[0],
			attemptEnergies[0] });
	QVector<double> yValues1( { attemptEnergies[1], attemptEnergies[1] }),
			yValues2( { attemptEnergies[2], attemptEnergies[2] }), yValues3( {
					attemptEnergies[3], attemptEnergies[3] });

	graph(graphLayer::attempt0)->clearData();
	graph(graphLayer::attempt0)->addData(xValues, yValues0);


	graph(graphLayer::attempt1)->clearData();
	graph(graphLayer::attempt1)->addData(xValues, yValues1);


	graph(graphLayer::attempt2)->clearData();
	graph(graphLayer::attempt2)->addData(xValues, yValues2);


	graph(graphLayer::attempt3)->clearData();
	graph(graphLayer::attempt3)->addData(xValues, yValues3);

	q->_ui->irc_cor->replot();

}

void CentralWidgetPrivate::doubleClickedIRC() Q_DECL_NOEXCEPT {

}

void CentralWidgetPrivate::resize(QResizeEvent* event) Q_DECL_NOEXCEPT {
	//Compute th
}

void CentralWidgetPrivate::enlargeIRCCor() Q_DECL_NOEXCEPT {
	Q_Q(widgets::CentralWidget);

	q->_ui->irc->hide();
	q->_ui->irc_pro->hide();
	q->_ui->horizontalLayout->removeItem(q->_ui->hSpacer_1);
	q->_ui->horizontalLayout->removeItem(q->_ui->hSpacer_2);
	q->_ui->irc_cor->updateGeometry();

	enlarged = true;
}

void CentralWidgetPrivate::shrinkIRCCor() Q_DECL_NOEXCEPT {
	Q_Q(widgets::CentralWidget);
	auto layout = [&q]() {return q->_ui->horizontalLayout;};
	q->_ui->irc->show();
	q->_ui->irc_pro->show();
	layout()->removeWidget(q->_ui->irc_cor);

	layout()->addItem(q->_ui->hSpacer_1);
	layout()->addWidget(q->_ui->irc_cor);
	layout()->addItem(q->_ui->hSpacer_2);

	enlarged = false;
}

void CentralWidgetPrivate::showContextMenu(const QPoint &point,
		QCustomPlot *plot) {
	Q_Q(widgets::CentralWidget);
	QMenu* menu = new QMenu(plot);
	q->mapToGlobal(point);
	QAction* reset = menu->addAction(tr("Reset Axis"));

	QAction* exp = menu->addAction(tr("Export Graph"));
	menu->move(plot->mapToGlobal(point));

	// Connect actions triggered function
	connect(reset, &QAction::triggered, this, [this,plot]() {
		this->resetAxisToStandard(plot);
	});

	connect(exp, &QAction::triggered, this, [this,plot]() {
		this->exportGraph(plot);
	});

	menu->show();

}

void CentralWidgetPrivate::resetAxisToStandard(QCustomPlot *plot) {
	if (!plot->graph(graphLayer::data))
		return;
	QList < QCPData > data = plot->graph(graphLayer::data)->data()->values();
	// If no data do nothing
	if (data.isEmpty())
		return;

	// Data is already sorted by x
	double xMin = data.first().key, xMax = data.last().key;

	// sort by y
	auto sortY = [](const QCPData &d1,const QCPData &d2) {
		return d1.value < d2.value;
	};

	qSort(data.begin(), data.end(), sortY);

	double yMin = data.first().value, yMax = data.last().value;
	xMin += xMin * AXIS_ADD_RANGE;
	xMax += xMax * AXIS_ADD_RANGE;
	yMin += yMin * AXIS_ADD_RANGE;
	yMax += yMax * AXIS_ADD_RANGE;
	plot->xAxis->setRange(xMin, xMax);
	plot->yAxis->setRange(yMin, yMax);
	plot->replot();

}

void CentralWidgetPrivate::exportGraph(QCustomPlot *plot) {
	Q_Q(widgets::CentralWidget);
	QSettings s;
	// First ask for size of picture
	ImageSizeDialog sizeDiag(q);
	int oldWidth = 0, oldHeight = 0;

	// Set to old values or 1024 when not valid
	oldWidth =
			s.value("centralWidget/oldImageWidth").isValid() ?
					s.value("centralWidget/oldImageWidth").toInt() : 1024;
	oldHeight =
			s.value("centralWidget/oldImageHeight").isValid() ?
					s.value("centralWidget/oldImageHeight").toInt() : 1024;

	// Set text of field
	sizeDiag._widthEdit->setText(QString("%1").arg(oldWidth));
	sizeDiag._heightEdit->setText(QString("%1").arg(oldHeight));

	// Execute dialog and return when not accepted
	if (sizeDiag.exec() != QDialog::Accepted)
		return;

	// Set width and height for export
	int width = sizeDiag._widthEdit->text().toInt(), height =
			sizeDiag._heightEdit->text().toInt(), scale = 1;

	// Set new size in settings
	s.setValue("centralWidget/oldImageWidth", width);
	s.setValue("centralWidget/oldImageHeight", height);




	QString oldPath = s.value("centralWidget/oldPath").toString();
	if (oldPath.isEmpty() | !QFile::exists(oldPath))
		oldPath = QStandardPaths::DesktopLocation;
	QString filename = QFileDialog::getSaveFileName(q, tr("Export Graph"),
			oldPath,
#ifdef QT_NO_PRINTER
			tr("Images (*.png *.bmp, *.jpg)"));
#else
			tr("Images (*.png *.bmp, *.jpg);;PDF (*.pdf)"));
#endif

	if (filename.contains(".png")) {
		if (plot->savePng(filename, width, height, scale))
			s.setValue("centralWidget/oldPath", filename);
	} else if (filename.contains(".jpg")) {
		if (plot->saveJpg(filename, width, height, scale))
			s.setValue("centralWidget/oldPath", filename);
	} else if (filename.contains(".bmp")) {
		if (plot->saveBmp(filename, width, height, scale))
			s.setValue("centralWidget/oldPath", filename);
	} else if (filename.contains(".pdf")) {
		if (plot->savePdf(filename, true, width, height))
			s.setValue("centralWidget/oldPath", filename);
	} else
		qWarning() << "Unknown file format";
}

}
}


#endif /* CentralWidget_p_h */
