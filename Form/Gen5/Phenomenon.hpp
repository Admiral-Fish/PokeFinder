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

#ifndef PHENOMENON_HPP
#define PHENOMENON_HPP

#include <Core/Global.hpp>
#include <QWidget>

class EncounterArea5;
class CheckList;
class IVCache;
class QLabel;
class Profile5;
class QRadioButton;
class SHA1Cache;
class SortFilterProxyModel;
class WildState5;
class WildGeneratorModel5;
class WildSearcherModel5;

namespace Ui
{
    class Phenomenon;
}

/**
 * @brief Provides settings and filters to RNG phenomenon encounters in Gen 5 games
 */
class Phenomenon : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the profiles have been modified
     */
    void profilesChanged(int);

public:
    /**
     * @brief Construct a new Phenomenon object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    Phenomenon(QWidget *parent = nullptr);

    /**
     * @brief Destroy the Phenomenon object
     */
    ~Phenomenon() override;

    /**
     * @brief Determines if any profiles exist
     *
     * @return true At least 1 profile exists
     * @return false 0 profiles exist
     */
    bool hasProfiles() const;

public slots:
    /**
     * @brief Reloads profiles
     */
    void updateProfiles();

private:
    Ui::Phenomenon *ui;

    IVCache *ivCache;
    CheckList *checkListGeneratorItem;
    CheckList *checkListSearcherItem;
    QLabel *labelGeneratorItem;
    QWidget *labelSearcherItem;
    QRadioButton *radioButtonSearcherItem;
    QRadioButton *radioButtonSearcherPokemon;
    Profile5 *currentProfile;
    SHA1Cache *shaCache;
    SortFilterProxyModel *proxyModel;
    std::vector<EncounterArea5> encounterGenerator;
    std::vector<EncounterArea5> encounterSearcher;
    std::vector<Profile5> profiles;
    WildGeneratorModel5 *generatorModel;
    WildSearcherModel5 *searcherModel;

    /**
     * @brief Determines if the IV seed cache can be used for fast IV search
     *
     * @return true Fast search is enabled
     * @return false Fast search is not disabled
     */
    bool fastSearchEnabled() const;

    /**
     * @brief Checks whether a state should be removed based on the selected pokemon and item filters
     *
     * @param state State to compare
     * @param itemFilter Item filter to compare
     * @return true State should be removed
     * @return false State should be kept
     */
    bool removeByGeneratorFilters(const WildState5 &state) const;

    /**
     * @brief Checks whether a state should be removed based on the selected searcher filter mode
     *
     * @param state State to compare
     * @return true State should be removed
     * @return false State should be kept
     */
    bool removeBySearcherFilters(const WildState5 &state) const;

    /**
     * @brief Toggles searcher pokemon/item filter controls based on selected mode
     */
    void updateSearcherFilterMode();

    /**
     * @brief Updates item filter visibility and entries based on the encounter type
     *
     * @param itemLabel Item filter label to update
     * @param itemFilter Item filter to update
     * @param area Area to use for item availability
     */
    void updateItemFilter(QWidget *itemLabel, CheckList *itemFilter, EncounterArea5 &area, bool checkAll);

private slots:
    /**
     * @brief Generates phenomenon encounters from a starting seed
     */
    void generate();

    /**
     * @brief Updates the locations listed. Also toggles what controls are displayed based on relevance to the current settings.
     *
     * @param index Encounter index
     */
    void generatorEncounterIndexChanged(int index);

    /**
     * @brief Updates the pokemon listed
     *
     * @param index Location index
     */
    void generatorLocationIndexChanged(int index);

    /**
     * @brief Updates the encounter slot filter based on the pokemon
     *
     * @param index Pokemon index
     */
    void generatorPokemonIndexChanged(int index);

    /**
     * @brief Updates the pokemon listed based on season
     *
     * @param index Season index
     */
    void generatorSeasonIndexChanged(int index);

    /**
     * @brief Opens adjacent seeds for the selected searcher result
     */
    void openAdjacentSeeds();

    /**
     * @brief Opens the advance finder dialog
     */
    void openAdvanceFinder();

    /**
     * @brief Updates displayed information for a profile.  Also toggles what controls are displayed based on relevance to the current
     * settings.
     *
     * @param index Profile index
     */
    void profileIndexChanged(int index);

    /**
     * @brief Opens the profile manager
     */
    void profileManager();

    /**
     * @brief Searches static encounters from the provided IVs
     */
    void search();

    /**
     * @brief Updates the locations listed. Also toggles what controls are displayed based on relevance to the current settings.
     *
     * @param index Encounter index
     */
    void searcherEncounterIndexChanged(int index);

    /**
     * @brief Updates fast search eligibility based on IV advances and IV filters.
     */
    void searcherFastSearchChanged();

    /**
     * @brief Updates the pokemon listed
     *
     * @param index Location index
     */
    void searcherLocationIndexChanged(int index);

    /**
     * @brief Updates the encounter slot filter based on the pokemon
     *
     * @param index Pokemon index
     */
    void searcherPokemonIndexChanged(int index);

    /**
     * @brief Updates the pokemon listed based on season
     *
     * @param index Season index
     */
    void searcherSeasonIndexChanged(int index);

    /**
     * @brief Transfers the filters from the active tab to the inactive tab
     * 
     * @param index Which tab widget to copy from
     */
    void transferFilters(int index);

    /**
     * @brief Transfers the settings from the active tab to the inactive tab
     * 
     * @param index Which tab widget to copy from
     */
    void transferSettings(int index);
};

#endif // PHENOMENON_HPP
