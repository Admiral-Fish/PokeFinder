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
#include <Forms/Controls/Controls.hpp>
#include <Forms/Gen3/Profile/ProfileManager3.hpp>
#include <Forms/Gen3/Tools/SeedTime3.hpp>
#include <Forms/Models/Gen3/WildModel3.hpp>
#include <QMenu>
#include <QSettings>
#include <QThread>
#include <QTimer>

Wild3::Wild3(QWidget *parent) : QWidget(parent), ui(new Ui::Wild3)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    setupModels();
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
    profiles = { Profile3() };
    auto completeProfiles = ProfileLoader3::getProfiles();
    std::copy_if(completeProfiles.begin(), completeProfiles.end(), std::back_inserter(profiles),
                 [](const Profile3 &profile) { return (profile.getVersion() & Game::GC) != Game::GC; });

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

void Wild3::setupModels()
{
    generatorModel = new WildGeneratorModel3(ui->tableViewGenerator);
    searcherModel = new WildSearcherModel3(ui->tableViewSearcher);

    generatorMenu = new QMenu(ui->tableViewGenerator);
    searcherMenu = new QMenu(ui->tableViewSearcher);

    ui->tableViewGenerator->setModel(generatorModel);
    ui->tableViewSearcher->setModel(searcherModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed32Bit);
    ui->textBoxGeneratorInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);

    ui->comboBoxGeneratorMethod->setup({ toInt(Method::MethodH1), toInt(Method::MethodH2), toInt(Method::MethodH4) });
    ui->comboBoxSearcherMethod->setup({ toInt(Method::MethodH1), toInt(Method::MethodH2), toInt(Method::MethodH4) });

    ui->filterSearcher->disableControls(Controls::UseDelay | Controls::DisableFilter);

    ui->comboBoxSearcherLead->setup({ toInt(Lead::Search), toInt(Lead::Synchronize), toInt(Lead::CuteCharm), toInt(Lead::None) });

    ui->toolButtonGeneratorLead->addAction(tr("None"), toInt(Lead::None));
    ui->toolButtonGeneratorLead->addMenu(tr("Synchronize"), *Translator::getNatures());
    ui->toolButtonGeneratorLead->addMenu(
        tr("Cute Charm"),
        { tr("♂ Lead (50% ♀ Target)"), tr("♂ Lead (75% ♀ Target)"), tr("♂ Lead (25% ♀ Target)"), tr("♂ Lead (12.5% ♀ Target)"),
          tr("♀ Lead (50% ♂ Target)"), tr("♀ Lead (75% ♂ Target)"), tr("♀ Lead (25% ♂ Target)"), tr("♀ Lead (87.5% ♂ Target)") },
        { toInt(Lead::CuteCharm50F), toInt(Lead::CuteCharm75F), toInt(Lead::CuteCharm25F), toInt(Lead::CuteCharm125F),
          toInt(Lead::CuteCharm50M), toInt(Lead::CuteCharm75M), toInt(Lead::CuteCharm25M), toInt(Lead::CuteCharm875M) });

    QAction *outputTXTGenerator = generatorMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVGenerator = generatorMenu->addAction(tr("Output Results to CSV"));
    connect(outputTXTGenerator, &QAction::triggered, this, [=] { ui->tableViewGenerator->outputModel(); });
    connect(outputCSVGenerator, &QAction::triggered, this, [=] { ui->tableViewGenerator->outputModel(true); });

    QAction *seedToTime = searcherMenu->addAction(tr("Generate times for seed"));
    QAction *outputTXTSearcher = searcherMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVSearcher = searcherMenu->addAction(tr("Output Results to CSV"));
    connect(seedToTime, &QAction::triggered, this, &Wild3::seedToTime);
    connect(outputTXTSearcher, &QAction::triggered, [=] { ui->tableViewSearcher->outputModel(); });
    connect(outputCSVSearcher, &QAction::triggered, [=] { ui->tableViewSearcher->outputModel(true); });

    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Wild3::profilesIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Wild3::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Wild3::search);
    connect(ui->tableViewGenerator, &QTableView::customContextMenuRequested, this, &Wild3::tableViewGeneratorContextMenu);
    connect(ui->tableViewSearcher, &QTableView::customContextMenuRequested, this, &Wild3::tableViewSearcherContextMenu);
    connect(ui->comboBoxGeneratorEncounter, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &Wild3::generatorEncounterIndexChanged);
    connect(ui->comboBoxSearcherEncounter, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &Wild3::searcherEncounterIndexChanged);
    connect(ui->comboBoxGeneratorLocation, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &Wild3::generatorLocationIndexChanged);
    connect(ui->comboBoxSearcherLocation, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Wild3::searcherLocationIndexChanged);
    connect(ui->comboBoxGeneratorPokemon, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Wild3::generatorPokemonIndexChanged);
    connect(ui->comboBoxSearcherPokemon, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Wild3::searcherPokemonIndexChanged);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Wild3::profileManager);

    updateProfiles();
    generatorEncounterIndexChanged(0);
    searcherEncounterIndexChanged(0);

    QSettings setting;
    if (setting.contains("wild3/geometry"))
    {
        this->restoreGeometry(setting.value("wild3/geometry").toByteArray());
    }
}

