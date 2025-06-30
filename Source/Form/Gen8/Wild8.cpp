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

#include "Wild8.hpp"
#include "ui_Wild8.h"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen8/EncounterArea8.hpp>
#include <Core/Gen8/Encounters8.hpp>
#include <Core/Gen8/Generators/WildGenerator8.hpp>
#include <Core/Gen8/Profile8.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen8/Profile/ProfileManager8.hpp>
#include <Model/Gen8/WildModel8.hpp>
#include <QMessageBox>
#include <QSettings>

Wild8::Wild8(QWidget *parent) : QWidget(parent), ui(new Ui::Wild8)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    model = new WildModel8(ui->tableView);
    ui->tableView->setModel(model);

    ui->textBoxSeed0->setValues(InputType::Seed64Bit);
    ui->textBoxSeed1->setValues(InputType::Seed64Bit);
    ui->textBoxInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxOffset->setValues(InputType::Advance32Bit);

    ui->comboMenuLead->addAction(tr("None"), toInt(Lead::None));
    ui->comboMenuLead->addMenu(tr("Cute Charm"), { tr("♂ Lead"), tr("♀ Lead") }, { toInt(Lead::CuteCharmM), toInt(Lead::CuteCharmF) });
    ui->comboMenuLead->addMenu(tr("Item Modifier"), { tr("Compound Eyes"), tr("Super Luck") },
                               { toInt(Lead::CompoundEyes), toInt(Lead::SuperLuck) });
    ui->comboMenuLead->addMenu(tr("Level Modifier"), { tr("Hustle"), tr("Pressure"), tr("Vital Spirit") },
                               { toInt(Lead::Hustle), toInt(Lead::Pressure), toInt(Lead::VitalSpirit) });
    ui->comboMenuLead->addMenu(
        tr("Slot Modifier"), { tr("Harvest"), tr("Flash Fire"), tr("Magnet Pull"), tr("Static"), tr("Storm Drain") },
        { toInt(Lead::Harvest), toInt(Lead::FlashFire), toInt(Lead::MagnetPull), toInt(Lead::Static), toInt(Lead::StormDrain) });
    ui->comboMenuLead->addMenu(tr("Synchronize"), Translator::getNatures());

    ui->comboBoxEncounter->setup({ toInt(Encounter::Grass), toInt(Encounter::HoneyTree), toInt(Encounter::Surfing),
                                   toInt(Encounter::OldRod), toInt(Encounter::GoodRod), toInt(Encounter::SuperRod) });

    ui->comboBoxLocation->enableAutoComplete();

    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &Wild8::profileIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Wild8::generate);
    connect(ui->comboBoxEncounter, &QComboBox::currentIndexChanged, this, &Wild8::encounterIndexChanged);
    connect(ui->comboBoxLocation, &QComboBox::currentIndexChanged, this, &Wild8::locationIndexChanged);
    connect(ui->comboBoxPokemon, &QComboBox::currentIndexChanged, this, &Wild8::pokemonIndexChanged);
    connect(ui->comboBoxReplacement0, &QComboBox::currentIndexChanged, this, [=] {
        if (ui->checkBoxReplacement->isChecked())
        {
            updateEncounters();
            locationIndexChanged(0);
        }
    });
    connect(ui->comboBoxReplacement1, &QComboBox::currentIndexChanged, this, [=] {
        if (ui->checkBoxReplacement->isChecked())
        {
            updateEncounters();
            locationIndexChanged(0);
        }
    });
    connect(ui->buttonGroup, &QButtonGroup::buttonClicked, this, [=] {
        updateEncounters();
        locationIndexChanged(0);
    });
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Wild8::profileManager);
    connect(ui->filter, &Filter::showStatsChanged, model, &WildModel8::setShowStats);

    updateProfiles();
    encounterIndexChanged(0);

    QSettings setting;
    setting.beginGroup("wild8");
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

