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

#include "model/tsvdata.h"
#include <QTextStream>
#include <QDebug>

using namespace WKB::model;


TSVData::TSVData(const TSVData &other):
AbstractXYDataFileModel(other)
{

}

bool TSVData::readDataFromFile() {
	if (!validateFileStructure())
		return false;
	if (!_fstream.isOpen()) {
		if (!_fstream.open(QIODevice::ReadOnly | QIODevice::Text)) {
			_fstream.close();
			return false;
		}
	}

	QTextStream in(&_fstream);

	//Delete old data and reserve new space

	this->_data.clear();

	while (!in.atEnd()) {
		QPointF xy;
		QString x, y;
		in >> x >> y;

		if (in.status() == QTextStream::Ok) {
			xy.setX(x.toFloat());
			xy.setY(y.toFloat());
			_data.append(xy);
			in.flush();
		}

	}
	auto comp = [](const QPointF &p1, const QPointF &p2) -> bool {
		return p1.x() < p2.x();
	};

	std::sort(_data.begin(), _data.end(), comp);

	return true;

}

bool TSVData::writeDataToFile() {
	// Get File name from User if QFile not specified
	if (!_fstream.isReadable())
		return false;

	_fstream.open(QIODevice::WriteOnly | QIODevice::Text);
	QTextStream out(&_fstream);
	//Save data to file
	for (auto &p : _data)
		out << p.x() << "\t \t" << p.y() << "\n";

	return _fstream.flush();
}

const QString TSVData::printData() const {
	QString formatString(QStringLiteral("%1 %2"));
	QString out;

	for (auto &p : _data) {
		out += formatString.arg(p.x(), 10).arg(p.y());

	}

	return out;
}

bool TSVData::validateFileStructure() {
	// Check xy data file through regex
	QString number(QStringLiteral("[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?"));
	QString seperator(QStringLiteral("\\s*"));
	QString space(QStringLiteral("\\s*"));

	QRegExp validator(space + number + seperator + number + space);

	if (!_fstream.open(QIODevice::ReadOnly | QIODevice::Text)) {
		_fstream.close();
		return false;
	}

	QTextStream in(&_fstream);
	QString line;
	while (!in.atEnd()) {
		line = in.readLine();
		// Ignore whitelines
		if (line.isEmpty())
			continue;
		if (!validator.exactMatch(line)) {
			return false;
		}
	}

	_fstream.close();
	return true;
}

AbstractXYDataFileModel* TSVData::copy() const{
    TSVData *data=new TSVData(*this);
    return data;
}
