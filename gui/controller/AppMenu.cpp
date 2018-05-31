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

#include "controller/AppMenu.h"
#include "controller/CentralWidget.h"
#include "controller/mainwindow.h"
#include <QMenu>
#include <QApplication>
#include <QScreen>
#include <QDebug>
#include <QUndoStack>
#include <QUndoGroup>
#include <QUndoView>
#include <QDockWidget>
#include <QKeyEvent>
#include <QWindow>
#include <QMessageBox>


using namespace WKB;
using namespace widgets;
using namespace WKB::common;


namespace WKB{
namespace common {

/*!
 *   The keys in the hash can be the following:
 *         - "New" -> menu entry name for starting a new Session aka window
 *         - "OpenIRC" -> The menu entry name for opening an IRC data file
 *         - "OpenPIRC" -> The menu entry name for opening an projected IRC data file
 *         - "ComputeCIRC" -> The menu item name for computing the IRC
 */

menuHash fileMenuItemNames = { { "New", QObject::tr("New Document") }, {
		"OpenIRC", QObject::tr("Import IRC") }, { "OpenPIRC", QObject::tr(
		"Import projected IRC") }, { "Open", QObject::tr("Open Document") }, {
		"Save", QObject::tr("Save") }, { "CloseWindow", QObject::tr(
		"Close Window") }
	}
	;
/*!
 * The keys in the hash can be the following:
 *        - "EditpIRC" -> The menu entry name for the editing the projected IRC
 *        - "Copy" -> The menu item name for the copy action
 */
menuHash editMenuItemNames = { { "Redo", QObject::tr("&Redo") }, { "Undo",
		QObject::tr("&Undo") }, { "Copy", QObject::tr("&Copy") } };



/*!
 * The keys in the hash can be the following:
 *        - "AxisIRC" -> menu item name for showing the IRC axis control to user
 *        - "AxisPIRC" -> menu item name for showing the projected IRC axis control to user
 *        - "AxisCIRC" -> menu item name for showing the corrected IRC axis control to user
 */
menuHash viewMenuItemNames = { { "ShowParam", QObject::tr("Show Parameters") },
		{ "ShowRes", QObject::tr("Show Results") } };

// Contains the shortcuts for the menues (needs to be same name as in maps)
// File Menu
QMap<QString, QKeySequence> FileShortCuts = {
	{	fileMenuItemNames["New"], QKeySequence("Ctrl+n")},
	{	fileMenuItemNames["OpenIRC"], QKeySequence("Ctrl+i")},
	{	fileMenuItemNames["OpenPIRC"], QKeySequence("Ctrl+f")},
	{	fileMenuItemNames["Open"], QKeySequence("Ctrl+o")},
	{	fileMenuItemNames["Save"], QKeySequence("Ctrl+s")},
	{
				fileMenuItemNames["CloseWindow"], QKeySequence("Ctrl+w") }
	}
	;

}
}

#ifdef Q_OS_MACX
QUndoGroup *AppMenu::_uStack=new QUndoGroup;
#endif

AppMenu::AppMenu(QWidget* parent) :
		QMenuBar(parent), _file(new QMenu(tr("&File"), this)), _edit(
				new QMenu(tr("&Edit"), this)), _view(
				new QMenu(tr("&Windows"), this)), _help(
				new QMenu(tr("&Help"), this))
{
	this->addMenu(_file);
	this->addMenu(_edit);
	this->addMenu(_view);
	this->addMenu(_help);

	_file->addAction(common::fileMenuItemNames["New"], this,
			SLOT(newWindowHandler()));
	_file->addAction(common::fileMenuItemNames["Open"], this,
			SLOT(openDocumentHandler()));
	_file->addAction(common::fileMenuItemNames["CloseWindow"], this,
			SLOT(closeWindowHandler()));
	_file->addAction(common::fileMenuItemNames["Save"]);
	_file->addAction(common::fileMenuItemNames["OpenIRC"]);
	_file->addAction(common::fileMenuItemNames["OpenPIRC"]);

	_view->addAction(common::viewMenuItemNames["ShowParam"]);
	_view->addAction(common::viewMenuItemNames["ShowRes"]);

	_help->addAction(QString(tr("About Qt")), this, SLOT(aboutQt()));
	_help->addAction(QString(tr("About")), this, SLOT(about()));

	// Add shortcuts file menu
	for (auto &action : _file->actions()) {
		if (common::FileShortCuts.contains(action->text())) {
			action->setShortcut(common::FileShortCuts[action->text()]);
		}
	}

	// Redo undo
	// On Mac the menubar holds the undo stack
	// On other systems each main window has its own menubar and undostack
#ifdef Q_OS_MACX
	_redo = _uStack->createRedoAction(this, common::editMenuItemNames["Redo"]);
	_redo->setShortcut(QKeySequence::Redo);

	QAction* _undo = _uStack->createUndoAction(this,
			common::editMenuItemNames["Undo"]);
	_undo->setShortcut(QKeySequence::Undo);

	_edit->addAction(_undo);
	_edit->addAction(_redo);
#endif

}

void AppMenu::newWindowHandler() {
	mainWindow *w = newMainWindow();
	w->show();
	w->setFocus();
}

void AppMenu::openDocumentHandler(){
	QWindow *oldWindow = QApplication::focusWindow();
	mainWindow* w = newMainWindow();
	
	if (!w->loadFromFile()) {
		delete w;
		if(oldWindow != Q_NULLPTR)
			oldWindow->setProperty("active", true);

		return;
	}
	w->show();
	w->setFocus();
}

void AppMenu::connectMainWindow(mainWindow *w){

	// This menu is informed when the main window gets focus
#ifdef Q_OS_MACX
	connect(w, &mainWindow::getFocus, this, [this,w]() {
		_uStack->setActiveStack(w->undoStack());
	});
#endif


	// Connect the action triggered slot to main window
	//connect(this, &AppMenu::triggered, w, &mainWindow::actionTriggered);

}

mainWindow* AppMenu::newMainWindow(){
	// If there is another mainWindow move new window a bit
	QWidget *active = QApplication::activeWindow();
	QPoint pos(0, 0);
	if (active != Q_NULLPTR)
		pos = active->pos();

	QUndoStack *stack = new QUndoStack(this);
#ifdef Q_OS_MACX
	_uStack->addStack(stack);
#endif
	mainWindow *w = new mainWindow(stack);
	w->hide();

// On non-macOS systems the main window handles the menu bar itself
#ifdef Q_OS_MACX
	connectMainWindow(w);
#endif
	w->move(pos.x() + 20, pos.y() + 20);
	return w;
}

#ifdef Q_OS_MACX
const QUndoGroup* AppMenu::getUndoGroup(){
	return _uStack;
}
#else
QMenu* AppMenu::editMenu() {
	return _edit;
	}
#endif

void AppMenu::closeWindowHandler() {
	QWidget *active = QApplication::activeWindow();
	if (active)
		active->close();
}

void AppMenu::aboutQt() {
	QMessageBox::aboutQt(0, tr("About Qt"));

}

void AppMenu::about() {
	QString message(tr("Tunnex Copyright (C) 2018 Henrik Quanz \n"));
	message.append(tr("This program comes with ABSOLUTELY NO WARRANTY; "));
	message.append(tr("This is free software, and you are welcome to "));
	message.append(tr("redistribute it under certain conditions;"));

	QMessageBox::about(0, tr("About TUNNEX"), message);

}





