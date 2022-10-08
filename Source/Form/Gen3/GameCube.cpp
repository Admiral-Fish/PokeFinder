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

#include "GameCube.hpp"
#include "ui_GameCube.h"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Enum/ShadowType.hpp>
#include <Core/Gen3/Encounters3.hpp>
#include <Core/Gen3/Generators/GameCubeGenerator.hpp>
#include <Core/Gen3/Profile3.hpp>
#include <Core/Gen3/Searchers/GameCubeSearcher.hpp>
#include <Core/Gen3/ShadowTemplate.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen3/Profile/ProfileManager3.hpp>
#include <Model/Gen3/GameCubeModel.hpp>
#include <QMenu>
#include <QSettings>
#include <QThread>
#include <QTimer>

GameCube::GameCube(QWidget *parent) : QWidget(parent), ui(new Ui::GameCube)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    generatorModel = new GameCubeGeneratorModel(ui->tableViewGenerator);
    searcherModel = new GameCubeSearcherModel(ui->tableViewSearcher);

    ui->tableViewGenerator->setModel(generatorModel);
    ui->tableViewSearcher->setModel(searcherModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed32Bit);
    ui->textBoxGeneratorStartingAdvance->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);

    ui->filterGenerator->disableControls(Controls::EncounterSlots);
    ui->filterSearcher->disableControls(Controls::EncounterSlots | Controls::UseDelay | Controls::DisableFilter);

    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &GameCube::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &GameCube::search);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &GameCube::profileManager);
    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &GameCube::profilesIndexChanged);
    connect(ui->comboBoxGeneratorCategory, &QComboBox::currentIndexChanged, this, &GameCube::generatorCategoryIndexChanged);
    connect(ui->comboBoxGeneratorPokemon, &QComboBox::currentIndexChanged, this, &GameCube::generatorPokemonIndexChanged);
    connect(ui->comboBoxSearcherCategory, &QComboBox::currentIndexChanged, this, &GameCube::searcherCategoryIndexChanged);
    connect(ui->comboBoxSearcherPokemon, &QComboBox::currentIndexChanged, this, &GameCube::searcherPokemonIndexChanged);
    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &GameCube::profilesIndexChanged);
    connect(ui->filterGenerator, &Filter::showStatsChanged, generatorModel, &GameCubeGeneratorModel::setShowStats);
    connect(ui->filterSearcher, &Filter::showStatsChanged, searcherModel, &GameCubeSearcherModel::setShowStats);

    updateProfiles();
    generatorCategoryIndexChanged(0);
    searcherCategoryIndexChanged(0);

    QSettings setting;
    if (setting.contains("gamecube/geometry"))
    {
        this->restoreGeometry(setting.value("gamecube/geometry").toByteArray());
    }
}

