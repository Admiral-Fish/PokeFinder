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

#include "IDs4.hpp"
#include "ui_IDs4.h"
#include <Core/Gen4/Generators/IDGenerator4.hpp>
#include <Core/Gen4/Searchers/IDSearcher4.hpp>
#include <Core/Gen4/States/EggState4.hpp>
#include <Models/Gen4/IDModel4.hpp>
#include <QSettings>
#include <QThread>
#include <QTimer>

IDs4::IDs4(QWidget *parent) : QWidget(parent), ui(new Ui::IDs4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    setupModels();
}

IDs4::~IDs4()
{
    QSettings setting;
    setting.setValue("ids4/geometry", this->saveGeometry());

    delete ui;
}

void IDs4::setupModels()
{
    shinyPID = new IDModel4(ui->tableViewShinyPID, false);
    ui->tableViewShinyPID->setModel(shinyPID);

    tidSID = new IDModel4(ui->tableViewTIDSID, false);
    ui->tableViewTIDSID->setModel(tidSID);

    seedFinder = new IDModel4(ui->tableViewSeedFinder, true);
    ui->tableViewSeedFinder->setModel(seedFinder);

    ui->textBoxTIDSIDTID->setValues(InputType::TIDSID);
    ui->textBoxTIDSIDSID->setValues(InputType::TIDSID);
    ui->textBoxTIDSIDYear->setValues(2000, 2099, 4, 10);
    ui->textBoxTIDSIDMinDelay->setValues(InputType::Delay);
    ui->textBoxTIDSIDMaxDelay->setValues(InputType::Delay);

    ui->textBoxShinyPIDPID->setValues(InputType::Seed32Bit);
    ui->textBoxShinyPIDTID->setValues(InputType::TIDSID);
    ui->textBoxShinyPIDYear->setValues(2000, 2099, 4, 10);
    ui->textBoxShinyPIDMinDelay->setValues(InputType::Delay);
    ui->textBoxShinyPIDMaxDelay->setValues(InputType::Delay);

    ui->textBoxSeedFinderTID->setValues(InputType::TIDSID);
    ui->textBoxSeedFinderMinDelay->setValues(InputType::Delay);
    ui->textBoxSeedFinderMaxDelay->setValues(InputType::Delay);

    connect(ui->pushButtonShinyPIDSearch, &QPushButton::clicked, this, &IDs4::shinyPIDSearch);
    connect(ui->pushButtonTIDSIDSearch, &QPushButton::clicked, this, &IDs4::tidSIDSearch);
    connect(ui->pushButtonSeedFinderSearch, &QPushButton::clicked, this, &IDs4::seedFinderSearch);

    QSettings setting;
    if (setting.contains("ids4/geometry"))
    {
        this->restoreGeometry(setting.value("ids4/geometry").toByteArray());
    }
}

