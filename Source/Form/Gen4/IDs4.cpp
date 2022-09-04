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

#include "IDs4.hpp"
#include "ui_IDs4.h"
#include <Core/Gen4/Generators/IDGenerator4.hpp>
#include <Core/Gen4/Searchers/IDSearcher4.hpp>
#include <Model/Gen4/IDModel4.hpp>
#include <QSettings>
#include <QThread>
#include <QTimer>

IDs4::IDs4(QWidget *parent) : QWidget(parent), ui(new Ui::IDs4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    searcherModel = new IDModel4(ui->tableViewSearcher, false);
    ui->tableViewSearcher->setModel(searcherModel);

    seedFinderModel = new IDModel4(ui->tableViewSeedFinder, true);
    ui->tableViewSeedFinder->setModel(seedFinderModel);

    ui->textBoxYear->setValues(2000, 2099, 4, 10);
    ui->textBoxSearcherMinDelay->setValues(InputType::Delay);
    ui->textBoxSearcherMaxDelay->setValues(InputType::Delay);

    ui->textBoxTID->setValues(InputType::TIDSID);
    ui->textBoxSeedFinderMinDelay->setValues(InputType::Delay);
    ui->textBoxSeedFinderMaxDelay->setValues(InputType::Delay);

    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &IDs4::search);
    connect(ui->pushButtonFind, &QPushButton::clicked, this, &IDs4::find);

    QSettings setting;
    if (setting.contains("ids4/geometry"))
    {
        this->restoreGeometry(setting.value("ids4/geometry").toByteArray());
    }
}

IDs4::~IDs4()
{
    QSettings setting;
    setting.setValue("ids4/geometry", this->saveGeometry());

    delete ui;
}

void IDs4::find()
{
    seedFinderModel->clearModel();

    QDateTime dateTime = ui->dateTimeEdit->dateTime();
    u8 month = static_cast<u8>(dateTime.date().month());
    u8 day = static_cast<u8>(dateTime.date().day());
    u16 year = static_cast<u16>(dateTime.date().year());
    u8 hour = static_cast<u8>(dateTime.time().hour());
    u8 minute = static_cast<u8>(dateTime.time().minute());
    u32 minDelay = ui->textBoxSeedFinderMinDelay->getUInt() + year - 2000;
    u32 maxDelay = ui->textBoxSeedFinderMaxDelay->getUInt() + year - 2000;

    IDFilter filter({ ui->textBoxTID->getUShort() }, {}, {}, {});
    IDGenerator4 generator(minDelay, maxDelay, year, month, day, hour, minute, filter);

    auto states = generator.generate();
    seedFinderModel->addItems(states);
}

void IDs4::search()
{
    searcherModel->clearModel();

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u16 year = ui->textBoxYear->getUShort();
    u32 minDelay = ui->textBoxSearcherMinDelay->getUInt() + year - 2000;
    u32 maxDelay = ui->textBoxSearcherMaxDelay->getUInt() + year - 2000;
    bool infinite = ui->checkBoxInfiniteSearch->isChecked();

    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(static_cast<int>(256 * 24 * (infinite ? 0xE8FFFF : (maxDelay - minDelay + 1))));

    IDFilter filter = ui->idFilter->getFilter();
    auto *searcher = new IDSearcher4(filter);

    auto *thread = QThread::create([=] { searcher->startSearch(infinite, year, minDelay, maxDelay); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(ui->pushButtonCancel, &QPushButton::clicked, [searcher] { searcher->cancelSearch(); });

    auto *timer = new QTimer();
    connect(timer, &QTimer::timeout, this, [=] {
        searcherModel->addItems(searcher->getResults());
        ui->progressBar->setValue(searcher->getProgress());
    });
    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed, this, [=] {
        ui->pushButtonSearch->setEnabled(true);
        ui->pushButtonCancel->setEnabled(false);
        searcherModel->addItems(searcher->getResults());
        ui->progressBar->setValue(searcher->getProgress());
        delete searcher;
    });

    thread->start();
    timer->start(1000);
}
