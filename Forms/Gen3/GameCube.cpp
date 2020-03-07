/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Gen3/ProfileLoader3.hpp>
#include <Core/Gen3/Searchers/GameCubeSearcher.hpp>
#include <Core/Util/Translator.hpp>
#include <Forms/Gen3/Profile/ProfileManager3.hpp>
#include <Forms/Gen3/Tools/GameCubeRTC.hpp>
#include <Models/Gen3/GameCubeModel.hpp>
#include <QClipboard>
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

    updateProfiles();
    setupModels();

    qRegisterMetaType<QVector<Frame>>("QVector<Frame>");
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
    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GameCube::profilesIndexChanged);

    profiles = { Profile3(tr("None"), Game::Gales, 12345, 54321) };
    for (const auto &profile : ProfileLoader3::getProfiles())
    {
        if (profile.getVersion() & Game::GC)
        {
            profiles.append(profile);
        }
    }

    ui->comboBoxProfiles->clear();
    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(profile.getName());
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
    ui->textBoxGeneratorStartingFrame->setValues(InputType::Frame32Bit);
    ui->textBoxGeneratorMaxResults->setValues(InputType::Frame32Bit);

    ui->comboBoxGeneratorMethod->setup({ Method::XDColo, Method::XD, Method::Colo, Method::Channel });
    ui->comboBoxSearcherMethod->setup({ Method::XDColo, Method::XD, Method::Colo, Method::Channel });

    ui->filterGenerator->disableControls(Controls::EncounterSlots);
    ui->filterSearcher->disableControls(Controls::EncounterSlots | Controls::UseDelay | Controls::DisableFilter);

    QAction *outputTXTGenerator = generatorMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVGenerator = generatorMenu->addAction(tr("Output Results to CSV"));
    connect(outputTXTGenerator, &QAction::triggered, this, [=] { ui->tableViewGenerator->outputModel(); });
    connect(outputCSVGenerator, &QAction::triggered, this, [=] { ui->tableViewGenerator->outputModel(true); });

    QAction *copySeedToClipboard = searcherMenu->addAction(tr("Copy Seed to Clipboard"));
    QAction *seedToTime = searcherMenu->addAction(tr("Generate times for seed"));
    QAction *outputTXTSearcher = searcherMenu->addAction(tr("Output Results to TXT"));
    QAction *outputCSVSearcher = searcherMenu->addAction(tr("Output Results to CSV"));
    connect(copySeedToClipboard, &QAction::triggered, this, &GameCube::copySeedToClipboard);
    connect(seedToTime, &QAction::triggered, this, &GameCube::seedToTime);
    connect(outputTXTSearcher, &QAction::triggered, this, [=] { ui->tableViewSearcher->outputModel(); });
    connect(outputCSVSearcher, &QAction::triggered, this, [=] { ui->tableViewSearcher->outputModel(true); });

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

void GameCube::updateProgress(const QVector<GameCubeFrame> &frames, int progress)
{
    searcherModel->addItems(frames);
    ui->progressBar->setValue(progress);
}

void GameCube::generate()
{
    generatorModel->clearModel();

    u32 seed = ui->textBoxGeneratorSeed->getUInt();
    u32 initialFrame = ui->textBoxGeneratorStartingFrame->getUInt();
    u32 maxResults = ui->textBoxGeneratorMaxResults->getUInt();
    u16 tid = currentProfile.getTID();
    u16 sid = currentProfile.getSID();
    u8 genderRatio = ui->filterGenerator->getGenderRatio();
    auto method = static_cast<Method>(ui->comboBoxGeneratorMethod->getCurrentInt());
    u32 offset = 0;
    if (ui->filterGenerator->useDelay())
    {
        offset = ui->filterGenerator->getDelay();
    }

    FrameFilter filter(ui->filterGenerator->getGender(), ui->filterGenerator->getAbility(), ui->filterGenerator->getShiny(),
                       ui->filterGenerator->getDisableFilters(), ui->filterGenerator->getMinIVs(), ui->filterGenerator->getMaxIVs(),
                       ui->filterGenerator->getNatures(), ui->filterGenerator->getHiddenPowers(), {});

    GameCubeGenerator generator(initialFrame, maxResults, tid, sid, genderRatio, method, filter);
    generator.setOffset(offset);

    if (method == Method::XD || method == Method::Colo)
    {
        generator.setShadowTeam(static_cast<u8>(ui->comboBoxGeneratorShadow->currentIndex()),
                                static_cast<u8>(ui->comboBoxGeneratorType->currentIndex()));
    }

    auto frames = generator.generate(seed);
    generatorModel->addItems(frames);
}

