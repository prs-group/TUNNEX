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

#ifndef new_tsvdata_hpp
#define new_tsvdata_hpp

#include "model/AbstractXYDataFileModel.h"
#include <QObject>
#include <exception>

namespace WKB {
namespace model {

class TSVData: public AbstractXYDataFileModel {

	Q_OBJECT

protected:
    TSVData(const TSVData &other);
	bool writeDataToFile() Q_DECL_OVERRIDE;
	bool readDataFromFile() Q_DECL_OVERRIDE;

public:
	TSVData(QObject *parent = 0) :
			AbstractXYDataFileModel(parent) {
	}
	TSVData(const QString &filename, QObject *parent = 0) :
			AbstractXYDataFileModel(filename, parent) {
		if (!this->readDataFromFile())
			qWarning() << "Error: Could not read file";
	}
	TSVData(const XYdata &data, QObject *parent = 0) :
			AbstractXYDataFileModel(data, parent) {
	}

    ~TSVData() Q_DECL_OVERRIDE {

    }
    
	const QString printData() const Q_DECL_OVERRIDE;
    
    AbstractXYDataFileModel* copy() const Q_DECL_OVERRIDE;

private:
	bool validateFileStructure();

};
}
}

#endif /* new_tsvdata_hpp */
