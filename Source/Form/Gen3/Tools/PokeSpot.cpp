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

PokeSpot::PokeSpot(QWidget *parent) : QWidget(parent), ui(new Ui::PokeSpot)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    model = new PokeSpotModel(ui->tableView);
    ui->tableView->setModel(model);

    ui->textBoxFoodSeed->setValues(InputType::Seed32Bit);
    ui->textBoxEncounterSeed->setValues(InputType::Seed32Bit);
    ui->textBoxFoodInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxFoodMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxEncounterInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxEncounterMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxFoodDelay->setValues(InputType::Advance32Bit);
    ui->textBoxEncounterDelay->setValues(InputType::Advance32Bit);

    encounters = Encounters3::getPokeSpotEncounters();

    ui->filter->setEncounterSlots(3);

    std::vector<u16> locs;
    std::transform(encounters.begin(), encounters.end(), std::back_inserter(locs),
                   [](const EncounterArea &area) { return area.getLocation(); });

    auto locations = Translator::getLocations(locs, Game::Gales);
    std::vector<int> indices(locations.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(), [&locations](int i, int j) { return locations[i] < locations[j]; });

    ui->comboBoxLocation->clear();
    for (int i : indices)
    {
        ui->comboBoxLocation->addItem(QString::fromStdString(locations[i]), i);
    }

    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &PokeSpot::generate);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &PokeSpot::profileManager);
    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &PokeSpot::profileIndexChanged);
    connect(ui->comboBoxLocation, &QComboBox::currentIndexChanged, this, &PokeSpot::locationIndexChanged);
    connect(ui->comboBoxPokemon, &QComboBox::currentIndexChanged, this, &PokeSpot::pokemonIndexChanged);
    connect(ui->filter, &Filter::showStatsChanged, model, &PokeSpotModel::setShowStats);

    updateProfiles();
    locationIndexChanged(0);

    QSettings setting;
    setting.beginGroup("pokespot");
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

PokeSpot::~PokeSpot()
{
    QSettings setting;
    setting.beginGroup("pokespot");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void PokeSpot::updateProfiles()
{
    profiles = { Profile3("-", Game::Gales, 12345, 54321, false) };
    auto completeProfiles = ProfileLoader3::getProfiles();
    std::copy_if(completeProfiles.begin(), completeProfiles.end(), std::back_inserter(profiles),
                 [](const Profile3 &profile) { return (profile.getVersion() & Game::Gales) != Game::None; });

    ui->comboBoxProfiles->clear();
    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("pokespot/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void PokeSpot::generate()
{
    model->clearModel();

    u32 seedFood = ui->textBoxFoodSeed->getUInt();
    u32 seedEncounter = ui->textBoxEncounterSeed->getUInt();
    u32 initialAdvancesFood = ui->textBoxFoodInitialAdvances->getUInt();
    u32 maxAdvancesFood = ui->textBoxFoodMaxAdvances->getUInt();
    u32 initialAdvancesEncounter = ui->textBoxEncounterInitialAdvances->getUInt();
    u32 maxAdvancesEncounter = ui->textBoxEncounterMaxAdvances->getUInt();
    u32 delayFood = ui->textBoxFoodDelay->getUInt();
    u32 delayEncounter = ui->textBoxEncounterDelay->getUInt();

    auto filter = ui->filter->getFilter<WildStateFilter, true>();
    PokeSpotGenerator generator(initialAdvancesFood, maxAdvancesFood, delayFood, initialAdvancesEncounter, maxAdvancesEncounter,
                                delayEncounter, *currentProfile, filter);

    auto states = generator.generate(seedFood, seedEncounter, encounters[ui->comboBoxLocation->getCurrentInt()]);
    model->addItems(states);
}

void PokeSpot::locationIndexChanged(int index)
{
    if (index >= 0)
    {
        auto &area = encounters[ui->comboBoxLocation->getCurrentInt()];
        auto species = area.getUniqueSpecies();
        auto names = area.getSpecieNames();

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
        auto flags = encounters[ui->comboBoxLocation->getCurrentInt()].getSlots(num);
        ui->filter->toggleEncounterSlots(flags);
    }
}

void PokeSpot::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = &profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile->getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile->getSID()));
        ui->labelProfileGameValue->setText(QString::fromStdString(Translator::getGame(currentProfile->getVersion())));
    }
}

void PokeSpot::profileManager()
{
    auto *manager = new ProfileManager3();
    connect(manager, &ProfileManager3::profilesModified, this, [=](int num) { emit profilesModified(num); });
    manager->show();
}
