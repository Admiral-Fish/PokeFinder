/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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

GameCubeRTC::GameCubeRTC(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GameCubeRTC)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);

    setupModels();

    qRegisterMetaType<QList<QStandardItem *>>("QList<QStandardItem *>");
    connect(this, SIGNAL(updateView(QList<QStandardItem *>)), this, SLOT(updateTableView(QList<QStandardItem *>)));
}

GameCubeRTC::~GameCubeRTC()
{
    delete ui;
    delete model;
    delete contextMenu;
}

void GameCubeRTC::changeEvent(QEvent *event)
{
    if (event != NULL)
    {
        switch (event->type())
        {
            case QEvent::LanguageChange:
                ui->retranslateUi(this);
                break;
            default:
                break;
        }
    }
}

void GameCubeRTC::setupModels()
{
    ui->textBoxStartSeed->setValues(0, 32, false);
    ui->textBoxEndSeed->setValues(0, 32, false);
    ui->textBoxMinFrame->setValues(1, 32, true);
    ui->textBoxMaxFrame->setValues(2, 32, true);

    model->setHorizontalHeaderLabels(QStringList() << tr("Time") << tr("Frame") << tr("Seed"));
    ui->tableViewGenerator->setModel(model);
    ui->tableViewGenerator->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QAction *copySeed = new QAction("Copy Seed to Clipboard", this);

    connect(copySeed, &QAction::triggered, this, &GameCubeRTC::copySeed);

    contextMenu->addAction(copySeed);
}

void GameCubeRTC::updateTableView(QList<QStandardItem *> row)
{
    model->appendRow(row);
}

void GameCubeRTC::calcRTC()
{
    isSearching = true;

    u32 initSeed = ui->textBoxStartSeed->text().toUInt(NULL, 16);
    u32 targetSeed = ui->textBoxEndSeed->text().toUInt(NULL, 16);
    u32 minFrame = ui->textBoxMinFrame->text().toUInt(NULL, 10);
    u32 maxFrame = ui->textBoxMaxFrame->text().toUInt(NULL, 10);

    LCRNG back = XDRNGR(targetSeed);
    back.advanceFrames(minFrame);
    targetSeed = back.seed;

    LCRNG rng = XDRNG(initSeed);

    u32 seconds = 0;
    u32 secoundCount = 0;
    bool targetHit = false;
    int minutes = 0;

    while (!targetHit)
    {
        rng.seed = initSeed;

        for (u32 x = 0; x < maxFrame; x++)
        {
            if (rng.nextUInt() == targetSeed)
            {
                QDateTime finalTime = date.addSecs(seconds);
                QList<QStandardItem *> row;
                QString time = finalTime.toString(Qt::SystemLocaleShortDate);
                row << (time.contains("M") ? new QStandardItem(time.insert((time.indexOf('M') - 2), ":" + QString::number(finalTime.time().second()))) : new QStandardItem(time.append(":" + QString::number(finalTime.time().second()))))
                    << new QStandardItem(QString::number(x + 2 + minFrame)) << new QStandardItem(QString::number(initSeed, 16).toUpper());
                emit updateView(row);
                isSearching = false;
                return;
            }
        }

        initSeed += 40500000;
        seconds += 1;
        secoundCount += 1;

        if (secoundCount == 60)
        {
            minutes += 1;
            secoundCount = 0;
        }
    }
}

void GameCubeRTC::on_pushButtonSearch_clicked()
{
    if (isSearching)
        return;
    model->removeRows(0, model->rowCount());
    ui->tableViewGenerator->viewport()->update();

    std::thread job(&GameCubeRTC::calcRTC, this);
    job.detach();
}

void GameCubeRTC::copySeed()
{
    QApplication::clipboard()->setText(ui->tableViewGenerator->model()->data(ui->tableViewGenerator->model()->index(lastIndex.row(), 2)).toString());
}

void GameCubeRTC::on_tableViewGenerator_customContextMenuRequested(const QPoint &pos)
{
    if (model->rowCount() == 0)
        return;

    lastIndex = ui->tableViewGenerator->indexAt(pos);

    contextMenu->popup(ui->tableViewGenerator->viewport()->mapToGlobal(pos));
}
