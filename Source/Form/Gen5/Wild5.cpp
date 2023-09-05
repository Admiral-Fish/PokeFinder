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

#include "Wild5.hpp"
#include "ui_Wild5.h"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen5/EncounterArea5.hpp>
#include <Core/Gen5/Encounters5.hpp>
#include <Core/Gen5/Generators/WildGenerator5.hpp>
#include <Core/Gen5/IVSeedCache.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/Searchers/IVSearcher5.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen5/Profile/ProfileManager5.hpp>
#include <Model/Gen5/WildModel5.hpp>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include <QTimer>

Wild5::Wild5(QWidget *parent) : QWidget(parent), ui(new Ui::Wild5)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    generatorModel = new WildGeneratorModel5(ui->tableViewGenerator);
    searcherModel = new WildSearcherModel5(ui->tableViewSearcher);

    ui->tableViewGenerator->setModel(generatorModel);
    ui->tableViewSearcher->setModel(searcherModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed64Bit);
    ui->textBoxGeneratorIVAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorOffset->setValues(InputType::Advance32Bit);

    ui->textBoxSearcherInitialIVAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMaxIVAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMaxAdvances->setValues(InputType::Advance32Bit);

    ui->comboBoxGeneratorEncounter->setup({ toInt(Encounter::Grass), toInt(Encounter::DoubleGrass), toInt(Encounter::SpecialGrass),
                                            toInt(Encounter::Surfing), toInt(Encounter::SpecialSurf), toInt(Encounter::SuperRod),
                                            toInt(Encounter::SpecialSuperRod) });
    ui->comboBoxSearcherEncounter->setup({ toInt(Encounter::Grass), toInt(Encounter::DoubleGrass), toInt(Encounter::SpecialGrass),
                                           toInt(Encounter::Surfing), toInt(Encounter::SpecialSurf), toInt(Encounter::SuperRod),
                                           toInt(Encounter::SpecialSuperRod) });

    ui->filterSearcher->disableControls(Controls::DisableFilter);

    ui->comboMenuGeneratorLead->addAction(tr("None"), toInt(Lead::None));
    ui->comboMenuGeneratorLead->addAction(tr("Compound Eyes"), toInt(Lead::CompoundEyes));
    ui->comboMenuGeneratorLead->addMenu(tr("Cute Charm"), { tr("♂ Lead"), tr("♀ Lead") },
                                        { toInt(Lead::CuteCharmM), toInt(Lead::CuteCharmF) });
    ui->comboMenuGeneratorLead->addMenu(tr("Level Modifier"), { tr("Hustle"), tr("Pressure"), tr("Vital Spirit") },
                                        { toInt(Lead::Hustle), toInt(Lead::Pressure), toInt(Lead::VitalSpirit) });
    ui->comboMenuGeneratorLead->addMenu(tr("Slot Modifier"), { tr("Magnet Pull"), tr("Static") },
                                        { toInt(Lead::MagnetPull), toInt(Lead::Static) });
    ui->comboMenuGeneratorLead->addMenu(tr("Synchronize"), Translator::getNatures());

    ui->comboMenuSearcherLead->addAction(tr("None"), toInt(Lead::None));
    ui->comboMenuSearcherLead->addAction(tr("Compound Eyes"), toInt(Lead::CompoundEyes));
    ui->comboMenuSearcherLead->addMenu(tr("Cute Charm"), { tr("♂ Lead"), tr("♀ Lead") },
                                       { toInt(Lead::CuteCharmM), toInt(Lead::CuteCharmF) });
    ui->comboMenuSearcherLead->addMenu(tr("Level Modifier"), { tr("Hustle"), tr("Pressure"), tr("Vital Spirit") },
                                       { toInt(Lead::Hustle), toInt(Lead::Pressure), toInt(Lead::VitalSpirit) });
    ui->comboMenuSearcherLead->addMenu(tr("Slot Modifier"), { tr("Magnet Pull"), tr("Static") },
                                       { toInt(Lead::MagnetPull), toInt(Lead::Static) });
    ui->comboMenuSearcherLead->addMenu(tr("Synchronize"), Translator::getNatures());

    ui->comboBoxGeneratorLocation->enableAutoComplete();
    ui->comboBoxSearcherLocation->enableAutoComplete();

    ui->comboBoxGeneratorLuckyPower->setup({ 0, 1, 2, 3, 3 });
    ui->comboBoxSearcherLuckyPower->setup({ 0, 1, 2, 3, 3 });

    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &Wild5::profileIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Wild5::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Wild5::search);
    connect(ui->comboBoxGeneratorEncounter, &QComboBox::currentIndexChanged, this, &Wild5::generatorEncounterIndexChanged);
    connect(ui->comboBoxSearcherEncounter, &QComboBox::currentIndexChanged, this, &Wild5::searcherEncounterIndexChanged);
    connect(ui->comboBoxGeneratorLocation, &QComboBox::currentIndexChanged, this, &Wild5::generatorLocationIndexChanged);
    connect(ui->comboBoxSearcherLocation, &QComboBox::currentIndexChanged, this, &Wild5::searcherLocationIndexChanged);
    connect(ui->comboBoxGeneratorPokemon, &QComboBox::currentIndexChanged, this, &Wild5::generatorPokemonIndexChanged);
    connect(ui->comboBoxSearcherPokemon, &QComboBox::currentIndexChanged, this, &Wild5::searcherPokemonIndexChanged);
    connect(ui->comboBoxGeneratorSeason, &QComboBox::currentIndexChanged, this, &Wild5::generatorSeasonIndexChanged);
    connect(ui->comboBoxSearcherSeason, &QComboBox::currentIndexChanged, this, &Wild5::searcherSeasonIndexChanged);
    connect(ui->textBoxSearcherInitialIVAdvances, &TextBox::textChanged, this, &Wild5::searcherFastSearchChanged);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Wild5::profileManager);
    connect(ui->filterGenerator, &Filter::showStatsChanged, generatorModel, &WildGeneratorModel5::setShowStats);
    connect(ui->filterSearcher, &Filter::showStatsChanged, searcherModel, &WildSearcherModel5::setShowStats);
    connect(ui->filterSearcher, &Filter::ivsChanged, this, &Wild5::searcherFastSearchChanged);

    updateProfiles();
    generatorEncounterIndexChanged(0);
    searcherEncounterIndexChanged(0);
    searcherFastSearchChanged();

    QSettings setting;
    setting.beginGroup("wild5");
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

