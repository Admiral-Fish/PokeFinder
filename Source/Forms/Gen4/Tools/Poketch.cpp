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

#include "Poketch.hpp"
#include "ui_Poketch.h"
#include <Core/Util/Utilities.hpp>

Poketch::Poketch(u32 advances, QWidget *parent) : QWidget(parent), ui(new Ui::Poketch)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    setupModels(advances);
}

void Poketch::setupModels(u32 advances)
{
    connect(ui->pushButtonClose, &QPushButton::clicked, this, &Poketch::on_pushButtonClose_clicked);

    u32 coin = 0;
    u32 happy = 0;

    if (advances < 12)
    {
        coin = advances;
        ui->labelNote->setText(tr("Do not switch to the happiness application at all"));
    }
    else
    {
        u32 target = advances - 12;
        happy = target / 12;
        coin = target % 12;

        if (happy == 0)
        {
            ui->labelNote->setText(tr("Switch to the happiness application once but do not click"));
        }
    }

    ui->labelHappyResult->setText(QString::number(happy));
    ui->labelCoinFlipsResult->setText(QString::number(coin));
}

Poketch::~Poketch()
{
    delete ui;
}

void Poketch::on_pushButtonClose_clicked()
{
    this->close();
}