void Wild3::updateLocationsGenerator()
{
    auto encounter = static_cast<Encounter>(ui->comboBoxGeneratorEncounter->currentData().toInt());
    encounterGenerator = Encounters3::getEncounters(encounter, currentProfile->getVersion());

    std::vector<u16> locs;
    std::transform(encounterGenerator.begin(), encounterGenerator.end(), std::back_inserter(locs),
                   [](const EncounterArea3 &area) { return area.getLocation(); });

    auto locations = Translator::getLocations(locs, currentProfile->getVersion());
    std::vector<int> indices(locations.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(), [&locations](int i, int j) { return locations[i] < locations[j]; });

    ui->comboBoxGeneratorLocation->clear();
    for (int index : indices)
    {
        ui->comboBoxGeneratorLocation->addItem(QString::fromStdString(locations[index]), index);
    }
}

void Wild3::updateLocationsSearcher()
{
    auto encounter = static_cast<Encounter>(ui->comboBoxSearcherEncounter->currentData().toInt());
    encounterSearcher = Encounters3::getEncounters(encounter, currentProfile->getVersion());

    std::vector<u16> locs;
    std::transform(encounterSearcher.begin(), encounterSearcher.end(), std::back_inserter(locs),
                   [](const EncounterArea3 &area) { return area.getLocation(); });

    auto locations = Translator::getLocations(locs, currentProfile->getVersion());
    std::vector<int> indices(locations.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(), [&locations](int i, int j) { return locations[i] < locations[j]; });

    ui->comboBoxSearcherLocation->clear();
    for (int index : indices)
    {
        ui->comboBoxSearcherLocation->addItem(QString::fromStdString(locations[index]), index);
    }
}

void Wild3::updatePokemonGenerator()
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

void Wild3::updatePokemonSearcher()
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

void Wild3::generate()
{
    generatorModel->clearModel();

    u32 seed = ui->textBoxGeneratorSeed->getUInt();
    u32 initialAdvances = ui->textBoxGeneratorInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();
    u16 tid = currentProfile->getTID();
    u16 sid = currentProfile->getSID();
    u8 genderRatio = ui->filterGenerator->getGenderRatio();
    auto method = static_cast<Method>(ui->comboBoxGeneratorMethod->getCurrentInt());
    u32 offset = 0;
    if (ui->filterGenerator->useDelay())
    {
        offset = ui->filterGenerator->getDelay();
    }

    StateFilter filter(ui->filterGenerator->getGender(), ui->filterGenerator->getAbility(), ui->filterGenerator->getShiny(),
                       ui->filterGenerator->getDisableFilters(), ui->filterGenerator->getMinIVs(), ui->filterGenerator->getMaxIVs(),
                       ui->filterGenerator->getNatures(), ui->filterGenerator->getHiddenPowers(), ui->filterGenerator->getEncounterSlots());

    WildGenerator3 generator(initialAdvances, maxAdvances, tid, sid, genderRatio, method, filter, currentProfile->getVersion());
    generator.setEncounter(static_cast<Encounter>(ui->comboBoxGeneratorEncounter->currentData().toInt()));
    generator.setOffset(offset);

    if (ui->toolButtonGeneratorLead->text().contains(tr("Synchronize")))
    {
        generator.setLead(Lead::Synchronize);
        generator.setSynchNature(ui->toolButtonGeneratorLead->getData());
    }
    else
    {
        generator.setLead(static_cast<Lead>(ui->toolButtonGeneratorLead->getData()));
    }

    auto states = generator.generate(seed, encounterGenerator[ui->comboBoxGeneratorLocation->currentData().toInt()]);
    generatorModel->addItems(states);
}

