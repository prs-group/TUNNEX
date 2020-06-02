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

#include "resultwidget.h"
#include <QHBoxLayout>
#include <QVariant>
#include <QList>
#include <QComboBox>
#include <QTableView>
#include <QDebug>
#include <QApplication>
#include <QResizeEvent>
#include <QHeaderView>
#include <QPushButton>
#include <QStyleOption>
#include <QClipboard>
#include "gui/conversion.h"
#include "gui/definitions.h" // For the display prec
#include "view/ui_resultwidget.h"

using namespace WKB::widgets;
using namespace WKB::model;

// resultDelegate impl

// resultWidget impl

ResultWidget::ResultWidget(QWidget *parent) :
		QWidget(parent), _ui(new Ui_resultWidget)
{
    _ui->setupUi(this);
    fillComboBox();
    resetHalfLives();
}

void ResultWidget::fillComboBox(){
    QList<QComboBox *> comboBoxes=this->findChildren<QComboBox*>();
    for(auto &cBox : comboBoxes){
        cBox->addItem(QStringLiteral("s"));
        cBox->addItem(QStringLiteral("m"));
        cBox->addItem(QStringLiteral("h"));
        cBox->addItem(QStringLiteral("d"));
        cBox->addItem(QStringLiteral("y"));
        cBox->setCurrentText("s");
        
        connect(cBox,static_cast<void (QComboBox::*)(int)>
                (&QComboBox::currentIndexChanged),
                this,&ResultWidget::updateValues);
    }
}

void ResultWidget::updateValues(){
    double h0=_halfLives[0];
    double h1=_halfLives[1];
    double h2=_halfLives[2];
    double h3=_halfLives[3];
    
    
    h0*=myTime::timeConvTable[0][_ui->combo_v0->currentIndex()];
    h1*=myTime::timeConvTable[0][_ui->combo_v1->currentIndex()];
    h2*=myTime::timeConvTable[0][_ui->combo_v2->currentIndex()];
    h3*=myTime::timeConvTable[0][_ui->combo_v3->currentIndex()];
    
    _ui->resV0->setText(QString("%1").arg(h0,4,'G',2));
    _ui->resV1->setText(QString("%1").arg(h1,4,'G',2));
    _ui->resV2->setText(QString("%1").arg(h2,4,'G',2));
    _ui->resV3->setText(QString("%1").arg(h3,4,'G',2));
}

const QVector<double>& ResultWidget::getHalfLives() const{
    return _halfLives;
}

void ResultWidget::setHalfLives(const QVector<double> &halfLives){
    Q_ASSERT(halfLives.length() == 4);

    _halfLives=halfLives;
    chooseReasonableUnit();
    updateValues();
}

void ResultWidget::resetHalfLives(){
    _halfLives={0.0,0.0,0.0,0.0};
    updateValues();
}

void ResultWidget::chooseReasonableUnit() const{
    // Used to save the unit indices per half life
    QVector<myTime::timeUnits> unitIndices;
    
    Q_CONSTEXPR double hours=60.0*60.0;
    Q_CONSTEXPR double days=hours*24;
    Q_CONSTEXPR double years=days*360;
    
    for(auto &hl : _halfLives){
        
        if( hl <= 60.0)
            unitIndices.append(myTime::s);
    
        else if( hl <= hours)
            unitIndices.append(myTime::m);
        
        else if( hl <= days)
            unitIndices.append(myTime::h);
        
        else if( hl <= years)
            unitIndices.append(myTime::d);
        
        else
            unitIndices.append(myTime::y);
        
    }
    
    
    // Set the current Indices of the comboBoxes
    _ui->combo_v0->setCurrentIndex(unitIndices.at(0));
    _ui->combo_v1->setCurrentIndex(unitIndices.at(1));
    _ui->combo_v2->setCurrentIndex(unitIndices.at(2));
    _ui->combo_v3->setCurrentIndex(unitIndices.at(3));

}
