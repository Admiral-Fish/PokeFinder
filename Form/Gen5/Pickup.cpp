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
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/ComboBox.hpp>
#include <Form/Controls/ComboBoxProxy.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Controls/Filter.hpp>
#include <Form/Controls/TextBox.hpp>
#include <Form/Gen5/Profile/ProfileManager5.hpp>
#include <Model/Gen5/PickupModel.hpp>
#include <Model/SortFilterProxyModel.hpp>
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QSizePolicy>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QThread>
#include <QTimer>
#include <algorithm>
#include <map>
#include <set>

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

    setupPickupUi();

    generatorModel = new PickupGeneratorModel5(ui->tableViewGenerator);
    ui->tableViewGenerator->setModel(generatorModel);
    connect(ui->filterGenerator, &Filter::showStatsChanged, generatorModel, &PickupGeneratorModel5::setShowStats);

    searcherModel = new PickupSearcherModel5(ui->tableViewSearcher);
    proxyModel = new SortFilterProxyModel(ui->tableViewSearcher, searcherModel);
    ui->tableViewSearcher->setModel(proxyModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed64Bit);
    generatorIVAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);

    ui->textBoxSearcherInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMaxAdvances->setValues(InputType::Advance32Bit);

    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &Pickup::profileIndexChanged);
    connect(ui->tabRNGSelector, &TabWidget::transferSettings, this, &Pickup::transferSettings);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Pickup::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Pickup::search);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Pickup::profileManager);

    updateProfiles();

    QSettings setting;
    setting.beginGroup("pickup");
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
    setting.beginGroup("pickup");
    setting.setValue("profile", ui->comboBoxProfiles->currentIndex());
    setting.setValue("geometry", saveGeometry());
    setting.setValue("startDate", ui->dateEditSearcherStartDate->date());
    setting.setValue("endDate", ui->dateEditSearcherEndDate->date());
    setting.endGroup();

    delete ui;
}

bool Pickup::hasProfiles() const
{
    return !profiles.empty();
}

void Pickup::updateProfiles()
{
    profiles = ProfileLoader5::getProfiles(Game::Gen5);

    ui->comboBoxProfiles->clear();
    for (const auto &profile : profiles)
    {
        ui->comboBoxProfiles->addItem(QString::fromStdString(profile.getName()));
    }

    QSettings setting;
    int val = setting.value("pickup/profile", 0).toInt();
    if (val < ui->comboBoxProfiles->count())
    {
        ui->comboBoxProfiles->setCurrentIndex(val);
    }
}

