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
    frlge = new IDModel3(ui->tableViewFRLGE);
    rs = new IDModel3(ui->tableViewFRLGE);

    ui->tableViewXDColo->setModel(xdcolo);
    ui->tableViewFRLGE->setModel(frlge);
    ui->tableViewRS->setModel(rs);

    ui->textBoxFRLGEPID->setValues(InputType::Seed32Bit);
    ui->textBoxFRLGETID->setValues(InputType::TIDSID);
    ui->textBoxFRLGESID->setValues(InputType::TIDSID);
    ui->textBoxFRLGEStartingFrame->setValues(InputType::Frame32Bit);
    ui->textBoxFRLGEMaxResults->setValues(InputType::Frame32Bit);

    ui->textBoxRSPID->setValues(InputType::Seed32Bit);
    ui->textBoxRSTID->setValues(InputType::TIDSID);
    ui->textBoxRSSID->setValues(InputType::TIDSID);
    ui->textBoxRSInitialSeed->setValues(InputType::Seed16Bit);
    ui->textBoxRSStartingFrame->setValues(InputType::Frame32Bit);
    ui->textBoxRSMaxResults->setValues(InputType::Frame32Bit);

    ui->textBoxXDColoPID->setValues(InputType::Seed32Bit);
    ui->textBoxXDColoTID->setValues(InputType::TIDSID);
    ui->textBoxXDColoSID->setValues(InputType::TIDSID);
    ui->textBoxXDColoSeed->setValues(InputType::Seed32Bit);
    ui->textBoxXDColoStartingFrame->setValues(InputType::Frame32Bit);
    ui->textBoxXDColoMaxResults->setValues(InputType::Frame32Bit);

    ui->dateTimeEdit->setDisplayFormat(QLocale::system().dateTimeFormat(QLocale::ShortFormat));

    connect(ui->pushButtonXDColoSearch, &QPushButton::clicked, this, &IDs3::xdColoSearch);
    connect(ui->pushButtonFRLGESearch, &QPushButton::clicked, this, &IDs3::frlgeSearch);
    connect(ui->pushButtonRSSearch, &QPushButton::clicked, this, &IDs3::rsSearch);
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
    u32 initialFrame = ui->textBoxXDColoStartingFrame->getUInt();
    u32 maxResults = ui->textBoxXDColoMaxResults->getUInt();

    QVector<u16> tidFilter;
    QVector<u16> sidFilter;
    QVector<u16> tsvFilter;

    if (ui->checkBoxXDColoTID->isChecked())
    {
        tidFilter.append(ui->textBoxXDColoTID->getUShort());
    }

    if (ui->checkBoxXDColoSID->isChecked())
    {
        sidFilter.append(ui->textBoxXDColoSID->getUShort());
    }

    if (ui->checkBoxXDColoPID->isChecked())
    {
        u32 pid = ui->textBoxXDColoPID->getUInt();
        tsvFilter.append(((pid >> 16) ^ (pid & 0xffff)) >> 3);
    }

    IDFilter filter(tidFilter, sidFilter, tsvFilter);
    IDGenerator3 generator(initialFrame, maxResults, filter);

    auto frames = generator.generateXDColo(seed);
    xdcolo->addItems(frames);
}

void IDs3::frlgeSearch()
{
    frlge->clearModel();

    u16 tid = ui->textBoxFRLGETID->getUShort();
    u32 initialFrame = ui->textBoxFRLGEStartingFrame->getUInt();
    u32 maxResults = ui->textBoxFRLGEMaxResults->getUInt();

    QVector<u16> sidFilter;
    QVector<u16> tsvFilter;

    if (ui->checkBoxFRLGESID->isChecked())
    {
        sidFilter.append(ui->textBoxFRLGESID->getUShort());
    }

    if (ui->checkBoxFRLGEPID->isChecked())
    {
        u32 pid = ui->textBoxFRLGEPID->getUInt();
        tsvFilter.append(((pid >> 16) ^ (pid & 0xffff)) >> 3);
    }

    IDFilter filter({ tid }, sidFilter, tsvFilter);
    IDGenerator3 generator(initialFrame, maxResults, filter);
    generator.setStaticTID(tid);

    auto frames = generator.generateFRLGE(tid);
    frlge->addItems(frames);
}

void IDs3::rsSearch()
{
    rs->clearModel();

    u16 seed;
    if (ui->radioButtonRSInitialSeed->isChecked())
    {
        seed = ui->textBoxRSInitialSeed->getUShort();
    }
    else
    {
        seed = Utilities::calcGen3Seed(ui->dateTimeEdit->dateTime());
    }
    u32 initialFrame = ui->textBoxRSStartingFrame->getUInt();
    u32 maxResults = ui->textBoxRSMaxResults->getUInt();

    QVector<u16> tidFilter;
    QVector<u16> sidFilter;
    QVector<u16> tsvFilter;

    if (ui->checkBoxRSTID->isChecked())
    {
        tidFilter.append(ui->textBoxRSTID->getUShort());
    }

    if (ui->checkBoxRSSID->isChecked())
    {
        sidFilter.append(ui->textBoxRSSID->getUShort());
    }

    if (ui->checkBoxRSPID->isChecked())
    {
        u32 pid = ui->textBoxRSPID->getUInt();
        tsvFilter.append(((pid >> 16) ^ (pid & 0xffff)) >> 3);
    }

    IDFilter filter(tidFilter, sidFilter, tsvFilter);
    IDGenerator3 generator(initialFrame, maxResults, filter);

    auto frames = generator.generateRS(seed);
    rs->addItems(frames);
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
