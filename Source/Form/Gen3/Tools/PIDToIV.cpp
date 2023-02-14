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

#include "PIDToIV.hpp"
#include "ui_PIDToIV.h"
#include <Core/Gen3/Tools/PIDToIVCalculator.hpp>
#include <Model/Gen3/PIDToIVModel.hpp>
#include <QSettings>

PIDToIV::PIDToIV(QWidget *parent) : QWidget(parent), ui(new Ui::PIDToIV)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    model = new PIDToIVModel(ui->tableView);
    ui->tableView->setModel(model);

    ui->textBoxPID->setValues(InputType::Seed32Bit);

    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &PIDToIV::generate);

    QSettings setting;
    if (setting.contains("pidToIVs/geometry"))
    {
        this->restoreGeometry(setting.value("pidToIVs/geometry").toByteArray());
    }
}

PIDToIV::~PIDToIV()
{
    QSettings setting;
    setting.setValue("pidToIVs/geometry", this->saveGeometry());

    delete ui;
}

void PIDToIV::generate()
{
    model->clearModel();
    u32 pid = ui->textBoxPID->getUInt();
    auto states = PIDToIVCalculator::calculateIVs(pid);
    model->addItems(states);
}
