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

#include "Eggs5.hpp"
#include "ui_Eggs5.h"
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Method.hpp>
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
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include <QTimer>

Eggs5::Eggs5(QWidget *parent) : QWidget(parent), ui(new Ui::Eggs5)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    generatorModel = new EggGeneratorModel5(ui->tableViewGenerator);
    ui->tableViewGenerator->setModel(generatorModel);

    searcherModel = new EggSearcherModel5(ui->tableViewSearcher);
    ui->tableViewSearcher->setModel(searcherModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed64Bit);
    ui->textBoxGeneratorInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorDelay->setValues(InputType::Advance32Bit);

    ui->textBoxSearcherMaxAdvances->setValues(InputType::Advance32Bit);

    ui->filterGenerator->disableControls(Controls::EncounterSlots);
    ui->filterSearcher->disableControls(Controls::EncounterSlots | Controls::DisableFilter);

    ui->eggSettingsGenerator->setup(Game::Gen5);
    ui->eggSettingsSearcher->setup(Game::Gen5);

    ui->filterGenerator->enableHiddenAbility();
    ui->filterSearcher->enableHiddenAbility();

    connect(ui->comboBoxProfiles, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &Eggs5::profileIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Eggs5::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Eggs5::search);
    connect(ui->pushButtonCalculateInitialAdvances, &QPushButton::clicked, this, &Eggs5::calculateInitialAdvances);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Eggs5::profileManager);
    connect(ui->eggSettingsGenerator, &EggSettings::showInheritanceChanged, generatorModel, &EggGeneratorModel5::setShowInheritance);
    connect(ui->eggSettingsSearcher, &EggSettings::showInheritanceChanged, searcherModel, &EggSearcherModel5::setShowInheritance);
    connect(ui->filterGenerator, &Filter::showStatsChanged, generatorModel, &EggGeneratorModel5::setShowStats);
    connect(ui->filterSearcher, &Filter::showStatsChanged, searcherModel, &EggSearcherModel5::setShowStats);

    updateProfiles();

    QSettings setting;
    setting.beginGroup("egg5");
    if (setting.contains("geometry"))
    {
        this->restoreGeometry(setting.value("geometry").toByteArray());
    }
    setting.endGroup();
}

Eggs5::~Eggs5()
{
    QSettings setting;
    setting.beginGroup("egg5");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.endGroup();

    delete ui;
}

bool Eggs5::hasProfiles() const
{
    return !profiles.empty();
}

void Eggs5::updateProfiles()
{
    profiles = ProfileLoader5::getProfiles();

    ui->comboBoxProfiles->clear();
    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("egg5/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
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

    generatorModel->clearModel();

    u64 seed = ui->textBoxGeneratorSeed->getULong();
    u32 initialAdvances = ui->textBoxGeneratorInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();
    u16 tid = currentProfile->getTID();
    u16 sid = currentProfile->getSID();
    u32 offset = ui->textBoxGeneratorDelay->getUInt();

    StateFilter5 filter(ui->filterGenerator->getGender(), ui->filterGenerator->getAbility(), ui->filterGenerator->getShiny(),
                        ui->filterGenerator->getDisableFilters(), ui->filterGenerator->getMinIVs(), ui->filterGenerator->getMaxIVs(),
                        ui->filterGenerator->getNatures(), ui->filterGenerator->getHiddenPowers());

    EggGenerator5 generator(initialAdvances, maxAdvances, offset, tid, sid, currentProfile->getVersion(), currentProfile->getShinyCharm(),
                            ui->eggSettingsGenerator->getDaycareSettings(), filter);

    auto states = generator.generate(seed);
    generatorModel->addItems(states);
}

void Eggs5::search()
{
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

    searcherModel->clearModel();

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u32 maxAdvances = ui->textBoxSearcherMaxAdvances->getUInt();
    u16 tid = currentProfile->getTID();
    u16 sid = currentProfile->getSID();

    StateFilter5 filter(ui->filterSearcher->getGender(), ui->filterSearcher->getAbility(), ui->filterSearcher->getShiny(),
                        ui->filterSearcher->getDisableFilters(), ui->filterSearcher->getMinIVs(), ui->filterSearcher->getMaxIVs(),
                        ui->filterSearcher->getNatures(), ui->filterSearcher->getHiddenPowers());

    EggGenerator5 generator(0, maxAdvances, 0, tid, sid, currentProfile->getVersion(), currentProfile->getShinyCharm(),
                            ui->eggSettingsSearcher->getDaycareSettings(), filter);

    auto *searcher = new Searcher5<EggGenerator5, EggState5>(*currentProfile);

    Date start = ui->dateEditSearcherStartDate->getDate();
    Date end = ui->dateEditSearcherEndDate->getDate();

    int maxProgress = Keypresses::getKeyPresses(currentProfile->getKeypresses(), currentProfile->getSkipLR()).size();
    maxProgress *= start.daysTo(end) + 1;
    maxProgress *= (currentProfile->getTimer0Max() - currentProfile->getTimer0Min() + 1);
    ui->progressBar->setRange(0, maxProgress);

    QSettings settings;
    int threads = settings.value("settings/threads").toInt();

    auto *thread = QThread::create([=] { searcher->startSearch(generator, threads, start, end); });
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

void Eggs5::calculateInitialAdvances()
{
    Game version = currentProfile->getVersion();

    u8 initialAdvances;
    if ((version & Game::BW) != Game::None)
    {
        initialAdvances = Utilities5::initialAdvancesBW(ui->textBoxGeneratorSeed->getULong());
    }
    else
    {
        initialAdvances = Utilities5::initialAdvancesBW2(ui->textBoxGeneratorSeed->getULong(), currentProfile->getMemoryLink());
    }

    ui->textBoxGeneratorInitialAdvances->setText(QString::number(initialAdvances));
}

void Eggs5::profileIndexChanged(int index)
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
        ui->labelProfileGameValue->setText(QString::fromStdString(*Translator::getGame(currentProfile->getVersion())));
    }
}

void Eggs5::profileManager()
{
    auto *manager = new ProfileManager5();
    connect(manager, &ProfileManager5::profilesModified, this, [=](int num) { emit profilesModified(num); });
    manager->show();
}
