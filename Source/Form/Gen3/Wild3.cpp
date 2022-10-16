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

#include "Wild3.hpp"
#include "ui_Wild3.h"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen3/Encounters3.hpp>
#include <Core/Gen3/Generators/WildGenerator3.hpp>
#include <Core/Gen3/Profile3.hpp>
#include <Core/Gen3/Searchers/WildSearcher3.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/Util/Nature.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen3/Profile/ProfileManager3.hpp>
#include <Model/Gen3/WildModel3.hpp>
#include <QMenu>
#include <QSettings>
#include <QThread>
#include <QTimer>

Wild3::Wild3(QWidget *parent) : QWidget(parent), ui(new Ui::Wild3)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    generatorModel = new WildGeneratorModel3(ui->tableViewGenerator);
    searcherModel = new WildSearcherModel3(ui->tableViewSearcher);

    ui->tableViewGenerator->setModel(generatorModel);
    ui->tableViewSearcher->setModel(searcherModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed32Bit);
    ui->textBoxGeneratorInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);

    ui->comboBoxGeneratorMethod->setup({ toInt(Method::Method1), toInt(Method::Method2), toInt(Method::Method4) });
    ui->comboBoxSearcherMethod->setup({ toInt(Method::Method1), toInt(Method::Method2), toInt(Method::Method4) });

    ui->comboBoxGeneratorEncounter->setup({ toInt(Encounter::Grass), toInt(Encounter::RockSmash), toInt(Encounter::Surfing),
                                            toInt(Encounter::OldRod), toInt(Encounter::GoodRod), toInt(Encounter::SuperRod) });
    ui->comboBoxSearcherEncounter->setup({ toInt(Encounter::Grass), toInt(Encounter::RockSmash), toInt(Encounter::Surfing),
                                           toInt(Encounter::OldRod), toInt(Encounter::GoodRod), toInt(Encounter::SuperRod) });

    ui->filterSearcher->disableControls(Controls::UseDelay | Controls::DisableFilter);

    ui->toolButtonGeneratorLead->addAction(tr("None"), toInt(Lead::None));
    ui->toolButtonGeneratorLead->addMenu(tr("Cute Charm"), { tr("♂ Lead"), tr("♀ Lead") },
                                         { toInt(Lead::CuteCharmM), toInt(Lead::CuteCharmF) });
    ui->toolButtonGeneratorLead->addAction(tr("Magnet Pull"), toInt(Lead::MagnetPull));
    ui->toolButtonGeneratorLead->addAction(tr("Pressure"), toInt(Lead::Pressure));
    ui->toolButtonGeneratorLead->addAction(tr("Static"), toInt(Lead::Static));
    ui->toolButtonGeneratorLead->addMenu(tr("Synchronize"), *Translator::getNatures());

    ui->comboBoxSearcherLead->setup({ toInt(Lead::None), toInt(Lead::CuteCharmM), toInt(Lead::CuteCharmF), toInt(Lead::MagnetPull),
                                      toInt(Lead::Pressure), toInt(Lead::Static), toInt(Lead::Synchronize) });

    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &Wild3::profilesIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Wild3::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Wild3::search);
    connect(ui->comboBoxGeneratorEncounter, &QComboBox::currentIndexChanged, this, &Wild3::generatorEncounterIndexChanged);
    connect(ui->comboBoxSearcherEncounter, &QComboBox::currentIndexChanged, this, &Wild3::searcherEncounterIndexChanged);
    connect(ui->comboBoxGeneratorLocation, &QComboBox::currentIndexChanged, this, &Wild3::generatorLocationIndexChanged);
    connect(ui->comboBoxSearcherLocation, &QComboBox::currentIndexChanged, this, &Wild3::searcherLocationIndexChanged);
    connect(ui->comboBoxGeneratorPokemon, &QComboBox::currentIndexChanged, this, &Wild3::generatorPokemonIndexChanged);
    connect(ui->comboBoxSearcherPokemon, &QComboBox::currentIndexChanged, this, &Wild3::searcherPokemonIndexChanged);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Wild3::profileManager);
    connect(ui->filterGenerator, &Filter::showStatsChanged, generatorModel, &WildGeneratorModel3::setShowStats);
    connect(ui->filterSearcher, &Filter::showStatsChanged, searcherModel, &WildSearcherModel3::setShowStats);

    updateProfiles();
    generatorEncounterIndexChanged(0);
    searcherEncounterIndexChanged(0);

    QSettings setting;
    if (setting.contains("wild3/geometry"))
    {
        this->restoreGeometry(setting.value("wild3/geometry").toByteArray());
    }
}

