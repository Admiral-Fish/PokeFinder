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

#include "EggSettings.hpp"
#include "ui_EggSettings.h"
#include <Core/Enum/Game.hpp>
#include <Core/Parents/Daycare.hpp>
#include <Core/Util/Translator.hpp>

EggSettings::EggSettings(QWidget *parent) : QWidget(parent), ui(new Ui::EggSettings)
{
    ui->setupUi(this);

    for (const std::string &nature : *Translator::getNatures())
    {
        ui->comboBoxParentANature->addItem(QString::fromStdString(nature));
        ui->comboBoxParentBNature->addItem(QString::fromStdString(nature));
    }

    for (const std::string &gender : *Translator::getGenders())
    {
        ui->comboBoxParentAGender->addItem(QString::fromStdString(gender));
        ui->comboBoxParentBGender->addItem(QString::fromStdString(gender));
    }
    ui->comboBoxParentAGender->addItem(QString::fromStdString(*Translator::getSpecie(132)));
    ui->comboBoxParentBGender->addItem(QString::fromStdString(*Translator::getSpecie(132)));

    ui->comboBoxParentAAbility->addItem("1");
    ui->comboBoxParentAAbility->addItem("2");

    ui->comboBoxParentBAbility->addItem("1");
    ui->comboBoxParentBAbility->addItem("2");

    ui->comboBoxParentAItem->addItem(tr("None"), 0);
    ui->comboBoxParentAItem->addItem(tr("Everstone"), 1);

    ui->comboBoxParentBItem->addItem(tr("None"), 0);
    ui->comboBoxParentBItem->addItem(tr("Everstone"), 1);

    connect(ui->checkBoxShowInheritance, &QCheckBox::clicked, this, &EggSettings::toggleInheritance);
}

EggSettings::~EggSettings()
{
    delete ui;
}

void EggSettings::setup(Game version)
{
    if ((version & Game::Gen3) != Game::None)
    {
        bool flag = (version & Game::Emerald) != Game::None;

        ui->labelAbility->setVisible(false);
        ui->labelItem->setVisible(flag);
        ui->labelNature->setVisible(flag);

        ui->comboBoxParentAAbility->setVisible(false);
        ui->comboBoxParentBAbility->setVisible(false);
        ui->comboBoxParentAItem->setVisible(flag);
        ui->comboBoxParentBItem->setVisible(flag);
        ui->comboBoxParentANature->setVisible(flag);
        ui->comboBoxParentBNature->setVisible(flag);

        ui->checkBoxNidoranVolbeat->setVisible(false);
        ui->checkBoxMasuda->setVisible(false);
    }
    else if ((version & Game::Gen4) != Game::None)
    {
        ui->labelAbility->setVisible(false);
        ui->labelItem->setVisible(false);
        ui->labelNature->setVisible(false);

        ui->comboBoxParentAAbility->setVisible(false);
        ui->comboBoxParentBAbility->setVisible(false);
        ui->comboBoxParentAItem->setVisible(false);
        ui->comboBoxParentBItem->setVisible(false);
        ui->comboBoxParentANature->setVisible(false);
        ui->comboBoxParentBNature->setVisible(false);

        ui->checkBoxNidoranVolbeat->setVisible(false);
    }
    else if ((version & Game::Gen5) != Game::None)
    {
        ui->comboBoxParentAItem->addItem(tr("Power Weight"), 2);
        ui->comboBoxParentAItem->addItem(tr("Power Bracer"), 3);
        ui->comboBoxParentAItem->addItem(tr("Power Belt"), 4);
        ui->comboBoxParentAItem->addItem(tr("Power Lens"), 5);
        ui->comboBoxParentAItem->addItem(tr("Power Band"), 6);
        ui->comboBoxParentAItem->addItem(tr("Power Anklet"), 7);

        ui->comboBoxParentBItem->addItem(tr("Power Weight"), 2);
        ui->comboBoxParentBItem->addItem(tr("Power Bracer"), 3);
        ui->comboBoxParentBItem->addItem(tr("Power Belt"), 4);
        ui->comboBoxParentBItem->addItem(tr("Power Lens"), 5);
        ui->comboBoxParentBItem->addItem(tr("Power Band"), 6);
        ui->comboBoxParentBItem->addItem(tr("Power Anklet"), 7);

        ui->comboBoxParentAAbility->addItem("H");
        ui->comboBoxParentBAbility->addItem("H");
    }
    else if ((version & Game::BDSP) != Game::None)
    {
        ui->comboBoxParentAItem->addItem(tr("Destiny Knot"), 8);
        ui->comboBoxParentBItem->addItem(tr("Destiny Knot"), 8);

        ui->comboBoxParentAAbility->addItem("H");
        ui->comboBoxParentBAbility->addItem("H");
    }
}

