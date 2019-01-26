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

#include "ChainedSID.hpp"
#include "ui_ChainedSID.h"

ChainedSID::ChainedSID(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChainedSID)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Widget | Qt::MSWindowsFixedSizeDialogHint);

    setupModels();
}

ChainedSID::~ChainedSID()
{
    delete ui;
}

void ChainedSID::setupModels()
{
    model = new QStandardItemModel(ui->tableView);
    ui->tableView->setModel(model);

    ui->textBoxTID->setValues(InputType::TIDSID);
}

void ChainedSID::on_pushButtonCalculate_clicked()
{

}

void ChainedSID::on_pushButtonClear_clicked()
{

}
