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
#include <Core/Gen5/Encounters5.hpp>
#include <Core/Gen5/Generators/HiddenGrottoGenerator.hpp>
#include <Core/Gen5/HiddenGrottoArea.hpp>
#include <Core/Gen5/Keypresses.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/Searchers/Searcher5.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Gen5/Profile/ProfileManager5.hpp>
#include <Model/Gen5/HiddenGrottoModel.hpp>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include <QTimer>

HiddenGrotto::HiddenGrotto(QWidget *parent) : QWidget(parent), ui(new Ui::HiddenGrotto), encounter(Encounters5::getHiddenGrottoEncounters())
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    generatorModel = new HiddenGrottoGeneratorModel5(ui->tableViewGenerator);
    ui->tableViewGenerator->setModel(generatorModel);

    searcherModel = new HiddenGrottoSearcherModel5(ui->tableViewSearcher);
    ui->tableViewSearcher->setModel(searcherModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed64Bit);
    ui->textBoxGeneratorInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorOffset->setValues(InputType::Advance32Bit);

    ui->textBoxSearcherInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMaxAdvances->setValues(InputType::Advance32Bit);

    ui->comboBoxGeneratorGrottoPower->setup({ 5, 15, 25, 35, 55 });
    ui->checkListGeneratorSlot->setup();
    ui->checkListGeneratorGroup->setup();
    ui->checkListGeneratorGender->setup();

    ui->comboBoxSearcherGrottoPower->setup({ 5, 15, 25, 35, 55 });
    ui->checkListSearcherSlot->setup();
    ui->checkListSearcherGroup->setup();
    ui->checkListSearcherGender->setup();

    ui->comboBoxGeneratorLocation->enableAutoComplete();
    ui->comboBoxSearcherLocation->enableAutoComplete();

    connect(ui->comboBoxGeneratorLocation, &QComboBox::currentIndexChanged, this, &HiddenGrotto::generatorLocationIndexChanged);
    connect(ui->comboBoxSearcherLocation, &QComboBox::currentIndexChanged, this, &HiddenGrotto::searcherLocationIndexChanged);
    connect(ui->comboBoxGeneratorPokemon, &QComboBox::currentIndexChanged, this, &HiddenGrotto::generatorUpdateFilter);
    connect(ui->comboBoxSearcherPokemon, &QComboBox::currentIndexChanged, this, &HiddenGrotto::searcherUpdateFilter);
    connect(ui->comboBoxGeneratorItems, &QComboBox::currentIndexChanged, this, &HiddenGrotto::generatorUpdateFilter);
    connect(ui->comboBoxSearcherItems, &QComboBox::currentIndexChanged, this, &HiddenGrotto::searcherUpdateFilter);
    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &HiddenGrotto::profileIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &HiddenGrotto::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &HiddenGrotto::search);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &HiddenGrotto::profileManager);

    std::vector<u16> locs;
    std::transform(encounter.begin(), encounter.end(), std::back_inserter(locs),
                   [](const HiddenGrottoArea &area) { return area.getLocation(); });

    auto locations = Translator::getLocations(locs, Game::BW2);
    std::vector<int> indices(locations.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(), [&locations](int i, int j) { return locations[i] < locations[j]; });

    for (int i : indices)
    {
        ui->comboBoxGeneratorLocation->addItem(QString::fromStdString(locations[i]), i);
        ui->comboBoxSearcherLocation->addItem(QString::fromStdString(locations[i]), i);
    }

    updateProfiles();

    QSettings setting;
    setting.beginGroup("hiddenGrotto");
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