GameCube::~GameCube()
{
    QSettings setting;
    setting.beginGroup("gamecube");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void GameCube::updateProfiles()
{
    profiles = { Profile3("-", Game::Gales, 12345, 54321, false) };
    auto completeProfiles = ProfileLoader3::getProfiles();
    std::copy_if(completeProfiles.begin(), completeProfiles.end(), std::back_inserter(profiles),
                 [](const Profile3 &profile) { return (profile.getVersion() & Game::GC) != Game::None; });

    ui->comboBoxProfiles->clear();
    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("gamecube/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void GameCube::generate()
{
    generatorModel->clearModel();

    Method method = ui->comboBoxGeneratorCategory->currentIndex() == 1 ? Method::Channel : Method::None;
    bool shadowLock = ui->comboBoxGeneratorCategory->currentIndex() == 2;

    u32 seed = ui->textBoxGeneratorSeed->getUInt();
    u32 initialAdvances = ui->textBoxGeneratorStartingAdvance->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();
    u16 tid = currentProfile->getTID();
    u16 sid = currentProfile->getSID();
    u32 offset = 0;
    if (ui->filterGenerator->useDelay())
    {
        offset = ui->filterGenerator->getDelay();
    }

    StateFilter3 filter(ui->filterGenerator->getGender(), ui->filterGenerator->getAbility(), ui->filterGenerator->getShiny(),
                        ui->filterGenerator->getDisableFilters(), ui->filterGenerator->getMinIVs(), ui->filterGenerator->getMaxIVs(),
                        ui->filterGenerator->getNatures(), ui->filterGenerator->getHiddenPowers());

    GameCubeGenerator generator(initialAdvances, maxAdvances, offset, tid, sid, currentProfile->getVersion(), method,
                                ui->checkBoxGeneratorFirstShadowUnset->isChecked(), filter);

    std::vector<GeneratorState3> states;
    if (shadowLock)
    {
        const ShadowTemplate *shadowTemplate = Encounters3::getShadowTeam(ui->comboBoxGeneratorPokemon->getCurrentInt());
        states = generator.generate(seed, shadowTemplate);
    }
    else
    {
        const StaticTemplate *staticTemplate = Encounters3::getStaticEncounter(ui->comboBoxGeneratorCategory->currentIndex() + 7,
                                                                               ui->comboBoxGeneratorPokemon->getCurrentInt());
        states = generator.generate(seed, staticTemplate);
    }

    generatorModel->addItems(states);
}

void GameCube::generatorCategoryIndexChanged(int index)
{
    if (index >= 0)
    {
        size_t size;
        ui->comboBoxGeneratorPokemon->clear();
        if (index == 2)
        {
            const ShadowTemplate *templates = Encounters3::getShadowTeams(&size);
            for (size_t i = 0; i < size; i++)
            {
                if ((currentProfile->getVersion() & templates[i].getVersion()) != Game::None)
                {
                    ui->comboBoxGeneratorPokemon->addItem(QString::fromStdString(*Translator::getSpecie(templates[i].getSpecie())),
                                                          QVariant::fromValue(i));
                }
            }
        }
        else
        {
            const StaticTemplate *templates = Encounters3::getStaticEncounters(index + 7, &size);
            for (size_t i = 0; i < size; i++)
            {
                if ((currentProfile->getVersion() & templates[i].getVersion()) != Game::None)
                {
                    ui->comboBoxGeneratorPokemon->addItem(QString::fromStdString(*Translator::getSpecie(templates[i].getSpecie())),
                                                          QVariant::fromValue(i));
                }
            }
        }
    }
}

void GameCube::generatorPokemonIndexChanged(int index)
{
    if (index >= 0)
    {
        if (ui->comboBoxGeneratorCategory->currentIndex() == 2)
        {
            const ShadowTemplate *shadowTemplate = Encounters3::getShadowTeam(ui->comboBoxGeneratorPokemon->getCurrentInt());
            ui->spinBoxGeneratorLevel->setValue(shadowTemplate->getLevel());

            ui->checkBoxGeneratorFirstShadowUnset->setVisible(shadowTemplate->getType() == ShadowType::SecondShadow
                                                              || shadowTemplate->getType() == ShadowType::Salamence);
        }
        else
        {
            const StaticTemplate *staticTemplate = Encounters3::getStaticEncounter(ui->comboBoxGeneratorCategory->currentIndex() + 7,
                                                                                   ui->comboBoxGeneratorPokemon->getCurrentInt());
            ui->spinBoxGeneratorLevel->setValue(staticTemplate->getLevel());

            ui->checkBoxGeneratorFirstShadowUnset->setVisible(false);
        }
    }
}

void GameCube::profilesIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = &profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile->getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile->getSID()));
        ui->labelProfileGameValue->setText(QString::fromStdString(*Translator::getGame(currentProfile->getVersion())));

        generatorCategoryIndexChanged(ui->comboBoxGeneratorCategory->currentIndex());
        searcherCategoryIndexChanged(ui->comboBoxSearcherCategory->currentIndex());
    }
}

void GameCube::profileManager()
{
    auto *manager = new ProfileManager3();
    connect(manager, &ProfileManager3::updateProfiles, this, [=] { emit alertProfiles(3); });
    manager->show();
}

