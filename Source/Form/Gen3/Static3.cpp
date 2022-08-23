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

#include "Static3.hpp"
#include "ui_Static3.h"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen3/Encounters3.hpp>
#include <Core/Gen3/Generators/StaticGenerator3.hpp>
#include <Core/Gen3/Profile3.hpp>
#include <Core/Gen3/Searchers/StaticSearcher3.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Parents/StaticTemplate.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen3/Profile/ProfileManager3.hpp>
#include <Model/Gen3/StaticModel3.hpp>
#include <QListView>
#include <QMenu>
#include <QSettings>
#include <QThread>
#include <QTimer>

Static3::Static3(QWidget *parent) : QWidget(parent), ui(new Ui::Static3)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    generatorModel = new StaticGeneratorModel3(ui->tableViewGenerator);
    searcherModel = new StaticSearcherModel3(ui->tableViewSearcher);

    ui->tableViewGenerator->setModel(generatorModel);
    ui->tableViewSearcher->setModel(searcherModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed32Bit);
    ui->textBoxGeneratorInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);

    ui->comboBoxGeneratorMethod->setup({ toInt(Method::Method1), toInt(Method::Method4) });
    ui->comboBoxSearcherMethod->setup({ toInt(Method::Method1), toInt(Method::Method4) });

    ui->filterGenerator->disableControls(Controls::EncounterSlots);
    ui->filterSearcher->disableControls(Controls::EncounterSlots | Controls::UseDelay | Controls::DisableFilter);

    auto *seedTime = new QAction(tr("Generate times for seed"), ui->tableViewSearcher);
    ui->tableViewSearcher->addAction(seedTime); // TODO: use convenience function when moving to Qt 6.3

    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Static3::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Static3::search);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Static3::profileManager);
    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Static3::profilesIndexChanged);
    connect(ui->comboBoxGeneratorCategory, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &Static3::generatorCategoryIndexChanged);
    connect(ui->comboBoxGeneratorPokemon, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &Static3::generatorPokemonIndexChanged);
    connect(ui->comboBoxSearcherCategory, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
            &Static3::searcherCategoryIndexChanged);
    connect(ui->comboBoxSearcherPokemon, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Static3::searcherPokemonIndexChanged);
    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Static3::profilesIndexChanged);
    connect(ui->filterGenerator, &Filter::showStatsChanged, generatorModel, &StaticGeneratorModel3::setShowStats);
    connect(ui->filterSearcher, &Filter::showStatsChanged, searcherModel, &StaticSearcherModel3::setShowStats);

    updateProfiles();
    generatorCategoryIndexChanged(0);
    searcherCategoryIndexChanged(0);

    QSettings setting;
    if (setting.contains("static3/geometry"))
    {
        this->restoreGeometry(setting.value("static3/geometry").toByteArray());
    }
}

