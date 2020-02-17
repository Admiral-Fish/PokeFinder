/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "ChainedSID.hpp"
#include "ui_ChainedSID.h"
#include <Core/Gen4/Tools/ChainedSIDCalc.hpp>
#include <Core/Util/Translator.hpp>
#include <QSettings>

ChainedSID::ChainedSID(QWidget *parent) : QWidget(parent), ui(new Ui::ChainedSID)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

ChainedSID::~ChainedSID()
{
    QSettings setting;
    setting.setValue("chainedSID/geometry", this->saveGeometry());

    delete ui;
    delete chainedCalc;
}

void ChainedSID::setupModels()
{
    model = new QStandardItemModel(ui->tableView);
    model->setHorizontalHeaderLabels(QStringList() << tr("IVs") << tr("Nature") << tr("Ability") << tr("Gender"));
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->textBoxTID->setValues(InputType::TIDSID);
    ui->comboBoxNature->addItems(Translator::getNatures());

    connect(ui->pushButtonCalculate, &QPushButton::clicked, this, &ChainedSID::calculate);
    connect(ui->pushButtonClear, &QPushButton::clicked, this, &ChainedSID::clear);

    QSettings setting;
    if (setting.contains("chainedSID/geometry"))
    {
        this->restoreGeometry(setting.value("chainedSID/geometry").toByteArray());
    }
}

void ChainedSID::calculate()
{
    if (!chainedCalc)
    {
        chainedCalc = new ChainedSIDCalc(ui->textBoxTID->getUShort());
        ui->textBoxTID->setEnabled(false);
    }

    u8 hp = ui->spinBoxHP->value();
    u8 atk = ui->spinBoxAtk->value();
    u8 def = ui->spinBoxDef->value();
    u8 spa = ui->spinBoxSpA->value();
    u8 spd = ui->spinBoxSpD->value();
    u8 spe = ui->spinBoxSpe->value();
    u8 nature = ui->comboBoxNature->currentIndex();
    u8 ability = ui->comboBoxAbility->currentIndex();
    u8 gender = ui->comboBoxGender->currentIndex();

    QList<QStandardItem *> row;
    row << new QStandardItem(QString("%1.%2.%3.%4.%5.%6").arg(hp).arg(atk).arg(def).arg(spa).arg(spd).arg(spe));
    row << new QStandardItem(Translator::getNature(nature));
    row << new QStandardItem(ui->comboBoxAbility->currentText());
    row << new QStandardItem(ui->comboBoxGender->currentText());
    model->appendRow(row);

    chainedCalc->addEntry({ hp, atk, def, spa, spd, spe }, nature, ability, gender);
    QVector<u16> sids = chainedCalc->getSIDs();
    if (sids.size() == 1)
    {
        ui->labelPossibleResults->setText(tr("SID Found: ") + QString::number(sids.at(0)));
    }
    else
    {
        ui->labelPossibleResults->setText(tr("Possible Results: ") + QString::number(sids.size()));
    }

    ui->spinBoxHP->setValue(0);
    ui->spinBoxAtk->setValue(0);
    ui->spinBoxDef->setValue(0);
    ui->spinBoxSpA->setValue(0);
    ui->spinBoxSpD->setValue(0);
    ui->spinBoxSpe->setValue(0);
    ui->comboBoxNature->setCurrentIndex(0);
    ui->comboBoxAbility->setCurrentIndex(0);
    ui->comboBoxGender->setCurrentIndex(0);
}

void ChainedSID::clear()
{
    if (chainedCalc)
    {
        delete chainedCalc;
        chainedCalc = nullptr;
        ui->textBoxTID->setEnabled(true);
    }

    model->removeRows(0, model->rowCount());
    ui->labelPossibleResults->setText("Possible Results: 8192");
}
