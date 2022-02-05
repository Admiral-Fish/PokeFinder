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
#include <Core/Enum/Lead.hpp>
#include <Core/Gen8/EncounterArea8.hpp>
#include <Core/Gen8/Encounters8.hpp>
#include <Core/Gen8/Generators/WildGenerator8.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/Controls/Controls.hpp>
#include <Forms/Gen8/Profile/ProfileManager8.hpp>
#include <Forms/Models/Gen8/WildModel8.hpp>
#include <QMenu>
#include <QSettings>

Wild8::Wild8(QWidget *parent) : QWidget(parent), ui(new Ui::Wild8)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_QuitOnClose, false);

    updateProfiles();
    setupModels();
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

bool Wild8::hasProfiles() const
{
    return !profiles.empty();
}

void Wild8::updateProfiles()
{
    profiles.clear();
    auto completeProfiles = ProfileLoader8::getProfiles();
    std::copy_if(completeProfiles.begin(), completeProfiles.end(), std::back_inserter(profiles),
                 [](const Profile &profile) { return (profile.getVersion() & Game::BDSP) != Game::None; });
    profiles.insert(profiles.begin(), Profile8(Game::BD));

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

    profilesIndexChanged(0);
}

void Wild8::setupModels()
{
    model = new WildModel8(ui->tableView);

    menu = new QMenu(ui->tableView);

    ui->tableView->setModel(model);

    ui->textBoxSeed0->setValues(InputType::Seed64Bit);
    ui->textBoxSeed1->setValues(InputType::Seed64Bit);
    ui->textBoxInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxMaxAdvances->setValues(InputType::Advance32Bit);

    ui->toolButtonLead->addAction(tr("None"), toInt(Lead::None));
    ui->toolButtonLead->addMenu(tr("Synchronize"), Translator::getNatures());
    ui->toolButtonLead->addMenu(tr("Cute Charm"), { tr("♂ Lead"), tr("♀ Lead") }, { toInt(Lead::CuteCharm), toInt(Lead::CuteCharmFemale) });

    ui->comboBoxEncounter->clear();
    ui->comboBoxEncounter->addItem(tr("Grass"), toInt(Encounter::Grass));
    ui->comboBoxEncounter->addItem(tr("Surfing"), toInt(Encounter::Surfing));
    ui->comboBoxEncounter->addItem(tr("Old Rod"), toInt(Encounter::OldRod));
    ui->comboBoxEncounter->addItem(tr("Good Rod"), toInt(Encounter::GoodRod));
    ui->comboBoxEncounter->addItem(tr("Super Rod"), toInt(Encounter::SuperRod));

    ui->filter->disableControls(Controls::GenderRatio);

    QAction *outputTXTGenerator = menu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVGenerator = menu->addAction(tr("Output Results to CSV"));
    connect(outputTXTGenerator, &QAction::triggered, [=] { ui->tableView->outputModel(); });
    connect(outputCSVGenerator, &QAction::triggered, [=] { ui->tableView->outputModel(true); });

    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Wild8::profilesIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Wild8::generate);
    connect(ui->comboBoxEncounter, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Wild8::encounterIndexChanged);
    connect(ui->comboBoxLocation, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Wild8::locationIndexChanged);
    connect(ui->comboBoxPokemon, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Wild8::pokemonIndexChanged);
    connect(ui->comboBoxTime, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Wild8::timeIndexChanged);
    connect(ui->tableView, &QTableView::customContextMenuRequested, this, &Wild8::tableViewContextMenu);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Wild8::profileManager);

    encounterIndexChanged(0);

    QSettings setting;
    setting.beginGroup("wild8");
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

