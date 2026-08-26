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

#include "PokeRadar.hpp"

#include "PokeRadarTile.hpp"
#include "PokeRadarGrass.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen4/Encounters4.hpp>
#include <Core/Gen4/Generators/PokeRadarGenerator.hpp>
#include <Core/Gen4/Generators/PokeRadarGenerator.hpp>
#include <Core/Gen4/Profile4.hpp>
#include <Core/Gen4/Searchers/PokeRadarSearcher.hpp>
#include <Core/Util/Translator.hpp>
#include <Form/Controls/CheckList.hpp>
#include <Form/Controls/ComboBox.hpp>
#include <Form/Controls/ComboMenu.hpp>
#include <Form/Controls/ComboBoxProxy.hpp>
#include <Form/Controls/Controls.hpp>
#include <Form/Controls/Filter.hpp>
#include <Form/Controls/TableView.hpp>
#include <Form/Controls/TabWidget.hpp>
#include <Form/Controls/TextBox.hpp>
#include <Form/Gen4/Profile/ProfileDisplay4.hpp>
#include <Form/Gen4/Tools/SeedToTime4.hpp>
#include <Form/Util/AdvanceFinder.hpp>
#include <Model/Gen4/PokeRadarModel.hpp>
#include <Model/SortFilterProxyModel.hpp>
#include <QAction>
#include <QAbstractItemModel>
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QEvent>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>
#include <QItemSelectionModel>
#include <QProgressBar>
#include <QPushButton>
#include <QSettings>
#include <QSizePolicy>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QFrame>
#include <QThread>
#include <QTimer>
#include <QVBoxLayout>
#include <atomic>
#include <algorithm>
#include <iterator>
#include <memory>
#include <ranges>
#include <sstream>
#include <string>
#include <utility>
#include <unordered_map>

static const QString settingPrefix = QStringLiteral("pokeRadar");

static constexpr u16 trophyGardenLocation = 117;
static constexpr int maxPokeRadarChain = 40;

static constexpr std::array<u16, 46> radarLocations = {
    139, 8,   9,   137, 134, 134, 136, 13,  140, 141, 142, 144, 143, 146, 145, 147,
    148, 149, 150, 157, 156, 159, 158, 160, 161, 162, 162, 163, 164, 165, 166, 167,
    169, 170, 171, 172, 181, 173, 174, 175, 182, 59,  56,  117, 7,   138
};

static std::vector<EncounterArea4> getRadarEncounters(const std::vector<EncounterArea4> &encounters)
{
    std::vector<EncounterArea4> radarEncounters;
    for (u16 location : radarLocations)
    {
        auto area = std::ranges::find_if(encounters, [location](const EncounterArea4 &encounter) { return encounter.getLocation() == location; });
        if (area != encounters.end())
        {
            radarEncounters.emplace_back(*area);
        }
    }
    return radarEncounters;
}

static std::vector<std::string> getRadarLocationNames(const std::vector<EncounterArea4> &encounters, Game version)
{
    std::vector<u16> locs;
    std::ranges::transform(encounters, std::back_inserter(locs), [](const EncounterArea4 &area) { return area.getLocation(); });

    auto names = Translator::getLocations(locs, version);
    int lakeVerityCount = 0;
    int route213Count = 0;
    for (int i = 0; i < encounters.size() && i < names.size(); i++)
    {
        u16 location = encounters[i].getLocation();
        if (location == 134)
        {
            names[i] = lakeVerityCount++ == 0 ? "Lake Verity (Small field)" : "Lake Verity (Large field)";
        }
        else if (location == 162 && route213Count++ == 1)
        {
            names[i] += " (On the Hill)";
        }
    }

    return names;
}

static u8 getRadarLocationOccurrence(const std::vector<EncounterArea4> &encounters, int locationIndex)
{
    if (locationIndex < 0 || locationIndex >= encounters.size())
    {
        return 1;
    }

    u8 occurrence = 0;
    u16 location = encounters[locationIndex].getLocation();
    for (int i = 0; i <= locationIndex; i++)
    {
        if (encounters[i].getLocation() == location)
        {
            occurrence++;
        }
    }

    return std::max<u8>(occurrence, 1);
}

static u32 getPostBattlePatchAdvances(u32 battleAdvances, Game version)
{
    if ((version & Game::DP) != Game::None && battleAdvances >= 4)
    {
        battleAdvances -= 4;
    }

    return battleAdvances;
}

static u32 getPostBattlePatchTypeAdvances(Game version)
{
    return (version & Game::DP) != Game::None ? 4 : 0;
}

static int getGrassFieldIndex(const PokeRadarControls &controls, const QPoint &position)
{
    return position.y() * controls.fieldWidth + position.x();
}

static bool isValidGrassFieldPosition(const PokeRadarControls &controls, const QPoint &position)
{
    return position.x() >= 0 && position.x() < controls.fieldWidth && position.y() >= 0 && position.y() < controls.fieldHeight;
}

static bool isPokeRadarChainLead(Lead lead)
{
    return lead <= Lead::SynchronizeEnd || lead == Lead::CuteCharmF || lead == Lead::CuteCharmM;
}

static Lead getPokeRadarGeneratorLead(const PokeRadarControls &controls)
{
    if (controls.lead == nullptr)
    {
        return Lead::None;
    }

    Lead lead = controls.lead->getEnum<Lead>();
    if (controls.chainCount->value() == 0 || isPokeRadarChainLead(lead) || lead == Lead::CompoundEyes)
    {
        return lead;
    }

    return Lead::None;
}

static bool setComboBoxData(QComboBox *comboBox, const QVariant &data)
{
    if (comboBox == nullptr)
    {
        return false;
    }

    int index = comboBox->findData(data);
    if (index < 0)
    {
        return false;
    }

    comboBox->setCurrentIndex(index);
    return true;
}

static QAction *findComboMenuAction(QMenu *menu, int data)
{
    if (menu == nullptr)
    {
        return nullptr;
    }

    for (QAction *action : menu->actions())
    {
        if (action->menu() != nullptr)
        {
            if (QAction *child = findComboMenuAction(action->menu(), data))
            {
                return child;
            }
        }
        else if (action->data().toInt() == data)
        {
            return action;
        }
    }

    return nullptr;
}

static void setComboMenuData(ComboMenu *comboMenu, int data)
{
    if (comboMenu == nullptr)
    {
        return;
    }

    if (QAction *action = findComboMenuAction(comboMenu->menu(), data))
    {
        action->setChecked(true);
        action->trigger();
    }
}

static void transferLead(ComboMenu *target, ComboMenu *source, bool targetIsSearcher)
{
    if (target == nullptr || source == nullptr)
    {
        return;
    }

    Lead lead = source->getEnum<Lead>();
    int data = toInt(lead);
    if (targetIsSearcher && lead <= Lead::SynchronizeEnd)
    {
        data = toInt(Lead::Synchronize);
    }

    setComboMenuData(target, data);
}

static int getResultSort(PokeRadarResult result)
{
    switch (result)
    {
    case PokeRadarResult::ManualActivation:
        return 0;
    case PokeRadarResult::Capture:
        return 1;
    case PokeRadarResult::Defeat:
        return 2;
    }

    return 3;
}

static std::string getSearcherResultKey(const PokeRadarState &state)
{
    std::ostringstream stream;
    stream << state.getAdvances() << '|'
           << state.getPatchAdvances() << '|'
           << state.getDistance() << '|'
           << state.getChain() << '|'
           << static_cast<int>(state.getNoGraceSkip()) << '|'
           << static_cast<int>(state.getGraceSkip()) << '|'
           << state.hasDisplayPatchType() << '|'
           << state.getDisplayPatchStrong() << '|'
           << state.getDisplayPatchShiny() << '|'
           << state.getPatchesVisible() << '|'
           << state.getBattlePatchesVisible();

    for (u32 advance : state.getBattleStartAdvances())
    {
        stream << '|' << advance;
    }

    auto appendDisplayedPatches = [&stream, &state](const std::array<PokeRadarPatch, 4> &patches) {
        std::vector<PokeRadarPatch> displayedPatches;
        for (const auto &patch : patches)
        {
            if (patch.active && (!state.hasDisplayPatchType()
                                 || (patch.strong == state.getDisplayPatchStrong() && patch.shiny == state.getDisplayPatchShiny())))
            {
                displayedPatches.emplace_back(patch);
            }
        }
        std::ranges::sort(displayedPatches, {}, [](const PokeRadarPatch &patch) { return std::pair { patch.y, patch.x }; });
        for (const auto &patch : displayedPatches)
        {
            stream << '|' << static_cast<int>(patch.x) << ',' << static_cast<int>(patch.y);
        }
    };
    appendDisplayedPatches(state.getPatches());

    if (state.hasSearcherPokemon())
    {
        const auto &pokemon = state.getSearcherPokemon();
        stream << '|'
               << pokemon.getSeed() << '|'
               << pokemon.getPID() << '|'
               << static_cast<int>(pokemon.getAbility()) << '|'
               << static_cast<int>(pokemon.getAbilityIndex()) << '|'
               << static_cast<int>(pokemon.getGender()) << '|'
               << static_cast<int>(pokemon.getLevel()) << '|'
               << static_cast<int>(pokemon.getNature()) << '|'
               << static_cast<int>(pokemon.getShiny()) << '|'
               << static_cast<int>(pokemon.getEncounterSlot()) << '|'
               << pokemon.getItem() << '|'
               << pokemon.getSpecie() << '|'
               << static_cast<int>(pokemon.getForm());
        for (u8 iv : pokemon.getIVs())
        {
            stream << '|' << static_cast<int>(iv);
        }
    }

    return stream.str();
}

static PokeRadarTileMark getTileMark(const PokeRadarPatch &patch)
{
    return patch.shiny ? PokeRadarTileMark::Shiny : patch.strong ? PokeRadarTileMark::Strong : PokeRadarTileMark::Weak;
}

static std::optional<PokeRadarPatch> getPatchAt(const std::array<PokeRadarPatch, 4> &patches, u8 x, u8 y)
{
    auto patch = std::ranges::find_if(patches, [x, y](const PokeRadarPatch &patch) {
        return patch.active && patch.x == x && patch.y == y;
    });
    if (patch == patches.end())
    {
        return std::nullopt;
    }

    return *patch;
}

static std::vector<PokeRadarState> mergeSearcherActivationResults(
    const std::vector<std::pair<PokeRadarResult, std::vector<PokeRadarState>>> &results)
{
    std::vector<PokeRadarState> merged;
    std::unordered_map<std::string, size_t> indexByKey;
    for (const auto &[result, states] : results)
    {
        for (PokeRadarState state : states)
        {
            std::string key = getSearcherResultKey(state);
            auto [iter, inserted] = indexByKey.emplace(key, merged.size());
            if (inserted)
            {
                state.setResults({ result });
                state.setResultPatches(result, state.getPatches());
                merged.emplace_back(state);
            }
            else
            {
                merged[iter->second].addResult(result);
                merged[iter->second].setResultPatches(result, state.getPatches());
            }
        }
    }

    for (auto &state : merged)
    {
        auto results = state.getResults();
        std::ranges::sort(results, [](PokeRadarResult left, PokeRadarResult right) {
            return getResultSort(left) < getResultSort(right);
        });
        state.setResults(results);
    }

    return merged;
}

