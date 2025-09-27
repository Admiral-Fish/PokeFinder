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

#ifndef WILD3_HPP
#define WILD3_HPP

#include <QWidget>

class EncounterArea3;
class Profile3;
class SortFilterProxyModel;
class WildGeneratorModel3;
class WildSearcherModel3;

namespace Ui
{
    class Wild3;
}

/**
 * @brief Provides settings and filters to RNG wild encounters in Gen 3 games
 */
class Wild3 : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the profiles have been modified
     */
    void profilesModified(int);

public:
    /**
     * @brief Construct a new Wild3 object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    Wild3(QWidget *parent = nullptr);

    /**
     * @brief Destroy the Wild3 object
     */
    ~Wild3() override;

public slots:
    /**
     * @brief Reloads profiles
     */
    void updateProfiles();

private:
    Ui::Wild3 *ui;

    Profile3 *currentProfile;
    SortFilterProxyModel *proxyModel;
    std::vector<EncounterArea3> encounterGenerator;
    std::vector<EncounterArea3> encounterSearcher;
    std::vector<Profile3> profiles;
    WildGeneratorModel3 *generatorModel;
    WildSearcherModel3 *searcherModel;

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
     * @brief Updates the locations listed
     *
     * @param index Encounter index
     */
    void generatorEncounterIndexChanged(int index);

    /**
     * @brief Updates filters for Feebas selection
     *
     * @param state Checked state
     */
    void generatorFeebasTileStateChanged(Qt::CheckState state);

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
     * @brief Updates displayed information for a profile
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
     * @brief Updates the locations listed
     *
     * @param index Encounter index
     */
    void searcherEncounterIndexChanged(int index);

    /**
     * @brief Updates filters for Feebas selection
     *
     * @param state Checked state
     */
    void searcherFeebasTileStateChanged(Qt::CheckState state);

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
     * @brief Opens seed to time.
     */
    void seedToTime();
};

#endif // WILD3_HPP
