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
}

GameCubeRTC::~GameCubeRTC()
{
    QSettings setting;
    setting.setValue("startSeed", ui->textBoxStartSeed->text());

    delete ui;
    delete model;
    delete contextMenu;
}

void GameCubeRTC::setupModels()
{
    ui->textBoxStartSeed->setValues(InputType::Seed32Bit);
    ui->textBoxTargetSeed->setValues(InputType::Seed32Bit);
    ui->textBoxMinFrame->setValues(InputType::Frame32Bit);
    ui->textBoxMaxFrame->setValues(InputType::Frame32Bit);

    model->setHorizontalHeaderLabels(QStringList() << tr("Time") << tr("Frame") << tr("Seed"));
    ui->tableViewGenerator->setModel(model);

    QAction *copySeed = new QAction("Copy Seed to Clipboard", this);
    connect(copySeed, &QAction::triggered, this, &GameCubeRTC::copySeed);
    contextMenu->addAction(copySeed);

    QSettings setting;
    if (setting.contains("startSeed")) ui->textBoxStartSeed->setText(setting.value("startSeed").toString());
}

void GameCubeRTC::on_pushButtonSearch_clicked()
{
    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u32 initial = ui->textBoxStartSeed->text().toUInt(nullptr, 16);
    u32 target = ui->textBoxTargetSeed->text().toUInt(nullptr, 16);
    u32 start = ui->textBoxMinFrame->text().toUInt();
    u32 end = ui->textBoxMaxFrame->text().toUInt();

    auto *search = new Search(initial, target, start, end);

    connect(search, &Search::finished, this, [ = ] { ui->pushButtonSearch->setEnabled(true); ui->pushButtonCancel->setEnabled(false); });
    connect(search, &Search::result, this, &GameCubeRTC::updateTableView);
    connect(ui->pushButtonCancel, &QPushButton::clicked, search, &Search::cancelSearch);

    search->start();
}

void GameCubeRTC::updateTableView(const QList<QStandardItem *> &row)
{
    model->appendRow(row);
}

void GameCubeRTC::copySeed()
{
    QApplication::clipboard()->setText(ui->tableViewGenerator->model()->data(ui->tableViewGenerator->model()->index(lastIndex.row(), 2)).toString());
}

void GameCubeRTC::on_tableViewGenerator_customContextMenuRequested(const QPoint &pos)
{
    if (model->rowCount() == 0)
    {
        return;
    }

    lastIndex = ui->tableViewGenerator->indexAt(pos);
    contextMenu->popup(ui->tableViewGenerator->viewport()->mapToGlobal(pos));
}

Search::Search(u32 initialSeed, u32 targetSeed, u32 minFrame, u32 maxFrame)
{
    this->initialSeed = initialSeed;
    this->targetSeed = targetSeed;
    this->minFrame = minFrame;
    this->maxFrame = maxFrame;
    cancel = false;

    connect(this, &QThread::finished, this, &QThread::deleteLater);
}

void Search::run()
{
    XDRNGR back(targetSeed, minFrame);
    targetSeed = back.getSeed();

    u32 seconds = 0;
    u32 minutes = 0;
    u8 secoundCount = 0;

    while (!cancel)
    {
        XDRNG rng(initialSeed);

        for (u32 x = 0; x < maxFrame; x++)
        {
            if (rng.nextUInt() == targetSeed)
            {
                QDateTime finalTime = date.addSecs(seconds);
                QList<QStandardItem *> row;
                QString time = finalTime.toString(Qt::SystemLocaleShortDate);
                row << (time.contains("M") ? new QStandardItem(time.insert((time.indexOf('M') - 2), ":" + QString::number(finalTime.time().second()))) : new QStandardItem(time.append(":" + QString::number(finalTime.time().second()))))
                    << new QStandardItem(QString::number(x + 2 + minFrame)) << new QStandardItem(QString::number(initialSeed, 16).toUpper());
                emit result(row);
                return;
            }
        }

        initialSeed += 40500000;
        seconds++;
        secoundCount++;

        if (secoundCount == 60)
        {
            minutes++;
            secoundCount = 0;
        }
    }
}

void Search::cancelSearch()
{
    cancel = true;
}
