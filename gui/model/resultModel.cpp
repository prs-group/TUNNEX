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

#include <QtCore>
#include <QFontMetrics>
#include <QApplication>
#include "gui/conversion.h"
#include "gui/definitions.h"
#include "model/resultModel.h"


using namespace WKB::model;

ResultModel::ResultModel(QObject* parent) :
		QAbstractTableModel(parent), names(new QList<QVariant*>()), values(
				new QList<QVariant*>()), units(new QList<QVariant*>()) {

	for (int i = 0; i < 4; i++) {
		QString name;
		name.append("half-life for v =");
		name.append(QVariant(i).toString());
		name.append(" :");
		names->append(new QVariant(name));
		values->append(new QVariant(0.0));
		units->append(new QVariant("s"));
	}
}

ResultModel::~ResultModel() {
	delete names;
	delete values;
	delete units;
}

int ResultModel::rowCount(const QModelIndex &parent) const {
	return names->length() > values->length() ?
			names->length() : values->length();
}

int ResultModel::columnCount(const QModelIndex &parent) const {
	return 3;
}

QVariant ResultModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid())
		return QVariant();
	if (index.column() > this->columnCount(index))
		return QVariant();

	if (role == Qt::SizeHintRole) {
		QString value;
		switch (index.column()) {
		case 0:
			value = names->at(index.row())->toString();
			break;
		case 1:
			value = values->at(index.row())->toString();
			break;
		case 2:
			return QSize();
			break;
		default:
			return QSize();
			break;
		}

		QFontMetrics m = QApplication::fontMetrics();
		QSize rec = m.size(Qt::TextSingleLine, value);
		;
		return QSize(rec.width() + 10, rec.height());

	}
	switch (index.column()) {
	case 0: {
		if (index.row() > names->length())
			return QVariant();
		if (role == Qt::DisplayRole)
			return names->at(index.row())->toString();
		break;
	}
	case 1: {
		if (index.row() > values->length())
			return QVariant();
		if (role == Qt::DisplayRole)
			return values->at(index.row())->toDouble();
		break;
	}
	case 2: {
		if (index.row() > units->length())
			return QVariant();
		if (role == Qt::EditRole)
			return *units->at(index.row());
		if (role == Qt::SizeHintRole) {
			return QSize();
		}
	}
	default:
		return QVariant();
		break;
	}
	return QVariant();
}

QVariant ResultModel::headerData(int section, Qt::Orientation orientation,
		int role) const {
	switch (orientation) {
	case Qt::Horizontal: {
		if (role == Qt::DisplayRole)
			return QVariant("Hor");
	}
		break;
	case Qt::Vertical: {
		if (role == Qt::DisplayRole)
			return QVariant("Ver");
	}
		break;
	default:
		return QVariant();
		break;
	}
	return QVariant();
}

bool ResultModel::setData(const QModelIndex &index, const QVariant &value,
		int role) {
	switch (index.column()) {
	case 0: {
		if (role == Qt::DisplayRole) {
			names->insert(index.row(), new QVariant(value));
			emit(dataChanged(index, index));
			return true;
		}
	}
		break;

	case 1: {
		/*QRegExp numberWithUnit("^([-+]?[0-9]*\\.?[0-9]+(?:[eE][-+]?[0-9]+)?)(?:[\\s]*)(s|m|d|y)");
		 
		 numberWithUnit.indexIn(value.toString());
		 if(numberWithUnit.captureCount() != 2)
		 return false;
		 values->removeAt(index.row());
		 units->removeAt(index.row());
		 values->insert(index.row(),new QVariant(numberWithUnit.cap(1)));
		 units->insert(index.row(),new QVariant(numberWithUnit.cap(2)));*/
		bool ok;
		double v = value.toDouble(&ok);
		if (!ok)
			return false;
		if (index.row() < values->size())
			values->removeAt(index.row());
		values->insert(index.row(), new QVariant(v));
		emit(dataChanged(index, index));
		return true;
	}
		break;
	case 2: {
		if (role == Qt::EditRole) {
			int oldUnit = mapStringToUnit(units->at(index.row())->toString());
			this->convertFromTo(oldUnit, mapStringToUnit(value.toString()),
					index.row());
			if (index.row() < values->size())
				units->removeAt(index.row());
			units->insert(index.row(), new QVariant(value));
			emit(dataChanged(index, index));
			return true;
		}
	}
		break;
	default:
		break;
	}
	return false;
}

Qt::ItemFlags ResultModel::flags(const QModelIndex &index) const {
	switch (index.column()) {
	case 0:
		return (Qt::ItemIsEnabled);
		break;
	case 1:
		return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
		break;
	case 2:
		return (Qt::ItemIsEditable | Qt::ItemIsEnabled);
	default:
		return Qt::NoItemFlags;
		break;
	}
	return Qt::NoItemFlags;
}

QSharedPointer<QString> ResultModel::mapUnitToString(int u) {
	switch (u) {
	case myTime::s:
		return QSharedPointer<QString>(new QString("s"));
		break;
	case myTime::m:
		return QSharedPointer<QString>(new QString("m"));
		break;
	case myTime::h:
		return QSharedPointer<QString>(new QString("h"));
		break;
	case myTime::d:
		return QSharedPointer<QString>(new QString("d"));
		break;
	case myTime::y:
		return QSharedPointer<QString>(new QString("y"));
		break;
	default:
		return QSharedPointer<QString>(new QString());
		break;
	}
}

int ResultModel::mapStringToUnit(QString s) {
	if (s == "s")
		return myTime::s;
	else if (s == "m")
		return myTime::m;
	else if (s == "h")
		return myTime::h;
	else if (s == "d")
		return myTime::d;
	else if (s == "y")
		return myTime::y;
	else
		return -1;
}

void ResultModel::convertFromTo(int f, int t, int row) {
	// This slot is invoked when the delegate sets the new unit
	QModelIndex index = this->index(row, 1);
	double converted = myTime::timeConvTable[f][t]
			* values->at(row)->toDouble();
	QString convertedString;
	convertedString.setNum(converted, 'g', DISPLAYPREC);
	emit(beginRemoveRows(index, row, row + 1));
	values->removeAt(row);
	emit (endRemoveRows());emit
	(beginInsertRows(index, row, row + 1));
	values->insert(row, new QVariant(convertedString));
	emit (endInsertRows());QVector
	<int> roles;
	roles.append(Qt::DisplayRole);
	roles.append(Qt::EditRole);
	emit(dataChanged(index, index, roles));
}

#ifdef DEBUG
void ResultModel::printUnderlyingStructure() {
	qDebug() << "names: ";
	for(auto *n : *names)
	qDebug() << *n;
	qDebug() << "values: ";
	for(auto *v : *values)
	qDebug() << *v;
	qDebug() << "units:";
	for(auto *u : *units)
	qDebug() << *u;
}
#endif
