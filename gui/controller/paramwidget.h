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

#ifndef PARAMWIDGET_H
#define PARAMWIDGET_H

#include <QDockWidget>
#include <QStyledItemDelegate>
#include "controller/mainwindow.h"

QT_BEGIN_NAMESPACE

class QHBoxLayout;

QT_END_NAMESPACE

class QTableView;
class Ui_paramWidget;

namespace WKB {
namespace widgets {

//TODO: Integrate QProperty system
class ParamWidget: public QWidget {
	Q_OBJECT
public:
	/**
	 * Standard Constructor
	 * @param parent The parent widget
	 */
	explicit ParamWidget(QUndoStack *_uStack,QWidget *parent = 0);
	/**
	 * Basic Deconstructor
	 * \details Deletes the reservered Table Widget
	 */
	~ParamWidget();

	//@{
	//! This sets the parameter as if the user
	//! did not enter the parameter by hand
    void setstartingMaterialEnergy(const QVariant &v);
    void setstartingMaterialZPVE(const QVariant &v);
    void setReactiveMode(const QVariant &v);
	void setSymmetryNumber(const QVariant &v);
	//@}

	const QVariant getStartingMaterialEnergy();
	const QVariant getStartingMaterialZPVE();
    const QVariant getReactiveMode();
	const QVariant getSymmetryNumber();

	bool didUserInputStartingMaterialEnergy();
	bool didUserInputStartingMaterialZPVE();
	bool didUserInputReactionMode();

public slots:
	//! Sets the parameter as if the user did
	//! enter the parameter by hand
	//! This used for example if the user opens
	//! a new document
	void startingMaterialEnergyChanged(const QVariant &p);
	void startingMaterialZPVEChanged(const QVariant &p);
	void reactionModeChanged(const QVariant &p);
	void symmetryNumberChanged(const int &p);

	signals:
	//@{
	//! Emitters for values
    void startingMaterialEnergyHasChanged(const QVariant &p);
    void startingMaterialZPVEHasChanged(const QVariant &p);
    void reactiveModeHasChanged(const QVariant &p);
	void symmetryNumberHasChanged(const QVariant &p);
    //@}

protected:


private:
    Ui_paramWidget* _ui;
    void setupConnections();
    QUndoStack *_uStack;
	bool _didUserInputStartingMaterialEnergy, _didUserInputStartingMaterialZPVE,
			_didUserInputReactionMode;

	// TODO: Fix this workaround which is used by mainWindows
	// openDocument function
	void setUserInputToTrue();

	friend class WKB::mainWindow;

};

}
}

#endif // PARAMWIDGET_H
