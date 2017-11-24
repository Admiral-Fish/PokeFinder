/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish and bumba
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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

#include "Researcher.hpp"
#include "ui_Researcher.h"
#include <iostream>

typedef u64 (*func)(u64, u64);
typedef unordered_map<string, func> Calculator;

Researcher::Researcher(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Researcher)
{
    ui->setupUi(this);
    setupModel();
}

Researcher::~Researcher()
{
    delete ui;
}

void Researcher::setupModel()
{
    QStandardItemModel *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({tr("32Bit"), tr("16Bit High"), tr("16BitLow"), tr("Custom1"), tr("Custom2"), tr("Custom3"), tr("Custom4"), tr("Custom5"), tr("Custom6"), tr("Custom7"), tr("Custom8"), tr("Custom9"), tr("Custom10"), tr("%3"), tr("% 25"), tr("%100"), tr("/656"), tr("LBit"), tr("HBit")});
    ui->tableView->setModel(model);
}

u64 Modulo(u64 x, u64 y) { return x % y; };

void Researcher::on_pushButtonGenerate32Bit_clicked()
{
    vector<ResearcherFrame> frames;
    Calculator calc;
    calc["%"] = &Modulo;

    cout << calc["%"](1,4);
}

ResearcherFrame::ResearcherFrame()
{
    for (int i = 0; i < 10; i++)
    {
        Custom[i] = 0;
    }
}

QList<QStandardItem *> ResearcherFrame::GetRow()
{
    QList<QStandardItem *> row;
    if (this->RNG64Bit)
    {
        row.append(new QStandardItem(QString::number(this->Full64)));
        row.append(new QStandardItem(QString::number(this->High32())));
        row.append(new QStandardItem(QString::number(this->Low32())));
        row.append(new QStandardItem(QString::number(this->High16())));
        row.append(new QStandardItem(QString::number(this->Low16())));
    }
    else
    {
        row.append(new QStandardItem(QString::number(this->Full32)));
        row.append(new QStandardItem(QString::number(this->High16())));
        row.append(new QStandardItem(QString::number(this->Low16())));
    }

    for (int i = 0; i < 10; i++)
    {
        row.append(new QStandardItem(QString::number(this->Custom[i])));
    }

    row.append(new QStandardItem(QString::number(this->Mod3())));
    row.append(new QStandardItem(QString::number(this->Mod25())));
    row.append(new QStandardItem(QString::number(this->Mod100())));
    row.append(new QStandardItem(QString::number(this->Div656())));
    row.append(new QStandardItem(QString::number(this->LowBit())));
    row.append(new QStandardItem(QString::number(this->HighBit())));

    for(int i = 0; i < row.count(); i++)
    {
        row.at(i)->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    }

    return row;
}

u64 Divide(u64 x, u64 y) { return y == 0 ? 0 : x / y; }



u64 ShiftRight(u64 x, u64 y) { return x >> y; }

u64 ShiftLeft(u64 x, u64 y) { return x << y; }

u64 And(u64 x, u64 y) { return x & y; }

u64 Or(u64 x, u64 y) { return x | y; }

u64 Xor(u64 x, u64 y) { return x ^ y; }

u64 Add(u64 x, u64 y) { return x + y; }

u64 Subtract(u64 x, u64 y) { return x - y; }

u64 Multiply(u64 x, u64 y) { return x * y; }
