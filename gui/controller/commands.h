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

#ifndef ADDDATA_H
#define ADDDATA_H

#include <QUndoCommand>
#include <QSharedPointer>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QSpinBox;
QT_END_NAMESPACE

// Forward declaration
namespace WKB{
class mainWindow;
namespace model {
class AbstractXYDataFileModel;
}
}

namespace WKB {
namespace widgets {
class addData: public QUndoCommand {

public:
	enum type {
		IRC, IRCpro
	};
	addData(mainWindow *w, QSharedPointer<model::AbstractXYDataFileModel> data,
			type t, QUndoCommand *parent = Q_NULLPTR);
	~addData();

	void redo() Q_DECL_OVERRIDE;
	void undo() Q_DECL_OVERRIDE;

private:
	mainWindow *_w;
	QSharedPointer<model::AbstractXYDataFileModel> _data, _oldData;
	void setData(type t, QSharedPointer<model::AbstractXYDataFileModel> data);
	QSharedPointer<model::AbstractXYDataFileModel> getData(type t);
	const QString undoName();
	type _type;
};
//addData


class undoLineEditCmd: public QUndoCommand {
	QLineEdit *_edit;
	QString _oldText, _newText;
public:
	undoLineEditCmd(QLineEdit *edit, const QString &oldText,
			const QString &newText, QUndoCommand* parent = Q_NULLPTR);
	void undo() Q_DECL_OVERRIDE;
	void redo() Q_DECL_OVERRIDE;
	bool mergeWith(const QUndoCommand* other) Q_DECL_OVERRIDE;
	int id() const Q_DECL_OVERRIDE;
};
// undoLineEditCmd

class undoQSpinBoxCmd: public QUndoCommand {
	QSpinBox* _spinBox;
	int _oldNumber, _newNumber;
public:
	undoQSpinBoxCmd(QSpinBox* spinBox,
			const int &oldValue,
			const int &newValue,
			QUndoCommand* parent = Q_NULLPTR);
	void undo() Q_DECL_OVERRIDE;
	void redo() Q_DECL_OVERRIDE;
	bool mergeWith(const QUndoCommand* other) Q_DECL_OVERRIDE;
	int id() const Q_DECL_OVERRIDE;

};

} //widgets
}// WKB


#endif // ADDDATA_H
