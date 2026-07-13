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

#include "Eggs5.hpp"
#include "ui_Eggs5.h"
#include <Core/Enum/Game.hpp>
#include <Core/Gen5/Generators/EggGenerator5.hpp>
#include <Core/Gen5/Keypresses.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/Searchers/Searcher5.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Core/Util/Utilities.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen5/Profile/ProfileManager5.hpp>
#include <Model/Gen5/EggModel5.hpp>
#include <Model/SortFilterProxyModel.hpp>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include <QTimer>

static const QString settingPrefix = QStringLiteral("egg5");

Eggs5::Eggs5(QWidget *parent) : QWidget(parent), ui(new Ui::Eggs5)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    ui->profileDisplay->setup(settingPrefix, Game::Gen5);

    generatorModel = new EggGeneratorModel5(ui->tableViewGenerator);
    ui->tableViewGenerator->setModel(generatorModel);

    searcherModel = new EggSearcherModel5(ui->tableViewSearcher);
    proxyModel = new SortFilterProxyModel(ui->tableViewSearcher, searcherModel);
    ui->tableViewSearcher->setModel(proxyModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed64Bit);
    ui->textBoxGeneratorInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorOffset->setValues(InputType::Advance32Bit);

    ui->textBoxSearcherInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMaxAdvances->setValues(InputType::Advance32Bit);

    ui->filterGenerator->disableControls(Controls::EncounterSlots | Controls::Height | Controls::Level | Controls::Weight);
    ui->filterSearcher->disableControls(Controls::DisableFilter | Controls::EncounterSlots | Controls::Height | Controls::Level
                                        | Controls::Weight);

    ui->eggSettingsGenerator->setup(Game::Gen5);
    ui->eggSettingsSearcher->setup(Game::Gen5);

    ui->filterGenerator->enableHiddenAbility();
    ui->filterSearcher->enableHiddenAbility();

    connect(ui->profileDisplay, &ProfileDisplay5::profileChanged, this, &Eggs5::profileChanged);
    connect(ui->profileDisplay, &ProfileDisplay5::profilesChanged, this, &Eggs5::profilesChanged);
    connect(ui->tabRNGSelector, &TabWidget::transferFilters, this, &Eggs5::transferFilters);
    connect(ui->tabRNGSelector, &TabWidget::transferSettings, this, &Eggs5::transferSettings);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Eggs5::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Eggs5::search);
    connect(ui->eggSettingsGenerator, &EggSettings::showInheritanceChanged, generatorModel, &EggGeneratorModel5::setShowInheritance);
    connect(ui->eggSettingsSearcher, &EggSettings::showInheritanceChanged, searcherModel, &EggSearcherModel5::setShowInheritance);
    connect(ui->filterGenerator, &Filter::showStatsChanged, generatorModel, &EggGeneratorModel5::setShowStats);
    connect(ui->filterSearcher, &Filter::showStatsChanged, searcherModel, &EggSearcherModel5::setShowStats);

    updateProfiles();

    QSettings setting;
    setting.beginGroup(settingPrefix);
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    if (setting.contains("startDate"))
    {
        ui->dateEditSearcherStartDate->setDate(setting.value("startDate").toDate());
    }
    if (setting.contains("endDate"))
    {
        ui->dateEditSearcherEndDate->setDate(setting.value("endDate").toDate());
    }
    setting.endGroup();
}

Eggs5::~Eggs5()
{
    QSettings setting;
    setting.beginGroup(settingPrefix);
    setting.setValue("geometry", this->saveGeometry());
    setting.setValue("startDate", ui->dateEditSearcherStartDate->date());
    setting.setValue("endDate", ui->dateEditSearcherEndDate->date());
    setting.endGroup();

    delete ui;
}

bool Eggs5::hasProfiles() const
{
    return ui->profileDisplay->hasProfiles();
}

void Eggs5::updateProfiles()
{
    ui->profileDisplay->updateProfiles();
}

void Eggs5::generate()
{
    if (!ui->eggSettingsGenerator->compatibleParents())
    {
        QMessageBox box(QMessageBox::Warning, tr("Incompatible Parents"), tr("Gender of selected parents are not compatible for breeding"));
        box.exec();
        return;
    }

    if (ui->eggSettingsGenerator->reorderParents())
    {
        QMessageBox box(QMessageBox::Information, tr("Parents Reordered"), tr("Parent were swapped to match the game"));
        box.exec();
    }

    if (!ui->filterGenerator->isValid())
    {
        return;
    }

    generatorModel->clearModel();

    u64 seed = ui->textBoxGeneratorSeed->getULong();
    u32 initialAdvances = ui->textBoxGeneratorInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();
    u32 offset = ui->textBoxGeneratorOffset->getUInt();
    Daycare daycare = ui->eggSettingsGenerator->getDaycare();

    auto filter = ui->filterGenerator->getFilter<StateFilter>();
    EggGenerator5 generator(initialAdvances, maxAdvances, offset, daycare, *currentProfile, filter);

    auto states = generator.generate(seed);
    generatorModel->addItems(states);
}

void Eggs5::search()
{
    Date start = ui->dateEditSearcherStartDate->getDate();
    Date end = ui->dateEditSearcherEndDate->getDate();
    if (start > end)
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid date range"), tr("Start date is after end date"));
        msg.exec();
        return;
    }

    if (!ui->eggSettingsSearcher->compatibleParents())
    {
        QMessageBox box(QMessageBox::Warning, tr("Incompatible Parents"), tr("Gender of selected parents are not compatible for breeding"));
        box.exec();
        return;
    }

    if (ui->eggSettingsSearcher->reorderParents())
    {
        QMessageBox box(QMessageBox::Information, tr("Parents Reordered"), tr("Parent were swapped to match the game"));
        box.exec();
    }

    if (!ui->filterSearcher->isValid())
    {
        return;
    }

    searcherModel->clearModel();

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u32 initialAdvances = ui->textBoxSearcherInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxSearcherMaxAdvances->getUInt();
    Daycare daycare = ui->eggSettingsSearcher->getDaycare();

    auto filter = ui->filterSearcher->getFilter<StateFilter>();
    EggGenerator5 generator(initialAdvances, maxAdvances, 0, daycare, *currentProfile, filter);
    auto *searcher = new Searcher5<EggGenerator5, EggState5>(generator, *currentProfile);
    searcher->setMaxProgress(searcher->getMaxProgress(start, end));

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

void Eggs5::profileChanged(const Profile5 &profile)
{
    currentProfile = &profile;
}

void Eggs5::transferFilters(int index)
{
    if (index == 0)
    {
        ui->filterSearcher->copyFrom(ui->filterGenerator);
    }
    else
    {
        ui->filterGenerator->copyFrom(ui->filterSearcher);
    }
}

void Eggs5::transferSettings(int index)
{
    if (index == 0)
    {
        ui->eggSettingsSearcher->copyFrom(ui->eggSettingsGenerator);
    }
    else
    {
        ui->eggSettingsGenerator->copyFrom(ui->eggSettingsSearcher);
    }
}
