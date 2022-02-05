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
#include <Core/Gen3/Generators/GameCubeGenerator.hpp>
#include <Core/Gen3/LockInfo.hpp>
#include <Core/Gen3/Searchers/GameCubeSearcher.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/Controls/Controls.hpp>
#include <Forms/Gen3/Profile/ProfileManager3.hpp>
#include <Forms/Gen3/Tools/GameCubeRTC.hpp>
#include <Forms/Models/Gen3/GameCubeModel.hpp>
#include <QMenu>
#include <QSettings>
#include <QThread>
#include <QTimer>

GameCube::GameCube(QWidget *parent) : QWidget(parent), ui(new Ui::GameCube)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    ui->labelSearcherShadow->setVisible(false);
    ui->comboBoxSearcherShadow->setVisible(false);
    ui->labelGeneratorShadow->setVisible(false);
    ui->comboBoxGeneratorShadow->setVisible(false);
    ui->labelGeneratorType->setVisible(false);
    ui->comboBoxGeneratorType->setVisible(false);

    setupModels();
    updateProfiles();
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
    profiles = { Profile3("-", Game::Gales, 12345, 54321) };
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

void GameCube::setupModels()
{
    generatorModel = new GameCubeGeneratorModel(ui->tableViewGenerator);
    searcherModel = new GameCubeSearcherModel(ui->tableViewSearcher, Method::XDColo);

    generatorMenu = new QMenu(ui->tableViewGenerator);
    searcherMenu = new QMenu(ui->tableViewSearcher);

    ui->tableViewGenerator->setModel(generatorModel);
    ui->tableViewSearcher->setModel(searcherModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed32Bit);
    ui->textBoxGeneratorStartingAdvance->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);

    ui->comboBoxGeneratorMethod->setup({ toInt(Method::XDColo), toInt(Method::XD), toInt(Method::Colo), toInt(Method::Channel) });
    ui->comboBoxSearcherMethod->setup({ toInt(Method::XDColo), toInt(Method::XD), toInt(Method::Colo), toInt(Method::Channel) });

    ui->filterGenerator->disableControls(Controls::EncounterSlots);
    ui->filterSearcher->disableControls(Controls::EncounterSlots | Controls::UseDelay | Controls::DisableFilter);

    QAction *outputTXTGenerator = generatorMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVGenerator = generatorMenu->addAction(tr("Output Results to CSV"));
    connect(outputTXTGenerator, &QAction::triggered, this, [=] { ui->tableViewGenerator->outputModel(); });
    connect(outputCSVGenerator, &QAction::triggered, this, [=] { ui->tableViewGenerator->outputModel(true); });

    QAction *seedToTime = searcherMenu->addAction(tr("Generate times for seed"));
    QAction *outputTXTSearcher = searcherMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVSearcher = searcherMenu->addAction(tr("Output Results to CSV"));
    connect(seedToTime, &QAction::triggered, this, &GameCube::seedToTime);
    connect(outputTXTSearcher, &QAction::triggered, this, [=] { ui->tableViewSearcher->outputModel(); });
    connect(outputCSVSearcher, &QAction::triggered, this, [=] { ui->tableViewSearcher->outputModel(true); });

    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GameCube::profilesIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &GameCube::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &GameCube::search);
    connect(ui->comboBoxGeneratorMethod, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GameCube::generatorMethodIndexChanged);
    connect(ui->comboBoxGeneratorShadow, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GameCube::generatorShadowIndexChanged);
    connect(ui->comboBoxSearcherMethod, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GameCube::searcherMethodIndexChanged);
    connect(ui->tableViewGenerator, &QTableView::customContextMenuRequested, this, &GameCube::tableViewGeneratorContextMenu);
    connect(ui->tableViewSearcher, &QTableView::customContextMenuRequested, this, &GameCube::tableViewSearcherContextMenu);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &GameCube::profileManager);

    QSettings setting;
    if (setting.contains("gamecube/geometry"))
    {
        this->restoreGeometry(setting.value("gamecube/geometry").toByteArray());
    }
}

