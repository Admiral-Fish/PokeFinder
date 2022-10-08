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

#ifndef GAMECUBE_HPP
#define GAMECUBE_HPP

#include <QWidget>

class GameCubeGeneratorModel;
class GameCubeSearcherModel;
class Profile3;

namespace Ui
{
    class GameCube;
}

class GameCube : public QWidget
{
    Q_OBJECT
signals:
    void alertProfiles(int);

public:
    /**
     * @brief Construct a new Static3 object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    explicit GameCube(QWidget *parent = nullptr);

    /**
     * @brief Destroy the Static3 object
     */
    ~GameCube() override;

    /**
     * @brief Reloads profiles
     */
    void updateProfiles();

private:
    Ui::GameCube *ui;

    GameCubeGeneratorModel *generatorModel;
    GameCubeSearcherModel *searcherModel;
    Profile3 *currentProfile;
    std::vector<Profile3> profiles;

private slots:
    /**
     * @brief Generates static encounters from a starting seed
     */
    void generate();

    /**
     * @brief Updates the pokemon listed
     *
     * @param index Category index
     */
    void generatorCategoryIndexChanged(int index);

    /**
     * @brief Updates the displayed information for a pokemon
     *
     * @param index Pokemon index
     */
    void generatorPokemonIndexChanged(int index);

    /**
     * @brief Updates displayed information for a profile
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
     * @brief Updates the pokemon listed
     *
     * @param index Category index
     */
    void searcherCategoryIndexChanged(int index);

    /**
     * @brief Updates the displayed information for a pokemon
     *
     * @param index Pokemon index
     */
    void searcherPokemonIndexChanged(int index);
};

#endif // GAMECUBE_HPP
