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
#include "model/paramlistmodel.h"
#include <QStringList>
#include <QMessageBox>
#include <QDebug>

using namespace WKB;
using namespace model;

ParamListModel::ParamListModel(QObject *parent) :
		QAbstractTableModel(parent), _names(new QStringList()), _parameters(
				new QStringList()) {
	*_names << tr("Reaction coordinate frequency")
			<< tr("Computed educt minimum") << tr("Computed educt ZPVE")
			<< tr("Offset Parameter");
	*_parameters << "0.0" << "0" << "0" << "1.0000";
}

ParamListModel::~ParamListModel() {
	delete _names;
	delete _parameters;
}

int ParamListModel::rowCount(const QModelIndex &parent) const {
	return 4;
}

int ParamListModel::columnCount(const QModelIndex &parent) const {
	return 2;
}

QVariant ParamListModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid())
		return QVariant();
	if (index.row() > _names->size())
		return QVariant();

	if (role == Qt::DisplayRole) {
		if (index.column() == 0)
			return _names->at(index.row());
		else if (index.column() == 1)
			return _parameters->at(index.row());
	}
	if (role == Qt::EditRole) {
		if (index.column() == 1)
			return _parameters->at(index.row());
	}
	return QVariant();
}

Qt::ItemFlags ParamListModel::flags(const QModelIndex &index) const {
	if (!index.isValid())
		return Qt::ItemIsEnabled;
	if (index.column() == 1)
		return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;

	return QAbstractItemModel::flags(index);

}

bool ParamListModel::setData(const QModelIndex &index, const QVariant &value,
		int role) {
	if (index.isValid() && role == Qt::EditRole) {
		bool ok;
		value.toDouble(&ok);
		if (!ok) {
			QMessageBox *msg = new QMessageBox;
			msg->setText(tr("Please enter a valid number"));
			msg->exec();
			delete msg;
			return false;
		}
		_parameters->replace(index.row(), value.toString());

		emit dataChanged(index, index);
		return true;
	}
	return false;
}

QVariant ParamListModel::getParameter(common::names name) {
	return QVariant(_parameters->at(name));
}
