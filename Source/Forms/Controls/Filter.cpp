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

#include "Filter.hpp"
#include "ui_Filter.h"
#include <Core/Util/Translator.hpp>
#include <array>

Filter::Filter(QWidget *parent) : QWidget(parent), ui(new Ui::Filter)
{
    ui->setupUi(this);

    ui->comboBoxAbility->setup({ 255, 0, 1 });
    ui->comboBoxGender->setup({ 255, 0, 1 });
    ui->comboBoxGenderRatio->setup({ 255, 127, 191, 63, 31, 0, 254 });
    ui->checkListHiddenPower->setup(Translator::getHiddenPowers());
    ui->checkListNature->setup(Translator::getNatures());
    ui->comboBoxShiny->setup({ 255, 1, 2, 3 });
    ui->textBoxDelay->setValues(InputType::Advance32Bit);

    connect(ui->pushButtonEncounterSlotAny, &QPushButton::clicked, ui->checkListEncounterSlot, &CheckList::resetChecks);
    connect(ui->pushButtonHiddenPowerAny, &QPushButton::clicked, ui->checkListHiddenPower, &CheckList::resetChecks);
    connect(ui->pushButtonNatureAny, &QPushButton::clicked, ui->checkListNature, &CheckList::resetChecks);
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

u8 Filter::getGenderRatio() const
{
    return ui->comboBoxGenderRatio->getCurrentByte();
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
    return ui->checkBoxUseDelay->isChecked();
}

u32 Filter::getDelay() const
{
    return ui->textBoxDelay->getUInt();
}

bool Filter::getDisableFilters()
{
    return ui->checkBoxDisableFilters->isChecked();
}

void Filter::disableControls(u16 control)
{
    if (control & Controls::IVs)
    {
        ui->ivFilter->setVisible(false);
    }

    if (control & Controls::Ability)
    {
        ui->labelAbility->setVisible(false);
        ui->comboBoxAbility->setVisible(false);
    }

    if (control & Controls::Gender)
    {
        ui->labelGender->setVisible(false);
        ui->comboBoxGender->setVisible(false);
    }

    if (control & Controls::GenderRatio)
    {
        ui->labelGenderRatio->setVisible(false);
        ui->comboBoxGenderRatio->setVisible(false);
    }

    if (control & Controls::EncounterSlots)
    {
        ui->labelEncounterSlot->setVisible(false);
        ui->checkListEncounterSlot->setVisible(false);
        ui->pushButtonEncounterSlotAny->setVisible(false);
    }

    if (control & Controls::HiddenPowers)
    {
        ui->labelHiddenPower->setVisible(false);
        ui->checkListHiddenPower->setVisible(false);
        ui->pushButtonHiddenPowerAny->setVisible(false);
    }

    if (control & Controls::Natures)
    {
        ui->labelNature->setVisible(false);
        ui->checkListNature->setVisible(false);
        ui->pushButtonNatureAny->setVisible(false);
    }

    if (control & Controls::Shiny)
    {
        ui->labelShiny->setVisible(false);
        ui->comboBoxShiny->setVisible(false);
    }

    if (control & Controls::UseDelay)
    {
        ui->checkBoxUseDelay->setVisible(false);
        ui->textBoxDelay->setVisible(false);
    }

    if (control & Controls::DisableFilter)
    {
        ui->checkBoxDisableFilters->setVisible(false);
    }
}

void Filter::setGenderRatio(u8 genderRatio)
{
    ui->comboBoxGenderRatio->setCurrentIndex(ui->comboBoxGenderRatio->findData(genderRatio));
}

void Filter::enableHiddenAbility()
{
    ui->comboBoxAbility->addItem("H", 2);
}
