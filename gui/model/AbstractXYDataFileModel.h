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

#ifndef AbstractDataFileModel_h
#define AbstractDataFileModel_h

#include <QObject>
#include <QVector>
#include <memory>
#include <QPoint>
#include <QFile>
#include <iostream>
#include <QDebug>
#include <memory>

// Definition of data types
//! \defgroup types Types in WKB
/*!
 * \ingroup types
 *  @{
 */

//! \brief stores the underlying data
using XYdata = QVector<QPointF>;
//! \brief a vector of real numbers
using realVector = QVector<qreal>;
//! \brief used for sharing data
using dVector = QSharedPointer<realVector>;

//!  @}

namespace WKB {
namespace model {
//!  An Abstract file handler model for XY data
/*!
  manages the interface for all XY based files.
  It is used throughout the program as an abstract interface
  and allows for a not yet implemented plugin system.
*/
class AbstractXYDataFileModel: public QObject {
	Q_OBJECT
private:
	//! Sorts the internal data by x
	/*!
	 *  The data is sorted from the lowest x value
	 *  to the highest x value by using std::sort
	 *  Note that a whole point is moved in the array
	 */

	void sortX() {
		// Lambda for std::sort algorithm
		auto sortX = [](const QPointF &p1,const QPointF &p2) -> bool {
			return p1.x() < p2.x();
		};
		std::sort(_data.begin(), _data.end(), sortX);
	}

	//! Sorts the internal data by y
	/*!
	 *  The data is sorted from the lowest y value
	 *  to the highest x value by using std::sort
	 *  Note that a whole point is moved in the array
	 */

	void sortY() {
		// Lambda for std::sort algorithm
		auto sortY = [](const QPointF &p1,const QPointF &p2) -> bool {
			return p1.y() < p2.y();
		};

		std::sort(_data.begin(), _data.end(), sortY);
	}

protected:
    //! Copy constructor which can be called by the Qt Meta systenn
    /*!
     * This constructor can be used to construct a copy of the object
     * @param other the other object
     */
    explicit AbstractXYDataFileModel(const AbstractXYDataFileModel &other);
	//! The data from file
	/*!
	 *  Stores the data read from file.
	 *  Therefore, it needs to be accessed
	 *  by subclass
	 */

	XYdata _data;
	//! The data file
	/*!
	 *  This variable is used by the subclass to read and write
	 *  to. Note that the _changed variable has to be set to true
	 *  when a write access on the file stream is performed
	 */
	QFile _fstream;

	//! Is set to false if write access to file stream was performed
	bool _changed = false;

	//! \name Virtual Functions
	//! To be implemented by subclass
	//! @{

	//! Invoked from base class when reading data from file is requested
	/*!
	 * \return Returns true if reading was successful
	 */
	virtual bool readDataFromFile() = 0;
	//! Invoked from base class when writing data to file is requested
	/*!
	 * \return Returns true if writing was successful
	 */
	virtual bool writeDataToFile() = 0;


	//!@}

public:
	//! Default Constructor
	/*!
	 * Constructs file model without file. Note: You need to initialize
	 * the model with setFile \sa setFile()
	 * @param parent The parent widget
	 */
	explicit AbstractXYDataFileModel(QObject *parent = 0) :
			QObject(parent),_data(XYdata()) {
	}

	//! Constructor for reading data
	/*!
	 * Used to construct file model with name of file
	 * \param filename The file name to open
	 * \param parent The parent widget
	 */

	explicit AbstractXYDataFileModel(const QString &filename, QObject *parent =
			0);

	//! Constructor for writing data
	/*!
	 * This constructor can be used when you want to write data to file.
	 * Note you have to set the file explicitly with \sa setFile()
	 * @param data The data you want to write to file
	 * @param parent The parent widget
	 */
	explicit AbstractXYDataFileModel(const XYdata &data, QObject *parent = 0);

    virtual ~AbstractXYDataFileModel();
    