void GameCube::search()
{
    searcherModel->clearModel();

    Method method = ui->comboBoxSearcherCategory->currentIndex() == 1 ? Method::Channel : Method::None;
    bool shadowLock = ui->comboBoxSearcherCategory->currentIndex() == 2;

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    std::array<u8, 6> min = ui->filterSearcher->getMinIVs();
    std::array<u8, 6> max = ui->filterSearcher->getMaxIVs();

    StateFilter3 filter(ui->filterSearcher->getGender(), ui->filterSearcher->getAbility(), ui->filterSearcher->getShiny(), false, min, max,
                        ui->filterSearcher->getNatures(), ui->filterSearcher->getHiddenPowers());

    u16 tid = currentProfile->getTID();
    u16 sid = currentProfile->getSID();

    auto *searcher
        = new GameCubeSearcher(tid, sid, currentProfile->getVersion(), method, ui->checkBoxSearcherFirstShadowUnset->isChecked(), filter);

    int maxProgress = 1;
    if (method != Method::Channel)
    {
        for (u8 i = 0; i < 6; i++)
        {
            maxProgress *= max[i] - min[i] + 1;
        }
    }
    else
    {
        maxProgress *= max[4] - min[4] + 1;
        maxProgress *= 0x7ffffff;
    }
    ui->progressBar->setRange(0, maxProgress);

    QThread *thread;
    if (shadowLock)
    {
        const ShadowTemplate *shadowTemplate = Encounters3::getShadowTeam(ui->comboBoxSearcherPokemon->getCurrentInt());
        thread = QThread::create([=] { searcher->startSearch(min, max, shadowTemplate); });
    }
    else
    {
        const StaticTemplate *staticTemplate = Encounters3::getStaticEncounter(ui->comboBoxSearcherCategory->currentIndex() + 7,
                                                                               ui->comboBoxSearcherPokemon->getCurrentInt());
        thread = QThread::create([=] { searcher->startSearch(min, max, staticTemplate); });
    }

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

void GameCube::searcherCategoryIndexChanged(int index)
{
    if (index >= 0)
    {
        size_t size;
        ui->comboBoxSearcherPokemon->clear();
        if (index == 2)
        {
            const ShadowTemplate *templates = Encounters3::getShadowTeams(&size);
            for (size_t i = 0; i < size; i++)
            {
                if ((currentProfile->getVersion() & templates[i].getVersion()) != Game::None)
                {
                    ui->comboBoxSearcherPokemon->addItem(QString::fromStdString(*Translator::getSpecie(templates[i].getSpecie())),
                                                         QVariant::fromValue(i));
                }
            }
        }
        else
        {
            const StaticTemplate *templates = Encounters3::getStaticEncounters(index + 7, &size);
            for (size_t i = 0; i < size; i++)
            {
                if ((currentProfile->getVersion() & templates[i].getVersion()) != Game::None)
                {
                    ui->comboBoxSearcherPokemon->addItem(QString::fromStdString(*Translator::getSpecie(templates[i].getSpecie())),
                                                         QVariant::fromValue(i));
                }
            }
        }
    }
}

void GameCube::searcherPokemonIndexChanged(int index)
{
    if (index >= 0)
    {
        if (ui->comboBoxSearcherCategory->currentIndex() == 2)
        {
            const ShadowTemplate *shadowTemplate = Encounters3::getShadowTeam(ui->comboBoxSearcherPokemon->getCurrentInt());
            ui->spinBoxSearcherLevel->setValue(shadowTemplate->getLevel());

            ui->checkBoxSearcherFirstShadowUnset->setVisible(shadowTemplate->getType() == ShadowType::SecondShadow
                                                             || shadowTemplate->getType() == ShadowType::Salamence);
        }
        else
        {
            const StaticTemplate *staticTemplate = Encounters3::getStaticEncounter(ui->comboBoxSearcherCategory->currentIndex() + 7,
                                                                                   ui->comboBoxSearcherPokemon->getCurrentInt());
            ui->spinBoxSearcherLevel->setValue(staticTemplate->getLevel());

            ui->checkBoxSearcherFirstShadowUnset->setVisible(false);
        }
    }
}