Wild3::~Wild3()
{
    QSettings setting;
    setting.beginGroup("wild3");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void Wild3::updateProfiles()
{
    profiles = { Profile3("None", Game::Emerald, 12345, 54321, false) };
    auto completeProfiles = ProfileLoader3::getProfiles();
    std::copy_if(completeProfiles.begin(), completeProfiles.end(), std::back_inserter(profiles),
                 [](const Profile3 &profile) { return (profile.getVersion() & Game::GC) == Game::None; });

    ui->comboBoxProfiles->clear();
    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("wild3/profile").toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void Wild3::generate()
{
    generatorModel->clearModel();

    u32 seed = ui->textBoxGeneratorSeed->getUInt();
    u32 initialAdvances = ui->textBoxGeneratorInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();
    u16 tid = currentProfile->getTID();
    u16 sid = currentProfile->getSID();
    auto method = ui->comboBoxGeneratorMethod->getEnum<Method>();
    auto encounter = ui->comboBoxGeneratorEncounter->getEnum<Encounter>();
    auto lead = ui->toolButtonGeneratorLead->getEnum<Lead>();

    u32 offset = 0;
    if (ui->filterGenerator->useDelay())
    {
        offset = ui->filterGenerator->getDelay();
    }

    WildStateFilter3 filter(ui->filterGenerator->getGender(), ui->filterGenerator->getAbility(), ui->filterGenerator->getShiny(),
                            ui->filterGenerator->getDisableFilters(), ui->filterGenerator->getMinIVs(), ui->filterGenerator->getMaxIVs(),
                            ui->filterGenerator->getNatures(), ui->filterGenerator->getHiddenPowers(),
                            ui->filterGenerator->getEncounterSlots());

    WildGenerator3 generator(initialAdvances, maxAdvances, offset, tid, sid, currentProfile->getVersion(), method, encounter, lead, filter);

    auto states = generator.generate(seed, encounterGenerator[ui->comboBoxGeneratorLocation->getCurrentInt()]);
    generatorModel->addItems(states);
}

void Wild3::generatorEncounterIndexChanged(int index)
{
    if (index >= 0)
    {
        std::vector<std::string> t;
        Encounter encounter = static_cast<Encounter>(ui->comboBoxGeneratorEncounter->currentData().toInt());
        switch (encounter)
        {
        case Encounter::Grass:
            t = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11" };
            break;
        case Encounter::RockSmash:
        case Encounter::Surfing:
        case Encounter::SuperRod:
            t = { "0", "1", "2", "3", "4" };
            break;
        case Encounter::OldRod:
            t = { "0", "1" };
            break;
        case Encounter::GoodRod:
            t = { "0", "1", "2" };
            break;
        default:
            break;
        }
        ui->filterGenerator->setEncounterSlots(t);

        encounterGenerator = Encounters3::getEncounters(encounter, currentProfile->getVersion());

        std::vector<u16> locs;
        std::transform(encounterGenerator.begin(), encounterGenerator.end(), std::back_inserter(locs),
                       [](const EncounterArea3 &area) { return area.getLocation(); });

        auto locations = Translator::getLocations(locs, currentProfile->getVersion());
        std::vector<int> indices(locations.size());
        std::iota(indices.begin(), indices.end(), 0);
        std::sort(indices.begin(), indices.end(), [&locations](int i, int j) { return locations[i] < locations[j]; });

        ui->comboBoxGeneratorLocation->clear();
        for (int i : indices)
        {
            ui->comboBoxGeneratorLocation->addItem(QString::fromStdString(locations[i]), i);
        }
    }
}

void Wild3::generatorLocationIndexChanged(int index)
{
    if (index >= 0)
    {
        auto area = encounterGenerator[ui->comboBoxGeneratorLocation->currentData().toInt()];
        auto species = area.getUniqueSpecies();
        auto names = area.getSpecieNames();

        ui->comboBoxGeneratorPokemon->clear();
        ui->comboBoxGeneratorPokemon->addItem("-");
        for (size_t i = 0; i < species.size(); i++)
        {
            ui->comboBoxGeneratorPokemon->addItem(QString::fromStdString(names[i]), species[i]);
        }
    }
}

void Wild3::generatorPokemonIndexChanged(int index)
{
    if (index <= 0)
    {
        ui->filterGenerator->resetEncounterSlots();
    }
    else
    {
        u16 num = static_cast<u16>(ui->comboBoxGeneratorPokemon->currentData().toUInt());
        auto flags = encounterGenerator[ui->comboBoxGeneratorLocation->getCurrentInt()].getSlots(num);
        ui->filterGenerator->toggleEncounterSlots(flags);
    }
}

void Wild3::profilesIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = &profiles[index];

        if (currentProfile->getDeadBattery())
        {
            ui->textBoxGeneratorSeed->setText("5a0");
        }

        ui->labelProfileTIDValue->setText(QString::number(currentProfile->getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile->getSID()));
        ui->labelProfileGameValue->setText(QString::fromStdString(*Translator::getGame(currentProfile->getVersion())));

        if ((currentProfile->getVersion() & Game::Emerald) != Game::None)
        {
            ui->labelGeneratorLead->setVisible(true);
            ui->toolButtonGeneratorLead->setVisible(true);

            ui->labelSearcherLead->setVisible(true);
            ui->comboBoxSearcherLead->setVisible(true);
        }
        else
        {
            ui->toolButtonGeneratorLead->clearSelection();
            ui->labelGeneratorLead->setVisible(false);
            ui->toolButtonGeneratorLead->setVisible(false);

            ui->comboBoxSearcherLead->setCurrentIndex(0);
            ui->labelSearcherLead->setVisible(false);
            ui->comboBoxSearcherLead->setVisible(false);
        }

        generatorEncounterIndexChanged(0);
        searcherEncounterIndexChanged(0);
    }
}

