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

#include "Pickup.hpp"
#include "ui_Pickup.h"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/Encounters5.hpp>
#include <Core/Gen5/Searchers/Searcher5.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/ComboBox.hpp>
#include <Form/Controls/ComboBoxProxy.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Controls/Filter.hpp>
#include <Form/Controls/TextBox.hpp>
#include <Model/Gen5/PickupModel.hpp>
#include <Model/SortFilterProxyModel.hpp>
#include <QCheckBox>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QThread>
#include <QTimer>
#include <algorithm>
#include <map>
#include <set>

static const QString settingPrefix = QStringLiteral("pickup");

static bool canSlotGenerateItem(u8 level, u16 item)
{
    auto items = PickupGenerator::getLevelItems(level);
    return std::ranges::find(items, item) != items.end();
}

static bool canSatisfyPickupRequests(const std::array<QCheckBox *, 6> &pickupSlots, const std::array<QSpinBox *, 6> &levels,
                                     const std::vector<PickupGenerator::Request> &requests)
{
    std::vector<u16> needed;
    for (const auto &request : requests)
    {
        needed.insert(needed.end(), request.count, request.item);
    }

    if (needed.empty())
    {
        return true;
    }

    std::ranges::sort(needed, [&](u16 left, u16 right) {
        auto countSlots = [&](u16 item) {
            u8 count = 0;
            for (int slot = 0; slot < 6; slot++)
            {
                if (pickupSlots[slot]->isChecked() && canSlotGenerateItem(static_cast<u8>(levels[slot]->value()), item))
                {
                    count++;
                }
            }
            return count;
        };
        return countSlots(left) < countSlots(right);
    });

    std::array<bool, 6> used = {};
    auto assign = [&](auto &&self, size_t index) -> bool {
        if (index == needed.size())
        {
            return true;
        }

        for (int slot = 0; slot < 6; slot++)
        {
            if (used[slot] || !pickupSlots[slot]->isChecked()
                || !canSlotGenerateItem(static_cast<u8>(levels[slot]->value()), needed[index]))
            {
                continue;
            }

            used[slot] = true;
            if (self(self, index + 1))
            {
                return true;
            }
            used[slot] = false;
        }

        return false;
    };

    return assign(assign, 0);
}