PokeRadar::PokeRadar(QWidget *parent) : QWidget(parent), currentProfile(nullptr)
{
    setAttribute(Qt::WA_QuitOnClose, false);
    setWindowTitle(tr("Gen 4 Poke Radar"));
    generator.grass.fill(nullptr);
    setupGrassField(generator, 36, 36);
    searcher.grass.fill(nullptr);
    setupGrassField(searcher, 9, 9);

    auto *mainLayout = new QGridLayout(this);

    profileDisplay = new ProfileDisplay4(this);
    profileDisplay->setup(settingPrefix, Game::DPPt);
    mainLayout->addWidget(profileDisplay, 0, 0);

    tabRNGSelector = new TabWidget(this);
    mainLayout->addWidget(tabRNGSelector, 1, 0);

    auto *generatorTab = new QWidget(this);
    auto *generatorLayout = new QGridLayout(generatorTab);
    generatorLayout->addWidget(createRNGInfo(generator, false), 0, 0);
    generatorLayout->addWidget(createSettings(generator, false), 0, 1);
    generatorLayout->addWidget(createGrassTiles(generator, false), 0, 2);
    generatorLayout->addWidget(createFilters(generator, false), 0, 3);
    generator.model = new PokeRadarModel4(generatorTab);
    generator.proxyModel = nullptr;
    generator.tableView = new TableView(generatorTab);
    generator.tableView->setModel(generator.model);
    QTimer::singleShot(0, this, [this] { generator.tableView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents); });
    connect(generator.filter, &Filter::showStatsChanged, generator.model, &PokeRadarModel4::setShowStats);
    auto *advanceFinder = generator.tableView->addAction(tr("Advance Finder"));
    connect(advanceFinder, &QAction::triggered, this, &PokeRadar::openAdvanceFinder);
    auto *jumpToBattleAdv = generator.tableView->addAction(tr("Jump to Battle Adv"));
    connect(jumpToBattleAdv, &QAction::triggered, this, &PokeRadar::jumpToBattleAdv);
    auto *markBattlePatches = generator.tableView->addAction(tr("Mark Battle Patches"));
    connect(markBattlePatches, &QAction::triggered, this, [this] { markSelectedPatches(generator, true); });
    auto *markManualPatches = generator.tableView->addAction(tr("Mark Manual Patches"));
    connect(markManualPatches, &QAction::triggered, this, [this] { markSelectedPatches(generator, false); });
    generatorLayout->addWidget(generator.tableView, 1, 0, 1, 4);
    generatorLayout->setColumnStretch(1, 1);
    generatorLayout->setColumnStretch(2, 0);
    generatorLayout->setColumnStretch(3, 1);
    generatorLayout->setRowStretch(1, 1);
    tabRNGSelector->addTab(generatorTab, tr("Generator"));

    auto *searcherTab = new QWidget(this);
    auto *searcherLayout = new QGridLayout(searcherTab);
    searcherLayout->addWidget(createRNGInfo(searcher, true), 0, 0);
    searcherLayout->addWidget(createSettings(searcher, true), 0, 1);
    searcherLayout->addWidget(createGrassTiles(searcher, true), 0, 2);
    searcherLayout->addWidget(createFilters(searcher, true), 0, 3);
    searcher.model = new PokeRadarModel4(searcherTab, true);
    searcher.proxyModel = new SortFilterProxyModel(searcherTab, searcher.model);
    searcher.tableView = new TableView(searcherTab);
    searcher.tableView->setModel(searcher.proxyModel);
    connect(searcher.filter, &Filter::showStatsChanged, searcher.model, &PokeRadarModel4::setShowStats);
    searcher.tableView->setSortingEnabled(true);
    searcher.tableView->horizontalHeader()->setSortIndicatorShown(true);
    auto *seedToTime = new QAction(tr("Generate times for seed"), searcher.tableView);
    connect(seedToTime, &QAction::triggered, this, &PokeRadar::seedToTime);
    searcher.tableView->addAction(seedToTime);
    auto *markPatches = new QAction(tr("Mark Patches"), searcher.tableView);
    connect(markPatches, &QAction::triggered, this, &PokeRadar::markSearcherPatches);
    searcher.tableView->addAction(markPatches);
    searcher.progressBar = new QProgressBar(searcherTab);
    searcher.progressBar->setValue(0);
    searcherLayout->addWidget(searcher.progressBar, 1, 0, 1, 4);
    searcherLayout->addWidget(searcher.tableView, 2, 0, 1, 4);
    searcherLayout->setColumnStretch(1, 1);
    searcherLayout->setColumnStretch(2, 0);
    searcherLayout->setColumnStretch(3, 1);
    searcherLayout->setRowStretch(2, 1);
    tabRNGSelector->addTab(searcherTab, tr("Searcher"));

    connect(profileDisplay, &ProfileDisplay4::profileChanged, this, &PokeRadar::profileChanged);
    connect(profileDisplay, &ProfileDisplay4::profilesChanged, this, &PokeRadar::profilesChanged);
    connect(tabRNGSelector, &TabWidget::transferFilters, this, &PokeRadar::transferFilters);
    connect(tabRNGSelector, &TabWidget::transferSettings, this, &PokeRadar::transferSettings);
    connect(generator.location, &QComboBox::currentIndexChanged, this, [this] { updatePokemon(generator, generatorEncounters); });
    connect(searcher.location, &QComboBox::currentIndexChanged, this, [this] { updatePokemon(searcher, searcherEncounters); });
    connect(generator.pokemon, &QComboBox::currentIndexChanged, this, [this](int) { updateEncounterSlots(generator, generatorEncounters); });
    connect(searcher.pokemon, &QComboBox::currentIndexChanged, this, [this](int) { updateEncounterSlots(searcher, searcherEncounters); });
    connect(generator.slot, &QComboBox::currentIndexChanged, this, [this](int) { updateEncounterSlots(generator, generatorEncounters); });
    connect(searcher.slot, &QComboBox::currentIndexChanged, this, [this](int) { updateEncounterSlots(searcher, searcherEncounters); });
    connect(generator.time, &QComboBox::currentIndexChanged, this, [this] { updateEncounters(generator, generatorEncounters); });
    connect(searcher.time, &QComboBox::currentIndexChanged, this, [this] { updateEncounters(searcher, searcherEncounters); });
    connect(generator.chainType, &QComboBox::currentIndexChanged, this, [this] {
        QSignalBlocker locationBlocker(generator.location);
        QSignalBlocker pokemonBlocker(generator.pokemon);
        updateEncounters(generator, generatorEncounters, false);
    });
    connect(searcher.patchTypes, &QComboBox::currentIndexChanged, this, [this] {
        updateEncounters(searcher, searcherEncounters);
    });
    connect(generator.swarm, &QCheckBox::stateChanged, this, [this] { updateEncounters(generator, generatorEncounters); });
    connect(searcher.swarm, &QCheckBox::stateChanged, this, [this] { updateEncounters(searcher, searcherEncounters); });
    connect(generator.dualSlot, &QCheckBox::stateChanged, this, [this] { updateEncounters(generator, generatorEncounters); });
    connect(searcher.dualSlot, &QCheckBox::stateChanged, this, [this] { updateEncounters(searcher, searcherEncounters); });
    connect(generator.dualSlotGame, &QComboBox::currentIndexChanged, this, [this] { updateEncounters(generator, generatorEncounters); });
    connect(searcher.dualSlotGame, &QComboBox::currentIndexChanged, this, [this] { updateEncounters(searcher, searcherEncounters); });
    connect(generator.replacement, &QCheckBox::stateChanged, this, [this] { updateEncounters(generator, generatorEncounters); });
    connect(searcher.replacement, &QCheckBox::stateChanged, this, [this] { updateEncounters(searcher, searcherEncounters); });
    connect(generator.replacement0, &QComboBox::currentIndexChanged, this, [this] {
        if (generator.replacement->isChecked())
        {
            updateEncounters(generator, generatorEncounters);
        }
    });
    connect(generator.replacement1, &QComboBox::currentIndexChanged, this, [this] {
        if (generator.replacement->isChecked())
        {
            updateEncounters(generator, generatorEncounters);
        }
    });
    connect(searcher.replacement0, &QComboBox::currentIndexChanged, this, [this] {
        if (searcher.replacement->isChecked())
        {
            updateEncounters(searcher, searcherEncounters);
        }
    });
    connect(searcher.replacement1, &QComboBox::currentIndexChanged, this, [this] {
        if (searcher.replacement->isChecked())
        {
            updateEncounters(searcher, searcherEncounters);
        }
    });

    QSettings setting;
    if (setting.contains(settingPrefix + "/geometry"))
    {
        restoreGeometry(setting.value(settingPrefix + "/geometry").toByteArray());
    }
    else
    {
        setWindowState(windowState() | Qt::WindowMaximized);
    }

    profileDisplay->updateProfiles();
}

PokeRadar::~PokeRadar()
{
    QSettings setting;
    setting.setValue(settingPrefix + "/geometry", saveGeometry());
}

bool PokeRadar::hasProfiles() const
{
    return currentProfile != nullptr;
}

bool PokeRadar::eventFilter(QObject *object, QEvent *event)
{
    if (auto *tile = qobject_cast<PokeRadarTile *>(object))
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            auto *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton)
            {
                bool searcherTile = tile->property("radarSearcher").toBool();
                int x = tile->property("radarColumn").toInt();
                int y = tile->property("radarRow").toInt();
                grassPaintValue = !tile->hasGrass();
                paintingGrass = true;
                tile->setGrass(grassPaintValue);
                storeGrassTile(searcherTile ? searcher : generator, x, y, grassPaintValue);
                return true;
            }
            if (mouseEvent->button() == Qt::RightButton && !tile->property("radarSearcher").toBool() && generator.currentPosition
                && !(tile->property("radarColumn").toInt() == 4 && tile->property("radarRow").toInt() == 4)
                && (tile->hasGrass() || tile->hasMark()))
            {
                int column = tile->property("radarColumn").toInt();
                int row = tile->property("radarRow").toInt();
                QString coordinate = QString("%1%2").arg(QChar('A' + column)).arg(row);
                QMenu menu(this);
                QAction *action = menu.addAction(tr("Mark %1 as new Position").arg(coordinate));
                if (menu.exec(mouseEvent->globalPosition().toPoint()) == action)
                {
                    moveGeneratorPositionToVisibleTile(column, row);
                }
                return true;
            }
        }
        else if (event->type() == QEvent::Enter && paintingGrass)
        {
            if (QApplication::mouseButtons() & Qt::LeftButton)
            {
                tile->setGrass(grassPaintValue);
                storeGrassTile(tile->property("radarSearcher").toBool() ? searcher : generator, tile->property("radarColumn").toInt(),
                               tile->property("radarRow").toInt(), grassPaintValue);
            }
            else
            {
                paintingGrass = false;
            }
            return true;
        }
        else if (event->type() == QEvent::MouseMove && paintingGrass)
        {
            auto *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->buttons() & Qt::LeftButton)
            {
                if (auto *hoveredTile = qobject_cast<PokeRadarTile *>(QApplication::widgetAt(mouseEvent->globalPosition().toPoint())))
                {
                    hoveredTile->setGrass(grassPaintValue);
                    storeGrassTile(hoveredTile->property("radarSearcher").toBool() ? searcher : generator,
                                   hoveredTile->property("radarColumn").toInt(), hoveredTile->property("radarRow").toInt(), grassPaintValue);
                }
                return true;
            }

            paintingGrass = false;
        }
        else if (event->type() == QEvent::MouseButtonRelease)
        {
            paintingGrass = false;
        }
    }
    else if (auto *label = qobject_cast<QLabel *>(object); event->type() == QEvent::MouseButtonPress && label != nullptr)
    {
        auto *mouseEvent = static_cast<QMouseEvent *>(event);
        if (mouseEvent->button() == Qt::LeftButton)
        {
            if (label->property("radarColumn").isValid())
            {
                if (label->property("radarSearcher").toBool())
                {
                    toggleGrassColumn(searcher, label->property("radarColumn").toInt());
                }
                else
                {
                    toggleGrassColumn(generator, label->property("radarColumn").toInt());
                }
                return true;
            }
            if (label->property("radarRow").isValid())
            {
                if (label->property("radarSearcher").toBool())
                {
                    toggleGrassRow(searcher, label->property("radarRow").toInt());
                }
                else
                {
                    toggleGrassRow(generator, label->property("radarRow").toInt());
                }
                return true;
            }
        }
    }

    return QWidget::eventFilter(object, event);
}

