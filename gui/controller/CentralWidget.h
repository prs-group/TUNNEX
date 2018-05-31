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

#ifndef CentralWidget_hpp
#define CentralWidget_hpp

#include <QWidget>
#include <QVector>
#include <QModelIndex>


QT_BEGIN_NAMESPACE
class QJsonObject;
class QAction;

QT_END_NAMESPACE

namespace WKB {
    namespace impldetail {
        class CentralWidgetPrivate;
    }
    namespace model {
        class AbstractXYDataFileModel;
 
        class AbstractInterpol;

    }
}

namespace WKB {
    
    namespace widgets {

 
        
        namespace Ui {
            class CentralWidget;
        }
        
        class CentralWidget: public QWidget {
            Q_OBJECT
            
        public:
            //! Standard constructor for a QWidget
            CentralWidget(QWidget *parent = 0) Q_DECL_NOEXCEPT;
            //! @brief Constructor with Document
            //! The Document contains information for restoring
            //! the latest session
            CentralWidget(QJsonObject *jonDoc, QWidget *parent = 0) Q_DECL_NOEXCEPT;
            //! Copy constructor
            CentralWidget(CentralWidget &other) Q_DECL_NOEXCEPT;

            ~CentralWidget() Q_DECL_NOEXCEPT;
            
            public slots:
            
            //@{
            /** Slots for updating the view when data is changed
             
             */
            void IRCDataChanged(QSharedPointer<WKB::model::AbstractXYDataFileModel> data) Q_DECL_NOEXCEPT;
            void IRCpDataChanged(QSharedPointer<WKB::model::AbstractXYDataFileModel>  data) Q_DECL_NOEXCEPT;
            void IRCcorDataChanged(QSharedPointer<WKB::model::AbstractXYDataFileModel>  data) Q_DECL_NOEXCEPT;
            void IRCInterpolHasChanged(QSharedPointer<WKB::model::AbstractInterpol>  inter) Q_DECL_NOEXCEPT;
            void pIRCInterpolHasChanged(QSharedPointer<WKB::model::AbstractInterpol>  inter) Q_DECL_NOEXCEPT;
            void corrIRCInterpolHasChanged(QSharedPointer<WKB::model::AbstractInterpol>  inter) Q_DECL_NOEXCEPT;
            void reactiveModeChanged(const QVariant &mode) Q_DECL_NOEXCEPT;
            //@}

            
        private:

            Ui::CentralWidget *_ui;

            Q_DECLARE_PRIVATE (WKB::impldetail::CentralWidget);
            
            WKB::impldetail::CentralWidgetPrivate *d_ptr;

            // The main window
        };
    }
}

#endif /* CentralWidget_h */
