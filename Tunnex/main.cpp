/* This file is part of TUNNEX.

 TUNNEX is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 laster any later version.

 Foobar is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with TUNNEX.  If not, see <http://www.gnu.org/licenses/>.

 TUNNEX 1.0.0 
 Copyright (c) Henrik Quanz
*/
#include <QScopedPointer>
#include <QApplication>
#include <QDebug>
#ifndef Q_OS_MACX
#include <QUndoStack>
#endif
#include "controller/mainwindow.h"
#include "controller/AppMenu.h"

int main(int argc, char *argv[]) {

    // For Debug Message
    qSetMessagePattern("[%{type}] - %{message}");
	QApplication *a = new QApplication(argc, argv);
	QCoreApplication::setOrganizationName("Justus-Liebig-University");
	short i = 1;
	for (auto &a : a->arguments())
		qDebug() << "Argument: " << i++ << a;

	QCoreApplication::setOrganizationDomain("uni-giessen.de");
	QCoreApplication::setApplicationName("TUNNEX");
#if QT_VERSION >= QT_VERSION_CHECK(5,6,0)
	QApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
#endif
#ifdef Q_OS_MACX
	WKB::widgets::AppMenu *menu=new WKB::widgets::AppMenu;
	menu->setObjectName("DefaultMenu");
	a->setQuitOnLastWindowClosed(false);
    menu->newWindowHandler();
#else
	WKB::mainWindow* mainWindow=new WKB::mainWindow(new QUndoStack);
	mainWindow->show();
#endif

	/* On OS X the default behavior for a multi window application
	 is a global menu bar shared by all window instances.
	 When the last window closes the app does not close.
	 On every other system the main window approach is the best,
	 because the app closes after the last main window quits.
	 */
    int res=a->exec();
    delete a;
	return res;
}