void PokeRadar::updateProfiles()
{
    profileDisplay->updateProfiles();
}

void PokeRadar::transferFilters(int index)
{
    if (index == 0)
    {
        searcher.filter->copyFrom(generator.filter);
        searcher.filter->disableControls(Controls::DisableFilter);
    }
    else
    {
        generator.filter->copyFrom(searcher.filter);
    }
}

void PokeRadar::transferSettings(int index)
{
    PokeRadarControls &source = index == 0 ? generator : searcher;
    PokeRadarControls &target = index == 0 ? searcher : generator;
    std::vector<EncounterArea4> &targetEncounters = index == 0 ? searcherEncounters : generatorEncounters;

    target.time->setCurrentIndex(source.time->currentIndex());
    target.dualSlot->setCheckState(source.dualSlot->checkState());
    target.dualSlotGame->setCurrentIndex(source.dualSlotGame->currentIndex());
    target.swarm->setCheckState(source.swarm->checkState());

    target.location->setCurrentIndex(source.location->currentIndex());
    target.pokemon->setCurrentIndex(source.pokemon->currentIndex());
    target.slot->setCurrentIndex(source.slot->currentIndex());

    updateEncounterSlots(target, targetEncounters);
}

EncounterSettings4 PokeRadar::getEncounterSettings(const PokeRadarControls &controls, bool radar) const
{
    EncounterSettings4 settings = {};
    settings.dppt.dual = controls.dualSlot->isChecked() ? controls.dualSlotGame->getEnum<Game>() : Game::None;
    settings.dppt.replacement = {};
    if (controls.replacement->isChecked() && controls.location->getCurrentUShort() == trophyGardenLocation)
    {
        settings.dppt.replacement[0] = controls.replacement0->getCurrentUShort();
        settings.dppt.replacement[1] = controls.replacement1->count() > 0 ? controls.replacement1->getCurrentUShort() : 0;
    }
    settings.dppt.feebasTile = false;
    settings.dppt.radar = radar;
    settings.time = controls.time->currentIndex();
    settings.swarm = controls.swarm->isChecked();
    return settings;
}

QGroupBox *PokeRadar::createFilters(PokeRadarControls &controls, bool searcherTab)
{
    auto *filters = new QGroupBox(tr("Filters"), this);
    auto *layout = new QGridLayout(filters);

    controls.patchFilter = nullptr;
    controls.filter = new Filter(filters);
    controls.filter->disableControls(Controls::Height | Controls::Weight);
    if (searcherTab)
    {
        controls.filter->disableControls(Controls::DisableFilter);
    }

    if (!searcherTab)
    {
        controls.patchFilter = new CheckList(controls.filter);
        controls.patchFilter->addItem(tr("None"));
        controls.patchFilter->addItem(tr("Weak"));
        controls.patchFilter->addItem(tr("Strong"));
        controls.patchFilter->addItem(tr("Shiny"));
        controls.filter->addCustomControlAfterShiny(tr("Patch"), controls.patchFilter);

        layout->addWidget(controls.filter, 0, 0);
    }
    else
    {
        layout->addWidget(controls.filter, 0, 0);
    }
    return filters;
}

QGroupBox *PokeRadar::createRNGInfo(PokeRadarControls &controls, bool searcherTab)
{
    auto *rngInfo = new QGroupBox(tr("RNG Info"), this);
    auto *layout = new QGridLayout(rngInfo);
    rngInfo->setMaximumWidth(260);

    controls.seed = nullptr;
    controls.minDelay = nullptr;
    controls.maxDelay = nullptr;
    controls.initialAdvances = new TextBox(rngInfo);
    controls.initialAdvances->setValues(InputType::Advance32Bit);
    controls.initialAdvances->setText(QStringLiteral("0"));
    controls.maxAdvances = new TextBox(rngInfo);
    controls.maxAdvances->setValues(InputType::Advance32Bit);
    controls.maxAdvances->setText(QStringLiteral("1000"));
    controls.lead = nullptr;
    auto setupLead = [this](ComboMenu *lead, bool generatorTab) {
        lead->addAction(tr("None"), toInt(Lead::None));
        lead->addAction(tr("Compound Eyes"), toInt(Lead::CompoundEyes));
        lead->addMenu(tr("Cute Charm"),
                      { { tr("Cute Charm: \u2640 Lead"), toInt(Lead::CuteCharmF) },
                        { tr("Cute Charm: \u2642 Lead"), toInt(Lead::CuteCharmM) } });
        lead->addMenu(tr("Level Modifier"),
                      { { tr("Hustle"), toInt(Lead::Hustle) },
                        { tr("Pressure"), toInt(Lead::Pressure) },
                        { tr("Vital Spirit"), toInt(Lead::VitalSpirit) } });
        lead->addMenu(tr("Slot Modifier"), { { tr("Magnet Pull"), toInt(Lead::MagnetPull) }, { tr("Static"), toInt(Lead::Static) } });
        if (generatorTab)
        {
            lead->addMenu(tr("Synchronize"), Translator::getNatures());
        }
        else
        {
            lead->addAction(tr("Synchronize"), toInt(Lead::Synchronize));
        }
    };

    if (searcherTab)
    {
        controls.minDelay = new TextBox(rngInfo);
        controls.minDelay->setValues(InputType::Delay);
        controls.minDelay->setText(QStringLiteral("600"));
        controls.maxDelay = new TextBox(rngInfo);
        controls.maxDelay->setValues(InputType::Delay);
        controls.maxDelay->setText(QStringLiteral("3000"));
        controls.lead = new ComboMenu(rngInfo);
        setupLead(controls.lead, false);
    }
    else
    {
        controls.seed = new TextBox(rngInfo);
        controls.seed->setValues(InputType::Seed32Bit);
        controls.lead = new ComboMenu(rngInfo);
        setupLead(controls.lead, true);
    }
    controls.minPatchDistance = nullptr;
    if (searcherTab)
    {
        controls.minPatchDistance = new QSpinBox(rngInfo);
        controls.minPatchDistance->setRange(0, 999999);
        controls.minPatchDistance->setValue(20);
    }
    controls.chainCount = new QSpinBox(rngInfo);
    controls.chainCount->setRange(0, 999);
    controls.chainCount->setKeyboardTracking(false);
    controls.chainType = nullptr;
    if (!searcherTab)
    {
        controls.chainType = new ComboBox(rngInfo);
        controls.chainType->addItem(tr("Weak"), static_cast<int>(PokeRadarChainType::Weak));
        controls.chainType->addItem(tr("Strong"), static_cast<int>(PokeRadarChainType::Strong));
        controls.chainType->addItem(tr("Weak Shiny"), static_cast<int>(PokeRadarChainType::WeakShiny));
        controls.chainType->addItem(tr("Strong Shiny"), static_cast<int>(PokeRadarChainType::StrongShiny));
    }
    controls.result = nullptr;
    controls.results = nullptr;

    if (searcherTab)
    {
        layout->addWidget(new QLabel(tr("Lead"), rngInfo), 0, 0);
        layout->addWidget(controls.lead, 0, 1);
        layout->addWidget(new QLabel(tr("Min Delay"), rngInfo), 1, 0);
        layout->addWidget(controls.minDelay, 1, 1);
        layout->addWidget(new QLabel(tr("Max Delay"), rngInfo), 2, 0);
        layout->addWidget(controls.maxDelay, 2, 1);
    }
    else
    {
        layout->addWidget(new QLabel(tr("Lead"), rngInfo), 0, 0);
        layout->addWidget(controls.lead, 0, 1);
        layout->addWidget(new QLabel(tr("Seed"), rngInfo), 1, 0);
        layout->addWidget(controls.seed, 1, 1);
    }
    layout->addWidget(new QLabel(searcherTab ? tr("Min Advance") : tr("Initial Advances"), rngInfo), searcherTab ? 3 : 2, 0);
    layout->addWidget(controls.initialAdvances, searcherTab ? 3 : 2, 1);
    layout->addWidget(new QLabel(searcherTab ? tr("Max Advance") : tr("Max Advances"), rngInfo), searcherTab ? 4 : 3, 0);
    layout->addWidget(controls.maxAdvances, searcherTab ? 4 : 3, 1);
    layout->addWidget(new QLabel(searcherTab ? tr("Max Chain") : tr("Chain"), rngInfo), 5, 0);
    layout->addWidget(controls.chainCount, 5, 1);
    if (!searcherTab)
    {
        layout->addWidget(new QLabel(tr("Chain Type"), rngInfo), 6, 0);
        layout->addWidget(controls.chainType, 6, 1);
    }
    else
    {
        layout->addWidget(new QLabel(tr("Min Patch Distance"), rngInfo), 6, 0);
        layout->addWidget(controls.minPatchDistance, 6, 1);
    }

    controls.button = new QPushButton(searcherTab ? tr("Search") : tr("Generate"), rngInfo);
    controls.cancel = nullptr;
    controls.progressBar = nullptr;
    if (searcherTab)
    {
        controls.cancel = new QPushButton(tr("Cancel"), rngInfo);
        controls.cancel->setEnabled(false);
        layout->addWidget(controls.button, 7, 0);
        layout->addWidget(controls.cancel, 7, 1);
    }
    else
    {
        controls.cancel = nullptr;
        controls.progressBar = nullptr;
        layout->addWidget(controls.button, 7, 0, 1, 2);
    }

    auto *control = &controls;
    connect(controls.chainCount, qOverload<int>(&QSpinBox::valueChanged), this,
            [this, control, searcherTab](int value) {
                if (value > maxPokeRadarChain)
                {
                    control->chainCount->setValue(maxPokeRadarChain);
                    return;
                }

                if (searcherTab && control->slot != nullptr)
                {
                    control->slot->setEnabled(value != 0);
                    control->pokemon->setEnabled(value == 0);
                    if (control->result != nullptr)
                    {
                        control->result->setEnabled(value != 0);
                        if (value == 0)
                        {
                            control->result->setCurrentIndex(control->result->findData(static_cast<int>(PokeRadarResult::ManualActivation)));
                        }
                    }
                    if (control->results != nullptr)
                    {
                        control->results->setEnabled(value != 0);
                        if (value == 0)
                        {
                            control->results->setChecks(std::vector<bool> { false, false, false });
                        }
                    }
                    updateSearcherPatchTypes();
                    updateEncounters(searcher, searcherEncounters);
                }
            });
    connect(controls.button, &QPushButton::clicked, this, searcherTab ? &PokeRadar::search : &PokeRadar::generate);

    return rngInfo;
}

