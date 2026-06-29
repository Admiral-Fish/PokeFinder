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
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen5/EncounterArea5.hpp>
#include <Core/Gen5/Encounters5.hpp>
#include <Core/Gen5/Generators/WildGenerator5.hpp>
#include <Core/Gen5/IVCache.hpp>
#include <Core/Gen5/PhenomenonArea.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/SHA1Cache.hpp>
#include <Core/Gen5/Searchers/IVSearcher5.hpp>
#include <Core/Gen5/States/SearcherState5.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/ProfileLoader.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/CheckList.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Gen5/Profile/ProfileManager5.hpp>
#include <Model/Gen5/WildModel5.hpp>
#include <Model/SortFilterProxyModel.hpp>
#include <QButtonGroup>
#include <QFileDialog>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QSettings>
#include <QThread>
#include <QTimer>
#include <algorithm>

namespace
{
    WildStateFilter getUnfilteredWildStateFilter()
    {
        std::array<u8, 6> ivMin {};
        std::array<u8, 6> ivMax;
        ivMax.fill(31);

        std::array<bool, 25> natures;
        natures.fill(true);

        std::array<bool, 16> powers;
        powers.fill(true);

        std::array<bool, 12> encounterSlots;
        encounterSlots.fill(true);

        return WildStateFilter(255, 255, 255, 1, 100, 0, 255, 0, 255, true, ivMin, ivMax, natures, powers, encounterSlots);
    }
}

