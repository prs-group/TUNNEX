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

#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H
#include <QWidget>
#include <QAbstractTableModel>
#include <QStyledItemDelegate>
#include <QString>
#include "gui/definitions.h"
#include "model/resultModel.h"


QT_BEGIN_NAMESPACE

class QHBoxLayout;
class QStringList;


QT_END_NAMESPACE

namespace WKB {
    namespace widgets {
        class Ui_resultWidget;
        /* TODO: Remove table in favor for Lables and
         Text Edits */
        class ResultWidget: public QWidget {
            Q_OBJECT
            Q_PROPERTY(QVector<double> halfLives
                       READ getHalfLives
                       WRITE setHalfLives
                       RESET resetHalfLives)
        private:

            Ui_resultWidget *_ui;
            QVector<double> _halfLives;
            void fillComboBox();
            void connectComboBox();
            
            void chooseReasonableUnit() const;
            
        public:
            explicit ResultWidget(QWidget *parent = 0);
            const QVector<double> &getHalfLives() const;
            void resetHalfLives();
            
        signals:
            void copyAvailable(bool);
            void dataChanged();
            
        public slots:
            void updateValues();
            void setHalfLives(const QVector<double> &halfLives);
            
            
            
        };
    }
}

#endif // RESULTWIDGET_H
