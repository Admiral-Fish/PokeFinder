/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
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
    saveSettings();

    delete ui;
    delete model;
    delete contextMenu;
}

void GameCubeRTC::changeEvent(QEvent *event)
{
    if (event)
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

    QAction *copySeed = new QAction("Copy Seed to Clipboard", this);
    connect(copySeed, &QAction::triggered, this, &GameCubeRTC::copySeed);
    contextMenu->addAction(copySeed);

    loadSettings();
}

void GameCubeRTC::saveSettings()
{
    QSettings setting;
    setting.setValue("startSeed", ui->textBoxStartSeed->text());
}

void GameCubeRTC::loadSettings()
{
    QSettings setting;
    if (setting.contains("startSeed")) ui->textBoxStartSeed->setText(setting.value("startSeed").toString());
}

void GameCubeRTC::updateTableView(QList<QStandardItem *> row)
{
    model->appendRow(row);
}

void GameCubeRTC::calcRTC()
{
    u32 initSeed = ui->textBoxStartSeed->text().toUInt(nullptr, 16);
    u32 targetSeed = ui->textBoxEndSeed->text().toUInt(nullptr, 16);
    u32 minFrame = ui->textBoxMinFrame->text().toUInt();
    u32 maxFrame = ui->textBoxMaxFrame->text().toUInt();

    XDRNGR back(targetSeed);
    back.advanceFrames(minFrame);
    targetSeed = back.getSeed();

    XDRNG rng(initSeed);

    u32 seconds = 0;
    u32 secoundCount = 0;
    bool targetHit = false;
    int minutes = 0;

    while (!targetHit && !cancel)
    {
        rng.setSeed(initSeed);

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
                targetHit = true;
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
    isSearching = false;
    ui->pushButtonSearch->setText(tr("Search"));
}

void GameCubeRTC::on_pushButtonSearch_clicked()
{
    if (isSearching)
    {
        cancel = true;
    }
    else
    {
        cancel = false;
        isSearching = true;
        model->removeRows(0, model->rowCount());
        ui->pushButtonSearch->setText(tr("Cancel"));

        std::thread job(&GameCubeRTC::calcRTC, this);
        job.detach();
    }
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
