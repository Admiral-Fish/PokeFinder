/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "IDs8.hpp"
#include "ui_IDs8.h"
#include <Core/Gen8/Generators/IDGenerator8.hpp>
#include <Core/Gen8/Profile8.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Model/Gen8/IDModel8.hpp>
#include <QMessageBox>
#include <QSettings>

IDs8::IDs8(QWidget *parent) : QWidget(parent), ui(new Ui::IDs8)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    model = new IDModel8(ui->tableView);
    ui->tableView->setModel(model);

    ui->textBoxInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxSeed0->setValues(InputType::Seed64Bit);
    ui->textBoxSeed1->setValues(InputType::Seed64Bit);

    ui->filter->enableDisplayTID();

    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &IDs8::generate);

    QSettings setting;
    if (setting.contains("ids8/geometry"))
    {
        this->restoreGeometry(setting.value("ids8/geometry").toByteArray());
    }
}

IDs8::~IDs8()
{
    QSettings settings;
    settings.setValue("ids8/geometry", this->saveGeometry());

    delete ui;
}

void IDs8::generate()
{
    u64 seed0 = ui->textBoxSeed0->getULong();
    u64 seed1 = ui->textBoxSeed1->getULong();
    if (seed0 == 0 && seed1 == 0)
    {
        QMessageBox msg(QMessageBox::Warning, tr("Missing seeds"), tr("Please insert missing seed information"));
        msg.exec();
        return;
    }

    model->clearModel();

    u32 initialAdvances = ui->textBoxInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxMaxAdvances->getUInt();

    IDFilter filter = ui->filter->getFilter(false);
    IDGenerator8 generator(initialAdvances, maxAdvances, filter);

    auto states = generator.generate(seed0, seed1);
    model->addItems(states);
}
