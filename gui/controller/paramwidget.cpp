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

#include "controller/paramwidget.h"
#include <QHBoxLayout>
#include <QTableView>
#include <QStringListModel>
#include <QHeaderView>
#include <QResizeEvent>
#include <QDebug>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QVariant>
#include <QSpinBox>
#include "view/ui_paramwidget.h"
#include "controller/commands.h"

#define PRECISION 6
using namespace WKB::widgets;

ParamWidget::ParamWidget(QUndoStack* stack,QWidget *parent) : QWidget(parent),
_ui(new Ui_paramWidget()),
_uStack(stack), _didUserInputStartingMaterialEnergy(
				false), _didUserInputStartingMaterialZPVE(false), _didUserInputReactionMode(
				false) {
    _ui->setupUi(this);
    setupConnections();
    // Set all validators for line edits to double
    // And connect their textChanged slot to dataChanged
    // Also install event filter to supress redo/undo
    for(auto &le : this->findChildren<QLineEdit*>()){
        le->setValidator(new QDoubleValidator());
        le->setText("0.0");
        le->installEventFilter(this);
    }

}

ParamWidget::~ParamWidget(){
    delete _ui;
}

void ParamWidget::setupConnections(){
	connect(_ui->startingMaterial_en, &QLineEdit::textEdited,
            this,
			&ParamWidget::startingMaterialEnergyChanged);
	connect(_ui->startingMaterial_ZPVE, &QLineEdit::textEdited,
            this,
			&ParamWidget::startingMaterialZPVEChanged);
	connect(_ui->startingMaterial_freq, &QLineEdit::textEdited,
            this,
			&ParamWidget::reactionModeChanged);

	connect(_ui->startingMaterial_en, &QLineEdit::textChanged, this,
			&ParamWidget::startingMaterialEnergyHasChanged);
	connect(_ui->startingMaterial_ZPVE, &QLineEdit::textChanged, this,
			&ParamWidget::startingMaterialZPVEHasChanged);
	connect(_ui->startingMaterial_freq, &QLineEdit::textChanged, this,
			&ParamWidget::reactiveModeHasChanged);

	connect(_ui->symmetry_number,
			static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &ParamWidget::symmetryNumberChanged);
}

void ParamWidget::setstartingMaterialEnergy(const QVariant &v){
	_ui->startingMaterial_en->setText(
			QString::number(v.toDouble(), 'f', PRECISION));
}

void ParamWidget::setstartingMaterialZPVE(const QVariant &v){
	_ui->startingMaterial_ZPVE->setText(
			QString::number(v.toDouble(), 'f', PRECISION));
}

void ParamWidget::setReactiveMode(const QVariant &v){
	_ui->startingMaterial_freq->setText(
			QString::number(v.toDouble(), 'f', PRECISION));
}

const QVariant ParamWidget::getStartingMaterialEnergy() {
    return QVariant(_ui->startingMaterial_en->text());
}

const QVariant ParamWidget::getStartingMaterialZPVE() {
    return QVariant(_ui->startingMaterial_ZPVE->text());
}

const QVariant ParamWidget::getReactiveMode(){
    return QVariant(_ui->startingMaterial_ZPVE->text());
}

const QVariant ParamWidget::getSymmetryNumber() {
	return (QVariant(_ui->symmetry_number->value()));
}

void ParamWidget::startingMaterialEnergyChanged(const QVariant &p) {
	_didUserInputStartingMaterialEnergy = true;
	emit(startingMaterialEnergyHasChanged(p));
}
void ParamWidget::startingMaterialZPVEChanged(const QVariant &p) {
	_didUserInputStartingMaterialZPVE = true;
	emit(startingMaterialZPVEHasChanged(p));

}
void ParamWidget::reactionModeChanged(const QVariant &p) {
	_didUserInputReactionMode = true;
	emit(reactiveModeHasChanged(p));
}

void ParamWidget::symmetryNumberChanged(const int &p) {
	emit(symmetryNumberHasChanged(QVariant(p)));
}
bool ParamWidget::didUserInputStartingMaterialEnergy() {
	return _didUserInputStartingMaterialEnergy;
}

bool ParamWidget::didUserInputStartingMaterialZPVE() {
	return _didUserInputStartingMaterialZPVE;
}

bool ParamWidget::didUserInputReactionMode() {
	return _didUserInputReactionMode;
}

void WKB::widgets::ParamWidget::setUserInputToTrue() {
	_didUserInputReactionMode = true;
	_didUserInputStartingMaterialEnergy = true;
	_didUserInputStartingMaterialZPVE = true;
}