Phenomenon::Phenomenon(QWidget *parent) : QWidget(parent), ui(new Ui::Phenomenon), ivCache(nullptr), shaCache(nullptr)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);

    generatorModel = new WildGeneratorModel5(ui->tableViewGenerator);
    searcherModel = new WildSearcherModel5(ui->tableViewSearcher);
    proxyModel = new SortFilterProxyModel(ui->tableViewSearcher, searcherModel);
    generatorModel->setShowPhenomenon(true);
    searcherModel->setShowPhenomenon(true);

    ui->tableViewGenerator->setModel(generatorModel);
    ui->tableViewSearcher->setModel(proxyModel);

    ui->textBoxGeneratorSeed->setValues(InputType::Seed64Bit);
    ui->textBoxGeneratorIVAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorMaxAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxGeneratorOffset->setValues(InputType::Advance32Bit);

    ui->textBoxSearcherInitialIVAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMaxIVAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherInitialAdvances->setValues(InputType::Advance32Bit);
    ui->textBoxSearcherMaxAdvances->setValues(InputType::Advance32Bit);

    ui->comboBoxGeneratorEncounter->setItemText(0, tr("Rustling Grass"));
    ui->comboBoxGeneratorEncounter->setItemText(1, tr("Dust Cloud"));
    ui->comboBoxGeneratorEncounter->setItemText(2, tr("Rippling Surfing"));
    ui->comboBoxGeneratorEncounter->setItemText(3, tr("Rippling Fishing"));
    ui->comboBoxGeneratorEncounter->setItemText(4, tr("Flying Shadow"));
    ui->comboBoxGeneratorEncounter->setup({ toInt(Encounter::GrassRustling), toInt(Encounter::DustCloud), toInt(Encounter::SurfingRippling),
                                            toInt(Encounter::SuperRodRippling), toInt(Encounter::FlyingShadow) });

    ui->comboBoxSearcherEncounter->setItemText(0, tr("Rustling Grass"));
    ui->comboBoxSearcherEncounter->setItemText(1, tr("Dust Cloud"));
    ui->comboBoxSearcherEncounter->setItemText(2, tr("Rippling Surfing"));
    ui->comboBoxSearcherEncounter->setItemText(3, tr("Rippling Fishing"));
    ui->comboBoxSearcherEncounter->setItemText(4, tr("Flying Shadow"));
    ui->comboBoxSearcherEncounter->setup({ toInt(Encounter::GrassRustling), toInt(Encounter::DustCloud), toInt(Encounter::SurfingRippling),
                                           toInt(Encounter::SuperRodRippling), toInt(Encounter::FlyingShadow) });

    checkListGeneratorItem = new CheckList(ui->groupBoxGeneratorSettings);
    checkListGeneratorItem->setUncheckedText(tr("None"));
    labelGeneratorItem = new QLabel(tr("Item"), ui->groupBoxGeneratorSettings);
    auto *generatorSettingsLayout = qobject_cast<QGridLayout *>(ui->groupBoxGeneratorSettings->layout());
    auto moveLayoutItem = [](QGridLayout *layout, int row, int column, int newRow, int newColumn, int rowSpan = 1, int columnSpan = 1) {
        if (QLayoutItem *item = layout->itemAtPosition(row, column))
        {
            layout->removeItem(item);
            layout->addItem(item, newRow, newColumn, rowSpan, columnSpan);
        }
    };
    moveLayoutItem(generatorSettingsLayout, 5, 0, 6, 0);
    moveLayoutItem(generatorSettingsLayout, 5, 1, 6, 1);
    moveLayoutItem(generatorSettingsLayout, 5, 2, 6, 2);
    moveLayoutItem(generatorSettingsLayout, 4, 0, 5, 0, 1, 3);
    generatorSettingsLayout->addWidget(labelGeneratorItem, 4, 0);
    generatorSettingsLayout->addWidget(checkListGeneratorItem, 4, 1, 1, 2);

    checkListSearcherItem = new CheckList(ui->groupBoxSearcherSettings);
    checkListSearcherItem->setUncheckedText(tr("None"));
    radioButtonSearcherPokemon = new QRadioButton(ui->groupBoxSearcherSettings);
    radioButtonSearcherItem = new QRadioButton(ui->groupBoxSearcherSettings);
    auto createSearcherFilterLabel = [](const QString &text, QRadioButton *button, QWidget *parent) {
        auto *widget = new QWidget(parent);
        auto *layout = new QHBoxLayout(widget);
        auto *label = new QPushButton(text, widget);
        label->setFlat(true);
        label->setFocusPolicy(Qt::NoFocus);
        label->setStyleSheet("QPushButton { border: none; text-align: left; padding: 0; background: transparent; }");
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(4);
        layout->addWidget(label, 1);
        layout->addWidget(button);
        QObject::connect(label, &QPushButton::clicked, button, [button] { button->setChecked(true); });
        return widget;
    };
    auto *labelSearcherPokemon = createSearcherFilterLabel(tr("Pokémon"), radioButtonSearcherPokemon, ui->groupBoxSearcherSettings);
    labelSearcherItem = createSearcherFilterLabel(tr("Item"), radioButtonSearcherItem, ui->groupBoxSearcherSettings);
    auto *searcherFilterGroup = new QButtonGroup(ui->groupBoxSearcherSettings);
    searcherFilterGroup->setExclusive(true);
    searcherFilterGroup->addButton(radioButtonSearcherPokemon);
    searcherFilterGroup->addButton(radioButtonSearcherItem);
    radioButtonSearcherPokemon->setChecked(true);
    auto *searcherSettingsLayout = qobject_cast<QGridLayout *>(ui->groupBoxSearcherSettings->layout());
    ui->labelSearcherPokemon->setVisible(false);
    searcherSettingsLayout->removeWidget(ui->labelSearcherPokemon);
    moveLayoutItem(searcherSettingsLayout, 7, 0, 8, 0, 1, 3);
    moveLayoutItem(searcherSettingsLayout, 6, 0, 7, 0, 1, 3);
    moveLayoutItem(searcherSettingsLayout, 5, 0, 6, 0);
    moveLayoutItem(searcherSettingsLayout, 5, 1, 6, 1);
    moveLayoutItem(searcherSettingsLayout, 5, 2, 6, 2);
    moveLayoutItem(searcherSettingsLayout, 4, 0, 5, 0, 1, 3);
    searcherSettingsLayout->addWidget(labelSearcherPokemon, 3, 0);
    searcherSettingsLayout->addWidget(labelSearcherItem, 4, 0);
    searcherSettingsLayout->addWidget(checkListSearcherItem, 4, 1, 1, 2);

    ui->filterGenerator->disableControls(Controls::Height | Controls::Weight);
    ui->filterSearcher->disableControls(Controls::DisableFilter | Controls::Height | Controls::Weight);

    ui->comboMenuGeneratorLead->addAction(tr("None"), toInt(Lead::None));
    ui->comboMenuGeneratorLead->addAction(tr("Compound Eyes"), toInt(Lead::CompoundEyes));
    ui->comboMenuGeneratorLead->addMenu(tr("Cute Charm"),
                                        { { tr("♂ Lead"), toInt(Lead::CuteCharmM) }, { tr("♀ Lead"), toInt(Lead::CuteCharmF) } });
    ui->comboMenuGeneratorLead->addMenu(tr("Level Modifier"),
                                        { { tr("Hustle"), toInt(Lead::Hustle) },
                                          { tr("Pressure"), toInt(Lead::Pressure) },
                                          { tr("Vital Spirit"), toInt(Lead::VitalSpirit) } });
    ui->comboMenuGeneratorLead->addMenu(tr("Slot Modifier"),
                                        { { tr("Magnet Pull"), toInt(Lead::MagnetPull) }, { tr("Static"), toInt(Lead::Static) } });
    ui->comboMenuGeneratorLead->addMenu(tr("Synchronize"), Translator::getNatures());

    ui->comboMenuSearcherLead->addAction(tr("None"), toInt(Lead::None));
    ui->comboMenuSearcherLead->addAction(tr("Compound Eyes"), toInt(Lead::CompoundEyes));
    ui->comboMenuSearcherLead->addMenu(tr("Cute Charm"),
                                       { { tr("♂ Lead"), toInt(Lead::CuteCharmM) }, { tr("♀ Lead"), toInt(Lead::CuteCharmF) } });
    ui->comboMenuSearcherLead->addMenu(tr("Level Modifier"),
                                       { { tr("Hustle"), toInt(Lead::Hustle) },
                                         { tr("Pressure"), toInt(Lead::Pressure) },
                                         { tr("Vital Spirit"), toInt(Lead::VitalSpirit) } });
    ui->comboMenuSearcherLead->addMenu(tr("Slot Modifier"),
                                       { { tr("Magnet Pull"), toInt(Lead::MagnetPull) }, { tr("Static"), toInt(Lead::Static) } });
    ui->comboMenuSearcherLead->addMenu(tr("Synchronize"), Translator::getNatures());

    ui->comboBoxGeneratorLocation->enableAutoComplete();
    ui->comboBoxSearcherLocation->enableAutoComplete();

    ui->comboBoxGeneratorLuckyPower->setup({ 0, 1, 2, 3 });
    ui->comboBoxSearcherLuckyPower->setup({ 0, 1, 2, 3 });

    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &Phenomenon::profileIndexChanged);
    connect(ui->tabRNGSelector, &TabWidget::transferFilters, this, &Phenomenon::transferFilters);
    connect(ui->tabRNGSelector, &TabWidget::transferSettings, this, &Phenomenon::transferSettings);
    connect(ui->pushButtonGenerate, &QPushButton::clicked, this, &Phenomenon::generate);
    connect(ui->pushButtonSearch, &QPushButton::clicked, this, &Phenomenon::search);
    connect(ui->comboBoxGeneratorEncounter, &QComboBox::currentIndexChanged, this, &Phenomenon::generatorEncounterIndexChanged);
    connect(ui->comboBoxSearcherEncounter, &QComboBox::currentIndexChanged, this, &Phenomenon::searcherEncounterIndexChanged);
    connect(ui->comboBoxGeneratorLocation, &QComboBox::currentIndexChanged, this, &Phenomenon::generatorLocationIndexChanged);
    connect(ui->comboBoxSearcherLocation, &QComboBox::currentIndexChanged, this, &Phenomenon::searcherLocationIndexChanged);
    connect(ui->comboBoxGeneratorPokemon, &QComboBox::currentIndexChanged, this, &Phenomenon::generatorPokemonIndexChanged);
    connect(ui->comboBoxSearcherPokemon, &QComboBox::currentIndexChanged, this, &Phenomenon::searcherPokemonIndexChanged);
    connect(ui->comboBoxGeneratorSeason, &QComboBox::currentIndexChanged, this, &Phenomenon::generatorSeasonIndexChanged);
    connect(ui->comboBoxSearcherSeason, &QComboBox::currentIndexChanged, this, &Phenomenon::searcherSeasonIndexChanged);
    connect(radioButtonSearcherPokemon, &QRadioButton::toggled, this, &Phenomenon::updateSearcherFilterMode);
    connect(radioButtonSearcherItem, &QRadioButton::toggled, this, &Phenomenon::updateSearcherFilterMode);
    connect(ui->pushButtonProfileManager, &QPushButton::clicked, this, &Phenomenon::profileManager);
    connect(ui->filterGenerator, &Filter::showStatsChanged, generatorModel, &WildGeneratorModel5::setShowStats);
    connect(ui->filterSearcher, &Filter::showStatsChanged, searcherModel, &WildSearcherModel5::setShowStats);
    connect(ui->comboBoxProfiles, &QComboBox::currentIndexChanged, this, &Phenomenon::searcherFastSearchChanged);
    connect(ui->filterSearcher, &Filter::ivsChanged, this, &Phenomenon::searcherFastSearchChanged);
    connect(ui->textBoxSearcherInitialIVAdvances, &TextBox::textChanged, this, &Phenomenon::searcherFastSearchChanged);
    connect(ui->textBoxSearcherMaxIVAdvances, &TextBox::textChanged, this, &Phenomenon::searcherFastSearchChanged);

    updateProfiles();
    if (hasProfiles())
    {
        generatorEncounterIndexChanged(0);
        searcherEncounterIndexChanged(0);
    }
    searcherFastSearchChanged();

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

    delete ivCache;
    delete shaCache;
    delete ui;
}