void Wild3::search()
{
    searcherModel->clearModel();

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    std::array<u8, 6> min = ui->filterSearcher->getMinIVs();
    std::array<u8, 6> max = ui->filterSearcher->getMaxIVs();

    StateFilter filter(ui->filterSearcher->getGender(), ui->filterSearcher->getAbility(), ui->filterSearcher->getShiny(), false, min, max,
                       ui->filterSearcher->getNatures(), ui->filterSearcher->getHiddenPowers(), ui->filterSearcher->getEncounterSlots());

    u16 tid = currentProfile->getTID();
    u16 sid = currentProfile->getSID();
    u8 genderRatio = ui->filterSearcher->getGenderRatio();
    auto method = static_cast<Method>(ui->comboBoxSearcherMethod->getCurrentInt());

    auto *searcher = new WildSearcher3(tid, sid, genderRatio, method, filter, currentProfile->getVersion());
    searcher->setEncounter(static_cast<Encounter>(ui->comboBoxSearcherEncounter->currentData().toInt()));
    searcher->setLead(static_cast<Lead>(ui->comboBoxSearcherLead->currentData().toInt()));
    searcher->setEncounterArea(encounterSearcher[ui->comboBoxSearcherLocation->currentData().toInt()]);

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
    connect(timer, &QTimer::timeout, [=] {
        searcherModel->addItems(searcher->getResults());
        ui->progressBar->setValue(searcher->getProgress());
    });
    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed, [=] {
        ui->pushButtonSearch->setEnabled(true);
        ui->pushButtonCancel->setEnabled(false);
        searcherModel->addItems(searcher->getResults());
        ui->progressBar->setValue(searcher->getProgress());
        delete searcher;
    });

    thread->start();
    timer->start(1000);
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
        ui->labelProfileGameValue->setText(QString::fromStdString(currentProfile->getVersionString()));

        bool flag = (currentProfile->getVersion() & Game::FRLG) != Game::None;
        ui->comboBoxGeneratorEncounter->clear();
        ui->comboBoxSearcherEncounter->clear();

        ui->comboBoxGeneratorEncounter->addItem(tr("Grass"), toInt(Encounter::Grass));
        ui->comboBoxSearcherEncounter->addItem(tr("Grass"), toInt(Encounter::Grass));
        ui->comboBoxGeneratorEncounter->addItem(tr("Rock Smash"), toInt(Encounter::RockSmash));
        ui->comboBoxSearcherEncounter->addItem(tr("Rock Smash"), toInt(Encounter::RockSmash));
        ui->comboBoxGeneratorEncounter->addItem(tr("Surfing"), toInt(Encounter::Surfing));
        ui->comboBoxSearcherEncounter->addItem(tr("Surfing"), toInt(Encounter::Surfing));
        ui->comboBoxGeneratorEncounter->addItem(tr("Old Rod"), toInt(Encounter::OldRod));
        ui->comboBoxSearcherEncounter->addItem(tr("Old Rod"), toInt(Encounter::OldRod));
        ui->comboBoxGeneratorEncounter->addItem(tr("Good Rod"), toInt(Encounter::GoodRod));
        ui->comboBoxSearcherEncounter->addItem(tr("Good Rod"), toInt(Encounter::GoodRod));
        ui->comboBoxGeneratorEncounter->addItem(tr("Super Rod"), toInt(Encounter::SuperRod));
        ui->comboBoxSearcherEncounter->addItem(tr("Super Rod"), toInt(Encounter::SuperRod));

        if ((currentProfile->getVersion() & Game::Emerald) != Game::None)
        {
            ui->toolButtonGeneratorLead->setEnabled(true);
            ui->comboBoxSearcherLead->setEnabled(true);
        }
        else
        {
            ui->toolButtonGeneratorLead->clearSelection();
            ui->toolButtonGeneratorLead->setEnabled(false);
            ui->comboBoxSearcherLead->setCurrentIndex(3);
            ui->comboBoxSearcherLead->setEnabled(false);
        }

        updateLocationsSearcher();
        updateLocationsGenerator();
    }
}

void Wild3::tableViewGeneratorContextMenu(QPoint pos)
{
    if (generatorModel->rowCount() > 0)
    {
        generatorMenu->popup(ui->tableViewGenerator->viewport()->mapToGlobal(pos));
    }
}

void Wild3::tableViewSearcherContextMenu(QPoint pos)
{
    if (searcherModel->rowCount() > 0)
    {
        searcherMenu->popup(ui->tableViewSearcher->viewport()->mapToGlobal(pos));
    }
}

void Wild3::seedToTime()
{
    QModelIndex index = ui->tableViewSearcher->currentIndex();
    index = searcherModel->index(index.row(), 0);

    u32 seed = searcherModel->data(index).toString().toUInt(nullptr, 16);
    auto *seedToTime = new SeedTime3(seed);
    seedToTime->show();
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
        updateLocationsGenerator();
    }
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
        updateLocationsSearcher();
    }
}

void Wild3::generatorLocationIndexChanged(int index)
{
    if (index >= 0)
    {
        updatePokemonGenerator();
    }
}

void Wild3::searcherLocationIndexChanged(int index)
{
    if (index >= 0)
    {
        updatePokemonSearcher();
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
        auto flags = encounterGenerator[ui->comboBoxGeneratorLocation->currentData().toInt()].getSlots(num);
        ui->filterGenerator->toggleEncounterSlots(flags);
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
        u16 num = static_cast<u16>(ui->comboBoxSearcherPokemon->currentData().toUInt());
        auto flags = encounterSearcher[ui->comboBoxSearcherLocation->currentData().toInt()].getSlots(num);
        ui->filterSearcher->toggleEncounterSlots(flags);
    }
}

void Wild3::profileManager()
{
    auto *manager = new ProfileManager3();
    connect(manager, &ProfileManager3::updateProfiles, this, [=] { emit alertProfiles(3); });
    manager->show();
}
