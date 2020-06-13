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

#include "EggSettings2.hpp"
#include "ui_EggSettings2.h"
#include <Core/Util/Translator.hpp>

EggSettings2::EggSettings2(QWidget *parent) : QWidget(parent), ui(new Ui::EggSettings2)
{
    ui->setupUi(this);
    setupModels();
}

EggSettings2::~EggSettings2()
{
    delete ui;
}

void EggSettings2::setup(u8 generation)
{
    if (generation == 5)
    {
        ui->comboBoxParentAItem->addItem(tr("Power Weight"));
        ui->comboBoxParentAItem->addItem(tr("Power Bracer"));
        ui->comboBoxParentAItem->addItem(tr("Power Belt"));
        ui->comboBoxParentAItem->addItem(tr("Power Lens"));
        ui->comboBoxParentAItem->addItem(tr("Power Band"));
        ui->comboBoxParentAItem->addItem(tr("Power Anklet"));

        ui->comboBoxParentBItem->addItem(tr("Power Weight"));
        ui->comboBoxParentBItem->addItem(tr("Power Bracer"));
        ui->comboBoxParentBItem->addItem(tr("Power Belt"));
        ui->comboBoxParentBItem->addItem(tr("Power Lens"));
        ui->comboBoxParentBItem->addItem(tr("Power Band"));
        ui->comboBoxParentBItem->addItem(tr("Power Anklet"));

        ui->comboBoxParentAAbility->addItem("H");
        ui->comboBoxParentBAbility->addItem("H");
    }
}

Daycare EggSettings2::getDaycareSettings() const
{
    QVector<u8> parent1IVs = { static_cast<u8>(ui->spinBoxParentAHP->value()),  static_cast<u8>(ui->spinBoxParentAAtk->value()),
                               static_cast<u8>(ui->spinBoxParentADef->value()), static_cast<u8>(ui->spinBoxParentASpA->value()),
                               static_cast<u8>(ui->spinBoxParentASpD->value()), static_cast<u8>(ui->spinBoxParentASpe->value()) };

    QVector<u8> parent2IVs = { static_cast<u8>(ui->spinBoxParentBHP->value()),  static_cast<u8>(ui->spinBoxParentBAtk->value()),
                               static_cast<u8>(ui->spinBoxParentBDef->value()), static_cast<u8>(ui->spinBoxParentBSpA->value()),
                               static_cast<u8>(ui->spinBoxParentBSpD->value()), static_cast<u8>(ui->spinBoxParentBSpe->value()) };

    u8 parent1Ability = ui->comboBoxParentAAbility->currentIndex();
    u8 parent2Ability = ui->comboBoxParentBAbility->currentIndex();

    u8 parent1Gender = ui->comboBoxParentAGender->currentIndex();
    u8 parent2Gender = ui->comboBoxParentBGender->currentIndex();

    u8 parent1Item = ui->comboBoxParentAItem->currentIndex();
    u8 parent2Item = ui->comboBoxParentBItem->currentIndex();

    u8 parent1Nature = ui->comboBoxParentANature->currentIndex();
    u8 parent2Nature = ui->comboBoxParentBNature->currentIndex();

    bool masuada = ui->checkBoxMasuada->isChecked();
    bool nidoranVolbeat = ui->checkBoxNidoranVolbeat->isChecked();

    return Daycare(parent1IVs, parent2IVs, parent1Ability, parent2Ability, parent1Gender, parent2Gender, parent1Item, parent2Item,
                   parent1Nature, parent2Nature, masuada, nidoranVolbeat);
}

bool EggSettings2::compatibleParents() const
{
    u8 parent1 = ui->comboBoxParentAGender->currentIndex();
    u8 parent2 = ui->comboBoxParentBGender->currentIndex();

    // Male/Female
    if ((parent1 == 0 && parent2 == 1) || (parent1 == 1 && parent2 == 0))
    {
        return true;
    }

    // Ditto/Female
    if ((parent1 == 3 && parent2 == 1) || (parent1 == 1 && parent2 == 3))
    {
        return true;
    }

    // Male/Ditto
    if ((parent1 == 0 && parent2 == 3) || (parent1 == 3 && parent2 == 0))
    {
        return true;
    }

    // Genderless Ditto
    if ((parent1 == 2 && parent2 == 3) || (parent1 == 3 && parent2 == 2))
    {
        return true;
    }

    return false;
}

void EggSettings2::reorderParents()
{
    // TODO
}

void EggSettings2::setupModels()
{
    ui->comboBoxParentANature->addItems(Translator::getNatures());
    ui->comboBoxParentBNature->addItems(Translator::getNatures());

    for (u8 i = 0; i < 3; i++)
    {
        ui->comboBoxParentAGender->addItem(Translator::getGender(i));
        ui->comboBoxParentBGender->addItem(Translator::getGender(i));
    }
    ui->comboBoxParentAGender->addItem(Translator::getSpecies(132));
    ui->comboBoxParentBGender->addItem(Translator::getSpecies(132));

    ui->comboBoxParentAAbility->addItem("1");
    ui->comboBoxParentAAbility->addItem("2");

    ui->comboBoxParentBAbility->addItem("1");
    ui->comboBoxParentBAbility->addItem("2");

    ui->comboBoxParentAItem->addItem("None");
    ui->comboBoxParentAItem->addItem("Everstone");

    ui->comboBoxParentBItem->addItem("None");
    ui->comboBoxParentBItem->addItem("Everstone");
}
