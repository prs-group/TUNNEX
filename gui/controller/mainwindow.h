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

#ifndef mainwindow_hpp
#define mainwindow_hpp

#include <QMainWindow>


namespace WKB {
namespace widgets {
class CentralWidget;
class AppMenu;
}
namespace model {
class AbstractXYDataFileModel;
class AbstractInterpol;
class WKBModel;

}


}



QT_BEGIN_NAMESPACE
class QDockWidget;
class QUndoStack;
QT_END_NAMESPACE

namespace WKB {
namespace widgets {
class AppMenu;
}
class mainWindow: public QMainWindow {
Q_OBJECT
	// Qt Property does not work with abstract types so a shared pointer is used
	Q_PROPERTY(QSharedPointer<WKB::model::AbstractXYDataFileModel> IRCData
			READ getIRC
			WRITE setIRC
			NOTIFY IRCDataChanged)
	Q_PROPERTY(QSharedPointer<WKB::model::AbstractXYDataFileModel> IRCprojectedData
			READ getpIRC
			WRITE setpIRC
			NOTIFY IRCpDataChanged)
	Q_PROPERTY(QSharedPointer<WKB::model::AbstractXYDataFileModel> IRCcorrectedData
			READ getCorrIRC
			WRITE setCorrIRC
			NOTIFY IRCpDataChanged)
	Q_PROPERTY(QString documentName
			READ documentName
			WRITE setDocumentName
			NOTIFY documentNameChanged)
	//@{
	//! The data of this session
	QSharedPointer<model::AbstractXYDataFileModel> _IRC, _pIRC, _corIRC;
	QSharedPointer<model::AbstractInterpol> _IRCinter, _pIRCinter, _corIRCinter;
	//@}

	//! The WKB model to compute tunneling half-lives.
	//! Receives changed from if graphs or parameter changes.
	model::WKBModel *_wkb;
	//! The document name of this session. Usually set to the
	//! filename of the open JSON document
	QString _documentName;

	//! Setup all the widgets to work together
	void setupWidgets() Q_DECL_NOEXCEPT;

	// TODO: May need to move some code to the widget themselves
	//! Setup the connection between the widgets which
	//! gets rather complicated for more widgets. May
	void setupConnections() Q_DECL_NOEXCEPT;

	/* TODO: Move this to CentralWidget class and register it
	 through Qt Property System */
	//! Is used to determine the plot of the central widget
	enum plot {
		IRC, IRCp, IRCcor
	};

	// TODO: Move to central widget
	//! Handles the acquisition of the data and loads them
	//! into the appropriate plot.
	void openEvent(plot which, QString* filename = Q_NULLPTR);

	//! To count the number of documents open
	static int documentNumber;

	//! The document object to save the data.
	//! Is updated immediately if a parameter changes.
	QJsonObject* _jOb;

	//! Flag to remember if the projected IRC has been set by the user
	bool _hasProjectedIRCBeenSetByUser;

	//! For saving data plot from JSON file
	bool writeDataToDocument(model::AbstractXYDataFileModel* m, QString name)
			Q_DECL_NOEXCEPT;

	//! For reading the data plot from JSON file
	QSharedPointer<model::AbstractXYDataFileModel> readDataFromDocument(
			QString name) Q_DECL_NOEXCEPT;

	//! Writing parameter to file
	bool writeParameterToJson(QString name, const QVariant &value)
			Q_DECL_NOEXCEPT;

	//! Reading parameter from file
	QVariant readParameterFromJson(QString name) Q_DECL_NOEXCEPT;

	//! Computes the ZPVE corrected IRC and sets
	//! the interpolation. Emits signals \sa{mainWindow::corrIRCInterChanged} and
	//! \sa{mainWindow::IRCcorrDataChanged}
	void computeCorIRC() Q_DECL_NOEXCEPT;

	//! Undo Stack for redo/undo
	QUndoStack *_uStack;
	//! This vector saves the current text from parameter line edits
	QVector<QString> _parameters;

	bool doesDocumentNameExistsApplicationWide(
			const QString &documentName) const;

	/** Helper function to retrieve filename for saving.
	// If the _documentName is a filename no user interaction
	// is needed. Otherwise this will spawn a QFilenameDialog.
	 *
	 */
	const QString retrieveFileNameForSaving();

	// On non OS X systems each main window has its own menubar
#ifndef Q_OS_MACX
	widgets::AppMenu *_menu;
#endif

protected:
	//! Need to know when active in order to set the right undo stack.
	void focusInEvent(QFocusEvent* event) Q_DECL_NOEXCEPT Q_DECL_OVERRIDE;

public:
	/**
	 * Initialized the main window
	 * @param parent Parent of the main window
	 */
	mainWindow(QUndoStack *stack, QWidget* parent = Q_NULLPTR) Q_DECL_NOEXCEPT;
	~mainWindow() Q_DECL_NOEXCEPT;

	//! Not needed right now, but later when the user can
	//! change between different interpolation types thanks
	//! to the abstract interface used in this class
	enum interpolType {
		alg1d, gsl
	};

