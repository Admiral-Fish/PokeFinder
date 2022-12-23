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

#ifndef EGGS3_HPP
#define EGGS3_HPP

#include <QWidget>

class EggModel3;
class Profile3;

namespace Ui
{
    class Eggs3;
}

/**
 * @brief Provides settings and filters to RNG egg encounters in Gen 3 games
 */
class Eggs3 : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the profiles have been modified
     */
    void profilesModified(int);

public:
    /**
     * @brief Construct a new Eggs3 object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    Eggs3(QWidget *parent = nullptr);

    /**
     * @brief Destroy the Eggs3 object
     */
    ~Eggs3() override;

    /**
     * @brief Reloads profiles
     */
    void updateProfiles();

private:
    Ui::Eggs3 *ui;

    EggModel3 *emerald;
    EggModel3 *rsfrlg;
    Profile3 *currentProfile;
    std::vector<Profile3> profiles;

private slots:
    /**
     * @brief Generates egg encounters for Emerald
     */
    void emeraldGenerate();

    /**
     * @brief Generates egg encounters for RS/FRLG
     */
    void rsfrlgGenerate();

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

#endif // EGGS3_HPP
