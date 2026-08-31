/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <QMessageBox>

Filter::Filter(QWidget *parent) : QWidget(parent), ui(new Ui::Filter)
{
    ui->setupUi(this);

    ui->comboBoxAbility->setup({ 255, 0, 1 });
    ui->comboBoxGender->setup({ 255, 0, 1 });
    ui->checkListHiddenPower->addItems(Translator::getHiddenPowers());
    ui->checkListNature->addItems(Translator::getNatures());
    ui->comboBoxShiny->setup({ 255, 1, 2, 3 });

    ui->checkListEncounterSlot->setToolTip(tr("Click holding ctrl to reset"));
    ui->checkListHiddenPower->setToolTip(tr("Click holding ctrl to reset"));
    ui->checkListNature->setToolTip(tr("Click holding ctrl to reset"));

    connect(ui->ivFilter, &IVFilter::ivsChanged, this, &Filter::ivsChanged);
    connect(ui->ivFilter, &IVFilter::showStatsChanged, this, &Filter::showStatsChanged);
}

Filter::~Filter()
{
    delete ui;
}

void Filter::copyFrom(const Filter *other)
{
    ui->ivFilter->copyFrom(other->ui->ivFilter);

    ui->comboBoxAbility->setCurrentIndex(other->ui->comboBoxAbility->currentIndex());
    ui->checkListEncounterSlot->setChecks(other->ui->checkListEncounterSlot->getChecked());
    ui->comboBoxGender->setCurrentIndex(other->ui->comboBoxGender->currentIndex());
    ui->spinBoxHeightMin->setValue(other->ui->spinBoxHeightMin->value());
    ui->spinBoxHeightMax->setValue(other->ui->spinBoxHeightMax->value());
    ui->checkListHiddenPower->setChecks(other->ui->checkListHiddenPower->getChecked());
    ui->spinBoxLevelMin->setValue(other->ui->spinBoxLevelMin->value());
    ui->spinBoxLevelMax->setValue(other->ui->spinBoxLevelMax->value());
    ui->checkListNature->setChecks(other->ui->checkListNature->getChecked());
    ui->comboBoxShiny->setCurrentIndex(other->ui->comboBoxShiny->currentIndex());
    ui->spinBoxWeightMin->setValue(other->ui->spinBoxWeightMin->value());
    ui->spinBoxWeightMax->setValue(other->ui->spinBoxWeightMax->value());
    ui->checkBoxDisableFilters->setChecked(other->ui->checkBoxDisableFilters->isChecked());
}

void Filter::disableControls(Controls control)
{
    if ((control & Controls::Ability) != Controls::None)
    {
        ui->labelAbility->hide();
        ui->comboBoxAbility->hide();
    }

    if ((control & Controls::DisableFilter) != Controls::None)
    {
        ui->checkBoxDisableFilters->hide();
    }

    if ((control & Controls::EncounterSlots) != Controls::None)
    {
        ui->labelEncounterSlot->hide();
        ui->checkListEncounterSlot->hide();
    }

    if ((control & Controls::Gender) != Controls::None)
    {
        ui->labelGender->hide();
        ui->comboBoxGender->hide();
    }

    if ((control & Controls::Height) != Controls::None)
    {
        ui->labelHeight->hide();
        ui->spinBoxHeightMin->hide();
        ui->spinBoxHeightMax->hide();
    }

    if ((control & Controls::HiddenPowers) != Controls::None)
    {
        ui->labelHiddenPower->hide();
        ui->checkListHiddenPower->hide();
    }

    if ((control & Controls::IVs) != Controls::None)
    {
        ui->ivFilter->disableControls();
    }

    if ((control & Controls::Level) != Controls::None)
    {
        ui->labelLevel->hide();
        ui->spinBoxLevelMin->hide();
        ui->spinBoxLevelMax->hide();
    }

    if ((control & Controls::Natures) != Controls::None)
    {
        ui->labelNature->hide();
        ui->checkListNature->hide();
    }

    if ((control & Controls::Shiny) != Controls::None)
    {
        ui->labelShiny->hide();
        ui->comboBoxShiny->hide();
    }

    if ((control & Controls::Weight) != Controls::None)
    {
        ui->labelWeight->hide();
        ui->spinBoxWeightMin->hide();
        ui->spinBoxWeightMax->hide();
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

u8 Filter::getHeightMax() const
{
    return static_cast<u8>(ui->spinBoxHeightMax->value());
}

u8 Filter::getHeightMin() const
{
    return static_cast<u8>(ui->spinBoxHeightMin->value());
}

std::array<bool, 16> Filter::getHiddenPowers() const
{
    return ui->checkListHiddenPower->getCheckedArray<16>();
}

u8 Filter::getLevelMax() const
{
    return static_cast<u8>(ui->spinBoxLevelMax->value());
}

u8 Filter::getLevelMin() const
{
    return static_cast<u8>(ui->spinBoxLevelMin->value());
}

std::array<u8, 6> Filter::getMaxIVs() const
{
    return ui->ivFilter->getMaxIVs();
}

std::array<u8, 6> Filter::getMinIVs() const
{
    return ui->ivFilter->getMinIVs();
}

std::array<bool, 25> Filter::getNatures() const
{
    return ui->checkListNature->getCheckedArray<25>();
}

u8 Filter::getShiny() const
{
    return ui->comboBoxShiny->getCurrentUChar();
}

bool Filter::isValid() const
{
    if (ui->checkBoxDisableFilters->isChecked())
    {
        return true;
    }

    if (!ui->ivFilter->isValid())
    {
        return false;
    }

    if (ui->spinBoxLevelMin->value() > ui->spinBoxLevelMax->value())
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid filter settings"), tr("Level minimum is greater than maximum"));
        msg.exec();
        return false;
    }

    if (ui->spinBoxHeightMin->value() > ui->spinBoxHeightMax->value())
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid filter settings"), tr("Height minimum is greater than maximum"));
        msg.exec();
        return false;
    }

    if (ui->spinBoxWeightMin->value() > ui->spinBoxWeightMax->value())
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid filter settings"), tr("Weight minimum is greater than maximum"));
        msg.exec();
        return false;
    }

    return true;
}

bool Filter::isValid(u32 min, u32 max) const
{
    if (ui->checkBoxDisableFilters->isChecked())
    {
        return true;
    }

    if (!isValid())
    {
        return false;
    }

    if ((min != 0 || max != 0) && (getLevelMax() < min || getLevelMin() > max))
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid level"), tr("Level filter outside of encounters level range"));
        msg.exec();
        return false;
    }

    return true;
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
    ui->checkListEncounterSlot->addItems(items);
}

void Filter::setLevelRange(u32 min, u32 max)
{
    ui->spinBoxLevelMin->setValue(min);
    ui->spinBoxLevelMax->setValue(max);
}

void Filter::toggleEncounterSlots(const std::vector<bool> &encounterSlots) const
{
    ui->checkListEncounterSlot->setChecks(encounterSlots);
}

u8 Filter::getWeightMax() const
{
    return static_cast<u8>(ui->spinBoxWeightMax->value());
}

u8 Filter::getWeightMin() const
{
    return static_cast<u8>(ui->spinBoxWeightMin->value());
}
