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

#include "PIDIV.hpp"
#include "ui_PIDIV.h"
#include <Core/Gen3/Tools/PIDIVCalculator.hpp>
#include <Models/Gen3/PIDIVModel.hpp>
#include <QClipboard>
#include <QSettings>

PIDIV::PIDIV(QWidget *parent) : QWidget(parent), ui(new Ui::PIDIV)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

PIDIV::~PIDIV()
{
    QSettings setting;
    setting.setValue("pidToIVs/geometry", this->saveGeometry());

    delete ui;
}

void PIDIV::setupModels()
{
    model = new PIDIVModel(ui->tableView);
    ui->tableView->setModel(model);

    contextMenu = new QMenu(ui->tableView);

    ui->textBoxPID->setValues(InputType::Seed32Bit);

    QAction *copySeed = contextMenu->addAction(tr("Copy Seed to Clipboard"));
    connect(copySeed, &QAction::triggered, this, &PIDIV::copySeed);

    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &PIDIV::generate);
    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &PIDIV::tableViewContextMenu);

    QSettings setting;
    if (setting.contains("pidToIVs/geometry"))
    {
        this->restoreGeometry(setting.value("pidToIVs/geometry").toByteArray());
    }
}

void PIDIV::generate()
{
    model->clearModel();

    u32 pid = ui->textBoxPID->getUInt();
    auto frames = PIDIVCalculator::calculateIVs(pid);
    model->addItems(frames);
}

void PIDIV::tableViewContextMenu(QPoint pos)
{
    if (model->rowCount() == 0)
    {
        return;
    }

    contextMenu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}

void PIDIV::copySeed()
{
    QVariant data = ui->tableView->model()->data(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0));
    QApplication::clipboard()->setText(data.toString());
}
