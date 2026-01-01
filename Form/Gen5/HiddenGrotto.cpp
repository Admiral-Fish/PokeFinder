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

#include "HiddenGrotto.hpp"
#include "ui_HiddenGrotto.h"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Gen5/Encounters5.hpp>
#include <Core/Gen5/Generators/HiddenGrottoGenerator.hpp>
#include <Core/Gen5/HiddenGrottoArea.hpp>
#include <Core/Gen5/IVCache.hpp>
#include <Core/Gen5/Keypresses.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/SHA1Cache.hpp>
#include <Core/Gen5/Searchers/IVSearcher5.hpp>
#include <Core/Gen5/Searchers/Searcher5.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen5/Profile/ProfileManager5.hpp>
#include <Model/Gen5/HiddenGrottoModel.hpp>
#include <Model/SortFilterProxyModel.hpp>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include <QTimer>

HiddenGrotto::HiddenGrotto(QWidget *parent) :
    QWidget(parent), ui(new Ui::HiddenGrotto), ivCache(nullptr), shaCache(nullptr), encounter(Encounters5::getHiddenGrottoEncounters())
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    grottoGeneratorModel = new HiddenGrottoSlotGeneratorModel5(ui->tableViewGrottoGenerator);
    ui->tableViewGrottoGenerator->setModel(grottoGeneratorModel);

    grottoSearcherModel = new HiddenGrottoSlotSearcherModel5(ui->tableViewGrottoSearcher);
    grottoProxyModel = new SortFilterProxyModel(ui->tableViewGrottoSearcher, grottoSearcherModel);
    ui->tableViewGrottoSearcher->setModel(grottoProxyModel);

    ui->textBoxGrottoGeneratorSeed->setValues(InputType::Seed64Bit);
    ui->textBoxGrottoGeneratorInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGrottoGeneratorMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGrottoGeneratorOffset->setValues(InputType::Advance32Bit);

    ui->textBoxGrottoSearcherInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGrottoSearcherMaxAdvances->setValues(InputType::Advance32Bit);

    ui->comboBoxGrottoGeneratorGrottoPower->setup({ 5, 15, 25, 35, 55 });

    ui->comboBoxGrottoSearcherGrottoPower->setup({ 5, 15, 25, 35, 55 });

    ui->comboBoxGrottoGeneratorLocation->enableAutoComplete();
    ui->comboBoxGrottoSearcherLocation->enableAutoComplete();

    pokemonGeneratorModel = new HiddenGrottoGeneratorModel5(ui->tableViewPokemonGenerator);
    ui->tableViewPokemonGenerator->setModel(pokemonGeneratorModel);

    pokemonSearcherModel = new HiddenGrottoSearcherModel5(ui->tableViewPokemonSearcher);
    pokemonProxyModel = new SortFilterProxyModel(ui->tableViewPokemonSearcher, pokemonSearcherModel);
    ui->tableViewPokemonSearcher->setModel(pokemonProxyModel);

    ui->textBoxPokemonGeneratorSeed->setValues(InputType::Seed64Bit);
    ui->textBoxPokemonGeneratorIVAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxPokemonGeneratorInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxPokemonGeneratorMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxPokemonGeneratorOffset->setValues(InputType::Advance32Bit);

    ui->textBoxPokemonSearcherInitialIVAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxPokemonSearcherMaxIVAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxPokemonSearcherInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxPokemonSearcherMaxAdvances->setValues(InputType::Advance32Bit);

    ui->filterPokemonGenerator->disableControls(Controls::EncounterSlots | Controls::Height | Controls::Shiny | Controls::Weight);
    ui->filterPokemonSearcher->disableControls(Controls::DisableFilter | Controls::EncounterSlots | Controls::Height | Controls::Shiny
                                               | Controls::Weight);

    ui->comboBoxPokemonGeneratorLocation->enableAutoComplete();
    ui->comboBoxPokemonSearcherLocation->enableAutoComplete();

    ui->comboMenuPokemonGeneratorLead->addAction(tr("None"), toInt(Lead::None));
    ui->comboMenuPokemonGeneratorLead->addMenu(tr("Synchronize"), Translator::getNatures());

    ui->comboMenuPokemonSearcherLead->addAction(tr("None"), toInt(Lead::None));
    ui->comboMenuPokemonSearcherLead->addMenu(tr("Synchronize"), Translator::getNatures());

    connect(ui->comboBoxGrottoGeneratorLocation, &QComboBox::currentIndexChanged, this, &HiddenGrotto::grottoGeneratorLocationIndexChanged);
    connect(ui->comboBoxGrottoSearcherLocation, &QComboBox::currentIndexChanged, this, &HiddenGrotto::grottoSearcherLocationIndexChanged);
    connect(ui->comboBoxGrottoGeneratorPokemon, &QComboBox::currentIndexChanged, this, &HiddenGrotto::grottoGeneratorUpdateFilter);
    connect(ui->comboBoxGrottoSearcherPokemon, &QComboBox::currentIndexChanged, this, &HiddenGrotto::grottoSearcherUpdateFilter);
    connect(ui->comboBoxGrottoGeneratorItems, &QComboBox::currentIndexChanged, this, &HiddenGrotto::grottoGeneratorUpdateFilter);
    connect(ui->comboBoxGrottoSearcherItems, &QComboBox::currentIndexChanged, this, &HiddenGrotto::grottoSearcherUpdateFilter);
    connect(ui->comboBoxPokemonGeneratorGroup, &QComboBox::currentIndexChanged, this, &HiddenGrotto::pokemonGeneratorGroupIndexChanged);
    connect(ui->comboBoxPokemonSearcherGroup, &QComboBox::currentIndexChanged, this, &HiddenGrotto::pokemonSearcherGroupIndexChanged);
    connect(ui->comboBoxPokemonGeneratorLocation, &QComboBox::currentIndexChanged, this,
            &HiddenGrotto::pokemonGeneratorLocationIndexChanged);
    connect(ui->comboBoxPokemonSearcherLocation, &QComboBox::currentIndexChanged, this, &HiddenGrotto::pokemonSearcherLocationIndexChanged);
    connect(ui->comboBoxPokemonGeneratorPokemon, &QComboBox::currentIndexChanged, this, &HiddenGrotto::pokemonGeneratorPokemonIndexChanged);
    connect(ui->comboBoxPokemonSearcherPokemon, &QComboBox::currentIndexChanged, this, &HiddenGrotto::pokemonSearcherPokemonIndexChanged);
    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &HiddenGrotto::profileIndexChanged);
    connect(ui->pushButtonGrottoGenerate, &QPushButton::clicked, this, &HiddenGrotto::grottoGenerate);
    connect(ui->pushButtonGrottoSearch, &QPushButton::clicked, this, &HiddenGrotto::grottoSearch);
    connect(ui->pushButtonPokemonGenerate, &QPushButton::clicked, this, &HiddenGrotto::pokemonGenerate);
    connect(ui->pushButtonPokemonSearch, &QPushButton::clicked, this, &HiddenGrotto::pokemonSearch);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &HiddenGrotto::profileManager);
    connect(ui->filterPokemonGenerator, &Filter::showStatsChanged, pokemonGeneratorModel, &HiddenGrottoGeneratorModel5::setShowStats);
    connect(ui->filterPokemonSearcher, &Filter::showStatsChanged, pokemonSearcherModel, &HiddenGrottoSearcherModel5::setShowStats);
    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &HiddenGrotto::pokemonSearcherFastSearchChanged);
    connect(ui->filterPokemonSearcher, &Filter::ivsChanged, this, &HiddenGrotto::pokemonSearcherFastSearchChanged);
    connect(ui->textBoxPokemonSearcherInitialIVAdvances, &TextBox::textChanged, this, &HiddenGrotto::pokemonSearcherFastSearchChanged);
    connect(ui->textBoxPokemonSearcherMaxIVAdvances, &TextBox::textChanged, this, &HiddenGrotto::pokemonSearcherFastSearchChanged);

    std::vector<u16> locs;
    std::transform(encounter.begin(), encounter.end(), std::back_inserter(locs),
                   [](const HiddenGrottoArea &area) { return area.getLocation(); });
    auto locations = Translator::getLocations(locs, Game::BW2);

    ui->comboBoxGrottoGeneratorLocation->addItems(locations);
    ui->comboBoxGrottoSearcherLocation->addItems(locations);

    ui->comboBoxPokemonGeneratorLocation->addItems(locations);
    ui->comboBoxPokemonSearcherLocation->addItems(locations);

    updateProfiles();
    pokemonSearcherFastSearchChanged();

    QSettings setting;
    setting.beginGroup("hiddenGrotto");
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    if (setting.contains("startDateGrotto"))
    {
        ui->dateEditGrottoSearcherStartDate->setDate(setting.value("startDateGrotto").toDate());
    }
    if (setting.contains("endDateGrotto"))
    {
        ui->dateEditGrottoSearcherEndDate->setDate(setting.value("endDateGrotto").toDate());
    }
    if (setting.contains("startDatePokemon"))
    {
        ui->dateEditPokemonSearcherStartDate->setDate(setting.value("startDatePokemon").toDate());
    }
    if (setting.contains("endDatePokemon"))
    {
        ui->dateEditPokemonSearcherEndDate->setDate(setting.value("endDatePokemon").toDate());
    }
    setting.endGroup();
}