void GameCube::generate()
{
    generatorModel->clearModel();

    u32 seed = ui->textBoxGeneratorSeed->getUInt();
    u32 initialAdvances = ui->textBoxGeneratorStartingAdvance->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->filterGenerator->getGenderRatio();
    auto method = static_cast<Method>(ui->comboBoxGeneratorMethod->getCurrentInt());
    u32 offset = 0;
    if (ui->filterGenerator->useDelay())
    {
        offset = ui->filterGenerator->getDelay();
    }

    StateFilter filter(ui->filterGenerator->getGender(), ui->filterGenerator->getAbility(), ui->filterGenerator->getShiny(),
                       ui->filterGenerator->getDisableFilters(), ui->filterGenerator->getMinIVs(), ui->filterGenerator->getMaxIVs(),
                       ui->filterGenerator->getNatures(), ui->filterGenerator->getHiddenPowers(), {});

    GameCubeGenerator generator(initialAdvances, maxAdvances, tid, sid, genderRatio, method, filter);
    generator.setOffset(offset);

    if (method == Method::XD || method == Method::Colo)
    {
        generator.setShadowTeam(static_cast<u8>(ui->comboBoxGeneratorShadow->currentIndex()),
                                static_cast<u8>(ui->comboBoxGeneratorType->currentIndex()));
    }

    auto states = generator.generate(seed);
    generatorModel->addItems(states);
}

void GameCube::search()
{
    auto method = static_cast<Method>(ui->comboBoxSearcherMethod->getCurrentInt());
    searcherModel->clearModel();
    searcherModel->setMethod(method);

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    std::array<u8, 6> min = ui->filterSearcher->getMinIVs();
    std::array<u8, 6> max = ui->filterSearcher->getMaxIVs();

    StateFilter filter(ui->filterSearcher->getGender(), ui->filterSearcher->getAbility(), ui->filterSearcher->getShiny(), false, min, max,
                       ui->filterSearcher->getNatures(), ui->filterSearcher->getHiddenPowers(), {});

    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->filterSearcher->getGenderRatio();

    auto *searcher = new GameCubeSearcher(tid, sid, genderRatio, method, filter);
    if (method == Method::XD || method == Method::Colo)
    {
        searcher->setupNatureLock(static_cast<u8>(ui->comboBoxSearcherShadow->currentIndex()));
    }

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

void GameCube::profilesIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile.getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile.getSID()));
        ui->labelProfileGameValue->setText(QString::fromStdString(currentProfile.getVersionString()));
    }
}

void GameCube::generatorMethodIndexChanged(int index)
{
    if (index >= 0)
    {
        auto method = static_cast<Method>(ui->comboBoxGeneratorMethod->currentData().toInt());
        ui->comboBoxGeneratorShadow->clear();

        if (method == Method::XD)
        {
            std::vector<std::string> species = Translator::getSpecies(
                { 334, 24,  354, 12,  113, 301, 85,  149, 51,  355, 125, 83,  55,  88,  58,  316, 316, 316, 107, 106, 97, 115, 131, 165,
                  108, 337, 219, 126, 82,  296, 310, 105, 303, 52,  122, 177, 299, 322, 46,  17,  204, 127, 62,  261, 57, 280, 78,  20,
                  315, 302, 373, 123, 273, 273, 273, 86,  285, 143, 361, 338, 21,  363, 363, 363, 167, 121, 220, 114, 49, 100, 37,  70 });
            for (size_t i = 0; i < species.size(); i++)
            {
                QString specie = QString::fromStdString(species[i]);
                if (i == 15 || i == 52 || i == 61)
                {
                    specie += tr(" (Citadark)");
                }
                else if (i == 16 || i == 53 || i == 62)
                {
                    specie += tr(" (Initial)");
                }
                else if (i == 17 || i == 54 || i == 63)
                {
                    specie += tr(" (Phenac)");
                }
                ui->comboBoxGeneratorShadow->addItem(specie);
            }

            ui->comboBoxGeneratorShadow->setVisible(true);
            ui->labelGeneratorShadow->setVisible(true);

            std::array<int, 14> shadows = { 0, 2, 3, 4, 14, 20, 22, 26, 34, 41, 49, 50, 57, 71 };
            bool flag = std::find(shadows.begin(), shadows.end(), ui->comboBoxGeneratorShadow->currentIndex()) != shadows.end();

            ui->comboBoxGeneratorType->setVisible(flag);
            ui->labelGeneratorType->setVisible(flag);
        }
        else if (method == Method::Colo)
        {
            std::vector<std::string> species = Translator::getSpecies({ 207, 214, 296, 179, 198, 212, 175, 217 });
            for (size_t i = 0; i < species.size(); i++)
            {
                QString specie = QString::fromStdString(species[i]);
                if (i == 3 || i == 5 || i == 6)
                {
                    specie += tr(" (E-Reader)");
                }
                ui->comboBoxGeneratorShadow->addItem(specie);
            }

            ui->comboBoxGeneratorShadow->setVisible(true);
            ui->labelGeneratorShadow->setVisible(true);
            ui->comboBoxGeneratorType->setVisible(false);
            ui->labelGeneratorType->setVisible(false);
        }
        else
        {
            ui->comboBoxGeneratorShadow->setVisible(false);
            ui->labelGeneratorShadow->setVisible(false);
            ui->comboBoxGeneratorType->setVisible(false);
            ui->labelGeneratorType->setVisible(false);
        }
    }
}

