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

#ifndef AbstractInterpol_hpp
#define AbstractInterpol_hpp

#include <QObject>
#include <QSharedPointer>


namespace WKB {
    namespace model{
        class AbstractXYDataFileModel;
        
        class AbstractInterpol : public QObject {
            Q_OBJECT
        private:
            QSharedPointer<AbstractXYDataFileModel> _interpData;
        protected:
            QSharedPointer<AbstractXYDataFileModel> _data;
            void recomputeInterpol();
            const AbstractXYDataFileModel* getData() const{
                return _data.data();
            }
            AbstractInterpol(const AbstractInterpol &other);
        public:
            AbstractInterpol(QObject *parent=Q_NULLPTR);

            AbstractInterpol(QSharedPointer<AbstractXYDataFileModel> data,QObject *parent=Q_NULLPTR);
            virtual ~AbstractInterpol();
            virtual double interpolate(const double &x) const = 0;


            virtual void setData(QSharedPointer<AbstractXYDataFileModel> m);
            virtual AbstractXYDataFileModel *getInterpData() const{
                return _interpData.data();
            };
            
            
            virtual double integrate(double lower,double upper) const = 0 ;
            
            virtual AbstractInterpol* copy() const = 0 ;
            
            double xmin() const;
            double xmax() const;
            
            
        };
    }
}



#endif /* AbstractInterpol_hpp */
