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

#ifndef AppMenu_hpp
#define AppMenu_hpp

#include <QMenuBar>
#include <QHash>

QT_BEGIN_NAMESPACE
class QUndoGroup;
class QUndoView;
class QDockWidget;
class QAction;
QT_END_NAMESPACE

namespace WKB {

class mainWindow;

namespace common {
/*!
 * \ingroup types
 * \brief Type for holding the menu item names information
 * \details Key: name id of menu item; Value: String shown to user
 */

using menuHash=QHash<QString, QString>;

//! \defgroup globals
//! @{
/*!
 * \var fileMenuItemNames
 * \brief maps functional name of the File items to name shown to the user

 * \var edit MenuItemNames
 * \brief maps functional name of the edit menu items to name shown to the user

 * \var viewMenuItemNames
 * \brief maps functional name of the view menu items to name shown to the user

 */

extern menuHash fileMenuItemNames;
extern menuHash editMenuItemNames;
extern menuHash viewMenuItemNames;
//! @}
}

namespace widgets {

class AppMenu: public QMenuBar {
Q_OBJECT
	///! @{
	/// The respective menus for each tab
	QMenu *_file, *_edit, *_view, *_help;
	//! @}
#ifdef Q_OS_MACX
	static QUndoGroup *_uStack;
#endif

	//! Cache of the undo/redo Action
	QAction *_undo, *_redo;

	void connectMainWindow(mainWindow* w);

	mainWindow* newMainWindow();


	public slots:
	//! Slot is called when "New" (Hashes) is selected
	void newWindowHandler();
	//! Slot is called when "Open" (Hashes) is selected
	void openDocumentHandler();
	//! Slot is called when "Close Window" (Hashes) is executed
	void closeWindowHandler();
	//! Slot is called when about qt action is asked
	void aboutQt();
	//! Slot is called when about is asked
	void about();


	public:
	//! Basic Constructor
	/*!
	 * Constructor of class AppMenu
	 * @param parent the parent widget
	 */
	AppMenu(QWidget* parent = 0);

#ifdef Q_OS_MACX
	static const QUndoGroup* getUndoGroup();
#else
	QMenu *editMenu();
#endif


};
}
}
#endif /* AppMenu_hpp */