void Wild8::updateLocations()
{
    auto encounter = static_cast<Encounter>(ui->comboBoxEncounter->currentData().toInt());
    int time = ui->comboBoxTime->currentIndex();

    encounters = Encounters8::getEncounters(encounter, time, currentProfile);

    std::vector<u16> locs;
    std::transform(encounters.begin(), encounters.end(), std::back_inserter(locs),
                   [](const EncounterArea8 &area) { return area.getLocation(); });

    std::vector<std::string> locations = Translator::getLocations(locs, currentProfile.getVersion());
    std::vector<int> indices(locations.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(), [&locations](int i, int j) { return locations[i] < locations[j]; });

    ui->comboBoxLocation->clear();
    for (int index : indices)
    {
        ui->comboBoxLocation->addItem(QString::fromStdString(locations[index]), index);
    }
}

void Wild8::updatePokemon()
{
    auto area = encounters[ui->comboBoxLocation->currentData().toInt()];
    std::vector<u16> species = area.getUniqueSpecies();

    std::vector<std::string> names = area.getSpecieNames();

    ui->comboBoxPokemon->clear();
    ui->comboBoxPokemon->addItem("-");
    for (size_t i = 0; i < species.size(); i++)
    {
        ui->comboBoxPokemon->addItem(QString::fromStdString(names[i]), species[i]);
    }
}

void Wild8::generate()
{
    model->clearModel();

    u64 seed0 = ui->textBoxSeed0->getULong();
    u64 seed1 = ui->textBoxSeed1->getULong();
    u32 initialAdvances = ui->textBoxInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxMaxAdvances->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u32 offset = 0;
    if (ui->filter->useDelay())
    {
        offset = ui->filter->getDelay();
    }

    StateFilter filter(ui->filter->getGender(), ui->filter->getAbility(), ui->filter->getShiny(), ui->filter->getDisableFilters(),
                       ui->filter->getMinIVs(), ui->filter->getMaxIVs(), ui->filter->getNatures(), ui->filter->getHiddenPowers(),
                       ui->filter->getEncounterSlots());

    WildGenerator8 generator(initialAdvances, maxAdvances, tid, sid, filter);
    generator.setOffset(offset);
    generator.setEncounter(static_cast<Encounter>(ui->comboBoxEncounter->getCurrentInt()));

    if (ui->toolButtonLead->text().contains(tr("Synchronize")))
    {
        generator.setLead(Lead::Synchronize);
        generator.setSynchNature(ui->toolButtonLead->getData());
    }
    else
    {
        generator.setLead(static_cast<Lead>(ui->toolButtonLead->getData()));
    }

    auto states = generator.generate(seed0, seed1, encounters[ui->comboBoxLocation->currentData().toInt()]);
    model->addItems(states);
}

void Wild8::profilesIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile.getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile.getSID()));
        ui->labelProfileGameValue->setText(QString::fromStdString(currentProfile.getVersionString()));

        updateLocations();
    }
}

void Wild8::encounterIndexChanged(int index)
{
    if (index >= 0)
    {
        std::vector<std::string> t;
        auto encounter = static_cast<Encounter>(ui->comboBoxEncounter->currentData().toInt());

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
        case Encounter::RockSmash:
            t = { "0", "1" };
            break;
        default:
            break;
        }

        ui->filter->setEncounterSlots(t);
        updateLocations();
    }
}

void Wild8::locationIndexChanged(int index)
{
    if (index >= 0)
    {
        updatePokemon();
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
        std::vector<bool> flags = encounters[ui->comboBoxLocation->currentData().toInt()].getSlots(num);
        ui->filter->toggleEncounterSlots(flags);
    }
}

void Wild8::timeIndexChanged(int index)
{
    if (index >= 0)
    {
        int position = ui->comboBoxLocation->currentIndex();
        updateLocations();
        ui->comboBoxLocation->setCurrentIndex(position);
    }
}

void Wild8::tableViewContextMenu(QPoint pos)
{
    if (model->rowCount() > 0)
    {
        menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
    }
}

void Wild8::profileManager()
{
    auto *manager = new ProfileManager8();
    connect(manager, &ProfileManager8::updateProfiles, this, [=] { emit alertProfiles(8); });
    manager->show();
}
