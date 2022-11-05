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

#include "Wild8.hpp"
#include "ui_Wild8.h"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Gen8/EncounterArea8.hpp>
#include <Core/Gen8/Encounters8.hpp>
#include <Core/Gen8/Generators/WildGenerator8.hpp>
#include <Core/Gen8/Profile8.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen8/Profile/ProfileManager8.hpp>
#include <Model/Gen8/WildModel8.hpp>
#include <QMenu>
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

    ui->toolButtonLead->addAction(tr("None"), toInt(Lead::None));
    ui->toolButtonLead->addAction(tr("Arena Trap"), toInt(Lead::ArenaTrap));
    ui->toolButtonLead->addAction(tr("Compound Eyes"), toInt(Lead::CompoundEyes));
    ui->toolButtonLead->addMenu(tr("Cute Charm"), { tr("♂ Lead"), tr("♀ Lead") }, { toInt(Lead::CuteCharmM), toInt(Lead::CuteCharmF) });
    ui->toolButtonLead->addMenu(
        tr("Encounter Modifier"), { tr("Harvest"), tr("Flash Fire"), tr("Magnet Pull"), tr("Static"), tr("Storm Drain") },
        { toInt(Lead::Harvest), toInt(Lead::FlashFire), toInt(Lead::MagnetPull), toInt(Lead::Static), toInt(Lead::StormDrain) });
    ui->toolButtonLead->addAction(tr("Pressure"), toInt(Lead::Pressure));
    ui->toolButtonLead->addMenu(tr("Synchronize"), *Translator::getNatures());

    ui->comboBoxEncounter->setup({ toInt(Encounter::Grass), toInt(Encounter::Surfing), toInt(Encounter::OldRod), toInt(Encounter::GoodRod),
                                   toInt(Encounter::SuperRod) });

    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &Wild8::profileIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Wild8::generate);
    connect(ui->comboBoxEncounter, &QComboBox::currentIndexChanged, this, &Wild8::encounterIndexChanged);
    connect(ui->comboBoxLocation, &QComboBox::currentIndexChanged, this, &Wild8::locationIndexChanged);
    connect(ui->comboBoxPokemon, &QComboBox::currentIndexChanged, this, &Wild8::pokemonIndexChanged);
    connect(ui->buttonGroup, &QButtonGroup::buttonClicked, this, [=] {
        updateEncounters();
        encounterIndexChanged(0);
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
    profiles.insert(profiles.begin(), Profile8("-", Game::BD, 12345, 54321, false, false));

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
    auto encounter = static_cast<Encounter>(ui->comboBoxEncounter->currentData().toInt());
    int time = ui->comboBoxTime->currentIndex();
    bool radar = ui->checkBoxRadar->isChecked();
    bool swarm = ui->checkBoxSwarm->isChecked();
    encounters = Encounters8::getEncounters(encounter, time, radar, swarm, currentProfile);
}

void Wild8::encounterIndexChanged(int index)
{
    if (index >= 0)
    {
        std::vector<std::string> t;
        auto encounter = ui->comboBoxEncounter->getEnum<Encounter>();
        switch (encounter)
        {
        case Encounter::Grass:
            t = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11" };
            break;
        case Encounter::Surfing:
        case Encounter::OldRod:
        case Encounter::GoodRod:
        case Encounter::SuperRod:
            t = { "0", "1", "2", "3", "4" };
            break;
        default:
            break;
        }
        ui->filter->setEncounterSlots(t);

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

    model->clearModel();

    u32 initialAdvances = ui->textBoxInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxMaxAdvances->getUInt();
    u16 tid = currentProfile->getTID();
    u16 sid = currentProfile->getSID();
    auto encounter = ui->comboBoxEncounter->getEnum<Encounter>();
    auto lead = ui->toolButtonLead->getEnum<Lead>();
    u32 offset = 0;
    if (ui->filter->useDelay())
    {
        offset = ui->filter->getDelay();
    }

    WildStateFilter8 filter(ui->filter->getGender(), ui->filter->getAbility(), ui->filter->getShiny(), ui->filter->getDisableFilters(),
                            ui->filter->getMinIVs(), ui->filter->getMaxIVs(), ui->filter->getNatures(), ui->filter->getHiddenPowers(),
                            ui->filter->getEncounterSlots());
    WildGenerator8 generator(initialAdvances, maxAdvances, offset, tid, sid, currentProfile->getVersion(), encounter, lead, filter);

    auto states = generator.generate(seed0, seed1, encounters[ui->comboBoxLocation->currentData().toInt()]);
    model->addItems(states);
}

void Wild8::locationIndexChanged(int index)
{
    if (index >= 0)
    {
        auto &area = encounters[ui->comboBoxLocation->currentData().toInt()];
        auto species = area.getUniqueSpecies();
        auto names = area.getSpecieNames();

        ui->comboBoxPokemon->clear();
        ui->comboBoxPokemon->addItem(QString("-"));
        for (size_t i = 0; i < species.size(); i++)
        {
            ui->comboBoxPokemon->addItem(QString::fromStdString(names[i]), species[i]);
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
        auto flags = encounters[ui->comboBoxLocation->currentData().toInt()].getSlots(num);
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
        ui->labelProfileGameValue->setText(QString::fromStdString(*Translator::getGame(currentProfile->getVersion())));

        encounterIndexChanged(0);
    }
}

void Wild8::profileManager()
{
    auto *manager = new ProfileManager8();
    connect(manager, &ProfileManager8::updateProfiles, this, [=] { emit alertProfiles(8); });
    manager->show();
}
