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

#ifndef WILD4_HPP
#define WILD4_HPP

#include <QWidget>

class EncounterArea4;
class Profile4;
class WildGeneratorModel4;
class WildSearcherModel4;

namespace Ui
{
    class Wild4;
}

/**
 * @brief Provides settings and filters to RNG wild encounters in Gen 4 games
 */
class Wild4 : public QWidget
{
    Q_OBJECT
signals:
    void alertProfiles(int);

public:
    /**
     * @brief Construct a new Wild4 object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    explicit Wild4(QWidget *parent = nullptr);

    /**
     * @brief Destroy the Wild4 object
     */
    ~Wild4() override;

    /**
     * @brief Reloads profiles
     */
    void updateProfiles();

private:
    Ui::Wild4 *ui;

    Profile4 *currentProfile;
    std::vector<EncounterArea4> encounterGenerator;
    std::vector<EncounterArea4> encounterSearcher;
    std::vector<Profile4> profiles;
    WildGeneratorModel4 *generatorModel;
    WildSearcherModel4 *searcherModel;

    /**
     * @brief Updates generator encounter tables
     */
    void updateEncounterGenerator();

    /**
     * @brief Update searcher encounter tables
     */
    void updateEncounterSearcher();

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
     * @brief Updates encounter tables and listed pokemon
     */
    void generatorEncounterUpdate();

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
     * @brief Toggles what controls are displayed based on relevance to Poke Radar
     *
     * @param state Checkbox state
     */
    void generatorPokeRadarStateChanged(int state);

    /**
     * @brief Updates displayed information for a profile.  Also toggles what controls are displayed based on relevance to the current
     * settings.
     *
     * @param index Profile index
     */
    void profilesIndexChanged(int index);

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
     * @brief Updates encounter tables and listed pokemon
     */
    void searcherEncounterUpdate();

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
     * @brief Toggles what controls are displayed based on relevance to Poke Radar
     *
     * @param state Checkbox state
     */
    void searcherPokeRadarStateChanged(int state);

    /**
     * @brief Opens seed to time. Provides current game version and seed from the selected tableview row.
     */
    void seedToTime();
};

#endif // WILD4_HPP
