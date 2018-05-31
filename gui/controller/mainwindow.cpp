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

#include <QDockWidget>
#include <QTableWidget>
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QCloseEvent>
#include <QUndoGroup>
#include <QUndoView>
#include <QLineEdit>
#include <QApplication>
#include <QErrorMessage>
#include <QSpinBox>
#include "controller/mainwindow.h"
#include "controller/AppMenu.h"
#include "controller/CentralWidget.h"
#include "controller/paramwidget.h"
#include "controller/resultwidget.h"
#include "controller/commands.h"
#include "model/paramlistmodel.h"
#include "model/resultModel.h"
#include "model/tsvdata.h"
#include "model/interpolation.h"
#include "model/WKBModel.h"

// Test

using namespace WKB;

int mainWindow::documentNumber=1;


mainWindow::mainWindow(QUndoStack *stack, QWidget* parent) Q_DECL_NOEXCEPT :
QMainWindow(parent), _IRC(Q_NULLPTR),
_pIRC(Q_NULLPTR), _corIRC(Q_NULLPTR),
_IRCinter(Q_NULLPTR),_pIRCinter(Q_NULLPTR),
_corIRCinter(Q_NULLPTR),_wkb(new model::WKBModel),
_jOb(new QJsonObject()),
_uStack(stack), _documentName(QString("Document_") + QString::number(documentNumber++))

{
    // Init parameters
	_parameters= {"","","","1"};
	Q_ASSERT(_parameters.size() >= 4);
	this->setMenuBar(new widgets::AppMenu);
    setupWidgets();
    setupConnections();
    this->setWindowTitle(documentName());

}

mainWindow::~mainWindow() Q_DECL_NOEXCEPT {
    delete _jOb;
    _IRC.reset();
    _pIRC.reset();
    _corIRC.reset();
    _IRCinter.reset();
    _pIRCinter.reset();
    _corIRCinter.reset();
}
// Init Functions
void mainWindow::setupWidgets() Q_DECL_NOEXCEPT{
    QWidget *centralW=new widgets::CentralWidget();
    QMenuBar *mBar=new widgets::AppMenu();

    centralW->setObjectName(QStringLiteral("CentralWidget"));
    mBar->setObjectName(QStringLiteral("MenuBar"));

    this->setCentralWidget(centralW);
    this->setMenuBar(mBar);

    // Setup the dock widgets

    widgets::ParamWidget* paramW = new widgets::ParamWidget(_uStack);
    paramW->setObjectName(QStringLiteral("WKB_Parameters"));
    widgets::ResultWidget* resW = new widgets::ResultWidget;
    resW->setObjectName(QStringLiteral("WKB_Results"));



    QDockWidget *paramDockWidget=new QDockWidget(tr("Parameters"),this);
    QDockWidget *resultDockWidget= new QDockWidget(tr("Results"),this);
    paramDockWidget->setWidget(paramW);
    resultDockWidget->setWidget(resW);

    paramDockWidget->setObjectName("ParamDockWidget");
    resultDockWidget->setObjectName("ResultDockWidget");

    this->addDockWidget(Qt::RightDockWidgetArea,
                        paramDockWidget);
    this->addDockWidget(Qt::RightDockWidgetArea,
                        resultDockWidget);
#ifndef Q_OS_MACX
	QAction *redo = _uStack->createRedoAction(this, common::editMenuItemNames["Redo"]);
	redo->setShortcut(QKeySequence::Redo);

	QAction* undo = _uStack->createUndoAction(this,
		common::editMenuItemNames["Undo"]);
	undo->setShortcut(QKeySequence::Undo);

	widgets::AppMenu *appmenu = qobject_cast<widgets::AppMenu*>(menuBar());
	Q_CHECK_PTR(appmenu);
	appmenu->editMenu()->addAction(undo);
	appmenu->editMenu()->addAction(redo);
#endif
}