Wild8::~Wild8()
{
    QSettings setting;
    setting.beginGroup("wild8");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void Wild8::updateProfiles()
{
    profiles.clear();
    auto completeProfiles = ProfileLoader8::getProfiles();
    std::copy_if(completeProfiles.begin(), completeProfiles.end(), std::back_inserter(profiles),
                 [](const Profile8 &profile) { return (profile.getVersion() & Game::BDSP) != Game::None; });
    profiles.insert(profiles.begin(), Profile8("-", Game::BD, 12345, 54321, false, false, false));

    ui->comboBoxProfiles->clear();

    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("wild8/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void Wild8::updateEncounters()
{
    auto encounter = ui->comboBoxEncounter->getEnum<Encounter>();

    EncounterSettings8 settings = {};
    settings.time = ui->comboBoxTime->currentIndex();
    if (ui->checkBoxReplacement->isChecked())
    {
        settings.replacement[0] = ui->comboBoxReplacement0->getCurrentUShort();
        settings.replacement[1] = ui->comboBoxReplacement1->count() > 0 ? ui->comboBoxReplacement1->getCurrentUShort() : 0;
    }
    settings.radar = ui->checkBoxRadar->isChecked();
    settings.swarm = ui->checkBoxSwarm->isChecked();

    encounters = Encounters8::getEncounters(encounter, settings, currentProfile);
}

void Wild8::encounterIndexChanged(int index)
{
    if (index >= 0)
    {
        auto encounter = ui->comboBoxEncounter->getEnum<Encounter>();

        bool honey = encounter == Encounter::HoneyTree;

        ui->labelTime->setVisible(!honey);
        ui->comboBoxTime->setVisible(!honey);
        ui->checkBoxRadar->setVisible(!honey);
        ui->checkBoxSwarm->setVisible(!honey);

        ui->comboMenuLead->hideAction(toInt(Lead::CompoundEyes), honey); // Also handles Super Luck
        ui->comboMenuLead->hideAction(toInt(Lead::Pressure), honey); // Also handles Hustle and Vital Spirit
        ui->comboMenuLead->hideAction(toInt(Lead::Harvest), honey);
        ui->comboMenuLead->hideAction(toInt(Lead::FlashFire), honey);
        ui->comboMenuLead->hideAction(toInt(Lead::MagnetPull), honey);
        ui->comboMenuLead->hideAction(toInt(Lead::Static), honey);
        ui->comboMenuLead->hideAction(toInt(Lead::StormDrain), honey);

        updateEncounters();

        std::vector<u16> locs;
        std::transform(encounters.begin(), encounters.end(), std::back_inserter(locs),
                       [](const EncounterArea8 &area) { return area.getLocation(); });

        auto locations = Translator::getLocations(locs, currentProfile->getVersion());
        std::vector<int> indices(locations.size());
        std::iota(indices.begin(), indices.end(), 0);
        std::sort(indices.begin(), indices.end(), [&locations](int i, int j) { return locations[i] < locations[j]; });

        ui->comboBoxLocation->clear();
        for (int i : indices)
        {
            ui->comboBoxLocation->addItem(QString::fromStdString(locations[i]), i);
        }
    }
}

void Wild8::generate()
{
    u64 seed0 = ui->textBoxSeed0->getULong();
    u64 seed1 = ui->textBoxSeed1->getULong();
    if (seed0 == 0 && seed1 == 0)
    {
        QMessageBox msg(QMessageBox::Warning, tr("Missing seeds"), tr("Please insert missing seed information"));
        msg.exec();
        return;
    }

    auto encounter = ui->comboBoxEncounter->getEnum<Encounter>();
    Method method = Method::None;
    u8 fixedSlot = 0;
    if (encounter == Encounter::HoneyTree)
    {
        std::array<bool, 12> encounters = ui->filter->getEncounterSlots();
        if (std::count(encounters.begin(), encounters.end(), true) != 1)
        {
            QMessageBox msg(QMessageBox::Warning, tr("Too many slots selected"),
                            tr("Please select a single encounter slot for Honey Tree"));
            msg.exec();
            return;
        }
        else
        {
            method = Method::HoneyTree;
            fixedSlot = std::find(encounters.begin(), encounters.end(), true) - encounters.begin();
        }
    }

    model->clearModel();

    u32 initialAdvances = ui->textBoxInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxMaxAdvances->getUInt();
    u32 offset = ui->textBoxOffset->getUInt();
    auto lead = ui->comboMenuLead->getEnum<Lead>();

    auto filter = ui->filter->getFilter<WildStateFilter, true>();
    WildGenerator8 generator(initialAdvances, maxAdvances, offset, method, lead, encounters[ui->comboBoxLocation->getCurrentInt()],
                             *currentProfile, filter);

    auto states = generator.generate(seed0, seed1, fixedSlot);
    model->addItems(states);
}

void Wild8::locationIndexChanged(int index)
{
    if (index >= 0)
    {
        auto &area = encounters[ui->comboBoxLocation->getCurrentInt()];
        auto species = area.getUniqueSpecies();
        auto names = area.getSpecieNames();
        bool greatMarsh = area.greatMarsh(currentProfile->getVersion());
        bool trophyGarden = area.trophyGarden(currentProfile->getVersion());

        ui->filter->setEncounterSlots(area.getCount());

        ui->checkBoxReplacement->setVisible(greatMarsh || trophyGarden);
        ui->comboBoxReplacement0->setVisible(greatMarsh || trophyGarden);
        ui->comboBoxReplacement1->setVisible(trophyGarden);

        ui->comboBoxPokemon->clear();
        ui->comboBoxPokemon->addItem(QString("-"));
        for (size_t i = 0; i < species.size(); i++)
        {
            ui->comboBoxPokemon->addItem(QString::fromStdString(names[i]), species[i]);
        }

        if (greatMarsh && index != 0)
        {
            // Block signals so we don't cause infinite signal recursion
            ui->comboBoxReplacement0->blockSignals(true);

            ui->comboBoxReplacement0->clear();
            for (u16 specie : Encounters8::getGreatMarshPokemon(currentProfile))
            {
                if (specie == 0)
                {
                    break;
                }
                ui->comboBoxReplacement0->addItem(QString::fromStdString(Translator::getSpecie(specie)), specie);
            }

            ui->comboBoxReplacement0->blockSignals(false);
        }
        else if (trophyGarden && index != 0)
        {
            // Block signals so we don't cause infinite signal recursion
            ui->comboBoxReplacement0->blockSignals(true);
            ui->comboBoxReplacement1->blockSignals(true);

            ui->comboBoxReplacement0->clear();
            ui->comboBoxReplacement1->clear();
            for (u16 specie : Encounters8::getTrophyGardenPokemon())
            {
                const auto &name = Translator::getSpecie(specie);
                ui->comboBoxReplacement0->addItem(QString::fromStdString(name), specie);
                ui->comboBoxReplacement1->addItem(QString::fromStdString(name), specie);
            }

            ui->comboBoxReplacement0->blockSignals(false);
            ui->comboBoxReplacement1->blockSignals(false);
        }
    }
}

void Wild8::pokemonIndexChanged(int index)
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

void Wild8::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = &profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile->getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile->getSID()));
        ui->labelProfileGameValue->setText(QString::fromStdString(Translator::getGame(currentProfile->getVersion())));

        encounterIndexChanged(0);
    }
}

void Wild8::profileManager()
{
    auto *manager = new ProfileManager8();
    connect(manager, &ProfileManager8::profilesModified, this, [=](int num) { emit profilesModified(num); });
    manager->show();
}
