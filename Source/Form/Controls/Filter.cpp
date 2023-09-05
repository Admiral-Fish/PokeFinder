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
#include <Form/Util/IVCalculator.hpp>
#include <QMouseEvent>

/**
 * @brief Updates min/max values based on control keys selected
 *
 * @param minBox Spinbox that has the minimum value
 * @param maxBox Spinbox that has the maximum value
 * @param type Control keys
 */
static void changeCompare(QSpinBox *minBox, QSpinBox *maxBox, int type)
{
    int min;
    int max;
    if (type == Qt::NoModifier)
    {
        min = 0;
        max = 31;
    }
    else if (type == Qt::ControlModifier)
    {
        min = 31;
        max = 31;
    }
    else if (type == Qt::AltModifier)
    {
        min = 30;
        max = 31;
    }
    else if (type & Qt::ControlModifier && type & Qt::AltModifier)
    {
        min = 0;
        max = 0;
    }
    else
    {
        return;
    }
    minBox->setValue(min);
    maxBox->setValue(max);
}

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

    QStringList tips = { tr("Click to clear"), tr("Click holding ctrl to set 31"), tr("Click holding alt to set 30-31"),
                         tr("Click holding ctrl+alt to set 0") };

    QString tip = tips.join('\n');
    ui->labelHP->setToolTip(tip);
    ui->labelAtk->setToolTip(tip);
    ui->labelDef->setToolTip(tip);
    ui->labelSpA->setToolTip(tip);
    ui->labelSpD->setToolTip(tip);
    ui->labelSpe->setToolTip(tip);

    ui->labelHP->installEventFilter(this);
    ui->labelAtk->installEventFilter(this);
    ui->labelDef->installEventFilter(this);
    ui->labelSpA->installEventFilter(this);
    ui->labelSpA->installEventFilter(this);
    ui->labelSpD->installEventFilter(this);
    ui->labelSpe->installEventFilter(this);

    connect(ui->spinBoxHPMin, &QSpinBox::valueChanged, this, &Filter::ivsChanged);
    connect(ui->spinBoxHPMax, &QSpinBox::valueChanged, this, &Filter::ivsChanged);
    connect(ui->spinBoxAtkMin, &QSpinBox::valueChanged, this, &Filter::ivsChanged);
    connect(ui->spinBoxAtkMax, &QSpinBox::valueChanged, this, &Filter::ivsChanged);
    connect(ui->spinBoxDefMin, &QSpinBox::valueChanged, this, &Filter::ivsChanged);
    connect(ui->spinBoxDefMax, &QSpinBox::valueChanged, this, &Filter::ivsChanged);
    connect(ui->spinBoxSpAMin, &QSpinBox::valueChanged, this, &Filter::ivsChanged);
    connect(ui->spinBoxSpAMax, &QSpinBox::valueChanged, this, &Filter::ivsChanged);
    connect(ui->spinBoxSpDMin, &QSpinBox::valueChanged, this, &Filter::ivsChanged);
    connect(ui->spinBoxSpDMax, &QSpinBox::valueChanged, this, &Filter::ivsChanged);
    connect(ui->spinBoxSpeMin, &QSpinBox::valueChanged, this, &Filter::ivsChanged);
    connect(ui->spinBoxSpeMax, &QSpinBox::valueChanged, this, &Filter::ivsChanged);
    connect(ui->checkBoxShowStats, &QCheckBox::stateChanged, this, [=](int state) { emit showStatsChanged(state == Qt::Checked); });
    connect(ui->pushButtonIVCalculator, &QPushButton::clicked, this, &Filter::openIVCalculator);
}

Filter::~Filter()
{
    delete ui;
}