void mainWindow::setupConnections()Q_DECL_NOEXCEPT{
    // First connect the central Widget for displaying
    widgets::CentralWidget* centralW=this->findChild<widgets::CentralWidget*>("CentralWidget");
    Q_CHECK_PTR(centralW);
    connect(this, &mainWindow::IRCDataChanged,
            centralW,&widgets::CentralWidget::IRCDataChanged);
    connect(this, &mainWindow::IRCpDataChanged,
            centralW,&widgets::CentralWidget::IRCpDataChanged);
    connect(this, &mainWindow::IRCcorrDataChanged,
            centralW,&widgets::CentralWidget::IRCcorDataChanged);

    connect(this, &mainWindow::IRCInterChanged,
            centralW,&widgets::CentralWidget::IRCInterpolHasChanged);
    connect(this,&mainWindow::pIRCInterChanged,
            centralW,&widgets::CentralWidget::pIRCInterpolHasChanged);
    connect(this, &mainWindow::corrIRCInterChanged,
            centralW, &widgets::CentralWidget::corrIRCInterpolHasChanged);

	// Connect the IRC changed slots to this object
	connect(this, &mainWindow::IRCInterChanged, this,
			&mainWindow::IRCInterpolDataHasChanged);
	connect(this, &mainWindow::pIRCInterChanged, this,
			&mainWindow::IRCpInterpolDataHasChanged);


    // The parameter widget
    widgets::ParamWidget *pW=this->findChild<widgets::ParamWidget*>("WKB_Parameters");
    Q_CHECK_PTR(pW);

    connect(pW, &widgets::ParamWidget::startingMaterialEnergyHasChanged,
            _wkb, &model::WKBModel::startingMaterialEnergyChanged);
    connect(pW, &widgets::ParamWidget::startingMaterialZPVEHasChanged,
            _wkb, &model::WKBModel::startingMaterialZPVEChanged);
    connect(pW, &widgets::ParamWidget::reactiveModeHasChanged,
            _wkb, &model::WKBModel::reactiveModeChanged);
	connect(pW, &widgets::ParamWidget::symmetryNumberHasChanged, _wkb,
			&model::WKBModel::symmetryNumberChanged);
    connect(pW,&widgets::ParamWidget::reactiveModeHasChanged,
            centralW,&widgets::CentralWidget::reactiveModeChanged);

    // Connect the line edit to this
    int i=0;
    for(auto lineEdit : pW->findChildren<QLineEdit*>()){
        lineEdit->installEventFilter(this);
        connect(lineEdit,&QLineEdit::textChanged,
                this,[this,i](const QString &text){
                    _parameters[i]=text;
                });
        
        connect(lineEdit,&QLineEdit::textEdited,
                this, [this,lineEdit,i](const QString &text){
                    QUndoCommand* cmd=new widgets::undoLineEditCmd(lineEdit,
                                                          _parameters.at(i),
                                                          text);
                    _uStack->push(cmd);
                    _parameters[i]=text;
                });
        
        i++;
    }

	i -= 1;

	// Connect the spin box to this
	for (auto spinBox : pW->findChildren<QSpinBox*>()) {
		spinBox->installEventFilter(this);

		connect(spinBox,
				static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
				this, [this,spinBox,i](const int &number) {
					QUndoCommand* cmd=new widgets::undoQSpinBoxCmd(spinBox,
					_parameters.at(i).toInt(),
					number);
					_uStack->push(cmd);
			_parameters[i]=QString::number(number);
				});
		i++;
	}

    // WKB model
    connect(this,&mainWindow::corrIRCInterChanged,
            _wkb,
			&model::WKBModel::correctedInterpolChanged);

    // Result Widget with WKB model
    widgets::ResultWidget* rW=this->findChild<widgets::ResultWidget*>("WKB_Results");
    Q_CHECK_PTR(rW);
    connect(_wkb,&model::WKBModel::halfLiveChanged,
            rW,&widgets::ResultWidget::setHalfLives);

    // Parameters with Json object and add to undo stack
    connect(pW,&widgets::ParamWidget::startingMaterialEnergyHasChanged,
            this,[this,pW](const QVariant &v){
				QString name("starting Material Energy");
                this->writeParameterToJson(name, v);
            });
    connect(pW,&widgets::ParamWidget::startingMaterialZPVEHasChanged,
            this,[this](const QVariant &v){
				QString name("starting Material ZPVE");
                this->writeParameterToJson(name, v);
            });
    connect(pW,&widgets::ParamWidget::reactiveModeHasChanged,
            this,[this](const QVariant &v){
                QString name("Reactive Mode");
                this->writeParameterToJson(name, v);
            });

	connect(pW, &widgets::ParamWidget::symmetryNumberHasChanged,
			this,
			[this](const QVariant &v) {
		QString name("Symmetry Number");
		this->writeParameterToJson(name,v);
	});

    connect(_uStack,&QUndoStack::indexChanged,
    		this,
			&mainWindow::undoIndexHasChanged);

	connect(this, &mainWindow::documentNameChanged, this,
			&mainWindow::documentNameHasChanged);

	// Now the menu bar
#ifdef Q_OS_MACX
	widgets::AppMenu *menu = this->findChild<widgets::AppMenu*>("MenuBar");
	Q_CHECK_PTR(menu);
	connect(menu, &widgets::AppMenu::triggered, this,
			&mainWindow::actionTriggered);
#else
	connect(menuBar(), &QMenuBar::triggered,
		this, &mainWindow::actionTriggered);
#endif

}

