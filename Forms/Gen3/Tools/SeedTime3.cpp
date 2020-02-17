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

#include "SeedTime3.hpp"
#include "ui_SeedTime3.h"
#include <Core/Gen3/Tools/SeedTimeCalculator3.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Models/Gen3/SeedTimeModel3.hpp>
#include <QSettings>

SeedTime3::SeedTime3(QWidget *parent) : QWidget(parent), ui(new Ui::SeedTime3)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

SeedTime3::SeedTime3(u32 seed, QWidget *parent) : QWidget(parent), ui(new Ui::SeedTime3)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();

    ui->textBoxSeed->setText(QString::number(seed, 16));
    find();
}

SeedTime3::~SeedTime3()
{
    QSettings setting;
    setting.beginGroup("seedToTime3");
    setting.setValue("year", ui->textBoxYear->text());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void SeedTime3::setupModels()
{
    model = new SeedTimeModel3(ui->tableView);
    ui->tableView->setModel(model);

    ui->textBoxSeed->setValues(InputType::Seed32Bit);

    // Hard cap upper year since game crashes above year 2037
    // Signed overflow error due to how the clock is setup
    ui->textBoxYear->setValues(2000, 2037, 4, 10);

    connect(ui->pushButtonFind, &QPushButton::clicked, this, &SeedTime3::find);

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

u16 SeedTime3::originSeed(u32 seed)
{
    PokeRNGR rng(seed);
    while (rng.getSeed() > 0xFFFF)
    {
        rng.nextUInt();
        frame++;
    }
    return static_cast<u16>(rng.getSeed());
}

void SeedTime3::find()
{
    u32 seed = ui->textBoxSeed->getUInt();
    u16 year = ui->textBoxYear->getUShort();
    frame = 1;

    if (seed > 0xFFFF)
    {
        seed = originSeed(seed);
        ui->textBoxSeed->setText(QString::number(seed, 16));
    }

    model->clearModel();
    auto frames = SeedTimeCalculator3::calculateTimes(seed, frame, year);
    model->addItems(frames);
}
