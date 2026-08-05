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

#ifndef POKERADAR_HPP
#define POKERADAR_HPP

#include <Core/Gen4/EncounterArea4.hpp>
#include <Core/Gen4/Encounters4.hpp>
#include <Core/Gen4/Generators/PokeRadarGenerator.hpp>
#include <Core/Gen4/States/PokeRadarState.hpp>
#include <QWidget>
#include <array>
#include <optional>
#include <vector>

class ComboBox;
class ComboBoxProxy;
class ComboMenu;
class Filter;
class CheckList;
class PokeRadarTile;
class PokeRadarModel4;
class Profile4;
class ProfileDisplay4;
class QCheckBox;
class QGridLayout;
class QGroupBox;
class QLabel;
class QProgressBar;
class QPushButton;
class QSpinBox;
class TableView;
class TabWidget;
class TextBox;
class SortFilterProxyModel;

struct PokeRadarControls
{
    TextBox *seed;
    TextBox *initialAdvances;
    TextBox *maxAdvances;
    QSpinBox *minPatchDistance;
    QSpinBox *maxPatchDistance;
    ComboMenu *lead;
    QSpinBox *chainCount;
    ComboBox *chainType;
    ComboBox *result;
    ComboBoxProxy *location;
    ComboBox *pokemon;
    ComboBox *time;
    QCheckBox *swarm;
    QCheckBox *dualSlot;
    ComboBox *dualSlotGame;
    ComboBox *slot;
    CheckList *patchTypes;
    CheckList *patchFilter;
    QSpinBox *levelMin;
    QSpinBox *levelMax;
    QSpinBox *minimumGraceSteps;
    Filter *filter;
    PokeRadarModel4 *model;
    SortFilterProxyModel *proxyModel;
    TableView *tableView;
    std::array<PokeRadarTile *, 81> grass;
    TextBox *minDelay;
    TextBox *maxDelay;
    QPushButton *button;
    QPushButton *cancel;
    QProgressBar *progressBar;
    bool hasRun = false;
};

class PokeRadar : public QWidget
{
    Q_OBJECT
signals:
    void profilesChanged(int);

public:
    PokeRadar(QWidget *parent = nullptr);
    ~PokeRadar() override;

    bool hasProfiles() const;

public slots:
    void updateProfiles();

protected:
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    ProfileDisplay4 *profileDisplay;
    TabWidget *tabRNGSelector;
    PokeRadarControls generator;
    PokeRadarControls searcher;
    bool paintingGrass = false;
    bool grassPaintValue = false;

    const Profile4 *currentProfile;
    std::vector<EncounterArea4> generatorEncounters;
    std::vector<EncounterArea4> searcherEncounters;

    EncounterSettings4 getEncounterSettings(const PokeRadarControls &controls, bool radar) const;
    QGroupBox *createFilters(PokeRadarControls &controls, bool searcherTab);
    QGroupBox *createGrassTiles(PokeRadarControls &controls, bool searcherTab);
    QGroupBox *createRNGInfo(PokeRadarControls &controls, bool searcherTab);
    QGroupBox *createSettings(PokeRadarControls &controls, bool searcherTab);
    std::array<bool, 81> getGrass(const PokeRadarControls &controls) const;
    bool getSelectedRadar(const PokeRadarControls &controls) const;
    bool getSelectedRadar(PokeRadarChainType chainType) const;
    bool getSelectedShiny(const PokeRadarControls &controls) const;
    bool getSelectedShiny(PokeRadarChainType chainType) const;
    bool matchesPatchFilter(const PokeRadarControls &controls, const PokeRadarState &state) const;
    void clearGrassMarks();
    void jumpToBattleAdv();
    void markSelectedPatches(const PokeRadarControls &controls);
    void markPatches(const PokeRadarState &state, bool showContinue);
    void resetGrass(PokeRadarControls &controls);
    void setupGrassGrid(PokeRadarControls &controls, QGridLayout *layout);
    void toggleGrassColumn(PokeRadarControls &controls, int column);
    void toggleGrassRow(PokeRadarControls &controls, int row);
    std::vector<WildGeneratorState4> getPokemonStates(PokeRadarControls &controls, const std::vector<EncounterArea4> &encounters,
                                                       PokeRadarChainType chainType, bool useChainSlot, bool ignoreFilters = false);
    std::vector<PokeRadarState> getStates(PokeRadarControls &controls, const std::vector<EncounterArea4> &encounters,
                                           std::optional<PokeRadarChainType> chainTypeOverride = std::nullopt);
    void updateEncounterSlots(PokeRadarControls &controls, const std::vector<EncounterArea4> &encounters);
    void updateEncounters(PokeRadarControls &controls, std::vector<EncounterArea4> &encounters);
    void updateMinimumGraceSteps(PokeRadarControls &controls, const std::vector<EncounterArea4> &encounters);
    void updateLocations(PokeRadarControls &controls, const std::vector<EncounterArea4> &encounters);
    void updatePokemon(PokeRadarControls &controls, const std::vector<EncounterArea4> &encounters);

private slots:
    void generate();
    void openAdvanceFinder();
    void profileChanged(const Profile4 &profile);
    void search();
    void seedToTime();
};

#endif // POKERADAR_HPP
