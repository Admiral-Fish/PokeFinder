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

#include "Event4.hpp"
#include "ui_Event4.h"
#include <Core/Enum/Game.hpp>
#include <Core/Gen4/Generators/EventGenerator4.hpp>
#include <Core/Gen4/Profile4.hpp>
#include <Core/Gen4/Searchers/EventSearcher4.hpp>
#include <Core/Gen4/States/State4.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Parents/States/State.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen4/Profile/ProfileManager4.hpp>
#include <Form/Gen4/Tools/SeedToTime4.hpp>
#include <Model/Gen4/EventModel4.hpp>
#include <QAction>
#include <QSettings>
#include <QThread>
#include <QTimer>

Event4::Event4(QWidget *parent) : QWidget(parent), ui(new Ui::Event4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    generatorModel = new EventGeneratorModel4(ui->tableViewGenerator);
    ui->tableViewGenerator->setModel(generatorModel);

    searcherModel = new EventSearcherModel4(ui->tableViewSearcher);
    ui->tableViewSearcher->setModel(searcherModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed32Bit);
    ui->textBoxGeneratorInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorDelay->setValues(InputType::Advance32Bit);

    ui->textBoxSearcherMinDelay->setValues(InputType::Delay);
    ui->textBoxSearcherMaxDelay->setValues(InputType::Delay);
    ui->textBoxSearcherMinAdvance->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMaxAdvance->setValues(InputType::Advance32Bit);

    for (const std::string &nature : Translator::getNatures())
    {
        ui->comboBoxGeneratorNature->addItem(QString::fromStdString(nature));
        ui->comboBoxSearcherNature->addItem(QString::fromStdString(nature));
    }

    ui->filterGenerator->disableControls(Controls::EncounterSlots | Controls::Ability | Controls::Gender | Controls::Natures
                                         | Controls::Shiny | Controls::IgnoreInheritance);
    ui->filterSearcher->disableControls(Controls::EncounterSlots | Controls::Ability | Controls::Gender | Controls::Natures
                                        | Controls::Shiny | Controls::DisableFilter | Controls::IgnoreInheritance);

    ui->filterGenerator->enableHiddenAbility();
    ui->filterSearcher->enableHiddenAbility();

    std::vector<u16> species(493);
    std::iota(species.begin(), species.end(), 1);
    for (const std::string &specie : Translator::getSpecies(species))
    {
        ui->comboBoxGeneratorSpecies->addItem(QString::fromStdString(specie));
        ui->comboBoxSearcherSpecies->addItem(QString::fromStdString(specie));
    }

    auto *seedToTime = ui->tableViewSearcher->addAction(tr("Generate times for seed"));
    connect(seedToTime, &QAction::triggered, this, &Event4::seedToTime);

    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Event4::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Event4::search);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Event4::profileManager);
    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &Event4::profileIndexChanged);
    connect(ui->filterGenerator, &Filter::showStatsChanged, generatorModel, &EventGeneratorModel4::setShowStats);
    connect(ui->filterSearcher, &Filter::showStatsChanged, searcherModel, &EventSearcherModel4::setShowStats);

    updateProfiles();

    QSettings setting;
    setting.beginGroup("event4");
    if (setting.contains("minDelay"))
    {
        ui->textBoxSearcherMinDelay->setText(setting.value("minDelay").toString());
    }
    if (setting.contains("maxDelay"))
    {
        ui->textBoxSearcherMaxDelay->setText(setting.value("maxDelay").toString());
    }
    if (setting.contains("minAdvance"))
    {
        ui->textBoxSearcherMinAdvance->setText(setting.value("minAdvance").toString());
    }
    if (setting.contains("maxAdvance"))
    {
        ui->textBoxSearcherMaxAdvance->setText(setting.value("maxAdvance").toString());
    }
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

Event4::~Event4()
{
    QSettings setting;
    setting.beginGroup("event4");
    setting.setValue("minDelay", ui->textBoxSearcherMinDelay->text());
    setting.setValue("maxDelay", ui->textBoxSearcherMaxDelay->text());
    setting.setValue("minAdvance", ui->textBoxSearcherMinAdvance->text());
    setting.setValue("maxAdvance", ui->textBoxSearcherMaxAdvance->text());
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void Event4::updateProfiles()
{
    profiles = ProfileLoader4::getProfiles();
    profiles.insert(profiles.begin(), Profile4("None", Game::Diamond, 12345, 54321, false));

    ui->comboBoxProfiles->clear();
    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("event4/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void Event4::generate()
{
    generatorModel->clearModel();
    generatorModel->setGame(currentProfile->getVersion());

    u32 seed = ui->textBoxGeneratorSeed->getUInt();
    u32 initialAdvances = ui->textBoxGeneratorInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();
    u32 delay = ui->textBoxGeneratorDelay->getUInt();

    auto filter = ui->filterGenerator->getFilter<StateFilter, FILTER_STATE_FILTER>();
    EventGenerator4 generator(initialAdvances, maxAdvances, delay, ui->comboBoxGeneratorSpecies->currentIndex() + 1,
                              ui->comboBoxGeneratorNature->currentIndex(), ui->spinBoxGeneratorLevel->value(), *currentProfile, filter);

    auto states = generator.generate(seed);
    generatorModel->addItems(states);
}

void Event4::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = &profiles[index];
    }
}

void Event4::profileManager()
{
    auto *manager = new ProfileManager4();
    connect(manager, &ProfileManager4::profilesModified, this, [=](int num) { emit profilesModified(num); });
    manager->show();
}

void Event4::search()
{
    searcherModel->clearModel();

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    std::array<u8, 6> min = ui->filterSearcher->getMinIVs();
    std::array<u8, 6> max = ui->filterSearcher->getMaxIVs();

    u32 minDelay = ui->textBoxSearcherMinDelay->getUInt();
    u32 maxDelay = ui->textBoxSearcherMaxDelay->getUInt();
    u32 minAdvance = ui->textBoxSearcherMinAdvance->getUInt();
    u32 maxAdvance = ui->textBoxSearcherMaxAdvance->getUInt();

    auto filter = ui->filterSearcher->getFilter<StateFilter, FILTER_STATE_FILTER>();
    auto *searcher = new EventSearcher4(minAdvance, maxAdvance, minDelay, maxDelay, *currentProfile, filter);

    int maxProgress = 1;
    for (u8 i = 0; i < 6; i++)
    {
        maxProgress *= max[i] - min[i] + 1;
    }
    ui->progressBar->setRange(0, maxProgress);

    auto *thread = QThread::create([=] {
        searcher->startSearch(min, max, ui->comboBoxGeneratorSpecies->currentIndex() + 1, ui->comboBoxSearcherNature->currentIndex(),
                              ui->spinBoxSearcherLevel->value());
    });
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

void Event4::seedToTime()
{
    QModelIndex index = ui->tableViewSearcher->currentIndex();
    const auto &state = searcherModel->getItem(index.row());

    auto *time = new SeedToTime4(state.getSeed(), currentProfile->getVersion());
    time->show();
}