QGroupBox *PokeRadar::createSettings(PokeRadarControls &controls, bool searcherTab)
{
    auto *settings = new QGroupBox(tr("Settings"), this);
    auto *layout = new QGridLayout(settings);

    controls.location = new ComboBoxProxy(settings);
    controls.location->enableAutoComplete();
    controls.pokemon = new ComboBox(settings);
    controls.time = new ComboBox(settings);
    controls.time->addItem(tr("Day"), 0);
    controls.time->addItem(tr("Night"), 1);
    controls.time->addItem(tr("Morning"), 2);
    controls.replacement = new QCheckBox(tr("Replacement"), settings);
    controls.replacement0 = new ComboBox(settings);
    controls.replacement1 = new ComboBox(settings);
    controls.replacement->setVisible(false);
    controls.replacement0->setVisible(false);
    controls.replacement1->setVisible(false);
    controls.dualSlot = new QCheckBox(tr("Dual Slot"), settings);
    controls.dualSlotGame = new ComboBox(settings);
    controls.dualSlotGame->addItem(tr("Ruby"), static_cast<int>(Game::Ruby));
    controls.dualSlotGame->addItem(tr("Sapphire"), static_cast<int>(Game::Sapphire));
    controls.dualSlotGame->addItem(tr("Emerald"), static_cast<int>(Game::Emerald));
    controls.dualSlotGame->addItem(tr("Fire Red"), static_cast<int>(Game::FireRed));
    controls.dualSlotGame->addItem(tr("Leaf Green"), static_cast<int>(Game::LeafGreen));
    controls.swarm = new QCheckBox(tr("Swarm"), settings);
    controls.slot = new ComboBox(settings);
    controls.patchTypes = nullptr;
    if (!searcherTab)
    {
        controls.result = new ComboBox(settings);
        controls.result->addItem(tr("Capture"), static_cast<int>(PokeRadarResult::Capture));
        controls.result->addItem(tr("Defeat"), static_cast<int>(PokeRadarResult::Defeat));
        controls.result->setCurrentIndex(controls.result->findData(static_cast<int>(PokeRadarResult::Defeat)));
    }
    else
    {
        controls.patchTypes = new ComboBox(settings);
        controls.patchTypes->addItem(tr("Weak"), static_cast<int>(PokeRadarChainType::Weak));
        controls.patchTypes->addItem(tr("Strong"), static_cast<int>(PokeRadarChainType::Strong));
        controls.results = new CheckList(settings);
        controls.results->addItem(tr("Manual"), static_cast<int>(PokeRadarResult::ManualActivation));
        controls.results->addItem(tr("Capture"), static_cast<int>(PokeRadarResult::Capture));
        controls.results->addItem(tr("Defeat"), static_cast<int>(PokeRadarResult::Defeat));
        controls.results->setChecks(std::vector<bool> { false, false, false });
        controls.results->setEnabled(controls.chainCount->value() != 0);
    }
    controls.levelMin = new QSpinBox(settings);
    controls.levelMin->setRange(0, 100);
    controls.levelMin->setEnabled(false);
    controls.levelMax = new QSpinBox(settings);
    controls.levelMax->setRange(0, 100);
    controls.levelMax->setEnabled(false);
    controls.minimumGraceSteps = new QSpinBox(settings);
    controls.minimumGraceSteps->setRange(0, 8);
    controls.minimumGraceSteps->setEnabled(false);
    controls.minimumGraceSteps->setValue(0);

    layout->addWidget(new QLabel(tr("Location"), settings), 0, 0);
    layout->addWidget(controls.location, 0, 1, 1, 3);
    layout->addWidget(new QLabel(tr("Pokémon"), settings), 1, 0);
    layout->addWidget(controls.pokemon, 1, 1, 1, 3);
    layout->addWidget(new QLabel(tr("Time"), settings), 2, 0);
    layout->addWidget(controls.time, 2, 1, 1, 3);
    layout->addWidget(controls.dualSlot, 3, 0);
    layout->addWidget(controls.dualSlotGame, 3, 1);
    layout->addWidget(controls.swarm, 3, 2);
    layout->addWidget(controls.replacement, 4, 0);
    layout->addWidget(controls.replacement0, 4, 1);
    layout->addWidget(controls.replacement1, 4, 2);
    layout->addWidget(new QLabel(tr("Chain Slot"), settings), 5, 0);
    layout->addWidget(controls.slot, 5, 1, 1, 3);
    if (searcherTab)
    {
        controls.slot->setEnabled(controls.chainCount->value() != 0);
        controls.pokemon->setEnabled(controls.chainCount->value() == 0);
    }
    else
    {
        layout->addWidget(new QLabel(tr("Activation"), settings), 6, 0);
        layout->addWidget(controls.result, 6, 1, 1, 3);
    }

    auto *line = new QFrame(settings);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    if (searcherTab)
    {
        layout->addWidget(new QLabel(tr("Patch"), settings), 6, 0);
        layout->addWidget(controls.patchTypes, 6, 1);
        layout->addWidget(new QLabel(tr("Activation"), settings), 6, 2, Qt::AlignRight);
        layout->addWidget(controls.results, 6, 3);
        layout->addWidget(line, 7, 0, 1, 4);
    }
    else
    {
        layout->addWidget(line, 7, 0, 1, 4);
    }

    int levelRow = 8;
    layout->addWidget(new QLabel(tr("Levels"), settings), levelRow, 0);
    layout->addWidget(controls.levelMin, levelRow, 1);
    layout->addWidget(controls.levelMax, levelRow, 2);
    layout->addWidget(new QLabel(tr("Grace Steps"), settings), levelRow + 1, 0);
    layout->addWidget(controls.minimumGraceSteps, levelRow + 1, 1);
    layout->setRowStretch(levelRow + 2, 1);

    return settings;
}

QGroupBox *PokeRadar::createGrassTiles(PokeRadarControls &controls, bool searcherTab)
{
    auto *grassBox = new QGroupBox(tr("Grass Tiles"), this);
    auto *layout = new QVBoxLayout(grassBox);

    auto *grassWidget = new QWidget(grassBox);
    auto *grassLayout = new QGridLayout(grassWidget);
    grassLayout->setContentsMargins(4, 4, 4, 4);
    grassLayout->setHorizontalSpacing(1);
    grassLayout->setVerticalSpacing(1);
    grassLayout->setColumnMinimumWidth(0, 14);
    for (int i = 1; i <= 9; i++)
    {
        grassLayout->setColumnMinimumWidth(i, 22);
        grassLayout->setColumnStretch(i, 0);
    }
    for (int i = 1; i <= 9; i++)
    {
        grassLayout->setRowMinimumHeight(i, 22);
        grassLayout->setRowStretch(i, 0);
    }

    setupGrassGrid(controls, grassLayout);
    grassWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    layout->addWidget(grassWidget, 0, Qt::AlignHCenter | Qt::AlignTop);

    auto *reset = new QPushButton(tr("Reset"), grassBox);
    QPushButton *removeMarking = nullptr;
    if (!searcherTab)
    {
        auto *buttonLayout = new QHBoxLayout;
        controls.choosePosition = new QPushButton(tr("Choose Position"), grassBox);
        removeMarking = new QPushButton(tr("Remove marking"), grassBox);
        buttonLayout->addWidget(controls.choosePosition);
        buttonLayout->addWidget(removeMarking);
        layout->addLayout(buttonLayout);
        layout->addWidget(reset);
    }
    else
    {
        layout->addWidget(reset);
        controls.choosePosition = nullptr;
    }
    layout->addStretch(1);

    auto *control = &controls;
    connect(reset, &QPushButton::clicked, this, [this, control] { resetGrass(*control); });
    if (removeMarking != nullptr)
    {
        connect(removeMarking, &QPushButton::clicked, this, [this] { clearGrassMarks(); });
    }
    if (!searcherTab)
    {
        connect(controls.choosePosition, &QPushButton::clicked, this, &PokeRadar::choosePosition);
        setGeneratorPosition(QPoint(controls.fieldWidth / 2, controls.fieldHeight / 2), true);
    }

    grassBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    return grassBox;
}

std::array<bool, 81> PokeRadar::getGrass(const PokeRadarControls &controls) const
{
    std::array<bool, 81> tiles;
    for (size_t i = 0; i < controls.grass.size(); i++)
    {
        tiles[i] = controls.grass[i]->hasGrass();
    }
    tiles[4 * 9 + 4] = false;
    return tiles;
}

bool PokeRadar::getSelectedRadar(const PokeRadarControls &controls) const
{
    if (controls.patchTypes != nullptr)
    {
        return getSelectedRadar(controls.patchTypes->getEnum<PokeRadarChainType>());
    }

    if (controls.chainType == nullptr)
    {
        return false;
    }

    return getSelectedRadar(controls.chainType->getEnum<PokeRadarChainType>());
}

bool PokeRadar::getSelectedRadar(PokeRadarChainType chainType) const
{
    return chainType == PokeRadarChainType::Strong || chainType == PokeRadarChainType::StrongShiny;
}

bool PokeRadar::getSelectedShiny(const PokeRadarControls &controls) const
{
    if (controls.patchTypes != nullptr)
    {
        return getSelectedShiny(controls.patchTypes->getEnum<PokeRadarChainType>());
    }

    if (controls.chainType == nullptr)
    {
        return false;
    }

    return getSelectedShiny(controls.chainType->getEnum<PokeRadarChainType>());
}

bool PokeRadar::getSelectedShiny(PokeRadarChainType chainType) const
{
    return chainType == PokeRadarChainType::WeakShiny || chainType == PokeRadarChainType::StrongShiny;
}

bool PokeRadar::matchesPatchFilter(const PokeRadarControls &controls, const PokeRadarState &state) const
{
    if (&controls != &generator || controls.patchFilter == nullptr || controls.filter->getDisableFilters())
    {
        return true;
    }

    auto checked = controls.patchFilter->getChecked();
    if (checked.size() < 4)
    {
        return true;
    }

    auto matchesPatches = [&checked](const std::array<PokeRadarPatch, 4> &patches, bool visible) -> bool {
        if (!visible)
        {
            return checked[0] != 0;
        }

        bool hasActivePatch = false;
        for (const auto &patch : patches)
        {
            if (!patch.active)
            {
                continue;
            }

            hasActivePatch = true;
            if ((checked[1] && !patch.strong && !patch.shiny) || (checked[2] && patch.strong && !patch.shiny)
                || (checked[3] && patch.shiny))
            {
                return true;
            }
        }

        return checked[0] && !hasActivePatch;
    };

    if (matchesPatches(state.getPatches(), state.getPatchesVisible()))
    {
        return true;
    }

    return matchesPatches(state.getBattlePatches(), state.getBattlePatchesVisible());
}

