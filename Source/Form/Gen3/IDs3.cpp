/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "IDs3.hpp"
#include "ui_IDs3.h"
#include <Core/Gen3/Generators/IDGenerator3.hpp>
#include <Core/Util/DateTime.hpp>
#include <Core/Util/Utilities.hpp>
#include <Model/Gen3/IDModel3.hpp>
#include <QSettings>

IDs3::IDs3(QWidget *parent) : QWidget(parent), ui(new Ui::IDs3), model(new IDModel3(this))
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    ui->tableView->setModel(model);

    ui->textBoxTIDFRLGE->setValues(InputType::TIDSID);
    ui->textBoxInitialAdvancesFRLGE->setValues(InputType::Advance32Bit);
    ui->textBoxMaxAdvancesFRLGE->setValues(InputType::Advance32Bit);

    ui->textBoxSeedRS->setValues(InputType::Seed16Bit);
    ui->textBoxInitialAdvancesRS->setValues(InputType::Advance32Bit);
    ui->textBoxMaxAdvancesRS->setValues(InputType::Advance32Bit);

    ui->textBoxSeedXDColo->setValues(InputType::Seed32Bit);
    ui->textBoxInitialAdvancesXDColo->setValues(InputType::Advance32Bit);
    ui->textBoxMaxAdvancesXDColo->setValues(InputType::Advance32Bit);

    ui->idFilter->enableDisplayTID(false);

    connect(ui->pushButtonGenerateXDColo, &QPushButton::clicked, this, &IDs3::generateXDColo);
    connect(ui->pushButtonGenerateFRLGE, &QPushButton::clicked, this, &IDs3::generateFRLGE);
    connect(ui->pushButtonGenerateRS, &QPushButton::clicked, this, &IDs3::generateRS);
    connect(ui->checkBoxDeadBatteryRS, &QCheckBox::clicked, this, &IDs3::toggleDeadBatteryRS);
    connect(ui->radioButtonDateTimeRS, &QRadioButton::toggled, this, &IDs3::toggleDateTimeRS);
    connect(ui->radioButtonSeedRS, &QRadioButton::toggled, this, &IDs3::toggleSeedRS);

    QSettings setting;
    if (setting.contains("ids3/geometry"))
    {
        this->restoreGeometry(setting.value("ids3/geometry").toByteArray());
    }
}

IDs3::~IDs3()
{
    QSettings setting;
    setting.setValue("ids3/geometry", this->saveGeometry());

    delete ui;
}

void IDs3::generateXDColo()
{
    model->clearModel();

    u32 seed = ui->textBoxSeedXDColo->getUInt();
    u32 initialAdvances = ui->textBoxInitialAdvancesXDColo->getUInt();
    u32 maxAdvances = ui->textBoxMaxAdvancesXDColo->getUInt();

    IDFilter filter = ui->idFilter->getFilter();
    IDGenerator3 generator(initialAdvances, maxAdvances, filter);

    auto states = generator.generateXDColo(seed);
    model->addItems(states);
}

void IDs3::generateFRLGE()
{
    model->clearModel();

    u16 tid = ui->textBoxTIDFRLGE->getUShort();
    u32 initialAdvances = ui->textBoxInitialAdvancesFRLGE->getUInt();
    u32 maxAdvances = ui->textBoxMaxAdvancesFRLGE->getUInt();

    IDFilter filter = ui->idFilter->getFilter();
    IDGenerator3 generator(initialAdvances, maxAdvances, filter);

    auto states = generator.generateFRLGE(tid);
    model->addItems(states);
}

void IDs3::generateRS()
{
    model->clearModel();

    u16 seed;
    if (ui->checkBoxDeadBatteryRS->isChecked())
    {
        seed = 0x5a0;
    }
    else if (ui->radioButtonDateTimeRS->isChecked())
    {
        seed = Utilities3::calcSeed(ui->dateTimeEditRS->getDateTime());
    }
    else
    {
        seed = ui->textBoxSeedRS->getUShort();
    }

    u32 initialAdvances = ui->textBoxInitialAdvancesRS->getUInt();
    u32 maxAdvances = ui->textBoxMaxAdvancesRS->getUInt();

    IDFilter filter = ui->idFilter->getFilter();
    IDGenerator3 generator(initialAdvances, maxAdvances, filter);

    auto states = generator.generateRS(seed);
    model->addItems(states);
}

void IDs3::toggleDeadBatteryRS(bool checked)
{
    ui->radioButtonDateTimeRS->setEnabled(!checked);
    ui->radioButtonSeedRS->setEnabled(!checked);
    ui->dateTimeEditRS->setEnabled(!checked ? ui->radioButtonDateTimeRS->isChecked() : false);
    ui->textBoxSeedRS->setEnabled(!checked ? ui->radioButtonSeedRS->isChecked() : false);
}

void IDs3::toggleDateTimeRS(bool checked)
{
    ui->dateTimeEditRS->setEnabled(checked);
}

void IDs3::toggleSeedRS(bool checked)
{
    ui->textBoxSeedRS->setEnabled(checked);
}
