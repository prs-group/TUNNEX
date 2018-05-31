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

#ifndef PLOTEDIT_H
#define PLOTEDIT_H

#include <QDialog>

namespace WKB {
namespace model {
class AbstractXYDataFileModel;
class TSVData;
}

namespace impldetail {
class PlotEditPrivate;
}
}

namespace WKB {
namespace widgets {
class PlotEdit: public QDialog {
	Q_OBJECT
	Q_PROPERTY(QRect geometry READ geometry WRITE setGeometry);

public:
	/**
	 * Reimplements QDialogs Constructor
	 * @param parent The parent widget
	 */
	explicit PlotEdit(QWidget *parent = 0);
	/**
	 * Basic deconstructor
	 */
	~PlotEdit();
	/**
	 * Sets the data for the plot
	 * @param data The data to be edited
	 */
	void setData(model::AbstractXYDataFileModel* data);
	/**
	 * Returns the edited data
	 * @return The edited data
	 */
	model::AbstractXYDataFileModel* getData();
protected:
	PlotEdit(impldetail::PlotEditPrivate &d, QWidget *parent = 0);

	signals:
	void dataChanged(model::AbstractXYDataFileModel*);protected slots:
	void aboutToClose(int);

private:
	Q_DECLARE_PRIVATE (WKB::impldetail::PlotEdit);
	impldetail::PlotEditPrivate* d_ptr;
};
}
}

#endif // PLOTEDIT_H
