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

#ifndef EGGS4_HPP
#define EGGS4_HPP

#include <QWidget>

class EggGeneratorModel4;
class EggSearcherModel4;
class Profile4;
class QMenu;

namespace Ui
{
    class Eggs4;
}

/**
 * @brief Provides settings and filters to RNG egg encounters in Gen 4 games
 */
class Eggs4 : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the profiles have been modified
     */
    void profilesModified(int);

public:
    /**
     * @brief Construct a new Eggs4 object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    Eggs4(QWidget *parent = nullptr);

    /**
     * @brief Destroy the Static3 object
     */
    ~Eggs4() override;

    /**
     * @brief Reloads profiles
     */
    void updateProfiles();

private:
    Ui::Eggs4 *ui;

    QAction *poketch;
    EggGeneratorModel4 *generatorModel;
    EggSearcherModel4 *searcherModel;
    Profile4 *currentProfile;
    std::vector<Profile4> profiles;

private slots:
    /**
     * @brief Computes number of interactions with the Poketch apps for IV advances
     */
    void calcPoketch();

    /**
     * @brief Generates egg encounters from a starting seed
     */
    void generate();

    /**
     * @brief Searches egg encounters
     */
    void search();

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
     * @brief Opens seed to time. Provides current game version and seed from the selected tableview row.
     */
    void seedToTime();
};

#endif // EGGS4_HPP