Static3::~Static3()
{
    QSettings setting;
    setting.beginGroup("static3");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void Static3::updateProfiles()
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
    int val = setting.value("static3/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void Static3::generate()
{
    generatorModel->clearModel();

    u32 seed = ui->textBoxGeneratorSeed->getUInt();
    u32 initialAdvances = ui->textBoxGeneratorInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();
    u16 tid = currentProfile->getTID();
    u16 sid = currentProfile->getSID();
    auto method = ui->comboBoxGeneratorMethod->getEnum<Method>();
    u32 offset = 0;
    if (ui->filterGenerator->useDelay())
    {
        offset = ui->filterGenerator->getDelay();
    }

    StateFilter3 filter(ui->filterGenerator->getGender(), ui->filterGenerator->getAbility(), ui->filterGenerator->getShiny(),
                        ui->filterGenerator->getDisableFilters(), ui->filterGenerator->getMinIVs(), ui->filterGenerator->getMaxIVs(),
                        ui->filterGenerator->getNatures(), ui->filterGenerator->getHiddenPowers());

    StaticGenerator3 generator(initialAdvances, maxAdvances, offset, tid, sid, currentProfile->getVersion(), method, filter);

    const StaticTemplate *staticTemplate
        = Encounters3::getStaticEncounter(ui->comboBoxGeneratorCategory->currentIndex(), ui->comboBoxGeneratorPokemon->getCurrentInt());

    auto states = generator.generate(seed, staticTemplate);
    generatorModel->addItems(states);
}

void Static3::generatorCategoryIndexChanged(int index)
{
    if (index >= 0)
    {
        size_t size;
        const StaticTemplate *templates = Encounters3::getStaticEncounters(index, &size);

        ui->comboBoxGeneratorPokemon->clear();
        for (size_t i = 0; i < size; i++)
        {
            if ((currentProfile->getVersion() & templates[i].getVersion()) != Game::None)
            {
                ui->comboBoxGeneratorPokemon->addItem(QString::fromStdString(*Translator::getSpecie(templates[i].getSpecie())), i);
            }
        }
    }
}

void Static3::generatorPokemonIndexChanged(int index)
{
    if (index >= 0)
    {
        const StaticTemplate *staticTemplate
            = Encounters3::getStaticEncounter(ui->comboBoxGeneratorCategory->currentIndex(), ui->comboBoxGeneratorPokemon->getCurrentInt());
        ui->spinBoxGeneratorLevel->setValue(staticTemplate->getLevel());
    }
}

void Static3::profileManager()
{
    auto *manager = new ProfileManager3();
    connect(manager, &ProfileManager3::updateProfiles, this, [=] { emit alertProfiles(3); });
    manager->show();
}

void Static3::profilesIndexChanged(int index)
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

        ui->comboBoxGeneratorCategory->setCurrentIndex(0);
        ui->comboBoxSearcherCategory->setCurrentIndex(0);

        bool flag = (currentProfile->getVersion() & Game::RS) != Game::None;
        auto *generatorView = qobject_cast<QListView *>(ui->comboBoxGeneratorCategory->view());
        auto *searcherView = qobject_cast<QListView *>(ui->comboBoxSearcherCategory->view());
        generatorView->setRowHidden(3, flag);
        searcherView->setRowHidden(3, flag);
    }
}

void Static3::search()
{
    searcherModel->clearModel();

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    std::array<u8, 6> min = ui->filterSearcher->getMinIVs();
    std::array<u8, 6> max = ui->filterSearcher->getMaxIVs();

    StateFilter3 filter(ui->filterSearcher->getGender(), ui->filterSearcher->getAbility(), ui->filterSearcher->getShiny(), false, min, max,
                        ui->filterSearcher->getNatures(), ui->filterSearcher->getHiddenPowers());

    u16 tid = currentProfile->getTID();
    u16 sid = currentProfile->getSID();
    auto method = ui->comboBoxSearcherMethod->getEnum<Method>();
    const StaticTemplate *staticTemplate
        = Encounters3::getStaticEncounter(ui->comboBoxSearcherCategory->currentIndex(), ui->comboBoxSearcherPokemon->getCurrentInt());

    auto *searcher = new StaticSearcher3(tid, sid, currentProfile->getVersion(), method, filter);

    int maxProgress = 1;
    for (u8 i = 0; i < 6; i++)
    {
        maxProgress *= max[i] - min[i] + 1;
    }
    ui->progressBar->setRange(0, maxProgress);

    auto *thread = QThread::create([=] { searcher->startSearch(min, max, staticTemplate); });
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

void Static3::searcherCategoryIndexChanged(int index)
{
    if (index >= 0)
    {
        size_t size;
        const StaticTemplate *templates = Encounters3::getStaticEncounters(index, &size);

        ui->comboBoxSearcherPokemon->clear();
        for (size_t i = 0; i < size; i++)
        {
            if ((currentProfile->getVersion() & templates[i].getVersion()) != Game::None)
            {
                ui->comboBoxSearcherPokemon->addItem(QString::fromStdString(*Translator::getSpecie(templates[i].getSpecie())), i);
            }
        }
    }
}

void Static3::searcherPokemonIndexChanged(int index)
{
    if (index >= 0)
    {
        const StaticTemplate *staticTemplate
            = Encounters3::getStaticEncounter(ui->comboBoxSearcherCategory->currentIndex(), ui->comboBoxSearcherPokemon->getCurrentInt());
        ui->spinBoxSearcherLevel->setValue(staticTemplate->getLevel());
    }
}