bool Phenomenon::hasProfiles() const
{
    return !profiles.empty();
}

void Phenomenon::updateProfiles()
{
    profiles = ProfileLoader5::getProfiles();

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
bool Phenomenon::fastSearchEnabled() const
{
    if (ivCache == nullptr)
    {
        return false;
    }

    u32 initialAdvances = ui->textBoxSearcherInitialIVAdvances->getUInt();
    u32 maxAdvances = ui->textBoxSearcherMaxIVAdvances->getUInt();

    if (initialAdvances < ivCache->getInitialAdvances()
        || (initialAdvances + maxAdvances) > (ivCache->getInitialAdvances() + ivCache->getMaxAdvances()))
    {
        return false;
    }

    auto min = ui->filterSearcher->getMinIVs();
    auto max = ui->filterSearcher->getMaxIVs();

    return min[0] >= 30 && min[2] >= 30 && min[4] >= 30 && (min[1] >= 30 || min[3] >= 30) && (min[5] >= 30 || max[5] <= 1);
}

bool Phenomenon::removeByGeneratorFilters(const WildState5 &state) const
{
    if (state.getPhenomenonItem())
    {
        auto itemCheckState = checkListGeneratorItem->getCheckState();
        if (itemCheckState == Qt::Unchecked)
        {
            return true;
        }

        bool pokemonSelected = ui->comboBoxGeneratorPokemon->currentIndex() > 0;
        bool itemFilterAny = itemCheckState == Qt::Checked;
        if (pokemonSelected && itemFilterAny)
        {
            return true;
        }

        auto items = checkListGeneratorItem->getCheckedData();
        return std::find(items.begin(), items.end(), state.getItem()) == items.end();
    }

    return false;
}

bool Phenomenon::removeBySearcherFilters(const WildState5 &state) const
{
    bool itemMode = radioButtonSearcherItem->isChecked();
    if (state.getPhenomenonItem())
    {
        if (!itemMode)
        {
            return true;
        }

        if (checkListSearcherItem->getCheckState() == Qt::Unchecked)
        {
            return true;
        }

        auto items = checkListSearcherItem->getCheckedData();
        return std::find(items.begin(), items.end(), state.getItem()) == items.end();
    }

    return itemMode;
}

void Phenomenon::updateSearcherFilterMode()
{
    bool itemMode = radioButtonSearcherItem->isChecked();
    ui->comboBoxSearcherPokemon->setEnabled(!itemMode);
    checkListSearcherItem->setEnabled(itemMode);

    if (itemMode)
    {
        ui->filterSearcher->resetEncounterSlots();
        ui->spinBoxSearcherLevelMin->setValue(0);
        ui->spinBoxSearcherLevelMax->setValue(0);
        ui->filterSearcher->setLevelRange(1, 100);
    }
    else
    {
        searcherPokemonIndexChanged(ui->comboBoxSearcherPokemon->currentIndex());
    }
}

void Phenomenon::updateItemFilter(QWidget *itemLabel, CheckList *itemFilter, EncounterArea5 &area, bool checkAll)
{
    itemFilter->clear();

    PhenomenonType type;
    if (area.getEncounter() == Encounter::DustCloud)
    {
        type = PhenomenonType::Cave;
    }
    else if (area.getEncounter() == Encounter::FlyingShadow)
    {
        type = PhenomenonType::Bridge;
    }
    else
    {
        if (itemFilter == checkListSearcherItem && radioButtonSearcherItem->isChecked())
        {
            radioButtonSearcherPokemon->setChecked(true);
        }

        if (itemFilter == checkListSearcherItem)
        {
            radioButtonSearcherPokemon->setVisible(false);
        }
        itemLabel->setVisible(false);
        itemFilter->setVisible(false);
        updateSearcherFilterMode();
        return;
    }

    PhenomenonArea phenomenonArea(area.getLocation(), type);
    auto items = phenomenonArea.getUniqueItems();
    auto names = phenomenonArea.getItemNames();
    for (size_t i = 0; i < items.size(); i++)
    {
        itemFilter->addItem(QString::fromStdString(names[i]), items[i]);
    }
    itemFilter->setChecks(std::vector<bool>(items.size(), checkAll));

    itemLabel->setVisible(true);
    itemFilter->setVisible(true);
    if (itemFilter == checkListSearcherItem)
    {
        radioButtonSearcherPokemon->setVisible(true);
    }
    updateSearcherFilterMode();
}

void Phenomenon::generate()
{
    if (!ui->filterGenerator->isValid(ui->spinBoxGeneratorLevelMin->value(), ui->spinBoxGeneratorLevelMax->value()))
    {
        return;
    }

    generatorModel->clearModel();

    u64 seed = ui->textBoxGeneratorSeed->getULong();
    u32 ivAdvances = ui->textBoxGeneratorIVAdvances->getUInt();
    u32 initialAdvances = ui->textBoxGeneratorInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxGeneratorMaxAdvances->getUInt();
    u32 offset = ui->textBoxGeneratorOffset->getUInt();
    auto lead = ui->comboMenuGeneratorLead->getEnum<Lead>();
    u8 luckyPower = ui->comboBoxGeneratorLuckyPower->getCurrentUChar();

    auto filter = ui->filterGenerator->getFilter<WildStateFilter, true>();
    WildGenerator5 generator(initialAdvances, maxAdvances, offset, Method::None, lead, luckyPower,
                             encounterGenerator[ui->comboBoxGeneratorLocation->currentIndex()], *currentProfile, filter);

    auto itemCheckState = checkListGeneratorItem->isVisible() ? checkListGeneratorItem->getCheckState() : Qt::Unchecked;
    bool pokemonSelected = ui->comboBoxGeneratorPokemon->currentIndex() > 0;
    bool generateItems = itemCheckState != Qt::Unchecked && !(pokemonSelected && itemCheckState == Qt::Checked);

    auto states = generator.generate(seed, ivAdvances, 0);
    if (generateItems)
    {
        std::erase_if(states, [](const WildState5 &state) { return state.getPhenomenonItem(); });

        WildGenerator5 itemGenerator(initialAdvances, maxAdvances, offset, Method::None, lead, luckyPower,
                                     encounterGenerator[ui->comboBoxGeneratorLocation->currentIndex()], *currentProfile,
                                     getUnfilteredWildStateFilter());
        auto itemStates = itemGenerator.generate(seed, ivAdvances, 0);
        std::erase_if(itemStates, [=, this](const WildState5 &state) {
            return !state.getPhenomenonItem() || removeByGeneratorFilters(state);
        });

        states.insert(states.end(), itemStates.begin(), itemStates.end());
        std::stable_sort(states.begin(), states.end(), [](const WildState5 &left, const WildState5 &right) {
            return left.getAdvances() < right.getAdvances();
        });
    }
    else
    {
        std::erase_if(states, [=, this](const WildState5 &state) {
            return removeByGeneratorFilters(state);
        });
    }
    generatorModel->addItems(states);
}

void Phenomenon::generatorEncounterIndexChanged(int index)
{
    if (index >= 0)
    {
        auto encounter = ui->comboBoxGeneratorEncounter->getEnum<Encounter>();
        u16 currentLocation = ui->comboBoxGeneratorLocation->getCurrentUShort();

        u8 season = ui->comboBoxGeneratorSeason->currentIndex();
        encounterGenerator = Encounters5::getEncounters(encounter, season, currentProfile);

        std::vector<u16> locs;
        std::ranges::transform(encounterGenerator, std::back_inserter(locs), [](const EncounterArea5 &area) { return area.getLocation(); });

        ui->comboBoxGeneratorLocation->clear();
        ui->comboBoxGeneratorLocation->addItems(Translator::getLocations(locs, currentProfile->getVersion()), locs);
        ui->comboBoxGeneratorLocation->setCurrentIndexByData(currentLocation);
    }
}

void Phenomenon::generatorLocationIndexChanged(int index)
{
    if (index >= 0)
    {
        auto &area = encounterGenerator[ui->comboBoxGeneratorLocation->currentIndex()];
        auto species = area.getUniqueSpecies();
        auto names = area.getSpecieNames();

        ui->filterGenerator->setEncounterSlots(area.getCount());

        ui->comboBoxGeneratorPokemon->clear();
        ui->comboBoxGeneratorPokemon->addItem("-");
        for (size_t i = 0; i < species.size(); i++)
        {
            ui->comboBoxGeneratorPokemon->addItem(QString::fromStdString(names[i]), species[i]);
        }

        updateItemFilter(labelGeneratorItem, checkListGeneratorItem, area, true);
    }
}

void Phenomenon::generatorPokemonIndexChanged(int index)
{
    if (index <= 0)
    {
        ui->filterGenerator->resetEncounterSlots();
        ui->spinBoxGeneratorLevelMin->setValue(0);
        ui->spinBoxGeneratorLevelMax->setValue(0);
        ui->filterGenerator->setLevelRange(1, 100);
    }
    else
    {
        u16 num = ui->comboBoxGeneratorPokemon->getCurrentUShort();
        auto flags = encounterGenerator[ui->comboBoxGeneratorLocation->currentIndex()].getSlots(num);
        ui->filterGenerator->toggleEncounterSlots(flags);

        auto range = encounterGenerator[ui->comboBoxGeneratorLocation->currentIndex()].getLevelRange(num);
        ui->spinBoxGeneratorLevelMin->setValue(range.first);
        ui->spinBoxGeneratorLevelMax->setValue(range.second);
        ui->filterGenerator->setLevelRange(range.first, range.second);
    }
}

void Phenomenon::generatorSeasonIndexChanged(int index)
{
    if (index >= 0)
    {
        generatorEncounterIndexChanged(0);
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

        if (ivCache)
        {
            delete ivCache;
            ivCache = nullptr;
        }

        if (shaCache)
        {
            delete shaCache;
            shaCache = nullptr;
        }

        auto ivCachePath = currentProfile->getIVCache();
        if (!ivCachePath.empty())
        {
            ivCache = new IVCache(ivCachePath);
        }

        auto shaCachePath = currentProfile->getSHACache();
        if (!shaCachePath.empty())
        {
            shaCache = new SHA1Cache(shaCachePath);
            ui->dateEditSearcherStartDate->setDateRange(shaCache->getStartDate(), shaCache->getEndDate());
            ui->dateEditSearcherEndDate->setDateRange(shaCache->getStartDate(), shaCache->getEndDate());
        }
        else
        {
            ui->dateEditSearcherStartDate->clearDateRange();
            ui->dateEditSearcherEndDate->clearDateRange();
        }

        bool flag = (currentProfile->getVersion() & Game::BW2) != Game::None;

        ui->labelGeneratorLuckyPower->setVisible(flag);
        ui->comboBoxGeneratorLuckyPower->setVisible(flag);

        ui->labelSearcherLuckyPower->setVisible(flag);
        ui->comboBoxSearcherLuckyPower->setVisible(flag);

        generatorEncounterIndexChanged(0);
        searcherEncounterIndexChanged(0);
    }
}

void Phenomenon::profileManager()
{
    auto *manager = new ProfileManager5();
    connect(manager, &ProfileManager5::profilesModified, this, [=](int num) { emit profilesModified(num); });
    manager->show();
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

    bool itemMode = radioButtonSearcherItem->isChecked();
    if (!itemMode && !ui->filterSearcher->isValid(ui->spinBoxSearcherLevelMin->value(), ui->spinBoxSearcherLevelMax->value()))
    {
        return;
    }

    searcherModel->clearModel();

    ui->pushButtonSearch->setEnabled(false);
    ui->pushButtonCancel->setEnabled(true);

    u32 initialIVAdvances = ui->textBoxSearcherInitialIVAdvances->getUInt();
    u32 maxIVAdvances = ui->textBoxSearcherMaxIVAdvances->getUInt();
    u32 initialAdvances = ui->textBoxSearcherInitialAdvances->getUInt();
    u32 maxAdvances = ui->textBoxSearcherMaxAdvances->getUInt();
    auto lead = ui->comboMenuSearcherLead->getEnum<Lead>();
    u8 luckyPower = ui->comboBoxSearcherLuckyPower->getCurrentUChar();

    auto filter = itemMode ? getUnfilteredWildStateFilter() : ui->filterSearcher->getFilter<WildStateFilter, true>();
    WildGenerator5 generator(initialAdvances, maxAdvances, 0, Method::Method5, lead, luckyPower,
                             encounterSearcher[ui->comboBoxSearcherLocation->currentIndex()], *currentProfile, filter);

    SearcherBase5<WildGenerator5, WildState5> *searcher;
    if (!itemMode && fastSearchEnabled())
    {
        auto ivMap = ivCache->getCache(initialIVAdvances, maxIVAdvances, currentProfile->getVersion(), CacheType::Normal, filter);
        if (shaCache && shaCache->isValid(*currentProfile))
        {
            auto shaMap = shaCache->getCache(initialIVAdvances, maxIVAdvances, start, end, ivMap, CacheType::Normal, *currentProfile);
            searcher = new IVSearcher5CacheFast<WildGenerator5, WildState5>(initialIVAdvances, maxIVAdvances, shaMap, ivMap, generator,
                                                                            *currentProfile);
        }
        else
        {
            searcher = new IVSearcher5Fast<WildGenerator5, WildState5>(initialIVAdvances, maxIVAdvances, ivMap, generator, *currentProfile);
        }
    }
    else
    {
        searcher = new IVSearcher5<WildGenerator5, WildState5>(initialIVAdvances, maxIVAdvances, generator, *currentProfile);
    }

    searcher->setMaxProgress(searcher->getMaxProgress(start, end));

    QSettings settings;
    int threads = settings.value("settings/threads").toInt();

    auto *thread = QThread::create([=] { searcher->startSearch(threads, start, end); });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(ui->pushButtonCancel, &QPushButton::clicked, [searcher] { searcher->cancelSearch(); });

    auto *timer = new QTimer();
    connect(timer, &QTimer::timeout, this, [=] {
        auto results = searcher->getResults();
        std::erase_if(results, [=, this](const SearcherState5<WildState5> &result) {
            const auto &state = result.getState();
            return removeBySearcherFilters(state);
        });
        searcherModel->addItems(results);
        ui->progressBar->setValue(searcher->getProgress());
    });
    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed, this, [=] {
        ui->pushButtonSearch->setEnabled(true);
        ui->pushButtonCancel->setEnabled(false);
        auto results = searcher->getResults();
        std::erase_if(results, [=, this](const SearcherState5<WildState5> &result) {
            const auto &state = result.getState();
            return removeBySearcherFilters(state);
        });
        searcherModel->addItems(results);
        ui->progressBar->setValue(searcher->getProgress());
        delete searcher;
    });

    thread->start();
    timer->start(1000);
}

