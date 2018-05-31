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

#include "controller/commands.h"
#include <QLineEdit>
#include <QEvent>
#include <QKeyEvent>
#include <QApplication>
#include <QUndoGroup>
#include <QSettings>
#include <QSpinBox>
#include "controller/mainwindow.h"
#include "model/AbstractXYDataFileModel.h"
#include "controller/AppMenu.h"

/* ***********************************************************************
 Implementation of addData
 *************************************************************************/
using namespace WKB::widgets;
using namespace WKB::model;

WKB::widgets::addData::addData(WKB::mainWindow *w,QSharedPointer<WKB::model::AbstractXYDataFileModel> data, type t,
                 QUndoCommand *parent):
QUndoCommand(parent)
{
    _w=w;
    _type=t;

    _oldData=getData(t);
    _data=data;
    setText(undoName());
}

WKB::widgets::addData::~addData(){
    _data.reset();
    _oldData.reset();
}

void WKB::widgets::addData::redo(){
    setData(_type, _data);
}

void addData::undo(){
    setData(_type, _oldData);
}

QSharedPointer<AbstractXYDataFileModel> addData::getData(type t){
    switch (t) {
        case type::IRC :
            return _w->getIRC();
            break;

        case type::IRCpro:
            return _w->getpIRC();
            break;

        default:
            break;

    }

    return QSharedPointer<AbstractXYDataFileModel>(Q_NULLPTR);
}

void addData::setData(type t, QSharedPointer<WKB::model::AbstractXYDataFileModel> data ){
    switch (t) {
        case type::IRC:
            _w->setIRC(data);
            break;

        case type::IRCpro:
            _w->setpIRC(data);
            break;

        default:
            break;
            
    }
}

const QString addData::undoName(){
    switch (_type) {
        case type::IRC:
            return QString(QObject::tr("Add Data IRC"));
            break;

        case type::IRCpro:
            return QString(QObject::tr("Add Data IRC Projected"));
            break;

        default:
            break;
    }
}

/* ***********************************************************************
 Implementation of undoLineEditCmd
 *************************************************************************/


undoLineEditCmd::undoLineEditCmd(QLineEdit* edit,
                                 const QString &oldText,
                                 const QString &newText,
                                 QUndoCommand* parent):
QUndoCommand(parent),_edit(edit),_oldText(oldText),_newText(newText){
    this->setText(_edit->objectName());

}

void undoLineEditCmd::undo(){
    _edit->setFocus(Qt::ShortcutFocusReason);
    _edit->setText(_oldText);
}

void undoLineEditCmd::redo(){
    _edit->setFocus(Qt::ShortcutFocusReason);
    _edit->setText(_newText);
}

bool undoLineEditCmd::mergeWith(const QUndoCommand* other){
    if (other->id() != this->id())
        return false;

    const undoLineEditCmd *cmd=static_cast<const undoLineEditCmd*>(other);
    if(cmd->_edit != this->_edit)
        return false;

    this->_newText=cmd->_newText;
    return true;
}

int undoLineEditCmd::id() const{
    return 20;
}

// Implementation of undoQSpinBoxCmd
WKB::widgets::undoQSpinBoxCmd::undoQSpinBoxCmd(QSpinBox* spinBox,
		const int &oldValue, const int &newValue, QUndoCommand* parent) :
		QUndoCommand(parent), _spinBox(spinBox), _oldNumber(oldValue), _newNumber(
				newValue) {
	this->setText(_spinBox->objectName());
}
void WKB::widgets::undoQSpinBoxCmd::undo() {
	_spinBox->setFocus(Qt::ShortcutFocusReason);
	_spinBox->setValue(_oldNumber);
	_spinBox->update();
}

void WKB::widgets::undoQSpinBoxCmd::redo() {
	_spinBox->setFocus(Qt::ShortcutFocusReason);
	_spinBox->setValue(_newNumber);
	_spinBox->update();
}

bool WKB::widgets::undoQSpinBoxCmd::mergeWith(const QUndoCommand* other) {
	if (other->id() != this->id())
		return false;

	const undoQSpinBoxCmd *cmd = static_cast<const undoQSpinBoxCmd*>(other);
	if (cmd->_spinBox != this->_spinBox)
		return false;

	this->_newNumber = cmd->_newNumber;

	return true;
		
}

int WKB::widgets::undoQSpinBoxCmd::id() const {
	return 21;
}