void PokeRadar::clearGrassMarks()
{
    clearGrassMarks(generator);
}

void PokeRadar::clearGrassMarks(const PokeRadarControls &controls)
{
    for (auto *tile : generator.grass)
    {
        if (tile != nullptr)
        {
            tile->clearMark();
        }
    }

    if (&controls == &searcher)
    {
        for (auto *tile : searcher.grass)
        {
            if (tile != nullptr)
            {
                tile->clearMark();
            }
        }
    }
}

void PokeRadar::jumpToBattleAdv()
{
    QModelIndex index = generator.tableView->currentIndex();
    if (!index.isValid())
    {
        return;
    }

    const PokeRadarState &state = generator.model->getItem(index.row());
    if (!state.hasPokemon())
    {
        return;
    }

    u32 battleAdvances = state.getDisplayedBattleAdvances();
    for (int row = 0; row < generator.model->rowCount(); row++)
    {
        if (generator.model->getItem(row).getAdvances() == battleAdvances)
        {
            QModelIndex target = generator.model->index(row, 0);
            generator.tableView->setCurrentIndex(target);
            generator.tableView->selectionModel()->select(target, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Current);
            generator.tableView->scrollTo(target, QAbstractItemView::PositionAtCenter);
            return;
        }
    }
}

void PokeRadar::markSelectedPatches(const PokeRadarControls &controls, bool battle)
{
    QModelIndex index = controls.tableView->currentIndex();
    if (!index.isValid())
    {
        return;
    }

    bool forceManualChainStart = !battle && controls.chainCount->value() == 0;
    markPatches(controls.model->getItem(index.row()), battle, battle || controls.chainCount->value() != 0 || forceManualChainStart,
                forceManualChainStart ? std::make_optional(controls.chainType->getEnum<PokeRadarChainType>()) : std::nullopt);
}

void PokeRadar::markSearcherPatches()
{
    QModelIndex index = searcher.tableView->currentIndex();
    if (!index.isValid())
    {
        return;
    }

    index = searcher.proxyModel->mapToSource(index);
    const PokeRadarState &state = searcher.model->getItem(index.row());
    clearGrassMarks(searcher);

    auto getStoredPatches = [&state](PokeRadarResult result) -> const std::array<PokeRadarPatch, 4> & {
        const auto &patches = state.getResultPatches(result);
        return patches ? *patches : state.getPatches();
    };

    bool hasCapture = std::ranges::find(state.getResults(), PokeRadarResult::Capture) != state.getResults().end();
    bool hasDefeat = std::ranges::find(state.getResults(), PokeRadarResult::Defeat) != state.getResults().end();
    if (hasCapture && hasDefeat)
    {
        const auto &capturePatches = getStoredPatches(PokeRadarResult::Capture);
        const auto &defeatPatches = getStoredPatches(PokeRadarResult::Defeat);
        for (u8 y = 0; y < 9; y++)
        {
            for (u8 x = 0; x < 9; x++)
            {
                auto capturePatch = getPatchAt(capturePatches, x, y);
                auto defeatPatch = getPatchAt(defeatPatches, x, y);
                if (!capturePatch && !defeatPatch)
                {
                    continue;
                }

                auto *tile = searcher.grass[y * 9 + x];
                if ((capturePatch && capturePatch->shiny) || (defeatPatch && defeatPatch->shiny))
                {
                    tile->setMark(PokeRadarTileMark::Shiny);
                }
                else if (capturePatch && defeatPatch)
                {
                    PokeRadarTileMark captureMark = getTileMark(*capturePatch);
                    PokeRadarTileMark defeatMark = getTileMark(*defeatPatch);
                    if (captureMark == defeatMark)
                    {
                        tile->setMark(captureMark);
                    }
                    else
                    {
                        tile->setSplitMark(defeatMark, captureMark);
                    }
                }
                else
                {
                    tile->setMark(capturePatch ? getTileMark(*capturePatch) : getTileMark(*defeatPatch));
                }
            }
        }
        return;
    }

    PokeRadarResult result = state.getResults().empty() ? PokeRadarResult::ManualActivation : state.getResults().front();
    const auto &patches = getStoredPatches(result);
    for (const auto &patch : patches)
    {
        if (patch.active)
        {
            searcher.grass[patch.y * 9 + patch.x]->setMark(getTileMark(patch));
        }
    }
}

void PokeRadar::markPatches(const PokeRadarState &state, bool battle, bool showContinue, std::optional<PokeRadarChainType> forceContinueChainType)
{
    clearGrassMarks();
    bool visible = battle ? state.getBattlePatchesVisible() : state.getPatchesVisible();
    if (!visible)
    {
        return;
    }

    const auto &patches = battle ? state.getBattlePatches() : state.getPatches();
    auto matchesForceType = [this, forceContinueChainType](const PokeRadarPatch &patch) {
        return forceContinueChainType
            && patch.strong == getSelectedRadar(*forceContinueChainType)
            && patch.shiny == getSelectedShiny(*forceContinueChainType);
    };
    for (const auto &patch : patches)
    {
        if (!patch.active)
        {
            continue;
        }

        PokeRadarTileMark mark = patch.shiny ? PokeRadarTileMark::Shiny : patch.strong ? PokeRadarTileMark::Strong : PokeRadarTileMark::Weak;
        generator.grass[patch.y * 9 + patch.x]->setMark(mark, showContinue && (forceContinueChainType ? matchesForceType(patch) : patch.continueChain));
    }
}

void PokeRadar::choosePosition()
{
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Choose Position"));

    auto *layout = new QVBoxLayout(&dialog);
    auto *grid = new QWidget(&dialog);
    auto *gridLayout = new QGridLayout(grid);
    gridLayout->setContentsMargins(6, 6, 6, 6);
    gridLayout->setHorizontalSpacing(1);
    gridLayout->setVerticalSpacing(1);
    gridLayout->setSizeConstraint(QLayout::SetFixedSize);
    grid->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    std::optional<QPoint> selectedPosition = generator.currentPosition ? generator.currentPosition : generator.startPosition;
    std::optional<QPoint> markedPosition = generator.currentPosition;
    QPushButton *selectedButton = nullptr;
    const QString normalStyle = QStringLiteral("QPushButton { background-color: #5a5a5a; border: 1px solid black; }");
    const QString inactiveStyle = QStringLiteral("QPushButton { background-color: #303030; border: 1px solid black; }");
    const QString selectedStyle = QStringLiteral("QPushButton { background-color: #c04040; border: 1px solid black; }");
    const QString weakStyle = QStringLiteral("QPushButton { background-color: #7fbd73; border: 1px solid black; }");
    const QString strongStyle = QStringLiteral("QPushButton { background-color: #1f5f2f; border: 1px solid black; }");
    const QString shinyStyle = QStringLiteral("QPushButton { background-color: #d9c63f; border: 1px solid black; }");
    int tileSize = generator.fieldHeight > 36 ? 15 : 20;
    std::vector<QPushButton *> buttons(generator.fieldWidth * generator.fieldHeight, nullptr);
    std::vector<PokeRadarTileMark> marks(generator.fieldWidth * generator.fieldHeight, PokeRadarTileMark::None);
    std::vector<bool> continues(generator.fieldWidth * generator.fieldHeight, false);

    auto updateMarks = [&] {
        std::ranges::fill(marks, PokeRadarTileMark::None);
        std::ranges::fill(continues, false);
        if (!markedPosition)
        {
            return;
        }

        for (int y = 0; y < 9; y++)
        {
            for (int x = 0; x < 9; x++)
            {
                auto *tile = generator.grass[y * 9 + x];
                if (!tile->hasMark())
                {
                    continue;
                }

                QPoint position = *markedPosition + QPoint(x - 4, y - 4);
                if (!isValidGrassFieldPosition(generator, position))
                {
                    continue;
                }

                int index = getGrassFieldIndex(generator, position);
                marks[index] = tile->getMark();
                continues[index] = tile->isContinueMark();
            }
        }
    };
    auto updateButton = [&](QPushButton *button, int x, int y) {
        int index = getGrassFieldIndex(generator, QPoint(x, y));
        button->setText(QString());
        if (selectedPosition && selectedPosition->x() == x && selectedPosition->y() == y)
        {
            button->setStyleSheet(selectedStyle);
            selectedButton = button;
        }
        else if (!generator.fieldGrass[index])
        {
            button->setStyleSheet(inactiveStyle);
        }
        else if (marks[index] == PokeRadarTileMark::Shiny)
        {
            button->setText(QStringLiteral("S"));
            button->setStyleSheet(shinyStyle);
        }
        else if (marks[index] == PokeRadarTileMark::Strong)
        {
            button->setText(continues[index] ? QStringLiteral("C") : QString());
            button->setStyleSheet(strongStyle);
        }
        else if (marks[index] == PokeRadarTileMark::Weak)
        {
            button->setText(continues[index] ? QStringLiteral("C") : QString());
            button->setStyleSheet(weakStyle);
        }
        else
        {
            button->setStyleSheet(normalStyle);
        }
    };
    auto updateButtons = [&] {
        selectedButton = nullptr;
        for (int y = 0; y < generator.fieldHeight; y++)
        {
            for (int x = 0; x < generator.fieldWidth; x++)
            {
                updateButton(buttons[y * generator.fieldWidth + x], x, y);
            }
        }
    };
    updateMarks();

    for (int y = 0; y < generator.fieldHeight; y++)
    {
        for (int x = 0; x < generator.fieldWidth; x++)
        {
            auto *button = new QPushButton(&dialog);
            button->setFixedSize(tileSize, tileSize);
            button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            bool grass = generator.fieldGrass[getGrassFieldIndex(generator, QPoint(x, y))];
            button->setEnabled(grass);
            buttons[y * generator.fieldWidth + x] = button;
            button->setProperty("radarColumn", x);
            button->setProperty("radarRow", y);
            updateButton(button, x, y);

            gridLayout->addWidget(button, y, x);
            connect(button, &QPushButton::clicked, &dialog,
                    [button, &selectedButton, &selectedPosition, &updateButton, x, y] {
                QPushButton *oldButton = selectedButton;
                int oldX = -1;
                int oldY = -1;
                if (selectedButton != nullptr)
                {
                    oldX = selectedButton->property("radarColumn").toInt();
                    oldY = selectedButton->property("radarRow").toInt();
                }
                selectedPosition = QPoint(x, y);
                if (oldButton != nullptr)
                {
                    updateButton(oldButton, oldX, oldY);
                }
                selectedButton = button;
                updateButton(button, x, y);
            });
        }
    }

    layout->addWidget(grid, 0, Qt::AlignCenter);

    auto *buttonLayout = new QHBoxLayout;
    auto *removeMarking = new QPushButton(tr("Remove marking"), &dialog);
    auto *cancel = new QPushButton(tr("Cancel"), &dialog);
    auto *save = new QPushButton(tr("Save Position"), &dialog);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(removeMarking);
    buttonLayout->addWidget(save);
    buttonLayout->addWidget(cancel);
    layout->addLayout(buttonLayout);

    connect(removeMarking, &QPushButton::clicked, &dialog, [this, &updateMarks, &updateButtons] {
        clearGrassMarks();
        updateMarks();
        updateButtons();
    });
    connect(cancel, &QPushButton::clicked, &dialog, &QDialog::reject);
    connect(save, &QPushButton::clicked, &dialog, [this, &dialog, &selectedPosition] {
        if (selectedPosition)
        {
            setGeneratorPosition(*selectedPosition, true);
            dialog.accept();
        }
    });

    dialog.exec();
}

