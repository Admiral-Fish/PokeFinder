/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "IVToPID.hpp"
#include "ui_IVToPID.h"
#include <Core/Enum/Method.hpp>
#include <Core/Util/IVToPIDCalculator.hpp>
#include <Core/Util/Translator.hpp>
#include <Model/Util/IVToPIDModel.hpp>
#include <QSettings>

IVToPID::IVToPID(QWidget *parent) : QWidget(parent), ui(new Ui::IVToPID)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    model = new IVToPIDModel(ui->tableView);
    ui->tableView->setModel(model);

    ui->textBoxTID->setValues(InputType::TIDSID);
    for (const std::string &nature : Translator::getNatures())
    {
        ui->comboBoxNature->addItem(QString::fromStdString(nature));
    }

    connect(ui->pushButtonFind, &QPushButton::clicked, this, &IVToPID::find);

    QSettings setting;
    if (setting.contains("ivToPID/geometry"))
    {
        this->restoreGeometry(setting.value("ivToPID/geometry").toByteArray());
    }
}

IVToPID::~IVToPID()
{
    QSettings setting;
    setting.setValue("ivToPID/geometry", this->saveGeometry());

    delete ui;
}

void IVToPID::find()
{
    model->clearModel();

    u8 hp = ui->spinBoxHP->value();
    u8 atk = ui->spinBoxAtk->value();
    u8 def = ui->spinBoxDef->value();
    u8 spa = ui->spinBoxSpA->value();
    u8 spd = ui->spinBoxSpD->value();
    u8 spe = ui->spinBoxSpe->value();
    u8 nature = ui->comboBoxNature->currentIndex();
    u16 tid = ui->textBoxTID->getUShort();

    auto states = IVToPIDCalculator::calculateIVs(hp, atk, def, spa, spd, spe, nature, tid);
    model->addItems(states);
}