void Phenomenon::searcherEncounterIndexChanged(int index)
{
    if (index >= 0)
    {
        auto encounter = ui->comboBoxSearcherEncounter->getEnum<Encounter>();
        u16 currentLocation = ui->comboBoxSearcherLocation->getCurrentUShort();

        u8 season = ui->comboBoxSearcherSeason->currentIndex();
        encounterSearcher = Encounters5::getEncounters(encounter, season, currentProfile);

        std::vector<u16> locs;
        std::ranges::transform(encounterSearcher, std::back_inserter(locs), [](const EncounterArea5 &area) { return area.getLocation(); });

        ui->comboBoxSearcherLocation->clear();
        ui->comboBoxSearcherLocation->addItems(Translator::getLocations(locs, currentProfile->getVersion()), locs);
        ui->comboBoxSearcherLocation->setCurrentIndexByData(currentLocation);
    }
}

void Phenomenon::searcherFastSearchChanged()
{
    if (fastSearchEnabled())
    {
        if (shaCache && shaCache->isValid(*currentProfile))
        {
            ui->labelIVFastSearch->setText(tr("Settings are configured for fast IV/SHA searching"));
        }
        else
        {
            ui->labelIVFastSearch->setText(
                tr("Settings are configured for fast IV searching.\nProfile is missing or has an incompatible SHA cache."));
        }
    }
    else
    {
        if (ivCache == nullptr)
        {
            ui->labelIVFastSearch->setText(tr("Profile does not have a IV cache file configured"));
        }
        else
        {
            QStringList text
                = { tr("Settings are not configured for fast searching"),
                    tr("Keep initial/max advances below %1/%2").arg(ivCache->getInitialAdvances()).arg(ivCache->getMaxAdvances()),
                    tr("Ensure IV filters are set to common spreads") };
            ui->labelIVFastSearch->setText(text.join('\n'));
        }
    }
}