void Pickup::setupPickupUi()
{
    ui->groupBoxGeneratorRNGInfo->setMaximumWidth(240);
    ui->groupBoxSearcherRNGInfo->setMaximumWidth(QWIDGETSIZE_MAX);
    ui->groupBoxSearcherRNGInfo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->groupBoxGeneratorSettings->setMinimumWidth(430);
    ui->groupBoxGeneratorSettings->setMaximumWidth(660);
    ui->groupBoxSearcherSettings->setMinimumWidth(430);
    ui->groupBoxSearcherSettings->setMaximumWidth(620);
    ui->groupBoxGeneratorFilters->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->groupBoxSearcherFilters->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->gridLayout_2->setColumnStretch(1, 0);
    ui->gridLayout_2->setColumnStretch(2, 2);
    ui->gridLayout_20->setColumnStretch(0, 1);
    ui->gridLayout_20->setColumnStretch(1, 0);
    ui->gridLayout_20->setColumnStretch(2, 2);
    ui->gridLayout->setColumnStretch(1, 0);
    ui->gridLayout->setColumnStretch(3, 0);
    ui->gridLayout->setAlignment(Qt::AlignTop);
    ui->gridLayout->setVerticalSpacing(10);
    ui->gridLayout_3->setColumnStretch(1, 0);
    ui->gridLayout_3->setColumnStretch(3, 0);
    ui->gridLayout_3->setAlignment(Qt::AlignTop);
    ui->gridLayout_19->setColumnStretch(0, 1);
    ui->gridLayout_19->setColumnStretch(1, 3);
    ui->gridLayout_24->setColumnStretch(0, 1);
    ui->gridLayout_24->setColumnStretch(1, 0);
    ui->gridLayout_24->setColumnStretch(2, 0);

    std::array<QWidget *, 27> hidden = {
        ui->labelGeneratorSlot1, ui->labelGeneratorSlot2, ui->labelGeneratorSlot3, ui->labelGeneratorSlot4, ui->labelGeneratorSlot5,
        ui->labelGeneratorSlot6, ui->labelGeneratorBadges, ui->comboBoxGeneratorSpecie1, ui->comboBoxGeneratorSpecie2,
        ui->comboBoxGeneratorSpecie3, ui->comboBoxGeneratorSpecie4, ui->comboBoxGeneratorSpecie5, ui->comboBoxGeneratorSpecie6,
        ui->comboBoxGeneratorGender1, ui->comboBoxGeneratorGender2, ui->comboBoxGeneratorGender3, ui->comboBoxGeneratorGender4,
        ui->comboBoxGeneratorGender5, ui->comboBoxGeneratorGender6, ui->spinBoxGeneratorBadges,
        ui->labelSearcherSlot1, ui->labelSearcherSlot2, ui->labelSearcherSlot3, ui->labelSearcherSlot4, ui->labelSearcherSlot5,
        ui->labelSearcherSlot6, ui->labelSearcherBadges
    };
    for (auto *widget : hidden)
    {
        widget->hide();
    }

    std::array<QWidget *, 14> hiddenSearcher = {
        ui->comboBoxSearcherSpecie1, ui->comboBoxSearcherSpecie2, ui->comboBoxSearcherSpecie3, ui->comboBoxSearcherSpecie4,
        ui->comboBoxSearcherSpecie5, ui->comboBoxSearcherSpecie6, ui->comboBoxSearcherGender1, ui->comboBoxSearcherGender2,
        ui->comboBoxSearcherGender3, ui->comboBoxSearcherGender4, ui->comboBoxSearcherGender5, ui->comboBoxSearcherGender6,
        ui->spinBoxSearcherBadges, ui->filterSearcher
    };
    for (auto *widget : hiddenSearcher)
    {
        widget->hide();
    }

    ui->filterGenerator->disableControls(Controls::Height | Controls::Weight);
    ui->gridLayout_19->removeWidget(ui->filterGenerator);
    ui->gridLayout_19->addWidget(ui->filterGenerator, 0, 1, 9, 1);

    generatorIVAdvances = new TextBox(this);
    generatorIVAdvances->setText("0");
    auto *labelGeneratorIVAdvances = new QLabel(tr("IV Advances"), this);
    ui->gridLayout_7->addWidget(labelGeneratorIVAdvances, 1, 0);
    ui->gridLayout_7->addWidget(generatorIVAdvances, 1, 1);
    ui->gridLayout_7->addWidget(ui->labelGeneratorInitialAdvances, 2, 0);
    ui->gridLayout_7->addWidget(ui->textBoxGeneratorInitialAdvances, 2, 1);
    ui->gridLayout_7->addWidget(ui->labelGeneratorMaxAdvances, 3, 0);
    ui->gridLayout_7->addWidget(ui->textBoxGeneratorMaxAdvances, 3, 1);
    ui->gridLayout_7->addWidget(ui->pushButtonGenerate, 4, 0, 1, 2);

    auto setFieldWidth = [](QWidget *widget) {
        widget->setMinimumWidth(170);
        widget->setMaximumWidth(210);
    };

    auto createEncounterControls = [this, setFieldWidth](QGridLayout *settings, ComboBox *&encounter, ComboBox *&season,
                                                         ComboBoxProxy *&location, ComboBox *&pokemon, bool includePokemon) {
        encounter = new ComboBox(this);
        encounter->addItem(tr("Grass"));
        encounter->addItem(tr("Dark Grass"));
        encounter->addItem(tr("Surfing"));
        encounter->setup({ toInt(Encounter::Grass), toInt(Encounter::GrassDark), toInt(Encounter::Surfing) });
        setFieldWidth(encounter);

        season = new ComboBox(this);
        season->addItem(tr("Spring"));
        season->addItem(tr("Summer"));
        season->addItem(tr("Autumn"));
        season->addItem(tr("Winter"));
        setFieldWidth(season);

        location = new ComboBoxProxy(this);
        location->enableAutoComplete();
        setFieldWidth(location);

        if (includePokemon)
        {
            pokemon = new ComboBox(this);
            setFieldWidth(pokemon);
            settings->addWidget(new QLabel(tr("Encounter"), this), 1, 2);
            settings->addWidget(encounter, 1, 3);
            settings->addWidget(new QLabel(tr("Season"), this), 2, 2);
            settings->addWidget(season, 2, 3);
            settings->addWidget(new QLabel(tr("Location"), this), 3, 2);
            settings->addWidget(location, 3, 3);
            settings->addWidget(new QLabel(tr("Pokémon"), this), 4, 2);
            settings->addWidget(pokemon, 4, 3);
        }
        else
        {
            settings->addWidget(new QLabel(tr("Encounter"), this), 1, 2);
            settings->addWidget(encounter, 1, 3);
            settings->addWidget(new QLabel(tr("Season"), this), 2, 2);
            settings->addWidget(season, 2, 3);
            settings->addWidget(new QLabel(tr("Location"), this), 3, 2);
            settings->addWidget(location, 3, 3);
        }
    };

    createEncounterControls(ui->gridLayout, generatorEncounter, generatorSeason, generatorLocation, generatorPokemon, true);
    ComboBox *searcherPokemon = nullptr;
    createEncounterControls(ui->gridLayout_3, searcherEncounter, searcherSeason, searcherLocation, searcherPokemon, false);

    auto setupGenerator = [this](QGridLayout *settings, QGridLayout *filters, int row, int maxItemWidth,
                                 std::array<QCheckBox *, 6> &pickupSlotWidgets, std::array<QSpinBox *, 6> &levels,
                                 std::array<ComboBox *, 6> &items) {
        settings->addWidget(new QLabel(tr("Pickup Slots"), this), row, 0);
        settings->addWidget(new QLabel(tr("Level"), this), row, 1);
        filters->addWidget(new QLabel(tr("Items"), this), row, 0);

        for (int i = 0; i < 6; i++)
        {
            auto *pickupSlot = new QCheckBox(tr("Slot %1").arg(i + 1), this);

            auto *level = new QSpinBox(this);
            level->setRange(0, 100);
            level->setValue(50);

            auto *item = new ComboBox(this);
            if (maxItemWidth > 0)
            {
                item->setMaximumWidth(maxItemWidth);
            }

            settings->addWidget(pickupSlot, row + i + 1, 0);
            settings->addWidget(level, row + i + 1, 1);
            filters->addWidget(item, row + i + 1, 0);

            pickupSlotWidgets[i] = pickupSlot;
            levels[i] = level;
            items[i] = item;

            connect(pickupSlot, &QCheckBox::toggled, this, [=] { updateItemList(pickupSlot, level, item); });
            connect(level, qOverload<int>(&QSpinBox::valueChanged), this, [=] { updateItemList(pickupSlot, level, item); });
            updateItemList(pickupSlot, level, item);
        }

        auto *clear = new QPushButton(tr("Clear"), this);
        filters->addWidget(clear, row + 7, 0);
        connect(clear, &QPushButton::clicked, this, [=] {
            for (auto *item : items)
            {
                item->setCurrentIndex(0);
            }
        });

        QWidget::setTabOrder(ui->textBoxGeneratorSeed, generatorIVAdvances);
        QWidget::setTabOrder(generatorIVAdvances, ui->textBoxGeneratorInitialAdvances);
        QWidget::setTabOrder(ui->textBoxGeneratorInitialAdvances, ui->textBoxGeneratorMaxAdvances);
        QWidget::setTabOrder(ui->textBoxGeneratorMaxAdvances, ui->pushButtonGenerate);
        QWidget::setTabOrder(ui->pushButtonGenerate, pickupSlotWidgets[0]);
        for (int i = 0; i < 6; i++)
        {
            QWidget::setTabOrder(pickupSlotWidgets[i], levels[i]);
            if (i < 5)
            {
                QWidget::setTabOrder(levels[i], pickupSlotWidgets[i + 1]);
            }
        }
        QWidget::setTabOrder(levels[5], generatorEncounter);
        QWidget::setTabOrder(generatorEncounter, generatorSeason);
        QWidget::setTabOrder(generatorSeason, generatorLocation);
        QWidget::setTabOrder(generatorLocation, generatorPokemon);
        QWidget::setTabOrder(generatorPokemon, items[0]);
        for (int i = 0; i < 5; i++)
        {
            QWidget::setTabOrder(items[i], items[i + 1]);
        }
        QWidget::setTabOrder(items[5], clear);
        QWidget::setTabOrder(clear, ui->filterGenerator);
        QWidget::setTabOrder(ui->filterGenerator, ui->comboBoxProfiles);
        QWidget::setTabOrder(ui->comboBoxProfiles, ui->pushButtonProfileManager);
        QWidget::setTabOrder(ui->pushButtonProfileManager, ui->tabRNGSelector);
        QWidget::setTabOrder(ui->tabRNGSelector, ui->textBoxGeneratorSeed);
    };

    auto setupSearcher = [this](QGridLayout *settings, QGridLayout *filters, int row, std::array<QCheckBox *, 6> &pickupSlotWidgets,
                                std::array<QSpinBox *, 6> &levels, std::array<ComboBox *, 6> &items,
                                std::array<ComboBox *, 6> &counts) {
        settings->addWidget(new QLabel(tr("Pickup Slots"), this), row, 0);
        settings->addWidget(new QLabel(tr("Level"), this), row, 1);
        filters->addWidget(new QLabel(tr("Items"), this), row, 0);
        filters->addWidget(new QLabel(tr("Count"), this), row, 1);

        for (int i = 0; i < 6; i++)
        {
            auto *pickupSlot = new QCheckBox(tr("Slot %1").arg(i + 1), this);

            auto *level = new QSpinBox(this);
            level->setRange(0, 100);
            level->setValue(50);

            auto *item = new ComboBox(this);
            auto *count = new ComboBox(this);
            auto *clear = new QPushButton(tr("Clear"), this);
            count->setMaximumWidth(45);

            settings->addWidget(pickupSlot, row + i + 1, 0);
            settings->addWidget(level, row + i + 1, 1);
            filters->addWidget(item, row + i + 1, 0);
            filters->addWidget(count, row + i + 1, 1);
            filters->addWidget(clear, row + i + 1, 2);

            pickupSlotWidgets[i] = pickupSlot;
            levels[i] = level;
            items[i] = item;
            counts[i] = count;
            searcherItemClearButtons[i] = clear;

            connect(pickupSlot, &QCheckBox::toggled, this, &Pickup::updateSearcherItemLists);
            connect(level, qOverload<int>(&QSpinBox::valueChanged), this, &Pickup::updateSearcherItemLists);
            connect(item, qOverload<int>(&QComboBox::currentIndexChanged), this, &Pickup::updateSearcherItemLists);
            connect(count, qOverload<int>(&QComboBox::currentIndexChanged), this, &Pickup::updateSearcherItemLists);
            connect(clear, &QPushButton::clicked, this, [=] {
                item->setCurrentIndex(0);
                count->setCurrentIndex(0);
                updateSearcherItemLists();
            });
        }

        for (int i = 0; i < 6; i++)
        {
            QWidget::setTabOrder(pickupSlotWidgets[i], levels[i]);
            if (i < 5)
            {
                QWidget::setTabOrder(levels[i], pickupSlotWidgets[i + 1]);
            }
        }
        QWidget::setTabOrder(levels[5], searcherEncounter);
        QWidget::setTabOrder(searcherEncounter, searcherSeason);
        QWidget::setTabOrder(searcherSeason, searcherLocation);
        QWidget::setTabOrder(searcherLocation, items[0]);
        for (int i = 0; i < 6; i++)
        {
            QWidget::setTabOrder(items[i], counts[i]);
            QWidget::setTabOrder(counts[i], searcherItemClearButtons[i]);
            if (i < 5)
            {
                QWidget::setTabOrder(searcherItemClearButtons[i], items[i + 1]);
            }
        }
    };

    setupGenerator(ui->gridLayout, ui->gridLayout_19, 0, 260, generatorSlots, generatorLevels, generatorItems);
    setupSearcher(ui->gridLayout_3, ui->gridLayout_24, 0, searcherSlots, searcherLevels, searcherItems, searcherItemCounts);
    updateSearcherItemLists();

    connect(generatorEncounter, &QComboBox::currentIndexChanged, this, &Pickup::generatorEncounterIndexChanged);
    connect(generatorSeason, &QComboBox::currentIndexChanged, this, &Pickup::generatorSeasonIndexChanged);
    connect(generatorLocation, &QComboBox::currentIndexChanged, this, &Pickup::generatorLocationIndexChanged);
    connect(generatorPokemon, &QComboBox::currentIndexChanged, this, &Pickup::generatorPokemonIndexChanged);
    connect(searcherEncounter, &QComboBox::currentIndexChanged, this, &Pickup::searcherEncounterIndexChanged);
    connect(searcherSeason, &QComboBox::currentIndexChanged, this, &Pickup::searcherSeasonIndexChanged);
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
    if (encounterGenerator.empty() || generatorLocation->currentIndex() < 0)
    {
        return;
    }

    WildStateFilter filter = ui->filterGenerator->getFilter<WildStateFilter, true>();
    PickupGenerator generator(ui->textBoxGeneratorInitialAdvances->getUInt(), ui->textBoxGeneratorMaxAdvances->getUInt(), settings,
                              encounterGenerator[generatorLocation->currentIndex()], *currentProfile, filter, true);
    generatorModel->addItems(generator.generate(ui->textBoxGeneratorSeed->getULong(), generatorIVAdvances->getUInt()));
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
    if (encounterSearcher.empty() || searcherLocation->currentIndex() < 0)
    {
        return;
    }

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    PickupGenerator generator(ui->textBoxSearcherInitialAdvances->getUInt(), ui->textBoxSearcherMaxAdvances->getUInt(), settings,
                              getSearcherRequests(), encounterSearcher[searcherLocation->currentIndex()], *currentProfile);
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

void Pickup::profileIndexChanged(int index)
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

        generatorEncounterIndexChanged(0);
        searcherEncounterIndexChanged(0);
    }
}

