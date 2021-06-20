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

#include "IDs3.hpp"
#include "ui_IDs3.h"
#include <Core/Gen3/Generators/IDGenerator3.hpp>
#include <Core/Parents/Filters/IDFilter.hpp>
#include <Core/Util/Utilities.hpp>
#include <Models/Gen3/IDModel3.hpp>
#include <Models/Gen3/LiveIDModel3.hpp>
#include <Models/Gen3/LiveXDColoIDModel3.hpp>
#include <QSettings>

IDs3::IDs3(QWidget *parent) : QWidget(parent), ui(new Ui::IDs3)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    setupModels();
}

IDs3::~IDs3()
{
    QSettings setting;
    setting.setValue("ids3/geometry", this->saveGeometry());

    delete ui;
}

void IDs3::setupModels()
{
    xdcolo = new IDModel3(ui->tableViewXDColo);
    xdcoloLive = new LiveXDColoIDModel3(ui->tableViewXDColoLive);
    frlge = new IDModel3(ui->tableViewFRLGE);
    rs = new IDModel3(ui->tableViewRS);
    rsLive = new LiveIDModel3(ui->tableViewRSLive);

    ui->tableViewXDColo->setModel(xdcolo);
    ui->tableViewXDColoLive->setModel(xdcoloLive);
    ui->tableViewFRLGE->setModel(frlge);
    ui->tableViewRS->setModel(rs);
    ui->tableViewRSLive->setModel(rsLive);

    ui->textBoxFRLGEPID->setValues(InputType::Seed32Bit);
    ui->textBoxFRLGETID->setValues(InputType::TIDSID);
    ui->textBoxFRLGESID->setValues(InputType::TIDSID);
    ui->textBoxFRLGEStartingAdvance->setValues(InputType::Advance32Bit);
    ui->textBoxFRLGEMaxAdvances->setValues(InputType::Advance32Bit);

    ui->textBoxRSPID->setValues(InputType::Seed32Bit);
    ui->textBoxRSTID->setValues(InputType::TIDSID);
    ui->textBoxRSSID->setValues(InputType::TIDSID);
    ui->textBoxRSInitialSeed->setValues(InputType::Seed16Bit);
    ui->textBoxRSStartingAdvance->setValues(InputType::Advance32Bit);
    ui->textBoxRSMaxAdvances->setValues(InputType::Advance32Bit);

    ui->textBoxXDColoPID->setValues(InputType::Seed32Bit);
    ui->textBoxXDColoTID->setValues(InputType::TIDSID);
    ui->textBoxXDColoSID->setValues(InputType::TIDSID);
    ui->textBoxXDColoSeed->setValues(InputType::Seed32Bit);
    ui->textBoxXDColoStartingAdvance->setValues(InputType::Advance32Bit);
    ui->textBoxXDColoMaxAdvances->setValues(InputType::Advance32Bit);

    ui->textBoxRSPIDLive->setValues(InputType::Seed32Bit);
    ui->textBoxRSTIDLive->setValues(InputType::TIDSID);

    ui->textBoxXDColoPIDLive->setValues(InputType::Seed32Bit);
    ui->textBoxXDColoTIDLive->setValues(InputType::TIDSID);

    connect(ui->pushButtonXDColoSearch, &QPushButton::clicked, this, &IDs3::xdColoSearch);
    connect(ui->pushButtonXDColoSearchLive, &QPushButton::clicked, this, &IDs3::xdColoSearchLive);
    connect(ui->pushButtonFRLGESearch, &QPushButton::clicked, this, &IDs3::frlgeSearch);
    connect(ui->pushButtonRSSearch, &QPushButton::clicked, this, &IDs3::rsSearch);
    connect(ui->pushButtonRSSearchLive, &QPushButton::clicked, this, &IDs3::rsSearchLive);
    connect(ui->checkBoxRSDeadBattery, &QCheckBox::clicked, this, &IDs3::rsDeadBattery);
    connect(ui->radioButtonRSDate, &QRadioButton::toggled, this, &IDs3::rsDate);
    connect(ui->radioButtonRSInitialSeed, &QRadioButton::toggled, this, &IDs3::rsInitialSeed);

    QSettings setting;
    if (setting.contains("ids3/geometry"))
    {
        this->restoreGeometry(setting.value("ids3/geometry").toByteArray());
    }
}

void IDs3::xdColoSearch()
{
    xdcolo->clearModel();

    u32 seed = ui->textBoxXDColoSeed->getUInt();
    u32 initialAdvances = ui->textBoxXDColoStartingAdvance->getUInt();
    u32 maxAdvances = ui->textBoxXDColoMaxAdvances->getUInt();

    std::vector<u16> tidFilter;
    std::vector<u16> sidFilter;
    std::vector<u16> tsvFilter;

    if (ui->checkBoxXDColoTID->isChecked())
    {
        tidFilter.emplace_back(ui->textBoxXDColoTID->getUShort());
    }

    if (ui->checkBoxXDColoSID->isChecked())
    {
        sidFilter.emplace_back(ui->textBoxXDColoSID->getUShort());
    }

    if (ui->checkBoxXDColoPID->isChecked())
    {
        u32 pid = ui->textBoxXDColoPID->getUInt();
        tsvFilter.emplace_back(((pid >> 16) ^ (pid & 0xffff)) >> 3);
    }

    IDFilter filter(tidFilter, sidFilter, tsvFilter);
    IDGenerator3 generator(initialAdvances, maxAdvances, filter);

    auto states = generator.generateXDColo(seed);
    xdcolo->addItems(states);
}