void Phenomenon::searcherLocationIndexChanged(int index)
{
    if (index >= 0)
    {
        auto &area = encounterSearcher[ui->comboBoxSearcherLocation->currentIndex()];
        auto species = area.getUniqueSpecies();
        auto names = area.getSpecieNames();

        ui->filterSearcher->setEncounterSlots(area.getCount());

        ui->comboBoxSearcherPokemon->clear();
        ui->comboBoxSearcherPokemon->addItem("-");
        for (size_t i = 0; i < species.size(); i++)
        {
            ui->comboBoxSearcherPokemon->addItem(QString::fromStdString(names[i]), species[i]);
        }

        updateItemFilter(labelSearcherItem, checkListSearcherItem, area, false);
    }
}

void Phenomenon::searcherPokemonIndexChanged(int index)
{
    if (index <= 0)
    {
        ui->filterSearcher->resetEncounterSlots();
        ui->spinBoxSearcherLevelMin->setValue(0);
        ui->spinBoxSearcherLevelMax->setValue(0);
        ui->filterSearcher->setLevelRange(1, 100);
    }
    else
    {
        u16 num = ui->comboBoxSearcherPokemon->getCurrentUShort();
        auto flags = encounterSearcher[ui->comboBoxSearcherLocation->currentIndex()].getSlots(num);
        ui->filterSearcher->toggleEncounterSlots(flags);

        auto range = encounterSearcher[ui->comboBoxSearcherLocation->currentIndex()].getLevelRange(num);
        ui->spinBoxSearcherLevelMin->setValue(range.first);
        ui->spinBoxSearcherLevelMax->setValue(range.second);
        ui->filterSearcher->setLevelRange(range.first, range.second);
    }
}

