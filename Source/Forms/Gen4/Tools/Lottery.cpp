/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "Lottery.hpp"
#include "ui_Lottery.h"
#include <Core/RNG/LCRNG.hpp>
#include <Core/Util/Translator.hpp>
#include <QSettings>

Lottery::Lottery(QWidget *parent) : QWidget(parent), ui(new Ui::Lottery)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels();
}

Lottery::~Lottery()
{
    QSettings setting;
    setting.setValue("lottery/geometry", this->saveGeometry());

    delete ui;
}

void Lottery::setupModels()
{
    model = new QStandardItemModel(ui->tableView);
    model->setHorizontalHeaderLabels(QStringList() << tr("Advance") << tr("Seed") << tr("Lotto"));
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->textBoxLotto1->setValues(InputType::TIDSID);
    ui->textBoxLotto2->setValues(InputType::TIDSID);
    ui->textBoxLotto3->setValues(InputType::TIDSID);
    ui->textBoxAdvances->setValues(InputType::Advance32Bit);

    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Lottery::generate);

    QSettings setting;
    if (setting.contains("lottery/geometry"))
    {
        this->restoreGeometry(setting.value("lottery/geometry").toByteArray());
    }
}

void Lottery::generate()
{
    model->removeRows(0, model->rowCount());
    u32 lotto1 = ui->textBoxLotto1->getUInt() << 16;
    u16 lotto2 = ui->textBoxLotto2->getUInt();
    u16 lotto3 = ui->textBoxLotto3->getUInt();
    u32 lottoseed = 0;
    bool found = false;
    for (u16 low = 0; low <= 0xFFFF; low++)
    {
        lottoseed = (lotto1 | low) * 0xEEB9EB65 + 0xA3561A1;
        u32 seed = lottoseed * 0x6c078965 + 0x1;
        u16 test = (seed * 0x41c64e6d + 0x6073) >> 16;
        if (test != lotto2)
        {
            continue;
        }
        seed = seed * 0x6c078965 + 0x1;
        test = (seed * 0x41c64e6d + 0x6073) >> 16;
        if (test != lotto3)
        {
            continue;
        }
        found = true;
        break;
    }
    if (!found)
    {
        return;
    }
    ARNG rng(lottoseed);
    for (u32 i = 0; i < ui->textBoxAdvances->getUInt(); i++)
    {
        QList<QStandardItem *> row;
        row << new QStandardItem(QString::number(i));
        row << new QStandardItem(QString::number(rng.getSeed(),16).toUpper());
        u16 lotto = (rng.getSeed() * 0x41C64E6D + 0x6073) >> 16;
        row << new QStandardItem(QString::number(lotto));
        model->appendRow(row);
        rng.next();
    }
}