Wild5::~Wild5()
{
    QSettings setting;
    setting.beginGroup("wild5");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

bool Wild5::hasProfiles() const
{
    return !profiles.empty();
}

void Wild5::updateProfiles()
{
    profiles = ProfileLoader5::getProfiles();

    ui->comboBoxProfiles->clear();
    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("wild5/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

bool Wild5::fastSearchEnabled() const
{
    u32 initialAdvances = ui->textBoxSearcherInitialIVAdvances->getUInt();
    u32 maxAdvances = ui->textBoxSearcherMaxIVAdvances->getUInt();

    if (initialAdvances + maxAdvances > 5)
    {
        return false;
    }

    auto min = ui->filterSearcher->getMinIVs();
    auto max = ui->filterSearcher->getMaxIVs();

    return min[0] >= 30 && min[2] >= 30 && min[4] >= 30 && (min[1] >= 30 || min[3] >= 30) && (min[5] >= 30 || max[5] <= 1);
}

void Wild5::generate()
{
    generatorModel->clearModel();

    u64 seed = ui->textBoxGeneratorSeed->getULong();
    u32 ivAdvances = ui->textBoxGeneratorIVAdvances->getUInt();
    u32 initialAdvances = ui->textBoxGeneratorInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();
    u32 offset = ui->textBoxGeneratorOffset->getUInt();
    auto lead = ui->comboMenuGeneratorLead->getEnum<Lead>();
    u8 luckyPower = ui->comboBoxGeneratorLuckyPower->getCurrentUChar();

    auto filter = ui->filterGenerator->getFilter<WildStateFilter, true>();
    WildGenerator5 generator(initialAdvances, maxAdvances, offset, Method::None, lead, luckyPower,
                             encounterGenerator[ui->comboBoxGeneratorLocation->getCurrentInt()], *currentProfile, filter);

    auto states = generator.generate(seed, ivAdvances, 0);
    generatorModel->addItems(states);
}

void Wild5::generatorEncounterIndexChanged(int index)
{
    if (index >= 0)
    {
        auto encounter = ui->comboBoxGeneratorEncounter->getEnum<Encounter>();
        switch (encounter)
        {
        case Encounter::Grass:
            ui->filterGenerator->setEncounterSlots(12);
            break;
        case Encounter::Surfing:
        case Encounter::SuperRod:
            ui->filterGenerator->setEncounterSlots(5);
            break;
        default:
            break;
        }

        u8 season = ui->comboBoxGeneratorSeason->currentIndex();
        encounterGenerator = Encounters5::getEncounters(encounter, season, currentProfile);

        std::vector<u16> locs;
        std::transform(encounterGenerator.begin(), encounterGenerator.end(), std::back_inserter(locs),
                       [](const EncounterArea5 &area) { return area.getLocation(); });

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

void Wild5::generatorLocationIndexChanged(int index)
{
    if (index >= 0)
    {
        auto &area = encounterGenerator[ui->comboBoxGeneratorLocation->getCurrentInt()];
        auto species = area.getUniqueSpecies();
        auto names = area.getSpecieNames();

        ui->comboBoxGeneratorPokemon->clear();
        ui->comboBoxGeneratorPokemon->addItem("-");
        for (size_t i = 0; i < species.size(); i++)
        {
            ui->comboBoxGeneratorPokemon->addItem(QString::fromStdString(names[i]), species[i]);
        }

        bool flag = area.getSeason();
        ui->labelGeneratorSeason->setVisible(flag);
        ui->comboBoxGeneratorSeason->setVisible(flag);
    }
}

void Wild5::generatorPokemonIndexChanged(int index)
{
    if (index <= 0)
    {
        ui->filterGenerator->resetEncounterSlots();
    }
    else
    {
        u16 num = ui->comboBoxGeneratorPokemon->getCurrentUShort();
        auto flags = encounterGenerator[ui->comboBoxGeneratorLocation->getCurrentInt()].getSlots(num);
        ui->filterGenerator->toggleEncounterSlots(flags);
    }
}

void Wild5::generatorSeasonIndexChanged(int index)
{
    if (index >= 0)
    {
        auto encounter = ui->comboBoxGeneratorEncounter->getEnum<Encounter>();
        encounterGenerator = Encounters5::getEncounters(encounter, index, currentProfile);
        generatorLocationIndexChanged(0);
    }
}

void Wild5::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = &profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile->getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile->getSID()));
        ui->labelProfileMACAddressValue->setText(QString::number(currentProfile->getMac(), 16));
        ui->labelProfileDSTypeValue->setText(QString::fromStdString(currentProfile->getDSTypeString()));
        ui->labelProfileVCountValue->setText(QString::number(currentProfile->getVCount(), 16));
        ui->labelProfileTimer0Value->setText(QString::number(currentProfile->getTimer0Min(), 16) + "-"
                                             + QString::number(currentProfile->getTimer0Max(), 16));
        ui->labelProfileGxStatValue->setText(QString::number(currentProfile->getGxStat()));
        ui->labelProfileVFrameValue->setText(QString::number(currentProfile->getVFrame()));
        ui->labelProfileKeypressesValue->setText(QString::fromStdString(currentProfile->getKeypressesString()));
        ui->labelProfileGameValue->setText(QString::fromStdString(Translator::getGame(currentProfile->getVersion())));

        bool flag = (currentProfile->getVersion() & Game::BW2) != Game::None;

        ui->labelGeneratorLuckyPower->setVisible(flag);
        ui->comboBoxGeneratorLuckyPower->setVisible(flag);

        ui->labelSearcherLuckyPower->setVisible(flag);
        ui->comboBoxSearcherLuckyPower->setVisible(flag);

        generatorEncounterIndexChanged(0);
        searcherEncounterIndexChanged(0);
    }
}

void Wild5::profileManager()
{
    auto *manager = new ProfileManager5();
    connect(manager, &ProfileManager5::profilesModified, this, [=](int num) { emit profilesModified(num); });
    manager->show();
}

void Wild5::search()
{
    Date start = ui->dateEditSearcherStartDate->getDate();
    Date end = ui->dateEditSearcherEndDate->getDate();
    if (start > end)
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid date range"), tr("Start date is after end date"));
        msg.exec();
        return;
    }

    searcherModel->clearModel();

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u32 initialIVAdvances = ui->textBoxSearcherInitialIVAdvances->getUInt();
    u32 maxIVAdvances = ui->textBoxSearcherMaxIVAdvances->getUInt();
    u32 initialAdvances = ui->textBoxSearcherInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxSearcherMaxAdvances->getUInt();
    auto lead = ui->comboMenuSearcherLead->getEnum<Lead>();
    u8 luckyPower = ui->comboBoxSearcherLuckyPower->getCurrentUChar();

    auto filter = ui->filterSearcher->getFilter<WildStateFilter, true>();
    WildGenerator5 generator(initialAdvances, maxAdvances, 0, Method::Method5, lead, luckyPower,
                             encounterSearcher[ui->comboBoxSearcherLocation->getCurrentInt()], *currentProfile, filter);

    IVSearcher5<WildGenerator5, WildState5> *searcher;
    if (fastSearchEnabled())
    {
        auto ivCache = IVSeedCache::getNormalCache(initialIVAdvances, maxIVAdvances, currentProfile->getVersion(), filter);
        searcher = new IVSearcher5<WildGenerator5, WildState5>(initialIVAdvances, maxIVAdvances, ivCache, generator, *currentProfile);
    }
    else
    {
        searcher = new IVSearcher5<WildGenerator5, WildState5>(initialIVAdvances, maxIVAdvances, generator, *currentProfile);
    }

    int maxProgress = Keypresses::getKeypresses(*currentProfile).size();
    maxProgress *= start.daysTo(end) + 1;
    maxProgress *= (currentProfile->getTimer0Max() - currentProfile->getTimer0Min() + 1);
    ui->progressBar->setRange(0, maxProgress);

    QSettings settings;
    int threads = settings.value("settings/threads").toInt();

    auto *thread = QThread::create([=] { searcher->startSearch(threads, start, end); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(ui->pushButtonCancel, &QPushButton::clicked, [searcher] { searcher->cancelSearch(); });

    auto *timer = new QTimer();
    connect(timer, &QTimer::timeout, this, [=] {
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

void Wild5::searcherEncounterIndexChanged(int index)
{
    if (index >= 0)
    {
        auto encounter = ui->comboBoxSearcherEncounter->getEnum<Encounter>();
        switch (encounter)
        {
        case Encounter::Grass:
            ui->filterSearcher->setEncounterSlots(12);
            break;
        case Encounter::Surfing:
        case Encounter::SuperRod:
            ui->filterSearcher->setEncounterSlots(5);
            break;
        default:
            break;
        }

        u8 season = ui->comboBoxSearcherSeason->currentIndex();
        encounterSearcher = Encounters5::getEncounters(encounter, season, currentProfile);

        std::vector<u16> locs;
        std::transform(encounterSearcher.begin(), encounterSearcher.end(), std::back_inserter(locs),
                       [](const EncounterArea5 &area) { return area.getLocation(); });

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

void Wild5::searcherFastSearchChanged()
{
    if (fastSearchEnabled())
    {
        ui->labelIVFastSearch->setText(tr("Settings are configured for fast searching."));
    }
    else
    {
        QStringList text = { tr("Settings are not configured for fast searching."), tr("Keep initial/max advances below 6."),
                             tr("Ensure IV filters are set to common spreads.") };
        ui->labelIVFastSearch->setText(text.join('\n'));
    }
}

void Wild5::searcherLocationIndexChanged(int index)
{
    if (index >= 0)
    {
        auto &area = encounterSearcher[ui->comboBoxSearcherLocation->getCurrentInt()];
        auto species = area.getUniqueSpecies();
        auto names = area.getSpecieNames();

        ui->comboBoxSearcherPokemon->clear();
        ui->comboBoxSearcherPokemon->addItem("-");
        for (size_t i = 0; i < species.size(); i++)
        {
            ui->comboBoxSearcherPokemon->addItem(QString::fromStdString(names[i]), species[i]);
        }

        bool flag = area.getSeason();
        ui->labelSearcherSeason->setVisible(flag);
        ui->comboBoxSearcherSeason->setVisible(flag);
    }
}

void Wild5::searcherPokemonIndexChanged(int index)
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

void Wild5::searcherSeasonIndexChanged(int index)
{
    if (index >= 0)
    {
        auto encounter = ui->comboBoxSearcherEncounter->getEnum<Encounter>();
        encounterSearcher = Encounters5::getEncounters(encounter, index, currentProfile);
        searcherLocationIndexChanged(0);
    }
}