Daycare EggSettings::getDaycareSettings() const
{
    std::array<std::array<u8, 6>, 2> parentIVs
        = { { { static_cast<u8>(ui->spinBoxParentAHP->value()), static_cast<u8>(ui->spinBoxParentAAtk->value()),
                static_cast<u8>(ui->spinBoxParentADef->value()), static_cast<u8>(ui->spinBoxParentASpA->value()),
                static_cast<u8>(ui->spinBoxParentASpD->value()), static_cast<u8>(ui->spinBoxParentASpe->value()) },
              { static_cast<u8>(ui->spinBoxParentBHP->value()), static_cast<u8>(ui->spinBoxParentBAtk->value()),
                static_cast<u8>(ui->spinBoxParentBDef->value()), static_cast<u8>(ui->spinBoxParentBSpA->value()),
                static_cast<u8>(ui->spinBoxParentBSpD->value()), static_cast<u8>(ui->spinBoxParentBSpe->value()) } } };

    std::array<u8, 2> parentAbility
        = { static_cast<u8>(ui->comboBoxParentAAbility->currentIndex()), static_cast<u8>(ui->comboBoxParentBAbility->currentIndex()) };

    std::array<u8, 2> parentGender
        = { static_cast<u8>(ui->comboBoxParentAGender->currentIndex()), static_cast<u8>(ui->comboBoxParentBGender->currentIndex()) };

    std::array<u8, 2> parentItem = { ui->comboBoxParentAItem->getCurrentByte(), ui->comboBoxParentBItem->getCurrentByte() };

    std::array<u8, 2> parentNature
        = { static_cast<u8>(ui->comboBoxParentANature->currentIndex()), static_cast<u8>(ui->comboBoxParentBNature->currentIndex()) };

    bool masuda = ui->checkBoxMasuda->isChecked();
    bool nidoranVolbeat = ui->checkBoxNidoranVolbeat->isChecked();

    return Daycare(parentIVs, parentAbility, parentGender, parentItem, parentNature, masuda, nidoranVolbeat);
}

bool EggSettings::compatibleParents() const
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

    // Genderless/Ditto
    if ((parent1 == 2 && parent2 == 3) || (parent1 == 3 && parent2 == 2))
    {
        return true;
    }

    return false;
}

bool EggSettings::reorderParents()
{
    u8 parent1 = ui->comboBoxParentAGender->currentIndex();
    u8 parent2 = ui->comboBoxParentBGender->currentIndex();

    // Female/Male -> Male/Female
    bool flag = parent1 == 1 && parent2 == 0;

    // Female/Ditto -> Ditto/Female
    flag |= parent1 == 1 && parent2 == 3;

    // Ditto/Male -> Male/Ditto
    flag |= parent1 == 3 && parent2 == 0;

    // Ditto/Genderless -> Genderless/Ditto
    flag |= parent1 == 3 && parent2 == 2;

    if (flag)
    {
        Daycare daycare = getDaycareSettings();

        ui->spinBoxParentAHP->setValue(daycare.getParentIV(1, 0));
        ui->spinBoxParentAAtk->setValue(daycare.getParentIV(1, 1));
        ui->spinBoxParentADef->setValue(daycare.getParentIV(1, 2));
        ui->spinBoxParentASpA->setValue(daycare.getParentIV(1, 3));
        ui->spinBoxParentASpD->setValue(daycare.getParentIV(1, 4));
        ui->spinBoxParentASpe->setValue(daycare.getParentIV(1, 5));
        ui->comboBoxParentAAbility->setCurrentIndex(daycare.getParentAbility(1));
        ui->comboBoxParentAGender->setCurrentIndex(daycare.getParentGender(1));
        ui->comboBoxParentAItem->setCurrentIndex(ui->comboBoxParentAItem->findData(daycare.getParentItem(1)));
        ui->comboBoxParentANature->setCurrentIndex(daycare.getParentNature(1));

        ui->spinBoxParentBHP->setValue(daycare.getParentIV(0, 0));
        ui->spinBoxParentBAtk->setValue(daycare.getParentIV(0, 1));
        ui->spinBoxParentBDef->setValue(daycare.getParentIV(0, 2));
        ui->spinBoxParentBSpA->setValue(daycare.getParentIV(0, 3));
        ui->spinBoxParentBSpD->setValue(daycare.getParentIV(0, 4));
        ui->spinBoxParentBSpe->setValue(daycare.getParentIV(0, 5));
        ui->comboBoxParentBAbility->setCurrentIndex(daycare.getParentAbility(0));
        ui->comboBoxParentBGender->setCurrentIndex(daycare.getParentGender(0));
        ui->comboBoxParentBItem->setCurrentIndex(ui->comboBoxParentBItem->findData(daycare.getParentItem(0)));
        ui->comboBoxParentBNature->setCurrentIndex(daycare.getParentNature(0));
    }

    return flag;
}

void EggSettings::showInheritance(bool checked)
{
    emit toggleInheritance(checked);
}