bool PokeRadar::moveGeneratorPositionToTile(int x, int y)
{
    QPoint position = *generator.currentPosition + QPoint(x - 4, y - 4);
    if (!isValidGrassFieldPosition(generator, position))
    {
        return false;
    }

    setGeneratorPosition(position, false);
    return true;
}

void PokeRadar::moveGeneratorPositionToVisibleTile(int x, int y)
{
    auto *tile = generator.grass[y * 9 + x];
    if (!tile->hasGrass() && !tile->hasMark())
    {
        return;
    }

    if (!moveGeneratorPositionToTile(x, y))
    {
        return;
    }
    clearGrassMarks();
    if (generator.hasRun)
    {
        generate();
    }
}

void PokeRadar::resetGrass(PokeRadarControls &controls)
{
    if (&controls == &searcher)
    {
        setupGrassField(controls, 9, 9, controls.fieldLocation, controls.fieldLocationOccurrence);
        controls.startPosition = QPoint(4, 4);
        controls.currentPosition = QPoint(4, 4);

        if (controls.fieldLocation == 134 && controls.fieldLocationOccurrence == 1)
        {
            static constexpr std::array<std::pair<int, int>, 14> unreachable = { {
                { 0, 0 },
                { 0, 1 },
                { 0, 2 },
                { 0, 3 },
                { 1, 0 },
                { 1, 1 },
                { 2, 0 },
                { 6, 8 },
                { 7, 7 },
                { 7, 8 },
                { 8, 5 },
                { 8, 6 },
                { 8, 7 },
                { 8, 8 },
            } };
            for (const auto &position : unreachable)
            {
                controls.fieldGrass[getGrassFieldIndex(controls, QPoint(position.first, position.second))] = false;
            }
        }

        updateGrassFromField(controls);
        for (auto *tile : controls.grass)
        {
            tile->clearMark();
        }
        return;
    }

    if (currentProfile != nullptr && controls.fieldLocation != 0xffff)
    {
        controls.fieldGrass = getPokeRadarGrassField(controls.fieldLocation, controls.fieldLocationOccurrence, currentProfile->getVersion());
        QPoint start = getPokeRadarGrassFieldStart(controls.fieldLocation, controls.fieldLocationOccurrence, currentProfile->getVersion());
        controls.startPosition = start;
        controls.currentPosition = start;
        updateGrassFromField(controls);
    }
    else
    {
        for (auto *tile : controls.grass)
        {
            tile->setGrass(true);
        }
    }

    for (auto *tile : controls.grass)
    {
        tile->clearMark();
    }
}

void PokeRadar::setGeneratorPosition(const QPoint &position, bool updateStart)
{
    if (!isValidGrassFieldPosition(generator, position))
    {
        return;
    }

    bool hadPosition = generator.currentPosition.has_value();
    if (hadPosition && isValidGrassFieldPosition(generator, *generator.currentPosition))
    {
        generator.fieldGrass[getGrassFieldIndex(generator, *generator.currentPosition)] = true;
    }
    generator.currentPosition = position;
    generator.fieldGrass[getGrassFieldIndex(generator, position)] = true;
    if (updateStart)
    {
        generator.startPosition = position;
        if (!hadPosition)
        {
            for (int y = 0; y < 9; y++)
            {
                for (int x = 0; x < 9; x++)
                {
                    if (x != 4 || y != 4)
                    {
                        storeGrassTile(generator, x, y, generator.grass[y * 9 + x]->hasGrass());
                    }
                }
            }
        }
    }
    updateGrassFromField(generator);
}

void PokeRadar::setupGrassField(PokeRadarControls &controls, int width, int height, u16 location, int locationOccurrence)
{
    controls.fieldWidth = width;
    controls.fieldHeight = height;
    controls.fieldLocation = location;
    controls.fieldLocationOccurrence = locationOccurrence;
    controls.fieldVersion = currentProfile == nullptr ? Game::None : currentProfile->getVersion();
    controls.fieldGrass.assign(width * height, true);
}

void PokeRadar::setupGrassGrid(PokeRadarControls &controls, QGridLayout *layout)
{
    bool searcherTab = &controls == &searcher;
    for (int x = 0; x < 9; x++)
    {
        auto *label = new QLabel(QString(QChar('A' + x)), this);
        label->setCursor(Qt::PointingHandCursor);
        label->setFixedWidth(22);
        label->setAlignment(Qt::AlignCenter);
        label->setProperty("radarColumn", x);
        label->setProperty("radarSearcher", searcherTab);
        label->installEventFilter(this);
        layout->addWidget(label, 0, x + 1, Qt::AlignCenter);
    }

    for (int y = 0; y < 9; y++)
    {
        auto *label = new QLabel(QString::number(y), this);
        label->setCursor(Qt::PointingHandCursor);
        label->setFixedWidth(14);
        label->setAlignment(Qt::AlignCenter);
        label->setProperty("radarRow", y);
        label->setProperty("radarSearcher", searcherTab);
        label->installEventFilter(this);
        layout->addWidget(label, y + 1, 0, Qt::AlignCenter);
        for (int x = 0; x < 9; x++)
        {
            auto *tile = new PokeRadarTile(x == 4 && y == 4, this);
            tile->setProperty("radarColumn", x);
            tile->setProperty("radarRow", y);
            tile->setProperty("radarSearcher", searcherTab);
            tile->installEventFilter(this);
            layout->addWidget(tile, y + 1, x + 1, Qt::AlignCenter);
            controls.grass[y * 9 + x] = tile;
        }
    }
}

void PokeRadar::storeGrassTile(PokeRadarControls &controls, int x, int y, bool grass)
{
    if (!controls.currentPosition || (x == 4 && y == 4))
    {
        return;
    }

    QPoint position = *controls.currentPosition + QPoint(x - 4, y - 4);
    if (isValidGrassFieldPosition(controls, position))
    {
        controls.fieldGrass[getGrassFieldIndex(controls, position)] = grass;
    }
}

void PokeRadar::toggleGrassColumn(PokeRadarControls &controls, int column)
{
    bool allSelected = true;
    for (int y = 0; y < 9; y++)
    {
        if (column == 4 && y == 4)
        {
            continue;
        }

        allSelected &= controls.grass[y * 9 + column]->hasGrass();
    }

    for (int y = 0; y < 9; y++)
    {
        controls.grass[y * 9 + column]->setGrass(!allSelected);
        storeGrassTile(controls, column, y, !allSelected);
    }
}

void PokeRadar::toggleGrassRow(PokeRadarControls &controls, int row)
{
    bool allSelected = true;
    for (int x = 0; x < 9; x++)
    {
        if (x == 4 && row == 4)
        {
            continue;
        }

        allSelected &= controls.grass[row * 9 + x]->hasGrass();
    }

    for (int x = 0; x < 9; x++)
    {
        controls.grass[row * 9 + x]->setGrass(!allSelected);
        storeGrassTile(controls, x, row, !allSelected);
    }
}

void PokeRadar::updateGrassFromField(PokeRadarControls &controls)
{
    if (!controls.currentPosition)
    {
        return;
    }

    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            auto *tile = controls.grass[y * 9 + x];
            QPoint position = *controls.currentPosition + QPoint(x - 4, y - 4);
            bool valid = isValidGrassFieldPosition(controls, position);
            tile->setGrass(valid && controls.fieldGrass[getGrassFieldIndex(controls, position)]);
        }
    }
}

std::vector<WildGeneratorState4> PokeRadar::getPokemonStates(PokeRadarControls &controls, const std::vector<EncounterArea4> &encounters,
                                                             PokeRadarChainType chainType, bool useChainSlot, bool ignoreFilters,
                                                             u32 extraAdvances)
{
    std::vector<WildGeneratorState4> pokemonStates;
    if (currentProfile == nullptr || controls.location->currentIndex() < 0)
    {
        return pokemonStates;
    }

    u16 location = controls.location->getCurrentUShort();
    auto areaIter = std::ranges::find_if(encounters, [location](const EncounterArea4 &encounter) { return encounter.getLocation() == location; });
    if (areaIter == encounters.end())
    {
        return pokemonStates;
    }

    auto encounterSlots = controls.filter->getEncounterSlots();
    if (ignoreFilters || std::ranges::none_of(encounterSlots, [](bool checked) { return checked; }))
    {
        encounterSlots.fill(true);
    }
    if (useChainSlot && controls.chainCount->value() != 0 && controls.slot != nullptr && controls.slot->currentIndex() >= 0)
    {
        encounterSlots.fill(false);
        encounterSlots[controls.slot->getCurrentUChar()] = true;
    }

    std::array<u8, 6> ivMin = {};
    std::array<u8, 6> ivMax = { 31, 31, 31, 31, 31, 31 };
    std::array<bool, 25> natures;
    natures.fill(true);
    std::array<bool, 16> powers;
    powers.fill(true);
    WildStateFilter filter(ignoreFilters ? 255 : controls.filter->getGender(), ignoreFilters ? 255 : controls.filter->getAbility(),
                           ignoreFilters ? 255 : controls.filter->getShiny(), ignoreFilters ? 1 : controls.filter->getLevelMin(),
                           ignoreFilters ? 100 : controls.filter->getLevelMax(), 0, 255, 0, 255, false,
                           ignoreFilters ? ivMin : controls.filter->getMinIVs(), ignoreFilters ? ivMax : controls.filter->getMaxIVs(),
                           ignoreFilters ? natures : controls.filter->getNatures(), ignoreFilters ? powers : controls.filter->getHiddenPowers(),
                           encounterSlots);
    u32 pokemonOffset = controls.chainCount->value() == 0 ? 1 : 0;
    PokeRadarPokemonGenerator generator(controls.initialAdvances->getUInt(), controls.maxAdvances->getUInt() + extraAdvances, pokemonOffset,
                                        getPokeRadarGeneratorLead(controls), getSelectedShiny(chainType), *areaIter, *currentProfile,
                                        filter);

    if (!useChainSlot || controls.chainCount->value() == 0)
    {
        return generator.generate(controls.seed->getUInt());
    }

    for (u8 slot = 0; slot < encounterSlots.size(); slot++)
    {
        if (!encounterSlots[slot])
        {
            continue;
        }

        auto slotStates = generator.generate(controls.seed->getUInt(), slot);
        pokemonStates.insert(pokemonStates.end(), slotStates.begin(), slotStates.end());
    }

    return pokemonStates;
}

