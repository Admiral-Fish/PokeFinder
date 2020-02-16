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
#include <Core/Gen3/Searchers/GameCubeRTCSearcher.hpp>
#include <Models/Gen3/GameCubeRTCModel.hpp>
#include <QClipboard>
#include <QSettings>
#include <QThread>

GameCubeRTC::GameCubeRTC(QWidget *parent) : QWidget(parent), ui(new Ui::GameCubeRTC)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();

    qRegisterMetaType<QVector<GameCubeRTCFrame>>("QVector<GameCubeRTCFrame>");
}

GameCubeRTC::GameCubeRTC(u32 seed, QWidget *parent) : QWidget(parent), ui(new Ui::GameCubeRTC)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
    ui->textBoxTargetSeed->setText(QString::number(seed, 16));

    qRegisterMetaType<QVector<GameCubeRTCFrame>>("QVector<GameCubeRTCFrame>");
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
    contextMenu = new QMenu(ui->tableView);
    ui->tableView->setModel(model);

    ui->textBoxStartSeed->setValues(InputType::Seed32Bit);
    ui->textBoxTargetSeed->setValues(InputType::Seed32Bit);
    ui->textBoxMinFrame->setValues(InputType::Frame32Bit);
    ui->textBoxMaxFrame->setValues(InputType::Frame32Bit);

    QAction *copySeed = contextMenu->addAction(tr("Copy Seed to Clipboard"));
    connect(copySeed, &QAction::triggered, this, &GameCubeRTC::copySeed);

    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &GameCubeRTC::search);
    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &GameCubeRTC::tableViewContextMenu);

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
    model->removeRows(0, model->rowCount());

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u32 initialSeed = ui->textBoxStartSeed->getUInt();
    u32 targetSeed = ui->textBoxTargetSeed->getUInt();
    u32 initialFrame = ui->textBoxMinFrame->getUInt();
    u32 maxResults = ui->textBoxMaxFrame->getUInt();

    auto *searcher = new GameCubeRTCSearcher();
    auto *thread = QThread::create([=] { searcher->startSearch(initialSeed, targetSeed, initialFrame, maxResults); });

    connect(thread, &QThread::finished, this, [=] {
        ui->pushButtonSearch->setEnabled(true);
        ui->pushButtonCancel->setEnabled(false);
    });
    connect(ui->pushButtonCancel, &QPushButton::clicked, [=] { searcher->cancelSearch(); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(thread, &QThread::destroyed, [=] {
        updateTableView(searcher->getResults());
        delete searcher;
    });

    thread->start();
}

void GameCubeRTC::updateTableView(const QVector<GameCubeRTCFrame> &results)
{
    model->addItems(results);
}

void GameCubeRTC::copySeed()
{
    QVariant data = ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 2));
    QApplication::clipboard()->setText(data.toString());
}

void GameCubeRTC::tableViewContextMenu(QPoint pos)
{
    if (model->rowCount() == 0)
    {
        return;
    }

    contextMenu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}
