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
    ui->textBoxFilter->setValues(InputType::TIDSID);
    ui->textBoxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxSeed->setValues(InputType::Seed32Bit);

    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Lottery::generate);
    connect(ui->radioButtonSeed, &QRadioButton::toggled, this, &Lottery::switchCalibration);
    connect(ui->checkBoxFilter, &QCheckBox::toggled, this, &Lottery::toggleFilter);

    QSettings setting;
    if (setting.contains("lottery/geometry"))
    {
        this->restoreGeometry(setting.value("lottery/geometry").toByteArray());
    }
}

void Lottery::generate()
{
    model->removeRows(0, model->rowCount());
    bool seedCalibration = ui->radioButtonSeed->isChecked();
    bool found = seedCalibration;
    bool filter = ui->checkBoxFilter->isChecked();
    u32 lottoseed = seedCalibration ? ui->textBoxSeed->getUInt() : 0;
    u32 reverseAdd = ui->radioButtonDPPT->isChecked() ? 0xFC77A683 : 0xA3561A1;
    u16 add = ui->radioButtonDPPT->isChecked() ? 0x3039 : 0x6073;
    u16 target = ui->textBoxFilter->getUShort();
    if (!seedCalibration) {
        u32 lotto1 = ui->textBoxLotto1->getUInt() << 16;
        u16 lotto2 = ui->textBoxLotto2->getUInt();
        u16 lotto3 = ui->textBoxLotto3->getUInt();
        for (u32 low = 0; low <= 0xFFFF; low++)
        {
            lottoseed = (lotto1 | low) * 0xEEB9EB65 + reverseAdd;
            u32 seed = lottoseed * 0x6c078965 + 0x1;
            u16 test = (seed * 0x41c64e6d + add) >> 16;
            if (test != lotto2)
            {
                continue;
            }
            seed = seed * 0x6c078965 + 0x1;
            test = (seed * 0x41c64e6d + add) >> 16;
            if (test != lotto3)
            {
                continue;
            }
            found = true;
            break;
        }
    }
    if (found)
    {
        ARNG rng(lottoseed);
        for (u32 i = 0; i < ui->textBoxAdvances->getUInt(); i++)
        {
            u16 lotto = (rng.getSeed() * 0x41C64E6D + add) >> 16;
            if (!filter || (filter && lotto == target))
            {
                QList<QStandardItem *> row;
                row << new QStandardItem(QString::number(i));
                row << new QStandardItem(QString::number(rng.getSeed(),16).toUpper());
                row << new QStandardItem(QString::number(lotto));
                model->appendRow(row);
            }
            rng.next();
        }
    }
}

void Lottery::switchCalibration(bool seed)
{
    ui->textBoxLotto1->setDisabled(seed);
    ui->textBoxLotto2->setDisabled(seed);
    ui->textBoxLotto3->setDisabled(seed);
    ui->textBoxSeed->setDisabled(!seed);
}

void Lottery::toggleFilter(bool on)
{
    ui->textBoxFilter->setDisabled(!on);
}