void IDs4::shinyPIDSearch()
{
    shinyPID->clearModel();

    ui->pushButtonShinyPIDSearch->setEnabled(false);
    ui->pushButtonShinyPIDCancel->setEnabled(true);

    std::vector<u16> tidList;
    if (ui->checkBoxShinyPIDSearchTID->isChecked())
    {
        tidList.emplace_back(ui->textBoxShinyPIDTID->getUShort());
    }

    u32 pid = ui->textBoxShinyPIDPID->getUInt();
    u16 psv = ((pid >> 16) ^ (pid & 0xffff)) >> 3;
    std::vector<u16> psvList = { psv };

    IDFilter filter(tidList, std::vector<u16>(), psvList);

    u16 year = ui->textBoxShinyPIDYear->getUShort();
    u32 minDelay = ui->textBoxShinyPIDMinDelay->getUInt() + year - 2000;
    u32 maxDelay = ui->textBoxShinyPIDMaxDelay->getUInt() + year - 2000;
    bool infinite = ui->checkBoxShinyPIDInfiniteSearch->isChecked();

    ui->progressBarShinyPID->setValue(0);
    ui->progressBarShinyPID->setMaximum(static_cast<int>(256 * 24 * (infinite ? 0xE8FFFF : (maxDelay - minDelay + 1))));

    auto *searcher = new IDSearcher4(filter);

    auto *thread = QThread::create([=] { searcher->startSearch(infinite, year, minDelay, maxDelay); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(ui->pushButtonShinyPIDCancel, &QPushButton::clicked, [searcher] { searcher->cancelSearch(); });

    auto *timer = new QTimer();
    connect(timer, &QTimer::timeout, [=] {
        shinyPID->addItems(searcher->getResults());
        ui->progressBarShinyPID->setValue(searcher->getProgress());
    });
    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed, [=] {
        ui->pushButtonShinyPIDSearch->setEnabled(true);
        ui->pushButtonShinyPIDCancel->setEnabled(false);
        shinyPID->addItems(searcher->getResults());
        ui->progressBarShinyPID->setValue(searcher->getProgress());
        delete searcher;
    });

    thread->start();
    timer->start(1000);
}

void IDs4::tidSIDSearch()
{
    tidSID->clearModel();

    ui->pushButtonTIDSIDSearch->setEnabled(false);
    ui->pushButtonTIDSIDCancel->setEnabled(true);

    std::vector<u16> tidList = { ui->textBoxTIDSIDTID->getUShort() };

    std::vector<u16> sidList;
    if (ui->checkBoxTIDSIDSearchSID->isChecked())
    {
        sidList.emplace_back(ui->textBoxTIDSIDSID->getUShort());
    }

    IDFilter filter(tidList, sidList, std::vector<u16>());

    u16 year = ui->textBoxTIDSIDYear->getUShort();
    u32 minDelay = ui->textBoxTIDSIDMinDelay->getUInt() + year - 2000;
    u32 maxDelay = ui->textBoxTIDSIDMaxDelay->getUInt() + year - 2000;
    bool infinite = ui->checkBoxTIDSIDInfiniteSearch->isChecked();

    ui->progressBarTIDSID->setValue(0);
    ui->progressBarTIDSID->setMaximum(static_cast<int>(256 * 24 * (infinite ? 0xE8FFFF : (maxDelay - minDelay + 1))));

    auto *searcher = new IDSearcher4(filter);

    auto *thread = QThread::create([=] { searcher->startSearch(infinite, year, minDelay, maxDelay); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(ui->pushButtonTIDSIDCancel, &QPushButton::clicked, [searcher] { searcher->cancelSearch(); });

    auto *timer = new QTimer();
    connect(timer, &QTimer::timeout, [=] {
        tidSID->addItems(searcher->getResults());
        ui->progressBarTIDSID->setValue(searcher->getProgress());
    });
    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed, [=] {
        ui->pushButtonTIDSIDSearch->setEnabled(true);
        ui->pushButtonTIDSIDCancel->setEnabled(false);
        tidSID->addItems(searcher->getResults());
        ui->progressBarTIDSID->setValue(searcher->getProgress());
        delete searcher;
    });

    thread->start();
    timer->start(1000);
}

void IDs4::seedFinderSearch()
{
    seedFinder->clearModel();

    QDateTime dateTime = ui->dateTimeEdit->dateTime();
    u8 month = static_cast<u8>(dateTime.date().month());
    u8 day = static_cast<u8>(dateTime.date().day());
    u16 year = static_cast<u16>(dateTime.date().year());
    u8 hour = static_cast<u8>(dateTime.time().hour());
    u8 minute = static_cast<u8>(dateTime.time().minute());
    u32 minDelay = ui->textBoxSeedFinderMinDelay->getUInt() + year - 2000;
    u32 maxDelay = ui->textBoxSeedFinderMaxDelay->getUInt() + year - 2000;

    IDGenerator4 generator(minDelay, maxDelay, year, month, day, hour, minute);
    IDFilter filter({ ui->textBoxSeedFinderTID->getUShort() }, std::vector<u16>(), std::vector<u16>());

    auto states = generator.generate(filter);
    seedFinder->addItems(states);
}
