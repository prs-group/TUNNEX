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

#include "model/AbstractXYDataFileModel.h"
#include "gui/definitions.h"

using namespace WKB::model;

AbstractXYDataFileModel::AbstractXYDataFileModel(const QString &file,
		QObject *parent) :
		QObject(parent), _data(XYdata()),_fstream(file) {

}

AbstractXYDataFileModel::AbstractXYDataFileModel(const XYdata &data,
		QObject *parent) :
		QObject(parent), _data(data) {

}

AbstractXYDataFileModel::AbstractXYDataFileModel(const AbstractXYDataFileModel &other):
    QObject(other.parent()),_data(other._data),
    _fstream(other._fstream.fileName()),_changed(other._changed){
}

AbstractXYDataFileModel::~AbstractXYDataFileModel(){

}

bool AbstractXYDataFileModel::isEmpty() const {
    
	return _data.isEmpty();
}

const XYdata& AbstractXYDataFileModel::getData() const {

    if (isEmpty()){
		qWarning() << "Error: Could not read your file";
		return XYdata();
    }

	return _data;
}
void AbstractXYDataFileModel::setData(const XYdata &d) {
    _data.clear();
	_data = XYdata(d);
}

bool AbstractXYDataFileModel::getFilename(QString &name) const {

	name = _fstream.fileName();

	if (name.isEmpty())
		return false;

	return true;
}

void AbstractXYDataFileModel::setFile(QString &f) {
	if (!_fstream.isOpen())
		_fstream.close();

	this->_fstream.setFileName(f);
	if (!this->readDataFromFile())
		qWarning() << "Error: Could not read file";

}

bool AbstractXYDataFileModel::hasChanged() const {
	return this->_changed;
}

dVector AbstractXYDataFileModel::getXValues() const {
	if (this->isEmpty())
        return dVector();

    dVector xvalues;
    xvalues.reset(new realVector(_data.size()));
	auto dIt = _data.begin();
	for (auto i = xvalues->begin(); i != xvalues->end(); i++) {
		*i = dIt->x();
		dIt++;
	}

	return xvalues;

}

dVector AbstractXYDataFileModel::getYValues() const {
	if (this->isEmpty()) {
		qWarning() << "Error: Could not read your file";
		return dVector();
	}

    dVector yvalues;
    yvalues.reset(new realVector(_data.size()));
	auto dIt = _data.begin();
	for (auto i = yvalues->begin(); i != yvalues->end(); i++) {
		*i = dIt->y();
		dIt++;
	}

	return yvalues;
}

qreal AbstractXYDataFileModel::xmin() {
	if (isEmpty())
        return 0.0f;
	sortX();
	return _data.first().x();
}

qreal AbstractXYDataFileModel::xmax() {
	if (isEmpty())
		return 0.0;
	sortX();
	return _data.last().x();
}

qreal AbstractXYDataFileModel::ymin() {
	if (isEmpty()) {
		qWarning() << "Error: Could not read your file";
		return 0.0;
	}
	sortY();
	return _data.first().y();
}

qreal AbstractXYDataFileModel::ymax() {
	if (isEmpty()) {
		qWarning() << "Error: Could not read your file";
		return 0.0;
	}
	sortY();
	return _data.last().y();
}

void AbstractXYDataFileModel::clear() {
	_data.clear();
}

namespace WKB {
namespace model {

void AbstractXYDataFileModel::autoCorrectData() {
	sortX();
	// Check duplicates x values
	for(int i = 0; i < _data.size() - 1; i ++){
		auto dataPoint1=_data.at(i);
		auto dataPoint2=_data.at(i+1);
		if(dataPoint1.x() == dataPoint2.x()){
			if((dataPoint1.y() - dataPoint2.y()) > FLOATTOLARANCE){
				qWarning() << "A duplicate X value has different Y values!"
						<< "The program will probably crash, because this can"
						<< "not be fixed!";
			} else {
				qDebug() << "Data removed! ";
				_data.remove(i);
				i--;
			}
		}
	}
}

QDebug& operator<<(QDebug &os, const AbstractXYDataFileModel &model) {
	os.operator<<(model.printData());
	return os;
}
}
}

dVector WKB::model::AbstractXYDataFileModel::getMonotonicXValues() {
	if(isEmpty())
		throw std::runtime_error("Error: Could not read your file");
	sortX();
	dVector monotonicXValues(new realVector);
	for(auto xyValue : _data)
		monotonicXValues->append(xyValue.x());

	return monotonicXValues;
}

dVector WKB::model::AbstractXYDataFileModel::getMonotonicYValues() {
	if(isEmpty())
		throw std::runtime_error("Error: Could not read your file");

	sortX();
	dVector monotonicYValues(new realVector);
	for(auto xyValue : _data)
		monotonicYValues->append(xyValue.y());

	return monotonicYValues;
}
