/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "PIDIVWild.hpp"
#include "ui_PIDIVWild.h"
#include <Core/Gen3/Tools/PIDIVCalculator.hpp>
#include <Forms/Models/Gen3/PIDIVModel.hpp>
#include <QClipboard>
#include <QSettings>

PIDIVWild::PIDIVWild(QWidget *parent) : QWidget(parent), ui(new Ui::PIDIVWild)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

PIDIVWild::~PIDIVWild()
{
    QSettings setting;
    setting.setValue("pidToIVs/geometry", this->saveGeometry());

    delete ui;
}

void PIDIVWild::setupModels()
{
    model = new PIDIVModel(ui->tableView);
    ui->tableView->setModel(model);

    ui->textBoxPID->setValues(InputType::Seed32Bit);

    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &PIDIVWild::generate);

    QSettings setting;
    if (setting.contains("pidToIVs/geometry"))
    {
        this->restoreGeometry(setting.value("pidToIVs/geometry").toByteArray());
    }
}

void PIDIVWild::generate()
{
    model->clearModel();

    u32 pid = ui->textBoxPID->getUInt();
    auto states = PIDIVCalculator::calculateWildIVs(pid);
    model->addItems(states);
}