void Pickup::profileManager()
{
    auto *manager = new ProfileManager5();
    connect(manager, &ProfileManager5::profilesChanged, this, [=](int num) { emit profilesChanged(num); });
    manager->show();
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
        searcherEncounter->setCurrentIndex(generatorEncounter->currentIndex());
        searcherSeason->setCurrentIndex(generatorSeason->currentIndex());
        searcherLocation->setCurrentIndexByData(generatorLocation->getCurrentUShort());
    }
    else
    {
        copySlots(searcherSlots, searcherLevels, generatorSlots, generatorLevels);
        generatorEncounter->setCurrentIndex(searcherEncounter->currentIndex());
        generatorSeason->setCurrentIndex(searcherSeason->currentIndex());
        generatorLocation->setCurrentIndexByData(searcherLocation->getCurrentUShort());
    }
}

void Pickup::generatorEncounterIndexChanged(int index)
{
    if (index >= 0)
    {
        updateEncounterLocations(generatorEncounter, generatorSeason, generatorLocation, encounterGenerator);
        generatorLocationIndexChanged(generatorLocation->currentIndex());
    }
}

void Pickup::generatorSeasonIndexChanged(int index)
{
    if (index >= 0)
    {
        updateEncounterLocations(generatorEncounter, generatorSeason, generatorLocation, encounterGenerator);
        generatorLocationIndexChanged(generatorLocation->currentIndex());
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

        generatorPokemon->clear();
        generatorPokemon->addItem("-");
        for (size_t i = 0; i < species.size(); i++)
        {
            generatorPokemon->addItem(QString::fromStdString(names[i]), species[i]);
        }
    }
}

void Pickup::generatorPokemonIndexChanged(int index)
{
    if (index <= 0 || generatorLocation->currentIndex() < 0)
    {
        ui->filterGenerator->resetEncounterSlots();
        ui->filterGenerator->setLevelRange(1, 100);
    }
    else
    {
        u16 num = generatorPokemon->getCurrentUShort();
        const auto &area = encounterGenerator[generatorLocation->currentIndex()];
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
        updateEncounterLocations(searcherEncounter, searcherSeason, searcherLocation, encounterSearcher);
    }
}

void Pickup::searcherSeasonIndexChanged(int index)
{
    if (index >= 0)
    {
        updateEncounterLocations(searcherEncounter, searcherSeason, searcherLocation, encounterSearcher);
    }
}
