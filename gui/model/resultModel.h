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

#ifndef resultModel_hpp
#define resultModel_hpp

#include <QAbstractTableModel>

namespace WKB {
namespace model {
class ResultModel: public QAbstractTableModel {
	Q_OBJECT
private:
	QList<QVariant*>* names;
	QList<QVariant*>* values;
	QList<QVariant*>* units;

public:
	explicit ResultModel(QObject* parent = 0);
	~ResultModel();
	int rowCount(const QModelIndex &parent) const;
	int columnCount(const QModelIndex &parent) const;
	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation,
			int role) const;
	bool setData(const QModelIndex &index, const QVariant &value, int role);
	Qt::ItemFlags flags(const QModelIndex &index) const;
	static QSharedPointer<QString> mapUnitToString(int u);
	static int mapStringToUnit(QString s);
	void convertFromTo(int f, int t, int row);
#ifdef DEBUG
	void printUnderlyingStructure();
#endif
};
}
}

#endif /* resultModel_hpp */