std::vector<PokeRadarState> PokeRadar::getStates(PokeRadarControls &controls, const std::vector<EncounterArea4> &encounters,
                                                 std::optional<PokeRadarChainType> chainTypeOverride)
{
    std::vector<PokeRadarState> result;
    if (currentProfile == nullptr || controls.location->currentIndex() < 0)
    {
        return result;
    }

    u16 location = controls.location->getCurrentUShort();
    auto areaIter = std::ranges::find_if(encounters, [location](const EncounterArea4 &encounter) { return encounter.getLocation() == location; });
    if (areaIter == encounters.end())
    {
        return result;
    }

    const EncounterArea4 &area = *areaIter;
    PokeRadarChainType chainType
        = chainTypeOverride.value_or(controls.chainType != nullptr ? controls.chainType->getEnum<PokeRadarChainType>() : PokeRadarChainType::Weak);
    auto previous = controls.result != nullptr ? controls.result->getEnum<PokeRadarResult>() : PokeRadarResult::Defeat;
    u16 chainCount = controls.chainCount->value();
    u32 extraAdvances = chainCount == 0 ? 0 : 1;
    PokeRadarGenerator manualRadar(controls.initialAdvances->getUInt(), controls.maxAdvances->getUInt() + extraAdvances, chainCount, chainType,
                                   PokeRadarResult::ManualActivation, getGrass(controls));
    auto patchStates = manualRadar.generate(controls.seed->getUInt());

    std::vector<WildGeneratorState4> pokemonStates
        = getPokemonStates(controls, encounters, chainType, true, controls.filter->getDisableFilters(), extraAdvances);

    u16 battleChainCount = std::min<u16>(chainCount + 1, maxPokeRadarChain);
    PokeRadarGenerator battleRadar(controls.initialAdvances->getUInt(), controls.maxAdvances->getUInt() + extraAdvances, battleChainCount,
                                   chainType, previous, getGrass(controls));
    std::unordered_map<u32, PokeRadarState> battlePatchStates;

    for (const auto &pokemon : pokemonStates)
    {
        if (pokemon.getAdvances() > controls.initialAdvances->getUInt() + controls.maxAdvances->getUInt())
        {
            continue;
        }

        u32 patchAdvances = pokemon.getAdvances();
        const PokeRadarState *patchState = nullptr;

        auto patch = std::ranges::find_if(patchStates, [patchAdvances](const PokeRadarState &state) { return state.getAdvances() == patchAdvances; });
        if (patch == patchStates.end())
        {
            continue;
        }
        patchState = &*patch;

        PokeRadarState combined = chainCount != 0 ? PokeRadarState(*patchState, pokemon, chainCount) : PokeRadarState(*patchState, pokemon);
        auto [noGraceSkip, graceSkip] = PokeRadarGenerator::getSkips(controls.seed->getUInt(), combined.getAdvances());
        combined.setSkip(noGraceSkip, graceSkip);

        u32 battleAdvances = pokemon.getBattleAdvances();
        bool patchesReachable = battleAdvances > pokemon.getAdvances();
        if (patchesReachable)
        {
            u32 battlePatchAdvances = getPostBattlePatchAdvances(battleAdvances, currentProfile->getVersion());
            u32 battlePatchTypeAdvances = getPostBattlePatchTypeAdvances(currentProfile->getVersion());
            auto battlePatch = battlePatchStates.find(battlePatchAdvances);
            if (battlePatch == battlePatchStates.end())
            {
                battlePatch = battlePatchStates
                                  .emplace(battlePatchAdvances,
                                           battleRadar.generatePrevious(controls.seed->getUInt(), battlePatchAdvances, battlePatchTypeAdvances))
                                  .first;
            }
            combined.setBattlePatches(battlePatch->second.getPatches());
            combined.setDisplayedBattleAdvances(battleAdvances + 4 + battleRadar.getPostBattleAdvanceConsumption(combined.getBattlePatches()));
        }
        combined.setPatchesVisible(true);
        combined.setBattlePatchesVisible(patchesReachable);
        combined.setDisplayedPatchAdvances(manualRadar.getAdvanceConsumption(controls.seed->getUInt(), patchAdvances, PokeRadarResult::ManualActivation));
        if (matchesPatchFilter(controls, combined))
        {
            result.emplace_back(combined);
        }
    }

    return result;
}

void PokeRadar::updateEncounterSlots(PokeRadarControls &controls, const std::vector<EncounterArea4> &encounters)
{
    std::optional<QSignalBlocker> slotBlocker;
    QVariant currentSlot;
    if (controls.slot != nullptr)
    {
        slotBlocker.emplace(controls.slot);
        currentSlot = controls.slot->currentData();
        controls.slot->clear();
    }
    if (controls.location->currentIndex() < 0)
    {
        return;
    }

    const auto &area = encounters[controls.location->currentIndex()];
    controls.filter->setEncounterSlots(area.getCount());

    for (u8 i = 0; i < 12; i++)
    {
        const auto &slot = area.getPokemon(i);
        if (controls.slot != nullptr)
        {
            controls.slot->addItem(QString("%1: %2").arg(i).arg(QString::fromStdString(Translator::getSpecie(slot.getSpecie()))), i);
        }
    }
    if (controls.slot != nullptr)
    {
        controls.slot->setCurrentIndex(std::max(0, controls.slot->findData(currentSlot)));
        controls.slot->setEnabled(&controls != &searcher || controls.chainCount->value() != 0);
    }
    if (&controls == &searcher)
    {
        controls.pokemon->setEnabled(controls.chainCount->value() == 0);
    }

    if (controls.pokemon->currentIndex() <= 0)
    {
        controls.filter->resetEncounterSlots();
        const auto &slot = area.getPokemon(controls.slot != nullptr && controls.slot->currentIndex() >= 0 ? controls.slot->getCurrentUChar() : 0);
        controls.levelMin->setValue(slot.getMinLevel());
        controls.levelMax->setValue(slot.getMaxLevel());
        controls.filter->setLevelRange(1, 100);
    }
    else
    {
        u16 specie = controls.pokemon->getCurrentUShort();
        auto matchingSlots = area.getSlots(specie);
        controls.filter->toggleEncounterSlots(matchingSlots);

        if (controls.slot != nullptr && controls.slot->currentIndex() >= 0 && !matchingSlots[controls.slot->getCurrentUChar()])
        {
            auto matchingSlot = std::ranges::find(matchingSlots, true);
            if (matchingSlot != matchingSlots.end())
            {
                controls.slot->setCurrentIndex(static_cast<int>(std::distance(matchingSlots.begin(), matchingSlot)));
            }
        }

        auto range = area.getLevelRange(specie);
        controls.levelMin->setValue(range.first);
        controls.levelMax->setValue(range.second);
        controls.filter->setLevelRange(range.first, range.second);
    }
}

void PokeRadar::updateEncounters(PokeRadarControls &controls, std::vector<EncounterArea4> &encounters, bool updateGrassField)
{
    if (currentProfile == nullptr)
    {
        return;
    }

    u16 currentLocation = controls.location->getCurrentUShort();
    int currentLocationIndex = controls.location->currentIndex();
    QVariant currentSlot;
    if (controls.slot != nullptr)
    {
        currentSlot = controls.slot->currentData();
    }
    QVariant currentReplacement0 = controls.replacement0->currentData();
    QVariant currentReplacement1 = controls.replacement1->currentData();

    encounters = getRadarEncounters(Encounters4::getEncounters(Encounter::Grass, getEncounterSettings(controls, getSelectedRadar(controls)), currentProfile));
    QSignalBlocker locationBlocker(controls.location);
    updateLocations(controls, encounters, updateGrassField);
    if (currentLocationIndex >= 0 && currentLocationIndex < encounters.size() && encounters[currentLocationIndex].getLocation() == currentLocation)
    {
        controls.location->setCurrentIndex(currentLocationIndex);
    }
    else
    {
        controls.location->setCurrentIndexByData(currentLocation);
    }

    updatePokemon(controls, encounters, updateGrassField);

    {
        QSignalBlocker replacement0Blocker(controls.replacement0);
        QSignalBlocker replacement1Blocker(controls.replacement1);
        controls.replacement0->setCurrentIndex(std::max(0, controls.replacement0->findData(currentReplacement0)));
        controls.replacement1->setCurrentIndex(std::max(0, controls.replacement1->findData(currentReplacement1)));
    }

    if (controls.slot != nullptr)
    {
        controls.slot->setCurrentIndex(std::max(0, controls.slot->findData(currentSlot)));
    }
}

void PokeRadar::updateLocations(PokeRadarControls &controls, const std::vector<EncounterArea4> &encounters, bool updateGrassField)
{
    controls.location->clear();
    std::vector<u16> locs;
    std::ranges::transform(encounters, std::back_inserter(locs), [](const EncounterArea4 &area) { return area.getLocation(); });
    controls.location->addItems(getRadarLocationNames(encounters, currentProfile->getVersion()), locs, false);
}

void PokeRadar::updateMinimumGraceSteps(PokeRadarControls &controls, const std::vector<EncounterArea4> &encounters)
{
    if (controls.minimumGraceSteps == nullptr || controls.location->currentIndex() < 0)
    {
        return;
    }

    const u8 rate = encounters[controls.location->currentIndex()].getRate();
    const u8 graceSteps = 8 - std::min<u8>(rate / 10, 8);
    controls.minimumGraceSteps->setValue(graceSteps == 0 ? 0 : graceSteps - 1);
}

