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

#include "PokeSpot.hpp"
#include "ui_PokeSpot.h"
#include <Core/Enum/Game.hpp>
#include <Core/Gen3/Encounters3.hpp>
#include <Core/Gen3/Generators/PokeSpotGenerator.hpp>
#include <Core/Gen3/Profile3.hpp>
#include <Core/Parents/EncounterArea.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen3/Profile/ProfileManager3.hpp>
#include <Model/Gen3/PokeSpotModel.hpp>
#include <QSettings>

static const QString settingPrefix = QStringLiteral("pokespot");

PokeSpot::PokeSpot(QWidget *parent) : QWidget(parent), ui(new Ui::PokeSpot)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    ui->profileDisplay->setup(settingPrefix, Game::Gales);

    model = new PokeSpotModel(ui->tableView);
    ui->tableView->setModel(model);

    ui->textBoxFoodSeed->setValues(InputType::Seed32Bit);
    ui->textBoxEncounterSeed->setValues(InputType::Seed32Bit);
    ui->textBoxFoodInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxFoodMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxEncounterInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxEncounterMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxFoodOffset->setValues(InputType::Advance32Bit);
    ui->textBoxEncounterOffset->setValues(InputType::Advance32Bit);

    encounters = Encounters3::getPokeSpotEncounters();

    std::vector<u16> locs;
    std::ranges::transform(encounters, std::back_inserter(locs), [](const EncounterArea &area) { return area.getLocation(); });

    ui->comboBoxLocation->clear();
    ui->comboBoxLocation->addItems(Translator::getLocations(locs, Game::Gales));

    connect(ui->profileDisplay, &ProfileDisplay3::profileChanged, this, &PokeSpot::profileChanged);
    connect(ui->profileDisplay, &ProfileDisplay3::profilesChanged, this, &PokeSpot::profilesChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &PokeSpot::generate);
    connect(ui->comboBoxLocation, &QComboBox::currentIndexChanged, this, &PokeSpot::locationIndexChanged);
    connect(ui->comboBoxPokemon, &QComboBox::currentIndexChanged, this, &PokeSpot::pokemonIndexChanged);
    connect(ui->filter, &Filter::showStatsChanged, model, &PokeSpotModel::setShowStats);

    updateProfiles();
    locationIndexChanged(0);

    QSettings setting;
    setting.beginGroup(settingPrefix);
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

PokeSpot::~PokeSpot()
{
    QSettings setting;
    setting.beginGroup(settingPrefix);
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void PokeSpot::updateProfiles()
{
    ui->profileDisplay->updateProfiles();
}

void PokeSpot::generate()
{
    if (!ui->filter->isValid())
    {
        return;
    }

    model->clearModel();

    u32 seedFood = ui->textBoxFoodSeed->getUInt();
    u32 seedEncounter = ui->textBoxEncounterSeed->getUInt();
    u32 initialAdvancesFood = ui->textBoxFoodInitialAdvances->getUInt();
    u32 maxAdvancesFood = ui->textBoxFoodMaxAdvances->getUInt();
    u32 initialAdvancesEncounter = ui->textBoxEncounterInitialAdvances->getUInt();
    u32 maxAdvancesEncounter = ui->textBoxEncounterMaxAdvances->getUInt();
    u32 offsetFood = ui->textBoxFoodOffset->getUInt();
    u32 offsetEncounter = ui->textBoxEncounterOffset->getUInt();

    auto filter = ui->filter->getFilter<WildStateFilter, true>();
    PokeSpotGenerator generator(initialAdvancesFood, maxAdvancesFood, offsetFood, initialAdvancesEncounter, maxAdvancesEncounter,
                                offsetEncounter, *currentProfile, filter);

    auto states = generator.generate(seedFood, seedEncounter, encounters[ui->comboBoxLocation->currentIndex()]);
    model->addItems(states);
}

void PokeSpot::locationIndexChanged(int index)
{
    if (index >= 0)
    {
        auto &area = encounters[ui->comboBoxLocation->currentIndex()];
        auto species = area.getUniqueSpecies();
        auto names = area.getSpecieNames();

        ui->filter->setEncounterSlots(area.getCount());

        ui->comboBoxPokemon->clear();
        ui->comboBoxPokemon->addItem("-");
        for (size_t i = 0; i < species.size(); i++)
        {
            ui->comboBoxPokemon->addItem(QString::fromStdString(names[i]), species[i]);
        }
    }
}

void PokeSpot::pokemonIndexChanged(int index)
{
    if (index <= 0)
    {
        ui->filter->resetEncounterSlots();
    }
    else
    {
        u16 num = ui->comboBoxPokemon->getCurrentUShort();
        auto flags = encounters[ui->comboBoxLocation->currentIndex()].getSlots(num);
        ui->filter->toggleEncounterSlots(flags);
    }
}

void PokeSpot::profileChanged(const Profile3 &profile)
{
    currentProfile = &profile;
}