void Phenomenon::searcherSeasonIndexChanged(int index)
{
    if (index >= 0)
    {
        searcherEncounterIndexChanged(0);
    }
}

void Phenomenon::transferFilters(int index)
{
    if (index == 0)
    {
        ui->filterSearcher->copyFrom(ui->filterGenerator);
        checkListSearcherItem->setChecks(checkListGeneratorItem->getChecked());
    }
    else
    {
        ui->filterGenerator->copyFrom(ui->filterSearcher);
        checkListGeneratorItem->setChecks(checkListSearcherItem->getChecked());
    }
}

void Phenomenon::transferSettings(int index)
{
    if (index == 0)
    {
        ui->comboBoxSearcherEncounter->setCurrentIndex(ui->comboBoxGeneratorEncounter->currentIndex());
        ui->comboBoxSearcherLocation->setCurrentIndex(ui->comboBoxGeneratorLocation->currentIndex());
        ui->comboBoxSearcherPokemon->setCurrentIndex(ui->comboBoxGeneratorPokemon->currentIndex());
        ui->comboBoxSearcherSeason->setCurrentIndex(ui->comboBoxGeneratorSeason->currentIndex());
    }
    else
    {
        ui->comboBoxGeneratorEncounter->setCurrentIndex(ui->comboBoxSearcherEncounter->currentIndex());
        ui->comboBoxGeneratorLocation->setCurrentIndex(ui->comboBoxSearcherLocation->currentIndex());
        ui->comboBoxGeneratorPokemon->setCurrentIndex(ui->comboBoxSearcherPokemon->currentIndex());
        ui->comboBoxGeneratorSeason->setCurrentIndex(ui->comboBoxSearcherSeason->currentIndex());
    }
}