void Wild3::profileManager()
{
    auto *manager = new ProfileManager3();
    connect(manager, &ProfileManager3::updateProfiles, this, [=] { emit alertProfiles(3); });
    manager->show();
}

void Wild3::search()
{
    searcherModel->clearModel();

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    std::array<u8, 6> min = ui->filterSearcher->getMinIVs();
    std::array<u8, 6> max = ui->filterSearcher->getMaxIVs();

    WildStateFilter3 filter(ui->filterSearcher->getGender(), ui->filterSearcher->getAbility(), ui->filterSearcher->getShiny(), false, min,
                            max, ui->filterSearcher->getNatures(), ui->filterSearcher->getHiddenPowers(),
                            ui->filterSearcher->getEncounterSlots());

    u16 tid = currentProfile->getTID();
    u16 sid = currentProfile->getSID();
    auto method = ui->comboBoxSearcherMethod->getEnum<Method>();
    auto encounter = ui->comboBoxSearcherEncounter->getEnum<Encounter>();
    auto lead = ui->comboBoxSearcherLead->getEnum<Lead>();

    auto *searcher = new WildSearcher3(tid, sid, currentProfile->getVersion(), method, encounter, lead,
                                       encounterSearcher[ui->comboBoxSearcherLocation->getCurrentInt()], filter);

    int maxProgress = 1;
    for (u8 i = 0; i < 6; i++)
    {
        maxProgress *= max[i] - min[i] + 1;
    }
    ui->progressBar->setRange(0, maxProgress);

    auto *thread = QThread::create([=] { searcher->startSearch(min, max); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(ui->pushButtonCancel, &QPushButton::clicked, [searcher] { searcher->cancelSearch(); });

    auto *timer = new QTimer();
    timer->callOnTimeout(this, [=] {
        searcherModel->addItems(searcher->getResults());
        ui->progressBar->setValue(searcher->getProgress());
    });
    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed, this, [=] {
        ui->pushButtonSearch->setEnabled(true);
        ui->pushButtonCancel->setEnabled(false);
        searcherModel->addItems(searcher->getResults());
        ui->progressBar->setValue(searcher->getProgress());
        delete searcher;
    });

    thread->start();
    timer->start(1000);
}

void Wild3::searcherEncounterIndexChanged(int index)
{
    if (index >= 0)
    {
        std::vector<std::string> t;
        Encounter encounter = static_cast<Encounter>(ui->comboBoxSearcherEncounter->currentData().toInt());
        switch (encounter)
        {
        case Encounter::Grass:
            t = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11" };
            break;
        case Encounter::RockSmash:
        case Encounter::Surfing:
        case Encounter::SuperRod:
            t = { "0", "1", "2", "3", "4" };
            break;
        case Encounter::OldRod:
            t = { "0", "1" };
            break;
        case Encounter::GoodRod:
            t = { "0", "1", "2" };
            break;
        default:
            break;
        }
        ui->filterSearcher->setEncounterSlots(t);

        encounterSearcher = Encounters3::getEncounters(encounter, currentProfile->getVersion());

        std::vector<u16> locs;
        std::transform(encounterSearcher.begin(), encounterSearcher.end(), std::back_inserter(locs),
                       [](const EncounterArea3 &area) { return area.getLocation(); });

        auto locations = Translator::getLocations(locs, currentProfile->getVersion());
        std::vector<int> indices(locations.size());
        std::iota(indices.begin(), indices.end(), 0);
        std::sort(indices.begin(), indices.end(), [&locations](int i, int j) { return locations[i] < locations[j]; });

        ui->comboBoxSearcherLocation->clear();
        for (int i : indices)
        {
            ui->comboBoxSearcherLocation->addItem(QString::fromStdString(locations[i]), i);
        }
    }
}

void Wild3::searcherLocationIndexChanged(int index)
{
    if (index >= 0)
    {
        auto area = encounterSearcher[ui->comboBoxSearcherLocation->currentData().toInt()];
        auto species = area.getUniqueSpecies();
        auto names = area.getSpecieNames();

        ui->comboBoxSearcherPokemon->clear();
        ui->comboBoxSearcherPokemon->addItem("-");
        for (size_t i = 0; i < species.size(); i++)
        {
            ui->comboBoxSearcherPokemon->addItem(QString::fromStdString(names[i]), species[i]);
        }
    }
}

void Wild3::searcherPokemonIndexChanged(int index)
{
    if (index <= 0)
    {
        ui->filterSearcher->resetEncounterSlots();
    }
    else
    {
        u16 num = ui->comboBoxSearcherPokemon->getCurrentUShort();
        auto flags = encounterSearcher[ui->comboBoxSearcherLocation->getCurrentInt()].getSlots(num);
        ui->filterSearcher->toggleEncounterSlots(flags);
    }
}