bool mainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::KeyPress){
        QKeyEvent* keyEvent=dynamic_cast<QKeyEvent*>(event);
        if(keyEvent->matches(QKeySequence::Undo)){
            if(_uStack->canUndo())
                _uStack->undo();
            return true;
        }
        if(keyEvent->matches(QKeySequence::Redo)){
            if(_uStack->canRedo())
                _uStack->redo();
            return true;
        }
    }

    return false;
    
}

// Getter methods
const QSharedPointer<model::AbstractXYDataFileModel> mainWindow::getIRC() const Q_DECL_NOEXCEPT{
    return _IRC;
}

const QSharedPointer<model::AbstractXYDataFileModel> mainWindow::getpIRC() const Q_DECL_NOEXCEPT{
    return _pIRC;
}

const QSharedPointer<model::AbstractXYDataFileModel> mainWindow::getCorrIRC() const Q_DECL_NOEXCEPT{
    return _corIRC;
}

const QString& WKB::mainWindow::documentName() const {
	return _documentName;
}


// Setter methods
void mainWindow::setIRC(QSharedPointer<model::AbstractXYDataFileModel> data,
                        bool doNotWriteToDocument) Q_DECL_NOEXCEPT{

    if(!data){
        _IRC.reset(Q_NULLPTR);
        _IRCinter.reset(Q_NULLPTR);
        emit(IRCDataChanged(_IRC));
        emit(IRCInterChanged(_IRCinter));
        computeCorIRC();
        return;
    }

    _IRC.reset(data->copy());
    _IRC->autoCorrectData();

    emit(IRCDataChanged(_IRC));
    if (_IRCinter == Q_NULLPTR)
        _IRCinter.reset(new model::spline1dALG(_IRC));
    else
        _IRCinter->setData(_IRC);

    emit(IRCInterChanged(_IRCinter));
    computeCorIRC();
    if(!doNotWriteToDocument){
        writeDataToDocument(_IRC.data(), "IRC");
        this->setWindowModified(true);
    }

}

void mainWindow::setpIRC(QSharedPointer<model::AbstractXYDataFileModel> data,
                         bool doNotWriteToDocument) Q_DECL_NOEXCEPT{
    if(!data){
        _pIRC.reset(Q_NULLPTR);
        _pIRCinter.reset(Q_NULLPTR);
        emit(IRCpDataChanged(_pIRC));
        emit(pIRCInterChanged(_pIRCinter));
        computeCorIRC();
        return;
    }

    _pIRC.reset(data->copy());
    _pIRC->autoCorrectData();

    emit(IRCpDataChanged(_pIRC));
    if (_pIRCinter == Q_NULLPTR)
        _pIRCinter.reset(new model::spline1dALG(_pIRC));
    else
        _pIRCinter->setData(_pIRC);


    emit(pIRCInterChanged(_pIRCinter));
    computeCorIRC();
    if(!doNotWriteToDocument){
        writeDataToDocument(data.data(), "IRC Projected");
        this->setWindowModified(true);
    }
}

void mainWindow::setCorrIRC(QSharedPointer<model::AbstractXYDataFileModel> data) Q_DECL_NOEXCEPT{
    if(!data){
        _corIRC.reset(Q_NULLPTR);
        _corIRCinter.reset(Q_NULLPTR);
        emit(IRCcorrDataChanged(_corIRC));
        emit(corrIRCInterChanged(_corIRCinter));
        return;
    }

    _corIRC.reset(data->copy());

    emit(IRCcorrDataChanged(_corIRC));

    if (_corIRCinter == Q_NULLPTR)
        _corIRCinter.reset(new model::spline1dALG(_corIRC));
    else
        _corIRCinter->setData(_corIRC);

    emit(corrIRCInterChanged(_corIRCinter));



}

