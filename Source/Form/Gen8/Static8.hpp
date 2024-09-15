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

#ifndef STATIC8_HPP
#define STATIC8_HPP

#include <QWidget>

class Profile8;
class StaticModel8;

namespace Ui
{
    class Static8;
}

/**
 * @brief Provides settings and filters to RNG static encounters in Gen 8 games
 */
class Static8 : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the profiles have been modified
     */
    void profilesModified(int);

public:
    /**
     * @brief Construct a new Static8 object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    Static8(QWidget *parent = nullptr);

    /**
     * @brief Destroy the Static8 object
     */
    ~Static8() override;

    /**
     * @brief Reloads profiles
     */
    void updateProfiles();

private:
    Ui::Static8 *ui;

    Profile8 *currentProfile;
    StaticModel8 *model;
    std::vector<Profile8> profiles;

private slots:
    /**
     * @brief Updates the pokemon listed
     *
     * @param index Category index
     */
    void categoryIndexChanged(int index);

    /**
     * @brief Generates static encounters from a starting seed
     */
    void generate();

    /**
     * @brief Updates the displayed information for a pokemon
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

#endif // STATIC8_HPP
