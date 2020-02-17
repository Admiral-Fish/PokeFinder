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

#include "EggSettings.hpp"
#include "ui_EggSettings.h"

EggSettings::EggSettings(QWidget *parent) : QWidget(parent), ui(new Ui::EggSettings)
{
    ui->setupUi(this);

    connect(ui->pushButtonSwapParents, &QPushButton::clicked, this, &EggSettings::swapParents);
    connect(ui->checkBoxShowInteritance, &QCheckBox::clicked, this, &EggSettings::toggleInheritance);
}

EggSettings::~EggSettings()
{
    delete ui;
}

QVector<u8> EggSettings::getParent1() const
{
    QVector<u8> parent1 = { static_cast<u8>(ui->spinBoxParent1HP->value()),  static_cast<u8>(ui->spinBoxParent1Atk->value()),
                            static_cast<u8>(ui->spinBoxParent1Def->value()), static_cast<u8>(ui->spinBoxParent1SpA->value()),
                            static_cast<u8>(ui->spinBoxParent1SpD->value()), static_cast<u8>(ui->spinBoxParent1Spe->value()) };

    return parent1;
}

QVector<u8> EggSettings::getParent2() const
{
    QVector<u8> parent2 = { static_cast<u8>(ui->spinBoxParent2HP->value()),  static_cast<u8>(ui->spinBoxParent2Atk->value()),
                            static_cast<u8>(ui->spinBoxParent2Def->value()), static_cast<u8>(ui->spinBoxParent2SpA->value()),
                            static_cast<u8>(ui->spinBoxParent2SpD->value()), static_cast<u8>(ui->spinBoxParent2Spe->value()) };

    return parent2;
}

void EggSettings::swapParents()
{
    QVector<u8> parent1 = getParent1();
    QVector<u8> parent2 = getParent2();

    ui->spinBoxParent1HP->setValue(parent2[0]);
    ui->spinBoxParent1Atk->setValue(parent2[1]);
    ui->spinBoxParent1Def->setValue(parent2[2]);
    ui->spinBoxParent1SpA->setValue(parent2[3]);
    ui->spinBoxParent1SpD->setValue(parent2[4]);
    ui->spinBoxParent1Spe->setValue(parent2[5]);

    ui->spinBoxParent2HP->setValue(parent1[0]);
    ui->spinBoxParent2Atk->setValue(parent1[1]);
    ui->spinBoxParent2Def->setValue(parent1[2]);
    ui->spinBoxParent2SpA->setValue(parent1[3]);
    ui->spinBoxParent2SpD->setValue(parent1[4]);
    ui->spinBoxParent2Spe->setValue(parent1[5]);
}

void EggSettings::showInheritance(bool checked)
{
    emit toggleInheritance(checked);
}
