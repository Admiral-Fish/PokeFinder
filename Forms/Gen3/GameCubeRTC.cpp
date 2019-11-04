/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2019 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Gen3/GameCubeRTCSearcher.hpp>
#include <QClipboard>
#include <QSettings>

namespace PokeFinderForms
{
    GameCubeRTC::GameCubeRTC(QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::GameCubeRTC)
    {
        ui->setupUi(this);
        setAttribute(Qt::WA_QuitOnClose, false);
        setAttribute(Qt::WA_DeleteOnClose);

        setupModels();

        qRegisterMetaType<QList<QStandardItem *>>("QList<QStandardItem *>");
    }

    GameCubeRTC::GameCubeRTC(u32 seed, QWidget *parent)
        : QWidget(parent)
        , ui(new Ui::GameCubeRTC)
    {
        ui->setupUi(this);
        setAttribute(Qt::WA_QuitOnClose, false);
        setAttribute(Qt::WA_DeleteOnClose);

        setupModels();
        ui->textBoxTargetSeed->setText(QString::number(seed, 16));

        qRegisterMetaType<QList<QStandardItem *>>("QList<QStandardItem *>");
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
        model = new QStandardItemModel(ui->tableView);
        contextMenu = new QMenu(ui->tableView);

        ui->textBoxStartSeed->setValues(InputType::Seed32Bit);
        ui->textBoxTargetSeed->setValues(InputType::Seed32Bit);
        ui->textBoxMinFrame->setValues(InputType::Frame32Bit);
        ui->textBoxMaxFrame->setValues(InputType::Frame32Bit);

        model->setHorizontalHeaderLabels(QStringList() << tr("Time") << tr("Frame") << tr("Seed"));
        ui->tableView->setModel(model);

        QAction *copySeed = contextMenu->addAction(tr("Copy Seed to Clipboard"));
        connect(copySeed, &QAction::triggered, this, &GameCubeRTC::copySeed);

        QSettings setting;
        setting.beginGroup("gamecubeRTC");
        if (setting.contains("seed"))
            ui->textBoxStartSeed->setText(setting.value("seed").toString());
        if (setting.contains("geometry"))
            this->restoreGeometry(setting.value("geometry").toByteArray());
        setting.endGroup();
    }

    void GameCubeRTC::on_pushButtonSearch_clicked()
    {
        model->removeRows(0, model->rowCount());

        ui->pushButtonSearch->setEnabled(false);
        ui->pushButtonCancel->setEnabled(true);

        u32 initial = ui->textBoxStartSeed->getUInt();
        u32 target = ui->textBoxTargetSeed->getUInt();
        u32 start = ui->textBoxMinFrame->getUInt();
        u32 end = ui->textBoxMaxFrame->getUInt();

        auto *search = new PokeFinderCore::GameCubeRTCSearcher(initial, target, start, end);

        connect(search, &PokeFinderCore::GameCubeRTCSearcher::finished, this, [=] {
            ui->pushButtonSearch->setEnabled(true);
            ui->pushButtonCancel->setEnabled(false);
        });
        connect(search, &PokeFinderCore::GameCubeRTCSearcher::result, this, &GameCubeRTC::updateTableView);
        connect(
            ui->pushButtonCancel, &QPushButton::clicked, search, &PokeFinderCore::GameCubeRTCSearcher::cancelSearch);

        search->startSearch();
    }

    void GameCubeRTC::updateTableView(const QList<QStandardItem *> &row) { model->appendRow(row); }

    void GameCubeRTC::copySeed()
    {
        QVariant data
            = ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 2));
        QApplication::clipboard()->setText(data.toString());
    }

    void GameCubeRTC::on_tableView_customContextMenuRequested(const QPoint &pos)
    {
        if (model->rowCount() == 0)
        {
            return;
        }

        contextMenu->popup(ui->tableView->viewport()->mapToGlobal(pos));
    }
}