void GameCube::search()
{
    auto method = static_cast<Method>(ui->comboBoxSearcherMethod->getCurrentInt());
    searcherModel->clearModel();
    searcherModel->setMethod(method);

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    QVector<u8> min = ui->filterSearcher->getMinIVs();
    QVector<u8> max = ui->filterSearcher->getMaxIVs();

    FrameFilter filter(ui->filterSearcher->getGender(), ui->filterSearcher->getAbility(), ui->filterSearcher->getShiny(), false, min, max,
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
            maxProgress *= max.at(i) - min.at(i) + 1;
        }
    }
    else
    {
        maxProgress *= max.at(4) - min.at(4) + 1;
        maxProgress *= 0x7ffffff;
    }
    ui->progressBar->setRange(0, maxProgress);

    auto *thread = QThread::create([=] { searcher->startSearch(min, max); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(ui->pushButtonCancel, &QPushButton::clicked, [searcher] { searcher->cancelSearch(); });

    auto *timer = new QTimer();
    connect(timer, &QTimer::timeout, [=] { updateProgress(searcher->getResults(), searcher->getProgress()); });
    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed, [=] {
        ui->pushButtonSearch->setEnabled(true);
        ui->pushButtonCancel->setEnabled(false);
        updateProgress(searcher->getResults(), searcher->getProgress());
        delete searcher;
    });

    thread->start();
    timer->start(1000);
}

void GameCube::profilesIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = profiles.at(index);

        ui->labelProfileTIDValue->setText(QString::number(currentProfile.getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile.getSID()));
        ui->labelProfileGameValue->setText(currentProfile.getVersionString());
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
            QStringList s = Translator::getSpecies(
                { 334, 24,  354, 12,  113, 301, 85,  149, 51,  355, 125, 83,  55,  88,  58,  316, 316, 316, 107, 106, 97, 115, 131, 165,
                  108, 337, 219, 126, 82,  296, 310, 105, 303, 52,  122, 177, 299, 322, 46,  17,  204, 127, 62,  261, 57, 280, 78,  20,
                  315, 302, 373, 123, 273, 273, 273, 86,  285, 143, 361, 338, 21,  363, 363, 363, 167, 121, 220, 114, 49, 100, 37,  70 });

            s[15] += tr(" (Citadark)");
            s[16] += tr(" (Initial)");
            s[17] += tr(" (Phenac)");
            s[52] += tr(" (Citadark)");
            s[53] += tr(" (Initial)");
            s[54] += tr(" (Phenac)");
            s[61] += tr(" (Citadark)");
            s[62] += tr(" (Initial)");
            s[63] += tr(" (Phenac)");

            ui->comboBoxGeneratorShadow->addItems(s);
            ui->comboBoxGeneratorShadow->setVisible(true);
            ui->labelGeneratorShadow->setVisible(true);

            QVector<int> secondShadows = { 0, 2, 3, 4, 14, 20, 22, 26, 34, 41, 49, 50, 57, 71 };
            ui->comboBoxGeneratorType->setVisible(secondShadows.contains(ui->comboBoxGeneratorShadow->currentIndex()));
            ui->labelGeneratorType->setVisible(secondShadows.contains(ui->comboBoxGeneratorShadow->currentIndex()));
        }
        else if (method == Method::Colo)
        {
            QStringList s = Translator::getSpecies({ 207, 214, 296, 179, 198, 212, 175, 217 });
            s[3] += tr(" (E-Reader)");
            s[5] += tr(" (E-Reader)");
            s[6] += tr(" (E-Reader)");
            ui->comboBoxGeneratorShadow->addItems(s);
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
            QVector<int> secondShadows = { 0, 2, 3, 4, 14, 20, 22, 26, 34, 41, 49, 50, 57, 71 };
            ui->comboBoxGeneratorType->setVisible(secondShadows.contains(index));
            ui->labelGeneratorType->setVisible(secondShadows.contains(index));
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
            QStringList s = Translator::getSpecies(
                { 334, 24,  354, 12,  113, 301, 85,  149, 51,  355, 125, 83,  55,  88,  58,  316, 316, 316, 107, 106, 97, 115, 131, 165,
                  108, 337, 219, 126, 82,  296, 310, 105, 303, 52,  122, 177, 299, 322, 46,  17,  204, 127, 62,  261, 57, 280, 78,  20,
                  315, 302, 373, 123, 273, 273, 273, 86,  285, 143, 361, 338, 21,  363, 363, 363, 167, 121, 220, 114, 49, 100, 37,  70 });

            s[15] += tr(" (Citadark)");
            s[16] += tr(" (Initial)");
            s[17] += tr(" (Phenac)");
            s[52] += tr(" (Citadark)");
            s[53] += tr(" (Initial)");
            s[54] += tr(" (Phenac)");
            s[61] += tr(" (Citadark)");
            s[62] += tr(" (Initial)");
            s[63] += tr(" (Phenac)");

            ui->comboBoxSearcherShadow->addItems(s);
            ui->comboBoxSearcherShadow->setVisible(true);
            ui->labelSearcherShadow->setVisible(true);
        }
        else if (method == Method::Colo)
        {
            QStringList s = Translator::getSpecies({ 207, 214, 296, 179, 198, 212, 175, 217 });
            s[3] += tr(" (E-Reader)");
            s[5] += tr(" (E-Reader)");
            s[6] += tr(" (E-Reader)");
            ui->comboBoxSearcherShadow->addItems(s);
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

void GameCube::copySeedToClipboard()
{
    QModelIndex index = ui->tableViewSearcher->currentIndex();
    index = searcherModel->index(index.row(), 0);

    QApplication::clipboard()->setText(searcherModel->data(index).toString());
}

void GameCube::profileManager()
{
    auto *manager = new ProfileManager3();
    connect(manager, &ProfileManager3::updateProfiles, this, [=] { emit alertProfiles(3); });
    manager->show();
}
