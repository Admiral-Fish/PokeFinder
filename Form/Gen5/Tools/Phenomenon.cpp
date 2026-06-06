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
#include <Core/Gen5/EncounterArea5.hpp>
#include <Core/Gen5/Encounters5.hpp>
#include <Core/Gen5/Generators/PhenomenonGenerator.hpp>
#include <Core/Gen5/Keypresses.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/Searchers/PhenomenonSearcher.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen5/Profile/ProfileManager5.hpp>
#include <Model/Gen5/PhenomenonModel.hpp>
#include <Model/SortFilterProxyModel.hpp>
#include <QMessageBox>
#include <QSettings>
#include <QTimer>

static const QString settingPrefix = QStringLiteral("phenomenon");

Phenomenon::Phenomenon(QWidget *parent) : QWidget(parent), ui(new Ui::Phenomenon)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setAttribute(Qt::WA_DeleteOnClose);

    ui->profileDisplay->setup(settingPrefix, Game::Gen5);

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

    connect(ui->profileDisplay, &ProfileDisplay5::profileChanged, this, &Phenomenon::profileChanged);
    connect(ui->profileDisplay, &ProfileDisplay5::profilesChanged, this, &Phenomenon::profilesChanged);
    connect(ui->tabRNGSelector, &TabWidget::transferFilters, this, &Phenomenon::transferFilters);
    connect(ui->tabRNGSelector, &TabWidget::transferSettings, this, &Phenomenon::transferSettings);
    connect(ui->comboBoxGeneratorLocation, &QComboBox::currentIndexChanged, this, &Phenomenon::generatorLocationIndexChanged);
    connect(ui->comboBoxSearcherLocation, &QComboBox::currentIndexChanged, this, &Phenomenon::searcherLocationIndexChanged);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Phenomenon::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Phenomenon::search);

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

Phenomenon::~Phenomenon()
{
    QSettings setting;
    setting.beginGroup(settingPrefix);
    setting.setValue("geometry", this->saveGeometry());
    setting.setValue("startDate", ui->dateEditSearcherStartDate->date());
    setting.setValue("endDate", ui->dateEditSearcherEndDate->date());
    setting.endGroup();

    delete ui;
}

bool Phenomenon::hasProfiles() const
{
    return ui->profileDisplay->hasProfiles();
}

void Phenomenon::updateProfiles()
{
    ui->profileDisplay->updateProfiles();
}

void Phenomenon::generate()
{
    generatorModel->clearModel();

    u64 seed = ui->textBoxGeneratorSeed->getULong();
    u32 initialAdvances = ui->textBoxGeneratorInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();
    u32 offset = ui->textBoxGeneratorOffset->getUInt();

    PhenomenonFilter filter(ui->checkListGeneratorItem->getCheckedData());
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

        bool bw = (currentProfile->getVersion() & Game::BW) != Game::None;

        auto items = area.getUniqueItems(bw);
        auto itemNames = area.getItemNames(bw);

        ui->checkListGeneratorItem->clear();
        ui->checkListGeneratorItem->addItems(itemNames, items);
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

    PhenomenonFilter filter(ui->checkListSearcherItem->getCheckedData());
    PhenomenonGenerator generator(initialAdvances, maxAdvances, 0, encounter[ui->comboBoxSearcherLocation->currentIndex()], *currentProfile,
                                  filter);
    auto *searcher = new PhenomenonSearcher(generator, *currentProfile);

    searcher->setMaxProgress(searcher->getMaxProgress(start, end));

    QSettings settings;
    int threads = settings.value("settings/threads").toInt();

    auto *timer = new QTimer(this);
    connect(ui->pushButtonCancel, &QPushButton::clicked, timer, [this, searcher] {
        searcher->cancelSearch();
        ui->pushButtonCancel->setEnabled(false);
    });
    connect(timer, &QTimer::timeout, this, [this, searcher, timer] {
        searcherModel->addItems(searcher->getResults());
        ui->progressBar->setValue(searcher->getProgress());

        if (!searcher->isSearching())
        {
            timer->stop();

            searcherModel->addItems(searcher->getResults());
            ui->progressBar->setValue(searcher->getProgress());

            ui->pushButtonSearch->setEnabled(true);
            ui->pushButtonCancel->setEnabled(false);

            delete searcher;
            timer->deleteLater();
        }
    });

    searcher->startSearch(threads, start, end);
    timer->start(1000);
}

void Phenomenon::searcherLocationIndexChanged(int index)
{
    if (index >= 0)
    {
        const auto &area = encounter[ui->comboBoxSearcherLocation->currentIndex()];

        bool bw = (currentProfile->getVersion() & Game::BW) != Game::None;

        std::vector<u16> items = area.getUniqueItems(bw);
        std::vector<std::string> itemNames = area.getItemNames(bw);

        ui->checkListSearcherItem->clear();
        ui->checkListSearcherItem->addItems(itemNames, items);
    }
}

void Phenomenon::profileChanged(const Profile5 &profile)
{
    currentProfile = &profile;

    EncounterSettings5 settings = { };
    encounter = Encounters5::getPhenomenonEncounters(currentProfile);

    std::vector<u16> locs;
    std::ranges::transform(encounter, std::back_inserter(locs), [](const EncounterArea5 &area) { return area.getLocation(); });
    auto locations = Translator::getLocations(locs, currentProfile->getVersion());

    u16 currentLocationGenerator = ui->comboBoxGeneratorLocation->getCurrentUShort();
    ui->comboBoxGeneratorLocation->clear();
    ui->comboBoxGeneratorLocation->addItems(locations, locs);
    ui->comboBoxGeneratorLocation->setCurrentIndexByData(currentLocationGenerator);

    u16 currentLocationSearcher = ui->comboBoxSearcherLocation->getCurrentUShort();
    ui->comboBoxSearcherLocation->clear();
    ui->comboBoxSearcherLocation->addItems(locations, locs);
    ui->comboBoxSearcherLocation->setCurrentIndexByData(currentLocationSearcher);
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
