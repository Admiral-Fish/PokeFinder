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

#include "SeedToTime3.hpp"
#include "ui_SeedToTime3.h"
#include <Core/Gen3/Tools/SeedToTimeCalculator3.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Model/Gen3/SeedToTimeModel3.hpp>
#include <QSettings>

SeedToTime3::SeedToTime3(QWidget *parent) : QWidget(parent), ui(new Ui::SeedToTime3)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    model = new SeedToTimeModel3(ui->tableView);
    ui->tableView->setModel(model);

    ui->textBoxSeed->setValues(InputType::Seed32Bit);

    // Hard cap upper year since game crashes above year 2037
    // Signed overflow error due to how the clock is setup
    ui->textBoxYear->setValues(2000, 2037, 4, 10);

    connect(ui->pushButtonFind, &QPushButton::clicked, this, &SeedToTime3::find);

    QSettings setting;
    setting.beginGroup("seedToTime3");
    if (setting.contains("year"))
    {
        ui->textBoxYear->setText(setting.value("year").toString());
    }
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

SeedToTime3::SeedToTime3(u32 seed, QWidget *parent) : SeedToTime3(parent)
{
    ui->textBoxSeed->setText(QString::number(seed, 16));
}

SeedToTime3::~SeedToTime3()
{
    QSettings setting;
    setting.beginGroup("seedToTime3");
    setting.setValue("year", ui->textBoxYear->text());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void SeedToTime3::find()
{
    u32 seed = ui->textBoxSeed->getUInt();
    u16 year = ui->textBoxYear->getUShort();
    u32 advances = 0;

    if (seed > 0xffff)
    {
        seed = SeedToTimeCalculator3::calculateOriginSeed(seed, advances);
        ui->textBoxSeed->setText(QString::number(seed, 16));
    }

    ui->textBoxAdvances->setText(QString::number(advances));

    model->clearModel();
    auto states = SeedToTimeCalculator3::calculateTimes(seed, year);
    model->addItems(states);
}
