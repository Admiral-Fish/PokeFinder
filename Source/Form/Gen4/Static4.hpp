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

#ifndef STATIC4_HPP
#define STATIC4_HPP

#include <QWidget>

class Profile4;
class StaticGeneratorModel4;
class StaticSearcherModel4;

namespace Ui
{
    class Static4;
}

/**
 * @brief Provides settings and filters to RNG static encounters in Gen 4 games
 */
class Static4 : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the profiles have been modified
     */
    void profilesModified(int);

public:
    /**
     * @brief Construct a new Static4 object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    Static4(QWidget *parent = nullptr);

    /**
     * @brief Destroy the Static3 object
     */
    ~Static4() override;

    /**
     * @brief Reloads profiles
     */
    void updateProfiles();

private:
    Ui::Static4 *ui;

    StaticGeneratorModel4 *generatorModel;
    StaticSearcherModel4 *searcherModel;
    std::vector<Profile4> profiles;
    Profile4 *currentProfile;

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

    /**
     * @brief Opens seed to time. Provides current game version and seed from the selected tableview row.
     */
    void seedToTime();
};

#endif // STATIC4_HPP
