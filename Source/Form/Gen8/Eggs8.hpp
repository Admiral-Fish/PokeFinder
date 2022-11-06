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

#ifndef EGGS8_HPP
#define EGGS8_HPP

#include <QWidget>

class EggModel8;
class Profile8;
class QMenu;

namespace Ui
{
    class Eggs8;
}

/**
 * @brief Provides settings and filters to RNG egg encounters in Gen 8 games
 */
class Eggs8 : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the profiles have been modified
     */
    void profilesModified(int);

public:
    /**
     * @brief Construct a new Eggs8 object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    Eggs8(QWidget *parent = nullptr);

    /**
     * @brief Destroy the Static8 object
     */
    ~Eggs8() override;

    /**
     * @brief Reloads profiles
     */
    void updateProfiles();

private:
    Ui::Eggs8 *ui;

    EggModel8 *model;
    Profile8 *currentProfile;
    std::vector<Profile8> profiles;

private slots:
    /**
     * @brief Generates static encounters from a starting seed
     */
    void generate();

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

#endif // EGGS8_HPP
