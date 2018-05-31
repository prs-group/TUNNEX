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

#ifndef PARAMLISTMODEL_H
#define PARAMLISTMODEL_H

#include <QAbstractTableModel>

namespace WKB {
namespace common {
enum names {
	reactionCoordinate = 0, eductZPVE = 1, eductEnergy = 2, offset = 3
};
}
namespace model {
class ParamListModel: public QAbstractTableModel {
	Q_OBJECT
public:
	/**
	 * Overrides Standard constructor for QAbstractTableModel
	 * @details For details see <a href="http://doc.qt.io/qt-5/qabstracttablemodel.html#QAbstractTableModel">
	 * Qt Documentation </a>
	 * @param parent
	 */
	explicit ParamListModel(QObject *parent = 0);
	/**
	 * Basic deconstructor deletes private members
	 */
	~ParamListModel();
	/**
	 * Override of QAbstractItemModels member
	 * @details For detail see <a href="http://doc.qt.io/qt-5/qabstractitemmodel.html#columnCount">
	 * Qt Documentation </a>
	 * @param parent  Parent widget
	 * @return Number of Columns
	 */
	int columnCount(const QModelIndex &parent) const;
	/**
	 * Override of QAbstractItemModels member
	 * @details For detail see <a href="http://doc.qt.io/qt-5/qabstractitemmodel.html#rowCount">
	 * Qt Documentation </a>
	 * @param parent  Parent widget
	 * @return Number of Columns
	 */
	int rowCount(const QModelIndex &parent) const;
	/**
	 * Override of QAbstractItemModels member
	 * @details see <a href="http://doc.qt.io/qt-5/qabstractitemmodel.html#data">
	 * Qt Documentation </a>
	 * @param index Item referred by index
	 * @param role Data for role
	 * @return The data
	 */
	QVariant data(const QModelIndex &index, int role) const;
	/**
	 * Override of QAbstractItemModels member
	 * @details see <a href="http://doc.qt.io/qt-5/qabstractitemmodel.html#flags">
	 * Qt Documentation </a>
	 * @param index ItemFlag for given index
	 * @return Qt::ItemFlag for given index
	 */
	Qt::ItemFlags flags(const QModelIndex &index) const;
	/**
	 * Override of QAbstactItemModels member
	 * @details see <a href="http://doc.qt.io/qt-5/qabstractitemmodel.html#setData">
	 * Qt Documentation </a>
	 * @param index The index to set
	 * @param value value for index
	 * @param role role for index
	 * @return True if successful
	 */
	bool setData(const QModelIndex &index, const QVariant &value, int role =
			Qt::EditRole);
	/**
	 * Returns Parameter for given name
	 * @param name The name of the parameter
	 * @return The parameter or 0
	 */
	QVariant getParameter(common::names name);


private:

	QStringList* _names, *_parameters;


};
}
}

#endif // PARAMLISTMODEL_H