void Filter::disableControls(Controls control)
{
    if ((control & Controls::IVs) != Controls::None)
    {
        ui->labelHP->setVisible(false);
        ui->spinBoxHPMin->setVisible(false);
        ui->spinBoxHPMax->setVisible(false);

        ui->labelAtk->setVisible(false);
        ui->spinBoxAtkMin->setVisible(false);
        ui->spinBoxAtkMax->setVisible(false);

        ui->labelDef->setVisible(false);
        ui->spinBoxDefMin->setVisible(false);
        ui->spinBoxDefMax->setVisible(false);

        ui->labelSpA->setVisible(false);
        ui->spinBoxSpAMin->setVisible(false);
        ui->spinBoxSpAMax->setVisible(false);

        ui->labelSpD->setVisible(false);
        ui->spinBoxSpDMin->setVisible(false);
        ui->spinBoxSpDMax->setVisible(false);

        ui->labelSpe->setVisible(false);
        ui->spinBoxSpeMin->setVisible(false);
        ui->spinBoxSpeMax->setVisible(false);

        ui->checkBoxShowStats->setVisible(false);
        ui->pushButtonIVCalculator->setVisible(false);
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
    std::array<u8, 6> high = { static_cast<u8>(ui->spinBoxHPMax->value()),  static_cast<u8>(ui->spinBoxAtkMax->value()),
                               static_cast<u8>(ui->spinBoxDefMax->value()), static_cast<u8>(ui->spinBoxSpAMax->value()),
                               static_cast<u8>(ui->spinBoxSpDMax->value()), static_cast<u8>(ui->spinBoxSpeMax->value()) };
    return high;
}

std::array<u8, 6> Filter::getMinIVs() const
{
    std::array<u8, 6> low = { static_cast<u8>(ui->spinBoxHPMin->value()),  static_cast<u8>(ui->spinBoxAtkMin->value()),
                              static_cast<u8>(ui->spinBoxDefMin->value()), static_cast<u8>(ui->spinBoxSpAMin->value()),
                              static_cast<u8>(ui->spinBoxSpDMin->value()), static_cast<u8>(ui->spinBoxSpeMin->value()) };
    return low;
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

bool Filter::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        auto *mouse = reinterpret_cast<QMouseEvent *>(event);
        if (object == ui->labelHP)
        {
            changeCompare(ui->spinBoxHPMin, ui->spinBoxHPMax, mouse->modifiers());
            return true;
        }
        else if (object == ui->labelAtk)
        {
            changeCompare(ui->spinBoxAtkMin, ui->spinBoxAtkMax, mouse->modifiers());
            return true;
        }
        else if (object == ui->labelDef)
        {
            changeCompare(ui->spinBoxDefMin, ui->spinBoxDefMax, mouse->modifiers());
            return true;
        }
        else if (object == ui->labelSpA)
        {
            changeCompare(ui->spinBoxSpAMin, ui->spinBoxSpAMax, mouse->modifiers());
            return true;
        }
        else if (object == ui->labelSpD)
        {
            changeCompare(ui->spinBoxSpDMin, ui->spinBoxSpDMax, mouse->modifiers());
            return true;
        }
        else if (object == ui->labelSpe)
        {
            changeCompare(ui->spinBoxSpeMin, ui->spinBoxSpeMax, mouse->modifiers());
            return true;
        }
    }
    return false;
}

void Filter::openIVCalculator() const
{
    auto *calculator = new IVCalculator();
    connect(calculator, &IVCalculator::ivsCalculated, this, &Filter::updateIVs);
    calculator->show();
}

void Filter::updateIVs(const std::array<std::vector<u8>, 6> &ivs)
{
    QList<QSpinBox *> minIVs
        = { ui->spinBoxHPMin, ui->spinBoxAtkMin, ui->spinBoxDefMin, ui->spinBoxSpAMin, ui->spinBoxSpDMin, ui->spinBoxSpeMin };
    QList<QSpinBox *> maxIVs
        = { ui->spinBoxHPMax, ui->spinBoxAtkMax, ui->spinBoxDefMax, ui->spinBoxSpAMax, ui->spinBoxSpDMax, ui->spinBoxSpeMax };
    for (size_t i = 0; i < ivs.size(); i++)
    {
        const auto &iv = ivs[i];
        u8 min = 0;
        u8 max = 31;
        // Vector is sorted, grab first/last as min/max
        if (!iv.empty())
        {
            min = iv.front();
            max = iv.back();
        }
        minIVs[i]->setValue(min);
        maxIVs[i]->setValue(max);
    }
}