void IDs3::xdColoSearchLive()
{
    xdcoloLive->clearModel();

    std::vector<u16> tidFilter;
    std::vector<u16> sidFilter;
    std::vector<u16> tsvFilter;


    u16 tid = ui->textBoxXDColoTIDLive->getUShort();
    u32 pid = ui->textBoxXDColoPIDLive->getUInt();


    IDFilter filter(tidFilter, sidFilter, tsvFilter);
    IDGenerator3 generator(0, 0, filter);

    auto states = generator.generateXDColoLive(pid,tid);
    xdcoloLive->addItems(states);
}

void IDs3::frlgeSearch()
{
    frlge->clearModel();

    u16 tid = ui->textBoxFRLGETID->getUShort();
    u32 initialAdvances = ui->textBoxFRLGEStartingAdvance->getUInt();
    u32 maxAdvances = ui->textBoxFRLGEMaxAdvances->getUInt();

    std::vector<u16> sidFilter;
    std::vector<u16> tsvFilter;

    if (ui->checkBoxFRLGESID->isChecked())
    {
        sidFilter.emplace_back(ui->textBoxFRLGESID->getUShort());
    }

    if (ui->checkBoxFRLGEPID->isChecked())
    {
        u32 pid = ui->textBoxFRLGEPID->getUInt();
        tsvFilter.emplace_back(((pid >> 16) ^ (pid & 0xffff)) >> 3);
    }

    IDFilter filter({ tid }, sidFilter, tsvFilter);
    IDGenerator3 generator(initialAdvances, maxAdvances, filter);

    auto states = generator.generateFRLGE(tid);
    frlge->addItems(states);
}

void IDs3::rsSearch()
{
    rs->clearModel();

    u16 seed;
    if (ui->checkBoxRSDeadBattery->isChecked())
    {
        seed = 0x5a0;
    }
    else if (ui->radioButtonRSDate->isChecked())
    {
        seed = Utilities::calcGen3Seed(ui->dateTimeEdit->getDateTime());
    }
    else
    {
        seed = ui->textBoxRSInitialSeed->getUShort();
    }

    u32 initialAdvances = ui->textBoxRSStartingAdvance->getUInt();
    u32 maxAdvances = ui->textBoxRSMaxAdvances->getUInt();

    std::vector<u16> tidFilter;
    std::vector<u16> sidFilter;
    std::vector<u16> tsvFilter;

    if (ui->checkBoxRSTID->isChecked())
    {
        tidFilter.emplace_back(ui->textBoxRSTID->getUShort());
    }

    if (ui->checkBoxRSSID->isChecked())
    {
        sidFilter.emplace_back(ui->textBoxRSSID->getUShort());
    }

    if (ui->checkBoxRSPID->isChecked())
    {
        u32 pid = ui->textBoxRSPID->getUInt();
        tsvFilter.emplace_back(((pid >> 16) ^ (pid & 0xffff)) >> 3);
    }

    IDFilter filter(tidFilter, sidFilter, tsvFilter);
    IDGenerator3 generator(initialAdvances, maxAdvances, filter);

    auto states = generator.generateRS(seed);
    rs->addItems(states);
}

void IDs3::rsDeadBattery(bool checked)
{
    ui->radioButtonRSDate->setEnabled(!checked);
    ui->radioButtonRSInitialSeed->setEnabled(!checked);
    ui->dateTimeEdit->setEnabled(!checked ? ui->radioButtonRSDate->isChecked() : false);
    ui->textBoxRSInitialSeed->setEnabled(!checked ? ui->radioButtonRSInitialSeed->isChecked() : false);
}

void IDs3::rsDate(bool checked)
{
    ui->dateTimeEdit->setEnabled(checked);
}

void IDs3::rsInitialSeed(bool checked)
{
    ui->textBoxRSInitialSeed->setEnabled(checked);
}

void IDs3::rsSearchLive()
{
    rsLive->clearModel();

    std::vector<u16> tidFilter;
    std::vector<u16> sidFilter;
    std::vector<u16> tsvFilter;


    u16 tid = ui->textBoxRSTIDLive->getUShort();
    u32 pid = ui->textBoxRSPIDLive->getUInt();


    IDFilter filter(tidFilter, sidFilter, tsvFilter);
    IDGenerator3 generator(0, 0, filter);

    auto states = generator.generateRSLive(pid,tid);
    rsLive->addItems(states);
}