HiddenGrotto::~HiddenGrotto()
{
    QSettings setting;
    setting.beginGroup("hiddenGrotto");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.setValue("startDateGrotto", ui->dateEditGrottoSearcherStartDate->date());
    setting.setValue("endDateGrotto", ui->dateEditGrottoSearcherEndDate->date());
    setting.setValue("startDatePokemon", ui->dateEditPokemonSearcherStartDate->date());
    setting.setValue("endDatePokemon", ui->dateEditPokemonSearcherEndDate->date());
    setting.endGroup();

    delete ivCache;
    delete shaCache;
    delete ui;
}

bool HiddenGrotto::hasProfiles() const
{
    return !profiles.empty();
}

void HiddenGrotto::updateProfiles()
{
    profiles.clear();
    auto completeProfiles = ProfileLoader5::getProfiles();
    std::copy_if(completeProfiles.begin(), completeProfiles.end(), std::back_inserter(profiles),
                 [](const Profile5 &profile) { return (profile.getVersion() & Game::BW2) != Game::None; });

    ui->comboBoxProfiles->clear();
    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("hiddenGrotto/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

bool HiddenGrotto::fastSearchEnabled() const
{
    if (ivCache == nullptr)
    {
        return false;
    }

    u32 initialAdvances = ui->textBoxPokemonSearcherInitialIVAdvances->getUInt();
    u32 maxAdvances = ui->textBoxPokemonSearcherMaxIVAdvances->getUInt();

    if (initialAdvances < ivCache->getInitialAdvances()
        || (initialAdvances + maxAdvances) > (ivCache->getInitialAdvances() + ivCache->getMaxAdvances()))
    {
        return false;
    }

    auto min = ui->filterPokemonSearcher->getMinIVs();
    auto max = ui->filterPokemonSearcher->getMaxIVs();

    return min[0] >= 30 && min[2] >= 30 && min[4] >= 30 && (min[1] >= 30 || min[3] >= 30) && (min[5] >= 30 || max[5] <= 1);
}

void HiddenGrotto::grottoGenerate()
{
    grottoGeneratorModel->clearModel();

    u64 seed = ui->textBoxGrottoGeneratorSeed->getULong();
    u32 initialAdvances = ui->textBoxGrottoGeneratorInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxGrottoGeneratorMaxAdvances->getUInt();
    u32 offset = ui->textBoxGrottoGeneratorOffset->getUInt();
    u8 powerLevel = ui->comboBoxGrottoGeneratorGrottoPower->currentData().toUInt();

    HiddenGrottoFilter filter(ui->checkListGrottoGeneratorSlot->getCheckedArray<11>(),
                              ui->checkListGrottoGeneratorGender->getCheckedArray<2>(),
                              ui->checkListGrottoGeneratorGroup->getCheckedArray<4>());
    HiddenGrottoSlotGenerator generator(initialAdvances, maxAdvances, offset, powerLevel,
                                        encounter[ui->comboBoxGrottoGeneratorLocation->currentIndex()], *currentProfile, filter);

    auto states = generator.generate(seed);
    grottoGeneratorModel->addItems(states);
}

void HiddenGrotto::grottoGeneratorLocationIndexChanged(int index)
{
    if (index >= 0)
    {
        auto &area = encounter[ui->comboBoxGrottoGeneratorLocation->currentIndex()];
        auto pokemon = area.getUniqueSpecies();
        auto pokemonNames = area.getSpecieNames();

        ui->comboBoxGrottoGeneratorPokemon->clear();
        ui->comboBoxGrottoGeneratorPokemon->addItem("-");
        for (int i = 0; i < pokemon.size(); i++)
        {
            ui->comboBoxGrottoGeneratorPokemon->addItem(QString::fromStdString(pokemonNames[i]), pokemon[i]);
        }

        auto item = area.getUniqueItems();
        auto itemNames = area.getItemNames();

        ui->comboBoxGrottoGeneratorItems->clear();
        ui->comboBoxGrottoGeneratorItems->addItem("-");
        for (int i = 0; i < item.size(); i++)
        {
            ui->comboBoxGrottoGeneratorItems->addItem(QString::fromStdString(itemNames[i]), item[i]);
        }
    }
}

void HiddenGrotto::grottoGeneratorUpdateFilter()
{
    std::vector<bool> groups(4, false);
    std::vector<bool> encounterSlots(11, false);

    const auto &area = encounter[ui->comboBoxGrottoGeneratorLocation->currentIndex()];
    u16 specie = ui->comboBoxGrottoGeneratorPokemon->getCurrentUShort();
    u16 item = ui->comboBoxGrottoGeneratorItems->getCurrentUShort();

    for (u8 group = 0; group < 4; group++)
    {
        u8 slot = 0;
        for (; slot < 3; slot++)
        {
            if (specie != 0 && specie == area.getPokemon(group, slot).getSpecie())
            {
                groups[group] = true;
                encounterSlots[slot] = true;
            }
        }

        for (; slot < 7; slot++)
        {
            if (item != 0 && item == area.getItem(group, slot - 3))
            {
                groups[group] = true;
                encounterSlots[slot] = true;
            }
        }

        for (; slot < 11; slot++)
        {
            if (item != 0 && item == area.getHiddenItem(group, slot - 7))
            {
                groups[group] = true;
                encounterSlots[slot] = true;
            }
        }

        ui->checkListGrottoGeneratorGroup->setChecks(groups);
        ui->checkListGrottoGeneratorSlot->setChecks(encounterSlots);
    }
}

void HiddenGrotto::grottoSearch()
{
    Date start = ui->dateEditGrottoSearcherStartDate->getDate();
    Date end = ui->dateEditGrottoSearcherEndDate->getDate();
    if (start > end)
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid date range"), tr("Start date is after end date"));
        msg.exec();
        return;
    }

    grottoSearcherModel->clearModel();
    ui->pushButtonGrottoSearch->setEnabled(false);
    ui->pushButtonGrottoCancel->setEnabled(true);

    u32 initialAdvances = ui->textBoxGrottoSearcherInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxGrottoSearcherMaxAdvances->getUInt();
    u8 powerLevel = ui->comboBoxGrottoSearcherGrottoPower->getCurrentUInt();

    HiddenGrottoFilter filter(ui->checkListGrottoSearcherSlot->getCheckedArray<11>(),
                              ui->checkListGrottoSearcherGender->getCheckedArray<2>(),
                              ui->checkListGrottoSearcherGroup->getCheckedArray<4>());
    HiddenGrottoSlotGenerator generator(initialAdvances, maxAdvances, 0, powerLevel,
                                        encounter[ui->comboBoxGrottoSearcherLocation->currentIndex()], *currentProfile, filter);
    auto *searcher = new Searcher5<HiddenGrottoSlotGenerator, HiddenGrottoState>(generator, *currentProfile);

    int maxProgress = Keypresses::getKeypresses(*currentProfile).size();
    maxProgress *= start.daysTo(end) + 1;
    maxProgress *= currentProfile->getTimer0Max() - currentProfile->getTimer0Min() + 1;
    searcher->setMaxProgress(maxProgress);

    QSettings settings;
    int threads = settings.value("settings/threads").toInt();

    auto *thread = QThread::create([=] { searcher->startSearch(threads, start, end); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(ui->pushButtonGrottoCancel, &QPushButton::clicked, [searcher] { searcher->cancelSearch(); });

    auto *timer = new QTimer();
    connect(timer, &QTimer::timeout, this, [=] {
        grottoSearcherModel->addItems(searcher->getResults());
        ui->progressBarGrotto->setValue(searcher->getProgress());
    });

    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed, this, [=] {
        ui->pushButtonGrottoSearch->setEnabled(true);
        ui->pushButtonGrottoCancel->setEnabled(false);
        grottoSearcherModel->addItems(searcher->getResults());
        ui->progressBarGrotto->setValue(searcher->getProgress());
        delete searcher;
    });

    thread->start();
    timer->start(1000);
}

void HiddenGrotto::grottoSearcherLocationIndexChanged(int index)
{
    if (index >= 0)
    {
        const auto &area = encounter[ui->comboBoxGrottoSearcherLocation->currentIndex()];

        std::vector<u16> pokemon = area.getUniqueSpecies();
        std::vector<std::string> pokemonNames = area.getSpecieNames();

        ui->comboBoxGrottoSearcherPokemon->clear();
        ui->comboBoxGrottoSearcherPokemon->addItem("-");
        for (int i = 0; i < pokemon.size(); i++)
        {
            ui->comboBoxGrottoSearcherPokemon->addItem(QString::fromStdString(pokemonNames[i]), pokemon[i]);
        }

        std::vector<u16> item = area.getUniqueItems();
        std::vector<std::string> itemNames = area.getItemNames();

        ui->comboBoxGrottoSearcherItems->clear();
        ui->comboBoxGrottoSearcherItems->addItem("-");
        for (int i = 0; i < item.size(); i++)
        {
            ui->comboBoxGrottoSearcherItems->addItem(QString::fromStdString(itemNames[i]), item[i]);
        }
    }
}

void HiddenGrotto::grottoSearcherUpdateFilter()
{
    std::vector<bool> groups(4, false);
    std::vector<bool> encounterSlots(11, false);

    const auto &area = encounter[ui->comboBoxGrottoSearcherLocation->currentIndex()];
    u16 specie = ui->comboBoxGrottoSearcherPokemon->getCurrentUShort();
    u16 item = ui->comboBoxGrottoSearcherItems->getCurrentUShort();

    for (u8 group = 0; group < 4; group++)
    {
        u8 slot = 0;
        for (; slot < 3; slot++)
        {
            if (specie != 0 && specie == area.getPokemon(group, slot).getSpecie())
            {
                groups[group] = true;
                encounterSlots[slot] = true;
            }
        }

        for (; slot < 7; slot++)
        {
            if (item != 0 && item == area.getItem(group, slot - 3))
            {
                groups[group] = true;
                encounterSlots[slot] = true;
            }
        }

        for (; slot < 11; slot++)
        {
            if (item != 0 && item == area.getHiddenItem(group, slot - 7))
            {
                groups[group] = true;
                encounterSlots[slot] = true;
            }
        }
    }

    ui->checkListGrottoSearcherGroup->setChecks(groups);
    ui->checkListGrottoSearcherSlot->setChecks(encounterSlots);
}

void HiddenGrotto::pokemonGenerate()
{
    pokemonGeneratorModel->clearModel();

    u64 seed = ui->textBoxPokemonGeneratorSeed->getULong();
    u32 ivAdvances = ui->textBoxPokemonGeneratorIVAdvances->getUInt();
    u32 initialAdvances = ui->textBoxPokemonGeneratorInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxPokemonGeneratorMaxAdvances->getUInt();
    u32 offset = ui->textBoxPokemonGeneratorOffset->getUInt();
    auto lead = ui->comboMenuPokemonGeneratorLead->getEnum<Lead>();
    u8 gender = ui->comboBoxPokemonGeneratorGender->getCurrentUChar();

    auto slot = encounter[ui->comboBoxPokemonGeneratorLocation->currentIndex()].getPokemon(
        ui->comboBoxPokemonGeneratorGroup->currentIndex(), ui->comboBoxPokemonGeneratorPokemon->currentIndex());

    auto filter = ui->filterPokemonGenerator->getFilter<StateFilter>();
    HiddenGrottoGenerator generator(initialAdvances, maxAdvances, offset, lead, gender, slot, *currentProfile, filter);

    auto states = generator.generate(seed, ivAdvances, 0);
    pokemonGeneratorModel->addItems(states);
}

void HiddenGrotto::pokemonGeneratorGroupIndexChanged(int index)
{
    if (index >= 0)
    {
        auto &area = encounter[ui->comboBoxPokemonGeneratorLocation->currentIndex()];

        ui->comboBoxPokemonGeneratorPokemon->clear();
        for (int i = 0; i < 3; i++)
        {
            auto pokemon = area.getPokemon(index, i);
            ui->comboBoxPokemonGeneratorPokemon->addItem(
                QString("%1: %2").arg(i).arg(QString::fromStdString(Translator::getSpecie(pokemon.getSpecie()))), pokemon.getSpecie());
        }
    }
}

void HiddenGrotto::pokemonGeneratorLocationIndexChanged(int index)
{
    if (index >= 0)
    {
        pokemonGeneratorGroupIndexChanged(ui->comboBoxPokemonGeneratorGroup->currentIndex());
    }
}

void HiddenGrotto::pokemonGeneratorPokemonIndexChanged(int index)
{
    if (index >= 0)
    {
        auto &area = encounter[ui->comboBoxPokemonGeneratorLocation->currentIndex()];
        auto pokemon
            = area.getPokemon(ui->comboBoxPokemonGeneratorGroup->currentIndex(), ui->comboBoxPokemonGeneratorPokemon->currentIndex());

        ui->comboBoxPokemonGeneratorGender->clear();
        switch (pokemon.getInfo()->getGender())
        {
        case 0: // Male Only
            ui->comboBoxPokemonGeneratorGender->addItem(QString::fromStdString(Translator::getGender(0)), 0);
            break;
        case 254: // Female Only
            ui->comboBoxPokemonGeneratorGender->addItem(QString::fromStdString(Translator::getGender(1)), 1);
            break;
        case 255: // Genderless
            ui->comboBoxPokemonGeneratorGender->addItem(QString::fromStdString(Translator::getGender(2)), 2);
            break;
        default:
            ui->comboBoxPokemonGeneratorGender->addItem(QString::fromStdString(Translator::getGender(0)), 0);
            ui->comboBoxPokemonGeneratorGender->addItem(QString::fromStdString(Translator::getGender(1)), 1);
            break;
        }
    }
}

void HiddenGrotto::pokemonSearch()
{
    Date start = ui->dateEditPokemonSearcherStartDate->getDate();
    Date end = ui->dateEditPokemonSearcherEndDate->getDate();
    if (start > end)
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid date range"), tr("Start date is after end date"));
        msg.exec();
        return;
    }

    pokemonSearcherModel->clearModel();

    ui->pushButtonPokemonSearch->setEnabled(false);
    ui->pushButtonPokemonCancel->setEnabled(true);

    u32 initialIVAdvances = ui->textBoxPokemonSearcherInitialIVAdvances->getUInt();
    u32 maxIVAdvances = ui->textBoxPokemonSearcherMaxIVAdvances->getUInt();
    u32 initialAdvances = ui->textBoxPokemonSearcherInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxPokemonSearcherMaxAdvances->getUInt();
    auto lead = ui->comboMenuPokemonSearcherLead->getEnum<Lead>();
    u8 gender = ui->comboBoxPokemonSearcherGender->getCurrentUChar();

    auto slot = encounter[ui->comboBoxPokemonSearcherLocation->currentIndex()].getPokemon(
        ui->comboBoxPokemonSearcherGroup->currentIndex(), ui->comboBoxPokemonSearcherPokemon->currentIndex());

    auto filter = ui->filterPokemonSearcher->getFilter<StateFilter>();
    HiddenGrottoGenerator generator(initialAdvances, maxAdvances, 0, lead, gender, slot, *currentProfile, filter);

    SearcherBase5<HiddenGrottoGenerator, State5> *searcher;
    if (fastSearchEnabled())
    {
        auto ivMap = ivCache->getCache(initialIVAdvances, maxIVAdvances, currentProfile->getVersion(), CacheType::Normal, filter);
        if (shaCache && shaCache->isValid(*currentProfile))
        {
            auto shaMap = shaCache->getCache(initialIVAdvances, maxIVAdvances, start, end, ivMap, CacheType::Normal, *currentProfile);
            searcher = new IVSearcher5CacheFast<HiddenGrottoGenerator, State5>(initialIVAdvances, maxIVAdvances, shaMap, ivMap, generator,
                                                                               *currentProfile);
        }
        else
        {
            searcher
                = new IVSearcher5Fast<HiddenGrottoGenerator, State5>(initialIVAdvances, maxIVAdvances, ivMap, generator, *currentProfile);
        }
    }
    else
    {
        searcher = new IVSearcher5<HiddenGrottoGenerator, State5>(initialIVAdvances, maxIVAdvances, generator, *currentProfile);
    }

    searcher->setMaxProgress(searcher->getMaxProgress(start, end));

    QSettings settings;
    int threads = settings.value("settings/threads").toInt();

    auto *thread = QThread::create([=] { searcher->startSearch(threads, start, end); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(ui->pushButtonPokemonCancel, &QPushButton::clicked, [searcher] { searcher->cancelSearch(); });

    auto *timer = new QTimer();
    connect(timer, &QTimer::timeout, this, [=] {
        pokemonSearcherModel->addItems(searcher->getResults());
        ui->progressBarPokemon->setValue(searcher->getProgress());
    });
    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed, this, [=] {
        ui->pushButtonPokemonSearch->setEnabled(true);
        ui->pushButtonPokemonCancel->setEnabled(false);
        pokemonSearcherModel->addItems(searcher->getResults());
        ui->progressBarPokemon->setValue(searcher->getProgress());
        delete searcher;
    });

    thread->start();
    timer->start(1000);
}

void HiddenGrotto::pokemonSearcherFastSearchChanged()
{
    if (fastSearchEnabled())
    {
        ui->labelPokemonSearcherIVFastSearch->setText(tr("Settings are configured for fast searching"));
    }
    else
    {
        if (ivCache == nullptr)
        {
            ui->labelPokemonSearcherIVFastSearch->setText(tr("Profile does not have a IV cache file configured"));
        }
        else
        {
            QStringList text
                = { tr("Settings are not configured for fast searching"),
                    tr("Keep initial/max advances below %1/%2").arg(ivCache->getInitialAdvances()).arg(ivCache->getMaxAdvances()),
                    tr("Ensure IV filters are set to common spreads") };
            ui->labelPokemonSearcherIVFastSearch->setText(text.join('\n'));
        }
    }
}

void HiddenGrotto::pokemonSearcherGroupIndexChanged(int index)
{
    if (index >= 0)
    {
        auto &area = encounter[ui->comboBoxPokemonSearcherLocation->currentIndex()];

        ui->comboBoxPokemonSearcherPokemon->clear();
        for (int i = 0; i < 3; i++)
        {
            auto pokemon = area.getPokemon(index, i);
            ui->comboBoxPokemonSearcherPokemon->addItem(
                QString("%1: %2").arg(i).arg(QString::fromStdString(Translator::getSpecie(pokemon.getSpecie()))), pokemon.getSpecie());
        }
    }
}

void HiddenGrotto::pokemonSearcherLocationIndexChanged(int index)
{
    if (index >= 0)
    {
        pokemonSearcherGroupIndexChanged(ui->comboBoxPokemonSearcherGroup->currentIndex());
    }
}

void HiddenGrotto::pokemonSearcherPokemonIndexChanged(int index)
{
    if (index >= 0)
    {
        auto &area = encounter[ui->comboBoxPokemonSearcherLocation->currentIndex()];
        auto pokemon
            = area.getPokemon(ui->comboBoxPokemonSearcherGroup->currentIndex(), ui->comboBoxPokemonSearcherPokemon->currentIndex());

        ui->comboBoxPokemonSearcherGender->clear();
        switch (pokemon.getInfo()->getGender())
        {
        case 0: // Male Only
            ui->comboBoxPokemonSearcherGender->addItem(QString::fromStdString(Translator::getGender(0)), 0);
            break;
        case 254: // Female Only
            ui->comboBoxPokemonSearcherGender->addItem(QString::fromStdString(Translator::getGender(1)), 1);
            break;
        case 255: // Genderless
            ui->comboBoxPokemonSearcherGender->addItem(QString::fromStdString(Translator::getGender(2)), 2);
            break;
        default:
            ui->comboBoxPokemonSearcherGender->addItem(QString::fromStdString(Translator::getGender(0)), 0);
            ui->comboBoxPokemonSearcherGender->addItem(QString::fromStdString(Translator::getGender(1)), 1);
            break;
        }
    }
}

void HiddenGrotto::profileIndexChanged(int index)
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

        if (ivCache)
        {
            delete ivCache;
            ivCache = nullptr;
        }

        if (shaCache)
        {
            delete shaCache;
            shaCache = nullptr;
        }

        auto ivCachePath = currentProfile->getIVCache();
        if (!ivCachePath.empty())
        {
            ivCache = new IVCache(ivCachePath);
        }

        auto shaCachePath = currentProfile->getSHACache();
        if (!shaCachePath.empty())
        {
            shaCache = new SHA1Cache(shaCachePath);
            ui->dateEditPokemonSearcherStartDate->setDateRange(shaCache->getStartDate(), shaCache->getEndDate());
            ui->dateEditPokemonSearcherEndDate->setDateRange(shaCache->getStartDate(), shaCache->getEndDate());
        }
        else
        {
            ui->dateEditPokemonSearcherStartDate->clearDateRange();
            ui->dateEditPokemonSearcherEndDate->clearDateRange();
        }
    }
}

void HiddenGrotto::profileManager()
{
    auto *manager = new ProfileManager5();
    connect(manager, &ProfileManager5::profilesModified, this, [=](int num) { emit profilesModified(num); });
    manager->show();
}