Pickup::Pickup(QWidget *parent) : QWidget(parent), ui(new Ui::Pickup)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    ui->profileDisplay->setup(settingPrefix, Game::Gen5);

    setupPickupUi();

    generatorModel = new PickupGeneratorModel5(ui->tableViewGenerator);
    ui->tableViewGenerator->setModel(generatorModel);
    connect(ui->filterGenerator, &Filter::showStatsChanged, generatorModel, &PickupGeneratorModel5::setShowStats);

    searcherModel = new PickupSearcherModel5(ui->tableViewSearcher);
    proxyModel = new SortFilterProxyModel(ui->tableViewSearcher, searcherModel);
    ui->tableViewSearcher->setModel(proxyModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed64Bit);
    ui->textBoxGeneratorIVAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);

    ui->textBoxSearcherInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMaxAdvances->setValues(InputType::Advance32Bit);

    connect(ui->profileDisplay, &ProfileDisplay5::profileChanged, this, &Pickup::profileChanged);
    connect(ui->profileDisplay, &ProfileDisplay5::profilesChanged, this, &Pickup::profilesChanged);
    connect(ui->tabRNGSelector, &TabWidget::transferSettings, this, &Pickup::transferSettings);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Pickup::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Pickup::search);

    updateProfiles();

    QSettings setting;
    setting.beginGroup(settingPrefix);
    if (setting.contains("geometry"))
    {
        restoreGeometry(setting.value("geometry").toByteArray());
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

Pickup::~Pickup()
{
    QSettings setting;
    setting.beginGroup(settingPrefix);
    setting.setValue("geometry", saveGeometry());
    setting.setValue("startDate", ui->dateEditSearcherStartDate->date());
    setting.setValue("endDate", ui->dateEditSearcherEndDate->date());
    setting.endGroup();

    delete ui;
}

bool Pickup::hasProfiles() const
{
    return ui->profileDisplay->hasProfiles();
}

void Pickup::updateProfiles()
{
    ui->profileDisplay->updateProfiles();
}

void Pickup::setupPickupUi()
{
    ui->filterGenerator->disableControls(Controls::Height | Controls::Weight);

    ui->gridLayout_2->setColumnStretch(2, 2);
    ui->gridLayout_19->setColumnStretch(0, 1);
    ui->gridLayout_19->setColumnStretch(2, 3);
    ui->gridLayout_20->setColumnStretch(0, 1);
    ui->gridLayout_20->setColumnStretch(2, 2);
    ui->gridLayout_24->setColumnStretch(0, 1);

    ui->comboBoxGeneratorEncounter->setup({ toInt(Encounter::Grass), toInt(Encounter::GrassDark), toInt(Encounter::Surfing) });
    ui->comboBoxSearcherEncounter->setup({ toInt(Encounter::Grass), toInt(Encounter::GrassDark), toInt(Encounter::Surfing) });
    ui->comboBoxGeneratorLocation->enableAutoComplete();
    ui->comboBoxSearcherLocation->enableAutoComplete();

    generatorSlots = { ui->checkBoxGeneratorSlot1, ui->checkBoxGeneratorSlot2, ui->checkBoxGeneratorSlot3, ui->checkBoxGeneratorSlot4,
                       ui->checkBoxGeneratorSlot5, ui->checkBoxGeneratorSlot6 };
    searcherSlots = { ui->checkBoxSearcherSlot1, ui->checkBoxSearcherSlot2, ui->checkBoxSearcherSlot3, ui->checkBoxSearcherSlot4,
                       ui->checkBoxSearcherSlot5, ui->checkBoxSearcherSlot6 };
    generatorLevels = { ui->spinBoxGeneratorLevel1, ui->spinBoxGeneratorLevel2, ui->spinBoxGeneratorLevel3, ui->spinBoxGeneratorLevel4,
                        ui->spinBoxGeneratorLevel5, ui->spinBoxGeneratorLevel6 };
    searcherLevels = { ui->spinBoxSearcherLevel1, ui->spinBoxSearcherLevel2, ui->spinBoxSearcherLevel3, ui->spinBoxSearcherLevel4,
                        ui->spinBoxSearcherLevel5, ui->spinBoxSearcherLevel6 };
    generatorItems = { ui->comboBoxGeneratorItem1, ui->comboBoxGeneratorItem2, ui->comboBoxGeneratorItem3, ui->comboBoxGeneratorItem4,
                       ui->comboBoxGeneratorItem5, ui->comboBoxGeneratorItem6 };
    searcherItems = { ui->comboBoxSearcherItem1, ui->comboBoxSearcherItem2, ui->comboBoxSearcherItem3, ui->comboBoxSearcherItem4,
                       ui->comboBoxSearcherItem5, ui->comboBoxSearcherItem6 };
    searcherItemCounts = { ui->comboBoxSearcherItemCount1, ui->comboBoxSearcherItemCount2, ui->comboBoxSearcherItemCount3,
                           ui->comboBoxSearcherItemCount4, ui->comboBoxSearcherItemCount5, ui->comboBoxSearcherItemCount6 };
    searcherItemClearButtons = { ui->pushButtonSearcherClearItem1, ui->pushButtonSearcherClearItem2, ui->pushButtonSearcherClearItem3,
                                 ui->pushButtonSearcherClearItem4, ui->pushButtonSearcherClearItem5, ui->pushButtonSearcherClearItem6 };

    for (int i = 0; i < 6; i++)
    {
        connect(generatorSlots[i], &QCheckBox::toggled, this,
                [=] { updateItemList(generatorSlots[i], generatorLevels[i], generatorItems[i]); });
        connect(generatorLevels[i], qOverload<int>(&QSpinBox::valueChanged), this,
                [=] { updateItemList(generatorSlots[i], generatorLevels[i], generatorItems[i]); });
        updateItemList(generatorSlots[i], generatorLevels[i], generatorItems[i]);

        connect(searcherSlots[i], &QCheckBox::toggled, this, &Pickup::updateSearcherItemLists);
        connect(searcherLevels[i], qOverload<int>(&QSpinBox::valueChanged), this, &Pickup::updateSearcherItemLists);
        connect(searcherItems[i], qOverload<int>(&QComboBox::currentIndexChanged), this, &Pickup::updateSearcherItemLists);
        connect(searcherItemCounts[i], qOverload<int>(&QComboBox::currentIndexChanged), this, &Pickup::updateSearcherItemLists);
        connect(searcherItemClearButtons[i], &QPushButton::clicked, this, [=] {
            searcherItems[i]->setCurrentIndex(0);
            searcherItemCounts[i]->setCurrentIndex(0);
            updateSearcherItemLists();
        });
    }

    connect(ui->pushButtonGeneratorClearItems, &QPushButton::clicked, this, [=] {
        for (auto *item : generatorItems)
        {
            item->setCurrentIndex(0);
        }
    });

    updateSearcherItemLists();

    connect(ui->comboBoxGeneratorEncounter, &QComboBox::currentIndexChanged, this, &Pickup::generatorEncounterIndexChanged);
    connect(ui->comboBoxGeneratorSeason, &QComboBox::currentIndexChanged, this, &Pickup::generatorSeasonIndexChanged);
    connect(ui->comboBoxGeneratorLocation, &QComboBox::currentIndexChanged, this, &Pickup::generatorLocationIndexChanged);
    connect(ui->comboBoxGeneratorPokemon, &QComboBox::currentIndexChanged, this, &Pickup::generatorPokemonIndexChanged);
    connect(ui->comboBoxSearcherEncounter, &QComboBox::currentIndexChanged, this, &Pickup::searcherEncounterIndexChanged);
    connect(ui->comboBoxSearcherSeason, &QComboBox::currentIndexChanged, this, &Pickup::searcherSeasonIndexChanged);
}

void Pickup::updateEncounterLocations(ComboBox *encounter, ComboBox *season, ComboBoxProxy *location, std::vector<EncounterArea5> &areas)
{
    if (currentProfile == nullptr || encounter->currentIndex() < 0)
    {
        return;
    }

    u16 currentLocation = location->getCurrentUShort();
    areas = Encounters5::getEncounters(encounter->getEnum<Encounter>(), season->currentIndex(), currentProfile);

    std::vector<u16> locs;
    std::ranges::transform(areas, std::back_inserter(locs), [](const EncounterArea5 &area) { return area.getLocation(); });

    location->clear();
    location->addItems(Translator::getLocations(locs, currentProfile->getVersion()), locs);
    location->setCurrentIndexByData(currentLocation);
}

std::array<PickupGenerator::Slot, 6> Pickup::getSettings(const std::array<QCheckBox *, 6> &pickupSlots,
                                                         const std::array<QSpinBox *, 6> &levels,
                                                         const std::array<ComboBox *, 6> &items) const
{
    std::array<PickupGenerator::Slot, 6> settings;
    for (int i = 0; i < 6; i++)
    {
        settings[i].active = pickupSlots[i]->isChecked();
        settings[i].level = static_cast<u8>(levels[i]->value());
        if (settings[i].active && items[i]->currentIndex() > 0)
        {
            settings[i].filter = { items[i]->getCurrentUShort() };
        }
        else
        {
            settings[i].filter = {};
        }
    }
    return settings;
}

std::array<PickupGenerator::Slot, 6> Pickup::getSearcherSettings() const
{
    std::array<PickupGenerator::Slot, 6> settings;
    for (int i = 0; i < 6; i++)
    {
        settings[i].active = searcherSlots[i]->isChecked();
        settings[i].level = static_cast<u8>(searcherLevels[i]->value());
        settings[i].filter = {};
    }
    return settings;
}

std::vector<PickupGenerator::Request> Pickup::getSearcherRequests() const
{
    std::map<u16, u8> requests;
    for (int i = 0; i < 6; i++)
    {
        if (!searcherSlots[i]->isChecked() || searcherItems[i]->currentIndex() <= 0)
        {
            continue;
        }

        u16 item = searcherItems[i]->getCurrentUShort();
        u8 count = searcherItemCounts[i]->getCurrentUChar();
        requests[item] += count;
    }

    std::vector<PickupGenerator::Request> result;
    result.reserve(requests.size());
    for (const auto &[item, count] : requests)
    {
        result.push_back({ item, count });
    }
    return result;
}

void Pickup::updateItemList(QCheckBox *slot, QSpinBox *level, ComboBox *items) const
{
    bool active = slot->isChecked();
    level->setEnabled(active);
    items->setEnabled(active);

    u16 currentItem = items->currentIndex() > 0 ? items->getCurrentUShort() : 0xffff;
    std::vector<u16> itemIds = PickupGenerator::getLevelItems(static_cast<u8>(level->value()));

    items->clear();
    items->addItem(tr("Any"), 0xffff);
    items->addItem(QString::fromStdString(Translator::getItem(0)), 0);
    for (u16 item : itemIds)
    {
        items->addItem(QString::fromStdString(Translator::getItem(item)), item);
    }
    items->setMaxVisibleItems(static_cast<int>(std::max<size_t>(itemIds.size() + 2, 1)));
    int index = items->findData(currentItem);
    items->setCurrentIndex(index >= 0 ? index : 0);
}

void Pickup::updateSearcherItemLists()
{
    std::set<u16> itemSet;
    for (int i = 0; i < 6; i++)
    {
        searcherLevels[i]->setEnabled(searcherSlots[i]->isChecked());
        if (!searcherSlots[i]->isChecked())
        {
            continue;
        }

        for (u16 item : PickupGenerator::getLevelItems(static_cast<u8>(searcherLevels[i]->value())))
        {
            itemSet.insert(item);
        }
    }

    std::vector<std::pair<QString, u16>> itemOptions;
    itemOptions.reserve(itemSet.size());
    for (u16 item : itemSet)
    {
        itemOptions.emplace_back(QString::fromStdString(Translator::getItem(item)), item);
    }
    std::ranges::sort(itemOptions, [](const auto &left, const auto &right) { return left.first < right.first; });

    auto getRequests = [&](int skipRow, u16 rowItem = 0, u8 rowCount = 0) {
        std::map<u16, u8> requests;
        for (int row = 0; row < 6; row++)
        {
            if (row == skipRow || !searcherSlots[row]->isChecked() || searcherItems[row]->currentIndex() <= 0)
            {
                continue;
            }

            requests[searcherItems[row]->getCurrentUShort()] += searcherItemCounts[row]->getCurrentUChar();
        }

        if (rowItem != 0 && rowCount != 0)
        {
            requests[rowItem] += rowCount;
        }

        std::vector<PickupGenerator::Request> result;
        result.reserve(requests.size());
        for (const auto &[item, count] : requests)
        {
            result.push_back({ item, count });
        }
        return result;
    };

    auto selectedInOtherRow = [&](int skipRow, u16 item) {
        for (int row = 0; row < 6; row++)
        {
            if (row != skipRow && searcherSlots[row]->isChecked() && searcherItems[row]->currentIndex() > 0
                && searcherItems[row]->getCurrentUShort() == item)
            {
                return true;
            }
        }
        return false;
    };

    for (int row = 0; row < 6; row++)
    {
        u16 currentItem = searcherItems[row]->currentIndex() >= 0 ? searcherItems[row]->getCurrentUShort() : 0;
        u8 currentCount = searcherItemCounts[row]->currentIndex() >= 0 ? searcherItemCounts[row]->getCurrentUChar() : 1;

        {
            QSignalBlocker blocker(searcherItems[row]);
            searcherItems[row]->clear();
            searcherItems[row]->addItem("-", 0);
            for (const auto &[name, item] : itemOptions)
            {
                if (selectedInOtherRow(row, item))
                {
                    continue;
                }

                u8 testCount = item == currentItem ? currentCount : 1;
                if (canSatisfyPickupRequests(searcherSlots, searcherLevels, getRequests(row, item, testCount)))
                {
                    searcherItems[row]->addItem(name, item);
                }
            }

            int index = searcherItems[row]->findData(currentItem);
            searcherItems[row]->setCurrentIndex(index >= 0 ? index : 0);
        }

        currentItem = searcherItems[row]->getCurrentUShort();
        u8 maxCount = 1;
        if (currentItem != 0)
        {
            maxCount = 0;
            for (u8 count = 1; count <= 6; count++)
            {
                if (canSatisfyPickupRequests(searcherSlots, searcherLevels, getRequests(row, currentItem, count)))
                {
                    maxCount = count;
                }
            }
        }

        {
            QSignalBlocker blocker(searcherItemCounts[row]);
            searcherItemCounts[row]->clear();
            for (u8 count = 1; count <= std::max<u8>(maxCount, 1); count++)
            {
                searcherItemCounts[row]->addItem(QString::number(count), count);
            }
            searcherItemCounts[row]->setCurrentIndex(std::min<int>(currentCount, std::max<u8>(maxCount, 1)) - 1);
        }

        bool enabled = searcherSlots[row]->isChecked() && !itemOptions.empty();
        searcherItems[row]->setEnabled(enabled);
        searcherItemCounts[row]->setEnabled(enabled && currentItem != 0);
        searcherItemClearButtons[row]->setEnabled(enabled);
    }
}

void Pickup::generate()
{
    auto settings = getSettings(generatorSlots, generatorLevels, generatorItems);
    bool disableFilters = ui->filterGenerator->getDisableFilters();
    if (disableFilters)
    {
        for (auto &slot : settings)
        {
            slot.filter.clear();
        }
    }

    if (std::ranges::none_of(settings, [](const auto &slot) { return slot.active; }))
    {
        QMessageBox msg(QMessageBox::Warning, tr("Missing settings"), tr("Select at least 1 Pickup Slot"));
        msg.exec();
        return;
    }
    if (!ui->filterGenerator->isValid())
    {
        return;
    }

    generatorModel->clearModel();
    if (encounterGenerator.empty() || ui->comboBoxGeneratorLocation->currentIndex() < 0)
    {
        return;
    }

    WildStateFilter filter = ui->filterGenerator->getFilter<WildStateFilter, true>();
    PickupGenerator generator(ui->textBoxGeneratorInitialAdvances->getUInt(), ui->textBoxGeneratorMaxAdvances->getUInt(), settings,
                              encounterGenerator[ui->comboBoxGeneratorLocation->currentIndex()], *currentProfile, filter, true);
    generatorModel->addItems(generator.generate(ui->textBoxGeneratorSeed->getULong(), ui->textBoxGeneratorIVAdvances->getUInt()));
}

void Pickup::search()
{
    Date start = ui->dateEditSearcherStartDate->getDate();
    Date end = ui->dateEditSearcherEndDate->getDate();
    if (start > end)
    {
        QMessageBox msg(QMessageBox::Warning, tr("Invalid date range"), tr("Start date is after end date"));
        msg.exec();
        return;
    }

    auto settings = getSearcherSettings();
    if (std::ranges::none_of(settings, [](const auto &slot) { return slot.active; }))
    {
        QMessageBox msg(QMessageBox::Warning, tr("Missing settings"), tr("Select at least 1 Pickup Slot"));
        msg.exec();
        return;
    }
    searcherModel->clearModel();
    if (encounterSearcher.empty() || ui->comboBoxSearcherLocation->currentIndex() < 0)
    {
        return;
    }

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    PickupGenerator generator(ui->textBoxSearcherInitialAdvances->getUInt(), ui->textBoxSearcherMaxAdvances->getUInt(), settings,
                              getSearcherRequests(), encounterSearcher[ui->comboBoxSearcherLocation->currentIndex()], *currentProfile);
    auto *searcher = new Searcher5<PickupGenerator, PickupState>(generator, *currentProfile);
    searcher->setMaxProgress(searcher->getMaxProgress(start, end));

    QSettings qsettings;
    int threads = qsettings.value("settings/threads").toInt();

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

void Pickup::profileChanged(const Profile5 &profile)
{
    currentProfile = &profile;

    generatorEncounterIndexChanged(0);
    searcherEncounterIndexChanged(0);
}

void Pickup::transferSettings(int index)
{
    auto copySlots = [](const std::array<QCheckBox *, 6> &fromSlots, const std::array<QSpinBox *, 6> &fromLevels,
                        const std::array<QCheckBox *, 6> &toSlots, const std::array<QSpinBox *, 6> &toLevels) {
        for (int i = 0; i < 6; i++)
        {
            toSlots[i]->setChecked(fromSlots[i]->isChecked());
            toLevels[i]->setValue(fromLevels[i]->value());
        }
    };

    if (index == 0)
    {
        copySlots(generatorSlots, generatorLevels, searcherSlots, searcherLevels);
        updateSearcherItemLists();
        ui->comboBoxSearcherEncounter->setCurrentIndex(ui->comboBoxGeneratorEncounter->currentIndex());
        ui->comboBoxSearcherSeason->setCurrentIndex(ui->comboBoxGeneratorSeason->currentIndex());
        ui->comboBoxSearcherLocation->setCurrentIndexByData(ui->comboBoxGeneratorLocation->getCurrentUShort());
    }
    else
    {
        copySlots(searcherSlots, searcherLevels, generatorSlots, generatorLevels);
        ui->comboBoxGeneratorEncounter->setCurrentIndex(ui->comboBoxSearcherEncounter->currentIndex());
        ui->comboBoxGeneratorSeason->setCurrentIndex(ui->comboBoxSearcherSeason->currentIndex());
        ui->comboBoxGeneratorLocation->setCurrentIndexByData(ui->comboBoxSearcherLocation->getCurrentUShort());
    }
}

void Pickup::generatorEncounterIndexChanged(int index)
{
    if (index >= 0)
    {
        updateEncounterLocations(ui->comboBoxGeneratorEncounter, ui->comboBoxGeneratorSeason, ui->comboBoxGeneratorLocation, encounterGenerator);
        generatorLocationIndexChanged(ui->comboBoxGeneratorLocation->currentIndex());
    }
}

void Pickup::generatorSeasonIndexChanged(int index)
{
    if (index >= 0)
    {
        updateEncounterLocations(ui->comboBoxGeneratorEncounter, ui->comboBoxGeneratorSeason, ui->comboBoxGeneratorLocation, encounterGenerator);
        generatorLocationIndexChanged(ui->comboBoxGeneratorLocation->currentIndex());
    }
}

void Pickup::generatorLocationIndexChanged(int index)
{
    if (index >= 0 && index < static_cast<int>(encounterGenerator.size()))
    {
        const auto &area = encounterGenerator[index];
        auto species = area.getUniqueSpecies();
        auto names = area.getSpecieNames();

        ui->filterGenerator->setEncounterSlots(area.getCount());

        ui->comboBoxGeneratorPokemon->clear();
        ui->comboBoxGeneratorPokemon->addItem("-");
        for (size_t i = 0; i < species.size(); i++)
        {
            ui->comboBoxGeneratorPokemon->addItem(QString::fromStdString(names[i]), species[i]);
        }
    }
}

void Pickup::generatorPokemonIndexChanged(int index)
{
    if (index <= 0 || ui->comboBoxGeneratorLocation->currentIndex() < 0)
    {
        ui->filterGenerator->resetEncounterSlots();
        ui->filterGenerator->setLevelRange(1, 100);
    }
    else
    {
        u16 num = ui->comboBoxGeneratorPokemon->getCurrentUShort();
        const auto &area = encounterGenerator[ui->comboBoxGeneratorLocation->currentIndex()];
        auto flags = area.getSlots(num);
        ui->filterGenerator->toggleEncounterSlots(flags);

        auto range = area.getLevelRange(num);
        ui->filterGenerator->setLevelRange(range.first, range.second);
    }
}

void Pickup::searcherEncounterIndexChanged(int index)
{
    if (index >= 0)
    {
        updateEncounterLocations(ui->comboBoxSearcherEncounter, ui->comboBoxSearcherSeason, ui->comboBoxSearcherLocation, encounterSearcher);
    }
}

void Pickup::searcherSeasonIndexChanged(int index)
{
    if (index >= 0)
    {
        updateEncounterLocations(ui->comboBoxSearcherEncounter, ui->comboBoxSearcherSeason, ui->comboBoxSearcherLocation, encounterSearcher);
    }
}
