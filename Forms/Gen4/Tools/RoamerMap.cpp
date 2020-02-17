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

#include "RoamerMap.hpp"
#include "ui_RoamerMap.h"
#include <Core/Util/Translator.hpp>

RoamerMap::RoamerMap(QWidget *parent) : QDialog(parent), ui(new Ui::RoamerMap)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);

    QStringList roamers = Translator::getSpecies({ 244, 243, 380, 381 });
    ui->labelEntei->setText(roamers.at(0));
    ui->labelRaikou->setText(roamers.at(1));
    ui->labelLatiasLatios->setText(roamers.at(2) + "/" + roamers.at(3));
}

RoamerMap::~RoamerMap()
{
    delete ui;
}