void WKB::mainWindow::setDocumentName(const QString& documentName) {

	QString newDocumentName = documentName;
	// If the document name exists append documentNumber
	if (doesDocumentNameExistsApplicationWide(documentName))
		newDocumentName.append(
				QString("_") + QString::number(documentNumber++));

	_documentName = newDocumentName;
	emit(documentNameChanged(_documentName));
}

// Menu slot implementation
void mainWindow::actionTriggered(QAction* a) Q_DECL_NOEXCEPT{
    // Only react if active window
    if(!this->isActiveWindow())
        return;

	// On linux it happened that the action text was prefixed with an &
	// So & is removed are taken

	QString actionText = a->text().remove("&");

	if (actionText == common::fileMenuItemNames["OpenIRC"])
        this->openEvent(plot::IRC);
	else if (actionText == common::fileMenuItemNames["OpenPIRC"])
        this->openEvent(plot::IRCp);
	else if (actionText == common::fileMenuItemNames["Save"])
        this->saveToFile();
	else if (actionText == common::viewMenuItemNames["ShowParam"]) {
        QDockWidget *pW=this->findChild<QDockWidget*>("ParamDockWidget");
        Q_CHECK_PTR(pW);
        pW->show();
    }
    else if (actionText == common::viewMenuItemNames["ShowRes"]) {
        QDockWidget *resW=this->findChild<QDockWidget*>("ResultDockWidget");
        Q_CHECK_PTR(resW);
        resW->show();
    }
    else
        return;
}

void mainWindow::openEvent(mainWindow::plot which,QString *filename) {
    QString file;
    // First grab the filename
    // If no filename set bring up file dialog
    if(filename == Q_NULLPTR){
        QSettings s;
        QString oldPath = s.value("wkb/lastFile").toString();
        if (oldPath.isEmpty())
            oldPath = "~/";
        file = QFileDialog::getOpenFileName(this,
                                            tr("select data"), oldPath,
                                            tr("TSV files (*.tsv *.txt)"));
        if (file.isEmpty())
            return;

        s.setValue("wkb/lastFile", file);

    } else
        file=*filename;

    // Now get the data from file
    QSharedPointer<model::AbstractXYDataFileModel> data;
    
    data.reset(new WKB::model::TSVData(file));
	if (data->isEmpty())
		return;
    switch (which) {
        case plot::IRC: {
            QUndoCommand *cmd=new widgets::addData(this,data,
                                                   widgets::addData::IRC);
            _uStack->push(cmd);
		// If the projected IRC is empty set it to 0
		bool empty;
		if (_pIRC.isNull())
			empty = true;
		else if (_pIRC->isEmpty())
			empty = true;
		else
			empty = false;

		if (empty) {
			double xmin = _IRC->xmin(), xmax = _IRC->xmax();
			XYdata data = { QPointF(xmin, 0.0), QPointF(0.0, 0.0), QPointF(xmax,
					0.0) };
			QSharedPointer<model::AbstractXYDataFileModel> ircPData;
			ircPData.reset(new model::TSVData());
			ircPData->setData(data);
			this->setpIRC(ircPData, true);
		}
        }
            break;

        case plot::IRCp:{
            QUndoCommand* cmd=new widgets::addData(this,data,
                                                   widgets::addData::IRCpro);
            _uStack->push(cmd);
        }
            break;
        default:
            break;
    }
}

void mainWindow::computeCorIRC() Q_DECL_NOEXCEPT{
    if (_IRCinter == Q_NULLPTR || _pIRCinter == Q_NULLPTR){
        QSharedPointer<model::AbstractXYDataFileModel> p(Q_NULLPTR);
        setCorrIRC(p);
        return;
    }

    XYdata dIRCcor, dIRC, dIRCp;
    dIRC = _IRC->getData();
    dIRCp = _pIRC->getData();


    if (this->_corIRC == Q_NULLPTR)
        _corIRC.reset(new model::TSVData);

    //Compute corrected IRC
    for (auto &xy : dIRC) {
        QPointF n = xy;
        double x = xy.x();
        n.setY(_IRCinter->interpolate(x) + _pIRCinter->interpolate(x));
        dIRCcor.push_back(n);
    }
    _corIRC->setData(dIRCcor);

    emit(IRCcorrDataChanged(_corIRC));

    if (_corIRCinter == Q_NULLPTR)
        _corIRCinter.reset(new model::spline1dALG(_corIRC));
    else
        _corIRCinter->setData(_corIRC);

    emit(corrIRCInterChanged(_corIRCinter));
}

