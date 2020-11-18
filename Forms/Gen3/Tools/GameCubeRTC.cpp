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

#include "GameCubeRTC.hpp"
#include "ui_GameCubeRTC.h"
#include <Core/Gen3/Searchers/RTCSearcher.hpp>
#include <Models/Gen3/GameCubeRTCModel.hpp>
#include <QSettings>
#include <QThread>
#include <QTimer>

GameCubeRTC::GameCubeRTC(QWidget *parent) : QWidget(parent), ui(new Ui::GameCubeRTC)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

GameCubeRTC::GameCubeRTC(u32 seed, QWidget *parent) : QWidget(parent), ui(new Ui::GameCubeRTC)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
    ui->textBoxTargetSeed->setText(QString::number(seed, 16));
}

GameCubeRTC::~GameCubeRTC()
{
    QSettings setting;
    setting.beginGroup("gamecubeRTC");
    setting.setValue("seed", ui->textBoxStartSeed->text());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void GameCubeRTC::setupModels()
{
    model = new GameCubeRTCModel(ui->tableView);
    ui->tableView->setModel(model);

    ui->textBoxStartSeed->setValues(InputType::Seed32Bit);
    ui->textBoxTargetSeed->setValues(InputType::Seed32Bit);
    ui->textBoxMinAdvance->setValues(InputType::Advance32Bit);
    ui->textBoxMaxAdvance->setValues(InputType::Advance32Bit);

    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &GameCubeRTC::search);

    QSettings setting;
    setting.beginGroup("gamecubeRTC");
    if (setting.contains("seed"))
    {
        ui->textBoxStartSeed->setText(setting.value("seed").toString());
    }
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

void GameCubeRTC::search()
{
    model->clearModel();

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u32 initialSeed = ui->textBoxStartSeed->getUInt();
    u32 targetSeed = ui->textBoxTargetSeed->getUInt();
    u32 initialAdvances = ui->textBoxMinAdvance->getUInt();
    u32 maxAdvances = ui->textBoxMaxAdvance->getUInt();
    DateTime end = ui->dateTimeEdit->getDateTime();

    auto *searcher = new RTCSearcher();

    auto *thread = QThread::create([=] { searcher->startSearch(initialSeed, targetSeed, initialAdvances, maxAdvances, end); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(ui->pushButtonCancel, &QPushButton::clicked, [searcher] { searcher->cancelSearch(); });

    auto *timer = new QTimer();
    connect(timer, &QTimer::timeout, [=] { updateProgress(searcher->getResults()); });
    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed, [=] {
        ui->pushButtonSearch->setEnabled(true);
        ui->pushButtonCancel->setEnabled(false);
        updateProgress(searcher->getResults());
        delete searcher;
    });

    thread->start();
    timer->start(1000);
}

void GameCubeRTC::updateProgress(const std::vector<GameCubeRTCState> &results)
{
    model->addItems(results);
}
