/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef POKESPOT_HPP
#define POKESPOT_HPP

#include <QWidget>

class EncounterArea;
class PokeSpotModel;
class Profile3;

namespace Ui
{
    class PokeSpot;
}

/**
 * @brief Provides settings and filters to RNG Poke Spot encounters in GameCube games
 */
class PokeSpot : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the profiles have been modified
     */
    void profilesModified(int);

public:
    /**
     * @brief Construct a new PokeSpot object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    PokeSpot(QWidget *parent = nullptr);

    /**
     * @brief Destroy the PokeSpot object
     */
    ~PokeSpot() override;

    /**
     * @brief Reloads profiles
     */
    void updateProfiles();

private:
    Ui::PokeSpot *ui;

    PokeSpotModel *model;
    Profile3 *currentProfile;
    std::vector<EncounterArea> encounters;
    std::vector<Profile3> profiles;

private slots:
    /**
     * @brief Generates static encounters from a starting seed
     */
    void generate();

    /**
     * @brief Updates the pokemon listed
     *
     * @param index Location index
     */
    void locationIndexChanged(int index);

    /**
     * @brief Updates the encounter slot filter based on the pokemon
     *
     * @param index Pokemon index
     */
    void pokemonIndexChanged(int index);

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
};

#endif // POKESPOT_HPP