bool mainWindow::writeDataToDocument(model::AbstractXYDataFileModel* m, QString name) Q_DECL_NOEXCEPT{
    if(m == Q_NULLPTR)
        return false;


    QJsonObject pointData, type=_jOb->take("data").toObject(QJsonObject());

    // We need to generate a QVariant List with points
    QVariantList xL,yL;
    for(auto &point : m->getData()){
        xL.append(point.x());
        yL.append(point.y());
    }
    pointData["X"]=QJsonArray::fromVariantList(xL);
    pointData["Y"]=QJsonArray::fromVariantList(yL);

    type[name]=pointData;
    (*_jOb)["data"]=type;

    return true;
}

QSharedPointer<model::AbstractXYDataFileModel> mainWindow::readDataFromDocument(QString name) Q_DECL_NOEXCEPT{


    if(!_jOb->contains("data"))
        return QSharedPointer<model::AbstractXYDataFileModel>(Q_NULLPTR);


    QJsonObject data=(*_jOb)["data"].toObject();

    if(!data.contains(name))
        return QSharedPointer<model::AbstractXYDataFileModel>(Q_NULLPTR);

    QSharedPointer<model::AbstractXYDataFileModel> model;
    model.reset(new model::TSVData);

    data=data[name].toObject();
    QVariantList xValues=data["X"].toArray().toVariantList(),
    yValues=data["Y"].toArray().toVariantList();

    // Check if length of the two arrays are the same
    if(xValues.length() != yValues.length())
        return QSharedPointer<model::AbstractXYDataFileModel>(Q_NULLPTR);

    auto xIt=xValues.begin(),yIt=yValues.begin();
    XYdata xyData;
    for(; xIt != xValues.end(); xIt++, yIt++){
        QPointF point(xIt->toFloat(),yIt->toFloat());
        xyData.append(point);
    }
    model->setData(xyData);
    return model;

}

bool mainWindow::saveToFile(){
	QFile saveFile(retrieveFileNameForSaving());
    if(!saveFile.open(QIODevice::WriteOnly)){
        qWarning("Could't open save file.");
        return false;
    }

    QJsonDocument document;
    document.setObject(*_jOb);
    saveFile.write(document.toJson());

	this->setWindowModified(false);
	this->setDocumentName(saveFile.fileName());

    return true;
}

bool mainWindow::loadFromFile(const QString filename) Q_DECL_NOEXCEPT{
	QSettings s;
	QString oldPath = s.value("wkb/lastFile").toString();
	QString caption = tr("Open Document");
    oldPath=oldPath.isEmpty() ? QDir::homePath() : oldPath;
    QFile loadFile;
    if(!filename.isEmpty())
        loadFile.setFileName(filename);
    else
        loadFile.setFileName(QFileDialog::getOpenFileName(this,
                                                          caption,
                                                          oldPath,
                                                          tr("Json Document (*.json)")));
	if (!loadFile.exists())
		return false;

    if(!loadFile.open(QIODevice::ReadOnly)){
        qWarning("Could't open load file.");
        return false;
    }

	if (doesDocumentNameExistsApplicationWide(loadFile.fileName())) {
		QMessageBox msg(this);
		msg.setModal(true);
		msg.addButton(QMessageBox::Ok);
		msg.setIcon(QMessageBox::Warning);
		msg.setText("Warning");
		msg.setInformativeText(tr("Document already open!"));
		msg.exec();
		return false;
	}

    s.setValue("wkb/lastFile", loadFile.fileName());

    QByteArray saveData= loadFile.readAll();

    QJsonDocument loadDocument(QJsonDocument::fromJson(saveData));
    delete _jOb;
    _jOb=new QJsonObject(loadDocument.object());

	// The parameter widget
	widgets::ParamWidget *pW = this->findChild<widgets::ParamWidget*>(
			"WKB_Parameters");

	if (pW == Q_NULLPTR)
		return false;

	pW->setUserInputToTrue();
    // We do not want the document to be modified
    setIRC(readDataFromDocument("IRC"),true);
    setpIRC(readDataFromDocument("IRC Projected"),true);

	// For the parameters
	pW->setstartingMaterialEnergy(readParameterFromJson("startingMaterial Energy"));
	pW->setstartingMaterialZPVE(readParameterFromJson("startingMaterial ZPVE"));
	pW->setReactiveMode(readParameterFromJson("Reactive Mode"));

	this->setDocumentName(loadFile.fileName());
	this->setWindowModified(false);

    return true;
}