	Q_ENUM(interpolType)
	//! The grabber method for the IRC data
	const QSharedPointer<model::AbstractXYDataFileModel> getIRC() const
			Q_DECL_NOEXCEPT;
	//! The grabber method for the projected IRC data
	const QSharedPointer<model::AbstractXYDataFileModel> getpIRC() const
			Q_DECL_NOEXCEPT;
	//! The grabber method for the corrected IRC data
	const QSharedPointer<model::AbstractXYDataFileModel> getCorrIRC() const
			Q_DECL_NOEXCEPT;

	//! Returns the document Name of the current window
	const QString& documentName() const;

	/** The setter methods for the IRC data
	 * The IRC interpolation is also set and the signals emitted
	 */

	void setIRC(QSharedPointer<model::AbstractXYDataFileModel> data,
			bool doNotWriteToDocument = false) Q_DECL_NOEXCEPT;
	/** The setter method for the projected IRC data.
	 * The projected IRC interpolation is also set and the signals emitted
	 */
	void setpIRC(QSharedPointer<model::AbstractXYDataFileModel> data,
			bool doNotWriteToDocument = false) Q_DECL_NOEXCEPT;
	/** The setter method for the corrected IRC data
	 * The corrected IRC interpolation is also set and the signals emitted.
	 */
	void setCorrIRC(QSharedPointer<model::AbstractXYDataFileModel> data)
			Q_DECL_NOEXCEPT;

	//! Sets the document Name of the current window
	void setDocumentName(const QString& documentName);

	//! eventFilter for override redo/undo
	bool eventFilter(QObject* watched, QEvent *event) Q_DECL_OVERRIDE;

	signals:
	/** The notifier of the IRC data
	 * Is emitted whenever the IRC data changes
	 */
	void IRCDataChanged(
			QSharedPointer<WKB::model::AbstractXYDataFileModel> data);
	/** The notifier of the projected IRC data
	 * Is emitted whenever the projected IRC data changes
	 */
	void IRCpDataChanged(
			QSharedPointer<WKB::model::AbstractXYDataFileModel> data);
	/** The notifier of the corrected IRC data
	 * Is emitted whenever the corrected IRC data changes
	 */
	void IRCcorrDataChanged(
			QSharedPointer<WKB::model::AbstractXYDataFileModel> data);
	/** The notifier of the IRC interpolation
	 * Is emitted whenever the IRC interpolation changes
	 */
	void IRCInterChanged(QSharedPointer<model::AbstractInterpol> inter);
	/** The notifier of the projected IRC interpolation
	 * Is emitted whenever the projected IRC interpolation changes
	 */
	void pIRCInterChanged(QSharedPointer<model::AbstractInterpol> inter);
	/** The notifier of the corrected IRC interpolation
	 * Is emitted whenever the corrected IRC interpolation changes
	 */
	void corrIRCInterChanged(QSharedPointer<model::AbstractInterpol> inter);

	//! Is emitted if the document name has changed
	void documentNameChanged(const QString &newDocumentName);

	/** Signal emitted when this window gets focus.
	 * Necessary for MacOS clients with unified menu bar.
	 * The menu bar needs to know this in order to set
	 * the correct undo stack
	 */

	void getFocus();
	/** Signal is emitted when this window looses focus.
	 * Necessary for MacOS clients with unified menu bar.
	 * The menu bar needs to know this in order to set
	 * the correct undo stack
	 */
	void lostFocus();

	public slots:
	/** Retrieves the action form the menu
	 *
	 */
	void actionTriggered(QAction* action) Q_DECL_NOEXCEPT;

	/**  Saves the data to a Document
	 *	Uses the JSON document of the current window to
	 *	generate a file. If the window name is a filename
	 *	this file file be taken. Otherwise the user is
	 *	prompted with a QFileDialog to choose the filename
	 */
	bool saveToFile();
	/** Loads the data from a JSON file
	 *
	 */
	bool loadFromFile(const QString filename = "") Q_DECL_NOEXCEPT;

	//! Reimplement closeEvent for saving
	void closeEvent(QCloseEvent* event) Q_DECL_NOEXCEPT Q_DECL_OVERRIDE;

	//! Function for the redo/undo slot
	void setUndoStack(QUndoStack *stack) Q_DECL_NOEXCEPT;
	//! Function for the redo/undo slot
	QUndoStack *undoStack() const Q_DECL_NOEXCEPT;

	//! This function handle the change of the IRC data. Set default
	//! values for starting material energy.
	void IRCInterpolDataHasChanged(
			QSharedPointer<model::AbstractInterpol> data);
	//! This function handle the change of the projected IRC data. and set default
	//! values for starting material ZPVE.
	void IRCpInterpolDataHasChanged(
			QSharedPointer<model::AbstractInterpol> data);

	//! Retrieves the document name changed event
	void documentNameHasChanged(const QString &documentName);

	//! This slots receives teh QUndoStack::indexChanged signal
	//! in order to determine if the document has been modified
	void undoIndexHasChanged(int idx);

	// TODO: Fix this workaround. Friends for the commands
	friend class addData;

};
}

#endif /* mainwindow_hpp */
