/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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
    ui->checkListHiddenPower->setup(Translator::getHiddenPowers());
    ui->checkListNature->setup(Translator::getNatures());
    ui->comboBoxShiny->setup({ 255, 1, 2, 3 });

    ui->checkListEncounterSlot->setToolTip(tr("Click holding ctrl to reset"));
    ui->checkListHiddenPower->setToolTip(tr("Click holding ctrl to reset"));
    ui->checkListNature->setToolTip(tr("Click holding ctrl to reset"));

    connect(ui->ivFilter, &IVFilter::showStatsChanged, this, [=](bool flag) { emit showStatsChanged(flag); });
}

Filter::~Filter()
{
    delete ui;
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
    }

    if ((control & Controls::HiddenPowers) != Controls::None)
    {
        ui->labelHiddenPower->setVisible(false);
        ui->checkListHiddenPower->setVisible(false);
    }

    if ((control & Controls::Natures) != Controls::None)
    {
        ui->labelNature->setVisible(false);
        ui->checkListNature->setVisible(false);
    }

    if ((control & Controls::Shiny) != Controls::None)
    {
        ui->labelShiny->setVisible(false);
        ui->comboBoxShiny->setVisible(false);
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

u8 Filter::getAbility() const
{
    return ui->comboBoxAbility->getCurrentUChar();
}

bool Filter::getDisableFilters() const
{
    return ui->checkBoxDisableFilters->isChecked();
}

std::array<bool, 12> Filter::getEncounterSlots() const
{
    // Encounter slot can vary depending on the encounter type, with the highest number being 12 currently
    // Opt to using array of 12 instead of vector for smaller memory usage and avoiding the heap
    return ui->checkListEncounterSlot->getCheckedArray<12>();
}

u8 Filter::getGender() const
{
    return ui->comboBoxGender->getCurrentUChar();
}

std::array<bool, 16> Filter::getHiddenPowers() const
{
    return ui->checkListHiddenPower->getCheckedArray<16>();
}

std::array<u8, 6> Filter::getMaxIVs() const
{
    return ui->ivFilter->getUpper();
}

std::array<u8, 6> Filter::getMinIVs() const
{
    return ui->ivFilter->getLower();
}

std::array<bool, 25> Filter::getNatures() const
{
    return ui->checkListNature->getCheckedArray<25>();
}

u8 Filter::getShiny() const
{
    return ui->comboBoxShiny->getCurrentUChar();
}

void Filter::resetEncounterSlots() const
{
    ui->checkListEncounterSlot->resetChecks();
}

void Filter::setEncounterSlots(u8 max) const
{
    std::vector<std::string> items;
    for (u8 i = 0; i < max; i++)
    {
        items.emplace_back(std::to_string(i));
    }
    ui->checkListEncounterSlot->setup(items);
}

void Filter::toggleEncounterSlots(const std::vector<bool> &encounterSlots) const
{
    ui->checkListEncounterSlot->setChecks(encounterSlots);
}