bool mainWindow::writeParameterToJson(QString name,const QVariant &value) Q_DECL_NOEXCEPT{
    if(!value.isValid())
        return false;

    QJsonObject parameter=_jOb->take("parameters").toObject();
    // The direct QVariant function does not work
    parameter.insert(name,QJsonValue(value.toString()));
    _jOb->insert("parameters", parameter);
    return true;
}

QVariant mainWindow::readParameterFromJson(QString name) Q_DECL_NOEXCEPT{
    if(!_jOb->contains("parameters"))
        return QVariant(0.0f);

    QJsonObject parameters=_jOb->value("parameters").toObject();

    if (!parameters.contains(name))
        return QVariant(0.0f);

    return QVariant(parameters[name].toVariant());
}

void mainWindow::closeEvent(QCloseEvent* event) Q_DECL_NOEXCEPT{
    if(isWindowModified() && !isHidden())   
        saveToFile();
    
    event->accept();
}

// Redo undo stack impl
void mainWindow::setUndoStack(QUndoStack *stack) Q_DECL_NOEXCEPT{
    Q_CHECK_PTR(stack);
    this->_uStack=stack;
}

QUndoStack* mainWindow::undoStack() const Q_DECL_NOEXCEPT{
    return _uStack;
}

void mainWindow::focusInEvent(QFocusEvent* event) Q_DECL_NOEXCEPT {
    if(event->reason() == Qt::ActiveWindowFocusReason)
        emit(getFocus());
    else
        emit(lostFocus());


    QMainWindow::focusInEvent(event);
}

void mainWindow::IRCInterpolDataHasChanged(
		QSharedPointer<model::AbstractInterpol> data) {
	// The parameter widget
	widgets::ParamWidget *pW = this->findChild<widgets::ParamWidget*>(
			"WKB_Parameters");
	if (pW) {
		if (!pW->didUserInputStartingMaterialEnergy()) {
			// Bug when user does undo the data is empty so check it first
			double yMin = 0.0;
			if (data)
				yMin = data->interpolate(data->xmin());
			pW->setstartingMaterialEnergy(QVariant(yMin));
		}
	}

}

void mainWindow::IRCpInterpolDataHasChanged(
		QSharedPointer<model::AbstractInterpol> data) {
	// The parameter widget
	widgets::ParamWidget *pW = this->findChild<widgets::ParamWidget*>(
			"WKB_Parameters");
	if (pW) {
		if (!pW->didUserInputStartingMaterialZPVE()) {
			double ymin = 0.0;
			if (data)
				ymin = data->interpolate(data->xmin());
			pW->setstartingMaterialZPVE(QVariant(ymin));
		}
	}

}

bool mainWindow::doesDocumentNameExistsApplicationWide(
		const QString& documentName) const {
	// Get all main windows in current application
	for (QWidget *widget : QApplication::allWidgets()) {
		if (widget == this)
			continue;

		QVariant documentNameOfWidget = widget->property("documentName");
		if (documentNameOfWidget.isValid()) {
			QString documentNameOfWidgetString =
					documentNameOfWidget.toString();
			if (documentNameOfWidgetString == documentName)
				return true;
		}
	}
	return false;
}

void mainWindow::undoIndexHasChanged(int idx) {
	this->setWindowModified(true);
}

void mainWindow::documentNameHasChanged(const QString &documentName) {
	// If the document Name is a file path only display filename
	QString windowTitle(documentName);
	if (documentName.contains(QDir::separator())) {
		auto filenameComponents = documentName.split(QDir::separator());
		if (!doesDocumentNameExistsApplicationWide(filenameComponents.last()))
			windowTitle = filenameComponents.last();
	}

	// If the document name is not unique display whole filename
	this->setWindowTitle(windowTitle);
}

const QString mainWindow::retrieveFileNameForSaving() {


	QString fileName = "";
	QSettings s;
	// Retrieve filename from document name
	if (QFile::exists(_documentName))
		fileName = _documentName;
	// Retrieve filename from file dialog
	else {
		QString oldPath = s.value("wkb/lastFile").toString();
		QString caption = tr("Save ") + this->windowTitle();
		fileName = QFileDialog::getSaveFileName(this, caption, oldPath,
			tr("Json Document (*.json)"));
	}

	if (!fileName.isEmpty())
		s.setValue("wkb/lastFile", fileName);

	return fileName;
                                             
}