void GameCube::generatorShadowIndexChanged(int index)
{
    if (index >= 0)
    {
        auto method = static_cast<Method>(ui->comboBoxGeneratorMethod->getCurrentInt());
        if (ui->comboBoxGeneratorShadow->isVisible() && method == Method::XD)
        {
            std::array<int, 14> shadows = { 0, 2, 3, 4, 14, 20, 22, 26, 34, 41, 49, 50, 57, 71 };
            bool flag = std::find(shadows.begin(), shadows.end(), ui->comboBoxGeneratorShadow->currentIndex()) != shadows.end();

            ui->comboBoxGeneratorType->setVisible(flag);
            ui->labelGeneratorType->setVisible(flag);
        }
        else
        {
            ui->comboBoxGeneratorType->setVisible(false);
            ui->labelGeneratorType->setVisible(false);
        }
    }
}

void GameCube::searcherMethodIndexChanged(int index)
{
    if (index >= 0)
    {
        auto method = static_cast<Method>(ui->comboBoxSearcherMethod->getCurrentInt());
        ui->comboBoxSearcherShadow->clear();

        if (method == Method::XD)
        {
            std::vector<std::string> species = Translator::getSpecies(
                { 334, 24,  354, 12,  113, 301, 85,  149, 51,  355, 125, 83,  55,  88,  58,  316, 316, 316, 107, 106, 97, 115, 131, 165,
                  108, 337, 219, 126, 82,  296, 310, 105, 303, 52,  122, 177, 299, 322, 46,  17,  204, 127, 62,  261, 57, 280, 78,  20,
                  315, 302, 373, 123, 273, 273, 273, 86,  285, 143, 361, 338, 21,  363, 363, 363, 167, 121, 220, 114, 49, 100, 37,  70 });
            for (size_t i = 0; i < species.size(); i++)
            {
                QString specie = QString::fromStdString(species[i]);
                if (i == 15 || i == 52 || i == 61)
                {
                    specie += tr(" (Citadark)");
                }
                else if (i == 16 || i == 53 || i == 62)
                {
                    specie += tr(" (Initial)");
                }
                else if (i == 17 || i == 54 || i == 63)
                {
                    specie += tr(" (Phenac)");
                }
                ui->comboBoxSearcherShadow->addItem(specie);
            }

            ui->comboBoxSearcherShadow->setVisible(true);
            ui->labelSearcherShadow->setVisible(true);
        }
        else if (method == Method::Colo)
        {
            std::vector<std::string> species = Translator::getSpecies({ 207, 214, 296, 179, 198, 212, 175, 217 });
            for (size_t i = 0; i < species.size(); i++)
            {
                QString specie = QString::fromStdString(species[i]);
                if (i == 3 || i == 5 || i == 6)
                {
                    specie += tr(" (E-Reader)");
                }
                ui->comboBoxSearcherShadow->addItem(specie);
            }

            ui->comboBoxSearcherShadow->setVisible(true);
            ui->labelSearcherShadow->setVisible(true);
        }
        else
        {
            ui->comboBoxSearcherShadow->setVisible(false);
            ui->labelSearcherShadow->setVisible(false);
        }
    }
}

void GameCube::tableViewGeneratorContextMenu(QPoint pos)
{
    if (generatorModel->rowCount() > 0)
    {
        generatorMenu->popup(ui->tableViewGenerator->viewport()->mapToGlobal(pos));
    }
}

void GameCube::tableViewSearcherContextMenu(QPoint pos)
{
    if (searcherModel->rowCount() > 0)
    {
        searcherMenu->popup(ui->tableViewSearcher->viewport()->mapToGlobal(pos));
    }
}

void GameCube::seedToTime()
{
    QModelIndex index = ui->tableViewSearcher->currentIndex();
    index = searcherModel->index(index.row(), 0);

    u32 seed = searcherModel->data(index).toString().toUInt(nullptr, 16);
    auto *rtc = new GameCubeRTC(seed);
    rtc->show();
    rtc->raise();
}

void GameCube::profileManager()
{
    auto *manager = new ProfileManager3();
    connect(manager, &ProfileManager3::updateProfiles, this, [=] { emit alertProfiles(3); });
    manager->show();
}
