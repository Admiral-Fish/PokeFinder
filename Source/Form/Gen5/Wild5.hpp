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

#ifndef WILD5_HPP
#define WILD5_HPP

#include <QWidget>

class EncounterArea5;
class Profile5;
class WildGeneratorModel5;
class WildSearcherModel5;

namespace Ui
{
    class Wild5;
}

/**
 * @brief Provides settings and filters to RNG wild encounters in Gen 5 games
 */
class Wild5 : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the profiles have been modified
     */
    void profilesModified(int);

public:
    /**
     * @brief Construct a new Wild5 object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    Wild5(QWidget *parent = nullptr);

    /**
     * @brief Destroy the Wild4 object
     */
    ~Wild5() override;

    /**
     * @brief Determines if any profiles exist
     *
     * @return true At least 1 profile exists
     * @return false 0 profiles exist
     */
    bool hasProfiles() const;

    /**
     * @brief Reloads profiles
     */
    void updateProfiles();

private:
    Ui::Wild5 *ui;

    Profile5 *currentProfile;
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

private slots:
    /**
     * @brief Generates wild encounters from a starting seed
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
};

#endif // WILD5_HPP
