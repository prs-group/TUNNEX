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

#include "controller/CentralWidget_p.h"
#include <QtCore>
#include <QtWidgets>
#include <QSharedPointer>
#include "controller/AppMenu.h"
#include "model/paramlistmodel.h"
#include "controller/resultwidget.h"
#include "controller/CentralWidget.h"

using namespace WKB::impldetail;
using namespace WKB::model;



namespace WKB {
    
    namespace widgets {
        CentralWidget::CentralWidget(QWidget* parent) Q_DECL_NOEXCEPT : QWidget(parent),
        d_ptr(new CentralWidgetPrivate(this)),_ui(new Ui::CentralWidget)
        {
  
        }

        CentralWidget::~CentralWidget() Q_DECL_NOEXCEPT{
            delete d_ptr;
        }
        // Slots for updating view
        void CentralWidget::IRCDataChanged(QSharedPointer<WKB::model::AbstractXYDataFileModel> data) Q_DECL_NOEXCEPT{
            Q_D(CentralWidget);
            d->dataChanged(CentralWidgetPrivate::IRC,data.data());
        }
        
        void CentralWidget::IRCpDataChanged(QSharedPointer<WKB::model::AbstractXYDataFileModel> data) Q_DECL_NOEXCEPT{
            Q_D(CentralWidget);
            d->dataChanged(CentralWidgetPrivate::IRCp,data.data());
        }
        void CentralWidget::IRCcorDataChanged(QSharedPointer<WKB::model::AbstractXYDataFileModel> data) Q_DECL_NOEXCEPT{
            Q_D(CentralWidget);

            d->dataChanged(CentralWidgetPrivate::IRCcorr,data.data());
        }
        
        void CentralWidget::IRCInterpolHasChanged(QSharedPointer<WKB::model::AbstractInterpol> inter) Q_DECL_NOEXCEPT{
            Q_D(CentralWidget);
            d->interpolChanged(CentralWidgetPrivate::IRC, inter.data());
            
        }
        
        void CentralWidget::pIRCInterpolHasChanged(QSharedPointer<WKB::model::AbstractInterpol> inter) Q_DECL_NOEXCEPT {
            Q_D(CentralWidget);
            d->interpolChanged(CentralWidgetPrivate::IRCp, inter.data());
        }
        
        void CentralWidget::corrIRCInterpolHasChanged(QSharedPointer<WKB::model::AbstractInterpol> inter) Q_DECL_NOEXCEPT
        {
            Q_D(CentralWidget);
            d->interpolChanged(CentralWidgetPrivate::IRCcorr, inter.data());
            
        }
        void CentralWidget::reactiveModeChanged(const QVariant &mode) Q_DECL_NOEXCEPT{
            Q_D(CentralWidget);
            d->reactiveModeChanged(mode);
        }

    }
}
