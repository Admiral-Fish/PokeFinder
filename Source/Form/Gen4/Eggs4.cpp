/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "Eggs4.hpp"
#include "ui_Eggs4.h"
#include <Core/Enum/Game.hpp>
#include <Core/Gen4/Generators/EggGenerator4.hpp>
#include <Core/Gen4/Profile4.hpp>
#include <Core/Gen4/Searchers/EggSearcher4.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen4/Profile/ProfileManager4.hpp>
#include <Form/Gen4/Tools/SeedToTime4.hpp>
#include <Model/Gen4/EggModel4.hpp>
#include <QAction>
#include <QContextMenuEvent>
#include <QMenu>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include <QTimer>

Eggs4::Eggs4(QWidget *parent) : QWidget(parent), ui(new Ui::Eggs4)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    generatorModel = new EggGeneratorModel4(ui->tableViewGenerator, Game::DPPt);
    searcherModel = new EggSearcherModel4(ui->tableViewSearcher);

    ui->tableViewGenerator->setModel(generatorModel);
    ui->tableViewSearcher->setModel(searcherModel);

    ui->textBoxGeneratorSeedHeld->setValues(InputType::Seed32Bit);
    ui->textBoxGeneratorInitialAdvancesHeld->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvancesHeld->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorDelayHeld->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorSeedPickup->setValues(InputType::Seed32Bit);
    ui->textBoxGeneratorInitialAdvancesPickup->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvancesPickup->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorDelayPickup->setValues(InputType::Advance32Bit);

    ui->textBoxSearcherInitialAdvancesHeld->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMaxAdvancesHeld->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherInitialAdvancesPickup->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMaxAdvancesPickup->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMinDelay->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMaxDelay->setValues(InputType::Advance32Bit);

    ui->filterGenerator->disableControls(Controls::EncounterSlots);
    ui->filterSearcher->disableControls(Controls::EncounterSlots | Controls::DisableFilter);

    ui->eggSettingsGenerator->setup(Game::Gen4);
    ui->eggSettingsSearcher->setup(Game::Gen4);

    poketch = new QAction(tr("Calculate Poketch"), ui->tableViewGenerator);
    ui->tableViewGenerator->addAction(poketch);
    poketch->setVisible(false);
    connect(poketch, &QAction::triggered, this, &Eggs4::calcPoketch);

    auto *seedToTime = new QAction(tr("Generate times for seed"), ui->tableViewSearcher);
    connect(seedToTime, &QAction::triggered, this, &Eggs4::seedToTime);
    ui->tableViewSearcher->addAction(seedToTime);

    auto *transferSettings = new QAction(tr("Transfer Settings to Generator"), this);
    connect(transferSettings, &QAction::triggered, this, &Eggs4::transferSettingsToGenerator);
    addAction(transferSettings);

    auto *transferFilters = new QAction(tr("Transfer Filters to Generator"), this);
    connect(transferFilters, &QAction::triggered, this, &Eggs4::transferFiltersToGenerator);
    addAction(transferFilters);

    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &Eggs4::profileIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Eggs4::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Eggs4::search);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Eggs4::profileManager);
    connect(ui->eggSettingsGenerator, &EggSettings::showInheritanceChanged, generatorModel, &EggGeneratorModel4::setShowInheritance);
    connect(ui->eggSettingsSearcher, &EggSettings::showInheritanceChanged, searcherModel, &EggSearcherModel4::setShowInheritance);
    connect(ui->filterGenerator, &Filter::showStatsChanged, generatorModel, &EggGeneratorModel4::setShowStats);
    connect(ui->filterSearcher, &Filter::showStatsChanged, searcherModel, &EggSearcherModel4::setShowStats);

    updateProfiles();

    QSettings setting;
    setting.beginGroup("eggs4");
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

