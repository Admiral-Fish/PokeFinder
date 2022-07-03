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

#include "Filter.hpp"
#include "ui_Filter.h"
#include <Core/Util/Translator.hpp>
#include <Form/Controls/Controls.hpp>

Filter::Filter(QWidget *parent) : QWidget(parent), ui(new Ui::Filter)
{
    ui->setupUi(this);

    ui->comboBoxAbility->setup({ 255, 0, 1 });
    ui->comboBoxGender->setup({ 255, 0, 1 });
    ui->checkListHiddenPower->setup(*Translator::getHiddenPowers());
    ui->checkListNature->setup(*Translator::getNatures());
    ui->comboBoxShiny->setup({ 255, 1, 2, 3 });
    ui->textBoxDelay->setValues(InputType::Advance32Bit);

    connect(ui->pushButtonEncounterSlot, &QPushButton::clicked, ui->checkListEncounterSlot, &CheckList::resetChecks);
    connect(ui->pushButtonHiddenPower, &QPushButton::clicked, ui->checkListHiddenPower, &CheckList::resetChecks);
    connect(ui->pushButtonNature, &QPushButton::clicked, ui->checkListNature, &CheckList::resetChecks);
}

Filter::~Filter()
{
    delete ui;
}

std::array<u8, 6> Filter::getMinIVs() const
{
    return ui->ivFilter->getLower();
}

std::array<u8, 6> Filter::getMaxIVs() const
{
    return ui->ivFilter->getUpper();
}

u8 Filter::getAbility() const
{
    return ui->comboBoxAbility->getCurrentByte();
}

u8 Filter::getGender() const
{
    return ui->comboBoxGender->getCurrentByte();
}

std::vector<bool> Filter::getEncounterSlots()
{
    return ui->checkListEncounterSlot->getChecked();
}

void Filter::setEncounterSlots(const std::vector<std::string> &encounterSlots) const
{
    ui->checkListEncounterSlot->setup(encounterSlots);
}

void Filter::toggleEncounterSlots(const std::vector<bool> &encounterSlots) const
{
    ui->checkListEncounterSlot->setChecks(encounterSlots);
}

void Filter::resetEncounterSlots() const
{
    ui->checkListEncounterSlot->resetChecks();
}

std::vector<bool> Filter::getHiddenPowers()
{
    return ui->checkListHiddenPower->getChecked();
}

std::vector<bool> Filter::getNatures()
{
    return ui->checkListNature->getChecked();
}

u8 Filter::getShiny() const
{
    return ui->comboBoxShiny->getCurrentByte();
}

bool Filter::useDelay() const
{
    return ui->checkBoxDelay->isChecked();
}

u32 Filter::getDelay() const
{
    return ui->textBoxDelay->getUInt();
}

bool Filter::getDisableFilters() const
{
    return ui->checkBoxDisableFilters->isChecked();
}

void Filter::enableControls(Controls control)
{
    if ((control & Controls::IVs) != Controls::None)
    {
        ui->ivFilter->setVisible(true);
    }

    if ((control & Controls::Ability) != Controls::None)
    {
        ui->labelAbility->setVisible(true);
        ui->comboBoxAbility->setVisible(true);
    }

    if ((control & Controls::Gender) != Controls::None)
    {
        ui->labelGender->setVisible(true);
        ui->comboBoxGender->setVisible(true);
    }

    if ((control & Controls::EncounterSlots) != Controls::None)
    {
        ui->labelEncounterSlot->setVisible(true);
        ui->checkListEncounterSlot->setVisible(true);
        ui->pushButtonEncounterSlot->setVisible(true);
    }

    if ((control & Controls::HiddenPowers) != Controls::None)
    {
        ui->labelHiddenPower->setVisible(true);
        ui->checkListHiddenPower->setVisible(true);
        ui->pushButtonHiddenPower->setVisible(true);
    }

    if ((control & Controls::Natures) != Controls::None)
    {
        ui->labelNature->setVisible(true);
        ui->checkListNature->setVisible(true);
        ui->pushButtonNature->setVisible(true);
    }

    if ((control & Controls::Shiny) != Controls::None)
    {
        ui->labelShiny->setVisible(true);
        ui->comboBoxShiny->setVisible(true);
    }

    if ((control & Controls::UseDelay) != Controls::None)
    {
        ui->checkBoxDelay->setVisible(true);
        ui->textBoxDelay->setVisible(true);
    }

    if ((control & Controls::DisableFilter) != Controls::None)
    {
        ui->checkBoxDisableFilters->setVisible(true);
    }
}

void Filter::disableControls(Controls control)
{
    if ((control & Controls::IVs) != Controls::None)
    {
        ui->ivFilter->setVisible(false);
    }

    if ((control & Controls::Ability) != Controls::None)
    {
        ui->labelAbility->setVisible(false);
        ui->comboBoxAbility->setVisible(false);
    }

    if ((control & Controls::Gender) != Controls::None)
    {
        ui->labelGender->setVisible(false);
        ui->comboBoxGender->setVisible(false);
    }

    if ((control & Controls::EncounterSlots) != Controls::None)
    {
        ui->labelEncounterSlot->setVisible(false);
        ui->checkListEncounterSlot->setVisible(false);
        ui->pushButtonEncounterSlot->setVisible(false);
    }

    if ((control & Controls::HiddenPowers) != Controls::None)
    {
        ui->labelHiddenPower->setVisible(false);
        ui->checkListHiddenPower->setVisible(false);
        ui->pushButtonHiddenPower->setVisible(false);
    }

    if ((control & Controls::Natures) != Controls::None)
    {
        ui->labelNature->setVisible(false);
        ui->checkListNature->setVisible(false);
        ui->pushButtonNature->setVisible(false);
    }

    if ((control & Controls::Shiny) != Controls::None)
    {
        ui->labelShiny->setVisible(false);
        ui->comboBoxShiny->setVisible(false);
    }

    if ((control & Controls::UseDelay) != Controls::None)
    {
        ui->checkBoxDelay->setVisible(false);
        ui->textBoxDelay->setVisible(false);
    }

    if ((control & Controls::DisableFilter) != Controls::None)
    {
        ui->checkBoxDisableFilters->setVisible(false);
    }
}

void Filter::enableHiddenAbility()
{
    ui->comboBoxAbility->addItem("H", 2);
}