    //! Tries to automatically correct data structure
    //! Will try to remove duplicates
    //! Will try to sort data
    virtual void autoCorrectData();

    //! Used by operator << in order to print underlying data (debug)
    /*!
     * see also operator<<(QDebug &os, const AbstractXYDataFileModel &model)
     * \return a formated string to print
     */
    virtual const QString printData() const = 0;

	//! If the model data is empty return false
	virtual bool isEmpty() const;
	//! Returns the data
	/*!
	 * Throws std::runtime_exception when data is empty
	 * \return The data handled by the object
	 */
	virtual const XYdata& getData() const;
	//! Sets the data of the object to data
	/*!
	 * Emits dataChanged() when successful
	 * \param d Sets data to data
	 */
	virtual void setData(const XYdata &data);

	//! Sets the filename to name if successful
	/*!
	 * Returns false if not successful when no file has been set
	 * \param name is set by function to filename
	 * \return success
	 */
	virtual bool getFilename(QString &name) const;
	//! Sets the file stream with file name given
	/*!
	 * Throws std::runtime error if file can not be read
	 * @param filename The file for the file to be read
	 */
	virtual void setFile(QString &filename);
	//! Returns true if data was changed and the file was not updated
	virtual bool hasChanged() const;

	//! Returns only x values in monotonic form
	//! starting with the smallest x value
	/*!
	 * throws std::runtime_exception if data is empty
	 * @return the x values in sotrted form as a shared pointer
	 */

	virtual dVector getMonotonicXValues();

	//! Returns only the y values with x in monotonic form
	//! see <a href="WKB::model::AbstractXYDatafile::getMonotonicXValues()">
	//! getMonotonicXValues() </a>
	/*!
	 * throws std::runtime_exception if data is empty
	 * @return the y values with x sorted monotonically
	 */

	virtual dVector getMonotonicYValues();

	//! Returns only x values from data
	/*!
	 * throws std::runtime_exception if data is empty
	 * @return the x values as shared pointer
	 */
	virtual dVector getXValues() const;
	//! Returns only y values from data
	/*!
	 * throws std::runtime_exception if data is empty
	 * @return the y values as shared pointer
	 */
	virtual dVector getYValues() const;
	//! Returns minimum value of x
	/*!
	 * Throws std::runtime_exception when data is empty
	 * Note that this functions sorts the data internally
	 * and than returns the first value of the array
	 * Therefore it is recommended that this function is used with care
	 * for large data
	 * @return the minimum x value
	 */
	qreal xmin();
	//! Returns maximum value of x
	/*!
	 * Throws std::runtime_exception when data is empty
	 * Note that this functions sorts the data internally
	 * and than returns the first value of the array
	 * Therefore it is recommended that this function is used with care
	 * for large data
	 * @return the maximum x value
	 */
	qreal xmax();
	//! Returns minimum value of y
	/*!
	 * Throws std::runtime_exception when data is empty
	 * Note that this functions sorts the data internally
	 * and than returns the first value of the array
	 * Therefore it is recommended that this function is used with care
	 * for large data
	 * @return the minimum y value
	 */
	qreal ymin();
	//! Returns maximum value of y
	/*!
	 * Throws std::runtime_exception when data is empty
	 * Note that this functions sorts the data internally
	 * and than returns the first value of the array
	 * Therefore it is recommended that this function is used with care
	 * for large data
	 * @return the maximum y value
	 */
	qreal ymax();
	//! Clears the data
	void clear();
	//! For Debug print only
	friend QDebug& operator<<(QDebug &os, const AbstractXYDataFileModel &model);
    
    //! This method is invoked for object copying
    virtual AbstractXYDataFileModel* copy() const = 0;


}; // class AbstractXYDataFileModel
}  // namespace model
}  // namespace WKB

Q_DECLARE_METATYPE(WKB::model::AbstractXYDataFileModel*)

#endif /* AbstractDataFileModel_h */