Eggs4::~Eggs4()
{
    QSettings setting;
    setting.beginGroup("eggs4");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

void Eggs4::updateProfiles()
{
    profiles = ProfileLoader4::getProfiles();
    profiles.insert(profiles.begin(), Profile4("None", Game::Diamond, 12345, 54321, false));

    ui->comboBoxProfiles->clear();
    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("eggs4/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void Eggs4::contextMenuEvent(QContextMenuEvent *event)
{
    if (ui->tabEggSelection->currentIndex() == 1)
    {
        QMenu::exec(actions(), event->globalPos(), nullptr, this);
    }
}

void Eggs4::calcPoketch()
{
    QModelIndex index = ui->tableViewGenerator->currentIndex();
    const auto &state = generatorModel->getItem(index.row());
    u32 advances = state.getAdvances();

    u32 coin = 0;
    u32 happy = 0;
    QString note;

    if (advances < 12)
    {
        coin = advances;
        note = tr("Do not switch to the happiness application at all");
    }
    else
    {
        u32 target = advances - 12;
        happy = target / 12;
        coin = target % 12;

        if (happy == 0)
        {
            note = tr("Switch to the happiness application once but do not click");
        }
    }

    QStringList info = { tr("Happiness Application Double Taps: %1").arg(happy), tr("Coin Flip Application Taps: %1").arg(coin) };
    if (!note.isEmpty())
    {
        info.emplace_back(note);
    }

    QMessageBox msg(QMessageBox::Information, tr("Poketch Taps"), info.join('\n'));
    msg.exec();
}

void Eggs4::generate()
{
    if (!ui->eggSettingsGenerator->compatibleParents())
    {
        QMessageBox box(QMessageBox::Warning, tr("Incompatible Parents"), tr("Gender of selected parents are not compatible for breeding"));
        box.exec();
        return;
    }

    generatorModel->clearModel();
    generatorModel->setVersion(currentProfile->getVersion());

    if ((currentProfile->getVersion() & Game::DPPt) != Game::None)
    {
        poketch->setVisible(true);
    }
    else
    {
        poketch->setVisible(false);
    }

    u32 seedHeld = ui->textBoxGeneratorSeedHeld->getUInt();
    u32 initialAdvancesHeld = ui->textBoxGeneratorInitialAdvancesHeld->getUInt();
    u32 maxAdvancesHeld = ui->textBoxGeneratorMaxAdvancesHeld->getUInt();
    u32 delayHeld = ui->textBoxGeneratorDelayHeld->getUInt();
    u32 seedPickup = ui->textBoxGeneratorSeedPickup->getUInt();
    u32 initialAdvancesPickup = ui->textBoxGeneratorInitialAdvancesPickup->getUInt();
    u32 maxAdvancesPickup = ui->textBoxGeneratorMaxAdvancesPickup->getUInt();
    u32 delayPickup = ui->textBoxGeneratorDelayPickup->getUInt();

    StateFilter filter = ui->filterGenerator->getFilter<StateFilter>();
    EggGenerator4 generator(initialAdvancesHeld, maxAdvancesHeld, delayHeld, initialAdvancesPickup, maxAdvancesPickup, delayPickup,
                            ui->eggSettingsGenerator->getDaycare(), *currentProfile, filter);

    auto states = generator.generate(seedHeld, seedPickup);
    generatorModel->addItems(states);
}

void Eggs4::search()
{
    if (!ui->eggSettingsSearcher->compatibleParents())
    {
        QMessageBox box(QMessageBox::Warning, tr("Incompatible Parents"), tr("Gender of selected parents are not compatible for breeding"));
        box.exec();
        return;
    }

    searcherModel->clearModel();

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u32 initialAdvancesHeld = ui->textBoxSearcherInitialAdvancesHeld->getUInt();
    u32 maxAdvancesHeld = ui->textBoxSearcherMaxAdvancesHeld->getUInt();
    u32 initialAdvancesPickup = ui->textBoxSearcherInitialAdvancesPickup->getUInt();
    u32 maxAdvancesPickup = ui->textBoxSearcherMaxAdvancesPickup->getUInt();
    u32 minDelay = ui->textBoxSearcherMinDelay->getUInt();
    u32 maxDelay = ui->textBoxSearcherMaxDelay->getUInt();

    StateFilter filter = ui->filterSearcher->getFilter<StateFilter>();
    EggGenerator4 generator(initialAdvancesHeld, maxAdvancesHeld, 0, initialAdvancesPickup, maxAdvancesPickup, 0,
                            ui->eggSettingsSearcher->getDaycare(), *currentProfile, filter);

    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(static_cast<int>(256 * 24 * (maxDelay - minDelay + 1)));

    auto *searcher = new EggSearcher4(minDelay, maxDelay, *currentProfile);

    auto *thread = QThread::create([=] { searcher->startSearch(generator); });
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

void Eggs4::profileIndexChanged(int index)
{
    if (index >= 0)
    {
        currentProfile = &profiles[index];

        ui->labelProfileTIDValue->setText(QString::number(currentProfile->getTID()));
        ui->labelProfileSIDValue->setText(QString::number(currentProfile->getSID()));
        ui->labelProfileGameValue->setText(QString::fromStdString(Translator::getGame(currentProfile->getVersion())));
    }
}

void Eggs4::profileManager()
{
    auto *manager = new ProfileManager4();
    connect(manager, &ProfileManager4::profilesModified, this, [=](int num) { emit profilesModified(num); });
    manager->show();
}

void Eggs4::seedToTime()
{
    QModelIndex index = ui->tableViewSearcher->currentIndex();
    const auto &state = searcherModel->getItem(index.row());

    auto *time = new SeedToTime4(state.getSeed(), currentProfile->getVersion());
    time->show();
}

void Eggs4::transferFiltersToGenerator()
{
    ui->filterGenerator->copyFrom(ui->filterSearcher);
}

void Eggs4::transferSettingsToGenerator()
{
    ui->eggSettingsGenerator->copyFrom(ui->eggSettingsSearcher);
}
