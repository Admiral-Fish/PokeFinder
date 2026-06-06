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

#include "Phenomenon.hpp"
#include "ui_Phenomenon.h"
#include <Core/Enum/Game.hpp>
#include <Core/Gen5/Encounters5.hpp>
#include <Core/Gen5/Generators/PhenomenonGenerator.hpp>
#include <Core/Gen5/Keypresses.hpp>
#include <Core/Gen5/PhenomenonArea.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/Searchers/Searcher5.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen5/Profile/ProfileManager5.hpp>
#include <Model/Gen5/PhenomenonModel.hpp>
#include <Model/SortFilterProxyModel.hpp>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QThread>
#include <QTimer>

Phenomenon::Phenomenon(QWidget *parent) : QWidget(parent), ui(new Ui::Phenomenon), encounter(Encounters5::getPhenomenonEncounters())
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    generatorModel = new PhenomenonGeneratorModel5(ui->tableViewGenerator);
    ui->tableViewGenerator->setModel(generatorModel);

    searcherModel = new PhenomenonSearcherModel5(ui->tableViewSearcher);
    proxyModel = new SortFilterProxyModel(ui->tableViewSearcher, searcherModel);
    ui->tableViewSearcher->setModel(proxyModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed64Bit);
    ui->textBoxGeneratorInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorOffset->setValues(InputType::Advance32Bit);

    ui->textBoxSearcherInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMaxAdvances->setValues(InputType::Advance32Bit);

    ui->comboBoxGeneratorLocation->enableAutoComplete();
    ui->comboBoxSearcherLocation->enableAutoComplete();

    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &Phenomenon::profileIndexChanged);
    connect(ui->tabRNGSelector, &TabWidget::transferFilters, this, &Phenomenon::transferFilters);
    connect(ui->tabRNGSelector, &TabWidget::transferSettings, this, &Phenomenon::transferSettings);
    connect(ui->comboBoxGeneratorLocation, &QComboBox::currentIndexChanged, this, &Phenomenon::generatorLocationIndexChanged);
    connect(ui->comboBoxSearcherLocation, &QComboBox::currentIndexChanged, this, &Phenomenon::searcherLocationIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Phenomenon::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Phenomenon::search);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Phenomenon::profileManager);

    std::vector<u16> locs;
    std::transform(encounter.begin(), encounter.end(), std::back_inserter(locs),
                   [](const PhenomenonArea &area) { return area.getLocation(); });
    auto locations = Translator::getLocations(locs, Game::BW2);

    ui->comboBoxGeneratorLocation->addItems(locations);
    ui->comboBoxSearcherLocation->addItems(locations);

    updateProfiles();

    QSettings setting;
    setting.beginGroup("phenomenon");
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

Phenomenon::~Phenomenon()
{
    QSettings setting;
    setting.beginGroup("phenomenon");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", this->saveGeometry());
    setting.setValue("startDate", ui->dateEditSearcherStartDate->date());
    setting.setValue("endDate", ui->dateEditSearcherEndDate->date());
    setting.endGroup();

    delete ui;
}

bool Phenomenon::hasProfiles() const
{
    return !profiles.empty();
}

void Phenomenon::updateProfiles()
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
    int val = setting.value("phenomenon/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void Phenomenon::generate()
{
    generatorModel->clearModel();

    u64 seed = ui->textBoxGeneratorSeed->getULong();
    u32 initialAdvances = ui->textBoxGeneratorInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();
    u32 offset = ui->textBoxGeneratorOffset->getUInt();

    PhenomenonFilter filter(ui->radioButtonGeneratorPokemon->isChecked(), ui->checkListGeneratorItem->getCheckedData());
    PhenomenonGenerator generator(initialAdvances, maxAdvances, offset, encounter[ui->comboBoxGeneratorLocation->currentIndex()],
                                  *currentProfile, filter);

    auto states = generator.generate(seed);
    generatorModel->addItems(states);
}

void Phenomenon::generatorLocationIndexChanged(int index)
{
    if (index >= 0)
    {
        auto &area = encounter[ui->comboBoxGeneratorLocation->currentIndex()];

        auto item = area.getUniqueItems();
        auto itemNames = area.getItemNames();

        ui->checkListGeneratorItem->clear();
        ui->checkListGeneratorItem->addItem("-");
        for (int i = 0; i < item.size(); i++)
        {
            ui->checkListGeneratorItem->addItem(itemNames[i], item[i]);
        }
    }
}

void Phenomenon::search()
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

    PhenomenonFilter filter(ui->radioButtonSearcherPokemon->isChecked(), ui->checkListSearcherItem->getCheckedData());
    PhenomenonGenerator generator(initialAdvances, maxAdvances, 0, encounter[ui->comboBoxSearcherLocation->currentIndex()], *currentProfile,
                                  filter);
    auto *searcher = new Searcher5<PhenomenonGenerator, PhenomenonState>(generator, *currentProfile);

    int maxProgress = Keypresses::getKeypresses(*currentProfile).size();
    maxProgress *= start.daysTo(end) + 1;
    maxProgress *= currentProfile->getTimer0Max() - currentProfile->getTimer0Min() + 1;
    searcher->setMaxProgress(maxProgress);

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

void Phenomenon::searcherLocationIndexChanged(int index)
{
    if (index >= 0)
    {
        const auto &area = encounter[ui->comboBoxSearcherLocation->currentIndex()];

        std::vector<u16> item = area.getUniqueItems();
        std::vector<std::string> itemNames = area.getItemNames();

        ui->checkListSearcherItem->clear();
        ui->checkListSearcherItem->addItem("-");
        for (int i = 0; i < item.size(); i++)
        {
            ui->checkListGeneratorItem->addItem(QString::fromStdString(itemNames[i]), item[i]);
        }
    }
}

void Phenomenon::profileIndexChanged(int index)
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

void Phenomenon::profileManager()
{
    auto *manager = new ProfileManager5();
    connect(manager, &ProfileManager5::profilesModified, this, [=](int num) { emit profilesModified(num); });
    manager->show();
}

void Phenomenon::transferFilters(int index)
{
    if (index == 0)
    {
        ui->checkListSearcherItem->setChecks(ui->checkListGeneratorItem->getChecked());
    }
    else
    {
        ui->checkListGeneratorItem->setChecks(ui->checkListSearcherItem->getChecked());
    }
}

void Phenomenon::transferSettings(int index)
{
    if (index == 0)
    {
        ui->comboBoxSearcherLocation->setCurrentIndex(ui->comboBoxGeneratorLocation->currentIndex());
    }
    else
    {
        ui->comboBoxGeneratorLocation->setCurrentIndex(ui->comboBoxSearcherLocation->currentIndex());
    }
}
