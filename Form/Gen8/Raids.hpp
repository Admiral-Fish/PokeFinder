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

#ifndef RAIDS_HPP
#define RAIDS_HPP

#include <QWidget>

class Profile8;
class StaticModel8;

namespace Ui
{
    class Raids;
}

/**
 * @brief Provides settings and filters to RNG raid encounters in Gen 8 games
 */
class Raids : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the profiles have been modified
     */
    void profilesModified(int);

public:
    /**
     * @brief Construct a new Raids object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    Raids(QWidget *parent = nullptr);

    /**
     * @brief Destroy the Raids object
     */
    ~Raids() override;

public slots:
    /**
     * @brief Reloads profiles
     */
    void updateProfiles();

private:
    Ui::Raids *ui;

    Profile8 *currentProfile;
    StaticModel8 *model;
    std::vector<Profile8> profiles;

private slots:
    /**
     * @brief Updates the pokemon listed
     *
     * @param index Den index
     */
    void denIndexChanged(int index);

    /**
     * @brief Generates raid encounters from a starting seed
     */
    void generate();

    /**
     * @brief Updates the dens listed
     *
     * @param index Location index
     */
    void locationIndexChanged(int index);

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
     * @brief Updates the pokemon listed
     *
     * @param index Rarity index
     */
    void rarityIndexChange(int index);

    /**
     * @brief Updates display information for a raid
     *
     * @param index Specie index
     */
    void specieIndexChanged(int index);
};

#endif // RAIDS_HPP
