/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "UnownFilter.hpp"
#include "ui_UnownFilter.h"
#include <Core/Util/Translator.hpp>
#include <array>

UnownFilter::UnownFilter(QWidget *parent) : QWidget(parent), ui(new Ui::UnownFilter)
{
    ui->setupUi(this);

    ui->comboBoxAbility->setup({ 255, 0, 1 });
    ui->comboBoxGender->setup({ 255, 0, 1 });
    ui->comboBoxGenderRatio->setup({ 255, 127, 191, 63, 31, 0, 254 });
    ui->checkListHiddenPower->setup(Translator::getHiddenPowers());
    ui->checkListNature->setup(Translator::getNatures());
    ui->checkListLetter->setup({ "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "!", "?" });
    ui->comboBoxShiny->setup({ 255, 1, 2, 3 });
    ui->textBoxDelay->setValues(InputType::Advance32Bit);

    connect(ui->pushButtonLetterAny, &QPushButton::clicked, ui->checkListLetter, &CheckList::resetChecks);
    connect(ui->pushButtonHiddenPowerAny, &QPushButton::clicked, ui->checkListHiddenPower, &CheckList::resetChecks);
    connect(ui->pushButtonNatureAny, &QPushButton::clicked, ui->checkListNature, &CheckList::resetChecks);
}

UnownFilter::~UnownFilter()
{
    delete ui;
}

std::array<u8, 6> UnownFilter::getMinIVs() const
{
    return ui->ivFilter->getLower();
}

std::array<u8, 6> UnownFilter::getMaxIVs() const
{
    return ui->ivFilter->getUpper();
}

u8 UnownFilter::getAbility() const
{
    return ui->comboBoxAbility->getCurrentByte();
}

u8 UnownFilter::getGender() const
{
    return ui->comboBoxGender->getCurrentByte();
}

u8 UnownFilter::getGenderRatio() const
{
    return ui->comboBoxGenderRatio->getCurrentByte();
}

std::vector<bool> UnownFilter::getLetters()
{
    return ui->checkListLetter->getChecked();
}

void UnownFilter::setLetters(const std::vector<std::string> &letters) const
{
    ui->checkListLetter->setup(letters);
}

void UnownFilter::toggleLetters(const std::vector<bool> &letters) const
{
    ui->checkListLetter->setChecks(letters);
}

void UnownFilter::resetLetters() const
{
    ui->checkListLetter->resetChecks();
}

std::vector<bool> UnownFilter::getHiddenPowers()
{
    return ui->checkListHiddenPower->getChecked();
}

std::vector<bool> UnownFilter::getNatures()
{
    return ui->checkListNature->getChecked();
}

u8 UnownFilter::getShiny() const
{
    return ui->comboBoxShiny->getCurrentByte();
}

bool UnownFilter::useDelay() const
{
    return ui->checkBoxUseDelay->isChecked();
}

u32 UnownFilter::getDelay() const
{
    return ui->textBoxDelay->getUInt();
}

bool UnownFilter::getDisableFilters()
{
    return ui->checkBoxDisableFilters->isChecked();
}

void UnownFilter::disableControls(u16 control)
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
        ui->labelLetter->setVisible(false);
        ui->checkListLetter->setVisible(false);
        ui->pushButtonLetterAny->setVisible(false);
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

void UnownFilter::setGenderRatio(u8 genderRatio)
{
    ui->comboBoxGenderRatio->setCurrentIndex(ui->comboBoxGenderRatio->findData(genderRatio));
}

void UnownFilter::enableHiddenAbility()
{
    ui->comboBoxAbility->addItem("H", 2);
}