HiddenGrotto::~HiddenGrotto()
{
    QSettings setting;
    setting.beginGroup("hiddenGrotto");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

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

void HiddenGrotto::generate()
{
    generatorModel->clearModel();

    u64 seed = ui->textBoxGeneratorSeed->getULong();
    u32 initialAdvances = ui->textBoxGeneratorInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();
    u32 offset = ui->textBoxGeneratorOffset->getUInt();
    u8 powerLevel = ui->comboBoxGeneratorGrottoPower->currentData().toUInt();

    HiddenGrottoFilter filter(ui->checkListGeneratorSlot->getCheckedArray<11>(), ui->checkListGeneratorGender->getCheckedArray<2>(),
                              ui->checkListGeneratorGroup->getCheckedArray<4>());
    HiddenGrottoGenerator generator(initialAdvances, maxAdvances, offset, powerLevel,
                                    encounter[ui->comboBoxGeneratorLocation->getCurrentInt()], *currentProfile, filter);

    auto states = generator.generate(seed);
    generatorModel->addItems(states);
}

void HiddenGrotto::generatorLocationIndexChanged(int index)
{
    if (index >= 0)
    {
        auto &area = encounter[ui->comboBoxGeneratorLocation->getCurrentInt()];
        auto pokemon = area.getUniqueSpecies();
        auto pokemonNames = area.getSpecieNames();

        ui->comboBoxGeneratorPokemon->clear();
        ui->comboBoxGeneratorPokemon->addItem("-");
        for (int i = 0; i < pokemon.size(); i++)
        {
            ui->comboBoxGeneratorPokemon->addItem(QString::fromStdString(pokemonNames[i]), pokemon[i]);
        }

        auto item = area.getUniqueItems();
        auto itemNames = area.getItemNames();

        ui->comboBoxGeneratorItems->clear();
        ui->comboBoxGeneratorItems->addItem("-");
        for (int i = 0; i < item.size(); i++)
        {
            ui->comboBoxGeneratorItems->addItem(QString::fromStdString(itemNames[i]), item[i]);
        }
    }
}

void HiddenGrotto::generatorUpdateFilter()
{
    std::vector<bool> groups(4, false);
    std::vector<bool> encounterSlots(11, false);

    const auto &area = encounter[ui->comboBoxGeneratorLocation->getCurrentInt()];
    u16 specie = ui->comboBoxGeneratorPokemon->getCurrentUShort();
    u16 item = ui->comboBoxGeneratorItems->getCurrentUShort();

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

        ui->checkListGeneratorGroup->setChecks(groups);
        ui->checkListGeneratorSlot->setChecks(encounterSlots);
    }
}

void HiddenGrotto::search()
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

    u32 initialAdvances = ui->textBoxSearcherInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxSearcherMaxAdvances->getUInt();
    u8 powerLevel = ui->comboBoxSearcherGrottoPower->getCurrentUInt();

    HiddenGrottoFilter filter(ui->checkListSearcherSlot->getCheckedArray<11>(), ui->checkListSearcherGender->getCheckedArray<2>(),
                              ui->checkListSearcherGroup->getCheckedArray<4>());
    HiddenGrottoGenerator generator(initialAdvances, maxAdvances, 0, powerLevel, encounter[ui->comboBoxSearcherLocation->getCurrentInt()],
                                    *currentProfile, filter);
    auto *searcher = new Searcher5<HiddenGrottoGenerator, HiddenGrottoState>(generator, *currentProfile);

    int maxProgress = Keypresses::getKeypresses(*currentProfile).size();
    maxProgress *= start.daysTo(end) + 1;
    maxProgress *= currentProfile->getTimer0Max() - currentProfile->getTimer0Min() + 1;
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

void HiddenGrotto::searcherLocationIndexChanged(int index)
{
    if (index >= 0)
    {
        const auto &area = encounter[ui->comboBoxSearcherLocation->getCurrentInt()];

        std::vector<u16> pokemon = area.getUniqueSpecies();
        std::vector<std::string> pokemonNames = area.getSpecieNames();

        ui->comboBoxSearcherPokemon->clear();
        ui->comboBoxSearcherPokemon->addItem("-");
        for (int i = 0; i < pokemon.size(); i++)
        {
            ui->comboBoxSearcherPokemon->addItem(QString::fromStdString(pokemonNames[i]), pokemon[i]);
        }

        std::vector<u16> item = area.getUniqueItems();
        std::vector<std::string> itemNames = area.getItemNames();

        ui->comboBoxSearcherItems->clear();
        ui->comboBoxSearcherItems->addItem("-");
        for (int i = 0; i < item.size(); i++)
        {
            ui->comboBoxSearcherItems->addItem(QString::fromStdString(itemNames[i]), item[i]);
        }
    }
}

void HiddenGrotto::searcherUpdateFilter()
{
    std::vector<bool> groups(4, false);
    std::vector<bool> encounterSlots(11, false);

    const auto &area = encounter[ui->comboBoxSearcherLocation->getCurrentInt()];
    u16 specie = ui->comboBoxSearcherPokemon->getCurrentUShort();
    u16 item = ui->comboBoxSearcherItems->getCurrentUShort();

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

    ui->checkListSearcherGroup->setChecks(groups);
    ui->checkListSearcherSlot->setChecks(encounterSlots);
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
    }
}

void HiddenGrotto::profileManager()
{
    auto *manager = new ProfileManager5();
    connect(manager, &ProfileManager5::profilesModified, this, [=](int num) { emit profilesModified(num); });
    manager->show();
}