void PokeRadar::updatePokemon(PokeRadarControls &controls, const std::vector<EncounterArea4> &encounters, bool updateGrassField)
{
    controls.pokemon->clear();
    controls.pokemon->addItem(QStringLiteral("-"));
    if (controls.location->currentIndex() < 0)
    {
        updateMinimumGraceSteps(controls, encounters);
        return;
    }

    const auto &area = encounters[controls.location->currentIndex()];
    bool trophyGarden = area.trophyGarden(currentProfile->getVersion());
    controls.replacement->setVisible(trophyGarden);
    controls.replacement0->setVisible(trophyGarden);
    controls.replacement1->setVisible(trophyGarden);
    if (trophyGarden)
    {
        QSignalBlocker replacement0Blocker(controls.replacement0);
        QSignalBlocker replacement1Blocker(controls.replacement1);

        controls.replacement0->clear();
        controls.replacement1->clear();
        for (u16 specie : Encounters4::getTrophyGardenPokemon(currentProfile))
        {
            const auto &name = Translator::getSpecie(specie);
            controls.replacement0->addItem(QString::fromStdString(name), specie);
            controls.replacement1->addItem(QString::fromStdString(name), specie);
        }
    }
    int locationIndex = controls.location->currentIndex();
    u8 locationOccurrence = getRadarLocationOccurrence(encounters, locationIndex);
    QSize fieldSize = getPokeRadarGrassFieldSize(area.getLocation(), locationOccurrence, currentProfile->getVersion());
    if (&controls == &searcher)
    {
        if (updateGrassField
            && (controls.fieldWidth != 9 || controls.fieldHeight != 9 || controls.fieldLocation != area.getLocation()
                || controls.fieldLocationOccurrence != locationOccurrence || controls.fieldVersion != currentProfile->getVersion()))
        {
            setupGrassField(controls, 9, 9, area.getLocation(), locationOccurrence);
            controls.startPosition = QPoint(4, 4);
            controls.currentPosition = QPoint(4, 4);

            if (area.getLocation() == 134 && locationOccurrence == 1)
            {
                static constexpr std::array<std::pair<int, int>, 14> unreachable = { {
                    { 0, 0 },
                    { 0, 1 },
                    { 0, 2 },
                    { 0, 3 },
                    { 1, 0 },
                    { 1, 1 },
                    { 2, 0 },
                    { 6, 8 },
                    { 7, 7 },
                    { 7, 8 },
                    { 8, 5 },
                    { 8, 6 },
                    { 8, 7 },
                    { 8, 8 },
                } };
                for (const auto &position : unreachable)
                {
                    controls.fieldGrass[getGrassFieldIndex(controls, QPoint(position.first, position.second))] = false;
                }
            }

            updateGrassFromField(controls);
        }
    }
    else if (updateGrassField
             && (fieldSize.width() != controls.fieldWidth || fieldSize.height() != controls.fieldHeight || controls.fieldLocation != area.getLocation()
                 || controls.fieldLocationOccurrence != locationOccurrence || controls.fieldVersion != currentProfile->getVersion()))
    {
        setupGrassField(controls, fieldSize.width(), fieldSize.height(), area.getLocation(), locationOccurrence);
        controls.fieldGrass = getPokeRadarGrassField(area.getLocation(), locationOccurrence, currentProfile->getVersion());
        QPoint start = getPokeRadarGrassFieldStart(area.getLocation(), locationOccurrence, currentProfile->getVersion());
        controls.startPosition = start;
        controls.currentPosition = start;
        updateGrassFromField(controls);
    }

    updateMinimumGraceSteps(controls, encounters);
    auto species = area.getUniqueSpecies();
    auto names = area.getSpecieNames();
    for (size_t i = 0; i < species.size(); i++)
    {
        controls.pokemon->addItem(QString::fromStdString(names[i]), species[i]);
    }

    updateEncounterSlots(controls, encounters);
}

void PokeRadar::updateSearcherPatchTypes()
{
    if (searcher.patchTypes == nullptr)
    {
        return;
    }

    QSignalBlocker blocker(searcher.patchTypes);
    auto current = searcher.patchTypes->currentData();
    bool chained = searcher.chainCount->value() != 0;

    searcher.patchTypes->clear();
    if (chained)
    {
        searcher.patchTypes->addItem(tr("Weak Shiny"), static_cast<int>(PokeRadarChainType::WeakShiny));
        searcher.patchTypes->addItem(tr("Strong Shiny"), static_cast<int>(PokeRadarChainType::StrongShiny));
    }
    else
    {
        searcher.patchTypes->addItem(tr("Weak"), static_cast<int>(PokeRadarChainType::Weak));
        searcher.patchTypes->addItem(tr("Strong"), static_cast<int>(PokeRadarChainType::Strong));
    }

    int index = searcher.patchTypes->findData(current);
    searcher.patchTypes->setCurrentIndex(index >= 0 ? index : 0);
}

void PokeRadar::generate()
{
    generator.hasRun = true;
    generator.model->clearModel();
    generator.model->addItems(getStates(generator, generatorEncounters));
}

void PokeRadar::openAdvanceFinder()
{
    if (currentProfile == nullptr)
    {
        return;
    }

    auto *advanceFinder = new AdvanceFinder(generator.model, generator.tableView, currentProfile, this);
    advanceFinder->show();
}

void PokeRadar::profileChanged(const Profile4 &profile)
{
    currentProfile = &profile;

    updateEncounters(generator, generatorEncounters);
    updateEncounters(searcher, searcherEncounters);
}

void PokeRadar::search()
{
    searcher.hasRun = true;
    searcher.model->clearModel();

    if (currentProfile == nullptr)
    {
        return;
    }

    auto chainType = searcher.patchTypes->getEnum<PokeRadarChainType>();
    std::vector<PokeRadarResult> activations;
    if (searcher.chainCount->value() == 0)
    {
        activations.emplace_back(PokeRadarResult::ManualActivation);
    }
    else
    {
        for (u16 data : searcher.results->getCheckedData())
        {
            activations.emplace_back(static_cast<PokeRadarResult>(data));
        }
        if (activations.empty())
        {
            activations.emplace_back(PokeRadarResult::ManualActivation);
            activations.emplace_back(PokeRadarResult::Capture);
            activations.emplace_back(PokeRadarResult::Defeat);
        }
    }
    bool postBattleSearch = std::ranges::any_of(activations, [](PokeRadarResult activation) {
        return activation != PokeRadarResult::ManualActivation;
    });
    searcher.model->setShowSearcherBattleAdvances(postBattleSearch);

    u32 minDistance = searcher.minPatchDistance->value();

    u32 maxAdvances = searcher.maxAdvances->getUInt();
    if (!postBattleSearch && maxAdvances < minDistance)
    {
        QMessageBox::warning(this, tr("Invalid Settings"),
                             tr("The searched advances end before the minimum patch distance can be reached."));
        return;
    }

    auto min = searcher.filter->getMinIVs();
    auto max = searcher.filter->getMaxIVs();
    auto *radarSearchers = new std::vector<std::pair<PokeRadarResult, PokeRadarSearcher *>>;
    auto *cancelled = new std::atomic_bool(false);

    auto encounters = Encounters4::getEncounters(Encounter::Grass, getEncounterSettings(searcher, getSelectedRadar(chainType)), currentProfile);
    if (searcher.location->currentIndex() >= 0)
    {
        u16 location = searcher.location->getCurrentUShort();
        auto areaIter = std::ranges::find_if(encounters, [location](const EncounterArea4 &encounter) { return encounter.getLocation() == location; });
        if (areaIter != encounters.end())
        {
            auto encounterSlots = searcher.filter->getEncounterSlots();
            if (std::ranges::none_of(encounterSlots, [](bool slot) { return slot; }))
            {
                encounterSlots.fill(true);
            }
            auto searchSlots = encounterSlots;
            if (getSelectedShiny(chainType) && searcher.chainCount->value() != 0)
            {
                searchSlots.fill(false);
                searchSlots[searcher.slot->getCurrentUChar()] = true;
            }

            WildStateFilter filter(searcher.filter->getGender(), searcher.filter->getAbility(), searcher.filter->getShiny(),
                                   searcher.filter->getLevelMin(), searcher.filter->getLevelMax(), searcher.filter->getHeightMin(),
                                   searcher.filter->getHeightMax(), searcher.filter->getWeightMin(), searcher.filter->getWeightMax(),
                                   searcher.filter->getDisableFilters(), searcher.filter->getMinIVs(), searcher.filter->getMaxIVs(),
                                   searcher.filter->getNatures(), searcher.filter->getHiddenPowers(), searchSlots);

            std::vector<std::pair<Lead, bool>> leads = { { searcher.lead->getEnum<Lead>(), false } };
            if (leads[0].first == Lead::Synchronize)
            {
                leads.clear();
                auto natures = searcher.filter->getNatures();
                for (u8 nature = 0; nature < natures.size(); nature++)
                {
                    if (natures[nature])
                    {
                        leads.emplace_back(static_cast<Lead>(nature), true);
                    }
                }
            }

            for (const auto &[lead, specificSynchronize] : leads)
            {
                for (PokeRadarResult activation : activations)
                {
                    radarSearchers->emplace_back(
                        activation,
                        new PokeRadarSearcher(searcher.initialAdvances->getUInt(), searcher.maxAdvances->getUInt(), searcher.minDelay->getUInt(),
                                              searcher.maxDelay->getUInt(), minDistance, searcher.chainCount->value(), searcher.slot->getCurrentUChar(),
                                              lead, chainType, activation, getGrass(searcher), searchSlots, *areaIter, *currentProfile, filter,
                                              specificSynchronize));
                }
            }
        }
    }

    if (radarSearchers->empty())
    {
        delete radarSearchers;
        delete cancelled;
        return;
    }

    searcher.button->setEnabled(false);
    searcher.cancel->setEnabled(true);
    searcher.progressBar->setValue(0);
    auto *accumulatedResults = new std::vector<std::pair<PokeRadarResult, std::vector<PokeRadarState>>>;
    accumulatedResults->reserve(radarSearchers->size());
    for (const auto &[activation, radarSearcher] : *radarSearchers)
    {
        accumulatedResults->emplace_back(activation, std::vector<PokeRadarState> {});
    }

    auto drainResults = [=] {
        bool changed = false;
        for (size_t i = 0; i < radarSearchers->size(); i++)
        {
            auto states = (*radarSearchers)[i].second->getResults();
            if (states.empty())
            {
                continue;
            }

            auto &accumulated = (*accumulatedResults)[i].second;
            accumulated.insert(accumulated.end(), states.begin(), states.end());
            changed = true;
        }

        if (changed)
        {
            searcher.model->clearModel();
            searcher.model->addItems(mergeSearcherActivationResults(*accumulatedResults));
        }
    };

    auto getSearchProgress = [=] {
        int progress = 0;
        for (auto &[activation, radarSearcher] : *radarSearchers)
        {
            progress += radarSearcher->getProgress();
        }
        return progress / static_cast<int>(radarSearchers->size());
    };

    auto *thread = QThread::create([=] {
        for (auto &[activation, radarSearcher] : *radarSearchers)
        {
            if (cancelled->load())
            {
                break;
            }

            radarSearcher->startSearch(min, max);
        }
    });
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(searcher.cancel, &QPushButton::clicked, thread, [=] {
        cancelled->store(true);
        for (auto &[activation, radarSearcher] : *radarSearchers)
        {
            radarSearcher->cancelSearch();
        }
    });

    auto *timer = new QTimer();
    timer->callOnTimeout(this, [=] {
        drainResults();
        int progress = getSearchProgress();
        if (progress == 100 && thread->isRunning())
        {
            progress = 99;
        }
        searcher.progressBar->setValue(progress);
    });
    connect(thread, &QThread::finished, timer, &QTimer::stop);
    connect(thread, &QThread::finished, timer, &QTimer::deleteLater);
    connect(timer, &QTimer::destroyed, this, [=] {
        drainResults();
        int progress = getSearchProgress();
        for (auto &[activation, radarSearcher] : *radarSearchers)
        {
            delete radarSearcher;
        }

        searcher.button->setEnabled(true);
        searcher.cancel->setEnabled(false);
        searcher.progressBar->setValue(progress);
        delete accumulatedResults;
        delete radarSearchers;
        delete cancelled;
    });

    thread->start();
    timer->start(1000);
}

void PokeRadar::seedToTime()
{
    if (currentProfile == nullptr || !searcher.tableView->currentIndex().isValid())
    {
        return;
    }

    QModelIndex index = searcher.proxyModel->mapToSource(searcher.tableView->currentIndex());
    const auto &state = searcher.model->getItem(index.row());
    if (!state.hasSearcherPokemon())
    {
        return;
    }

    auto *time = new SeedToTime4(state.getSearcherPokemon().getSeed(), currentProfile->getVersion());
    time->show();
}
