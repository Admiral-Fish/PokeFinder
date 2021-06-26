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

#include "AGuAcaTE.hpp"
#include "ui_AGuAcaTE.h"
#include <Core/Enum/Method.hpp>
#include <QSettings>

AGuAcaTE::AGuAcaTE(QWidget *parent) : QWidget(parent), ui(new Ui::AGuAcaTE)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);
    ui->textBoxAdvance->setValues(InputType::Advance32Bit);
    connect(ui->pushButtonRun, &QPushButton::clicked, this, &AGuAcaTE::run);
}

AGuAcaTE::~AGuAcaTE()
{
    QSettings setting;
    setting.setValue("AGuAcaTE/geometry", this->saveGeometry());
    delete ui;
}

void AGuAcaTE::run()
{
    ui->labelGC->setText("");
    ui->labelGBA->setText("");
    ui->labelDifference->setText("");
    ui->labelClosest->setText("");
    u16 target = 29279;
    u32 advances = ui->textBoxAdvance->getUInt()+1;
    u32 closestCycle = (advances-target)/2792;
    u32 difference = advances - (target + closestCycle*2792);
    u32 highestBound = 4*difference/27;
    u32 lowestBound = difference/7;
    if ((highestBound >= lowestBound || difference == 0) && closestCycle >= 0)
    {
        ui->labelClosest->setText("Closest cycle: " + QString::number(closestCycle));

        u32 gcRedeems = -1*difference + 7 * highestBound;
        u32 gbaRedeems = 4*difference - 27 * highestBound;

        ui->labelGC->setText("Number of Pikachu/Celebi GC Reedems: " + QString::number(gcRedeems));
        ui->labelGBA->setText("Number of Pikachu GBA Reedems: " + QString::number(gbaRedeems));
        ui->labelDifference->setText("Difference: " + QString::number(difference));
    }
    else
    {
        ui->labelClosest->setText("Not Possible");
    }
}
