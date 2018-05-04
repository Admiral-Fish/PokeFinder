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

#include "RoamerMap.hpp"
#include "ui_RoamerMap.h"

RoamerMap::RoamerMap(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RoamerMap)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);

    ui->labelMap->setPixmap(QPixmap::fromImage(QImage(":/roamers.png")));
    ui->labelEntei->setPixmap(QPixmap::fromImage(QImage(":/entei.png")));
    ui->labelRaikou->setPixmap(QPixmap::fromImage(QImage(":/raikou.png")));
    ui->labelLatias->setPixmap(QPixmap::fromImage(QImage(":/latias.png")));
    ui->labelLatios->setPixmap(QPixmap::fromImage(QImage(":/latios.png")));
}

RoamerMap::~RoamerMap()
{
    delete ui;
}
