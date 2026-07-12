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

#ifndef ADJACENTSEEDS_HPP
#define ADJACENTSEEDS_HPP

#include <Core/Global.hpp>
#include <QWidget>

class AdjacentSeedsModel;
class DateTime;
class Profile5;
enum class Buttons : u16;

namespace Ui
{
    class AdjacentSeeds;
}

/**
 * @brief Provides settings to calibrate nearby seeds
 */
class AdjacentSeeds : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the profiles have been changed
     */
    void profilesChanged(int);

public:
    /**
     * @brief Construct a new AdjacentSeeds object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    AdjacentSeeds(QWidget *parent = nullptr);

    /**
     * @brief Construct a new AdjacentSeeds object
     *
     * @param roamer Whether encounter is roamer or not
     * @param buttons Current button presses
     * @param dateTime Starting date/time
     * @param profile Profile information
     * @param parent Parent widget, which takes memory ownership
     */
    AdjacentSeeds(bool roamer, Buttons buttons, const DateTime &dateTime, const Profile5 &profile, QWidget *parent = nullptr);

    /**
     * @brief Destroy the AdjacentSeeds object
     */
    ~AdjacentSeeds() override;

    /**
     * @brief Determines if any profiles exist
     *
     * @return true At least 1 profile exists
     * @return false 0 profiles exist
     */
    bool hasProfiles() const;

public slots:
    /**
     * @brief Reloads profiles
     */
    void updateProfiles();

private:
    Ui::AdjacentSeeds *ui;

    AdjacentSeedsModel *model;
    const Profile5 *currentProfile;

private slots:
    /**
     * @brief Generates adjacent seeds from a date/time and keypresses
     */
    void generate();

    /**
     * @brief Opens IV Calculator for IV search method
     */
    void openIVCalculator();

    /**
     * @brief Updates showing profile related information
     *
     * @param profile Selected profile
     */
    void profileChanged(const Profile5 &profile);

    /**
     * @brief Updates chatot/needle string for seed verification purposes
     */
    void updatePreview();
};

#endif // ADJACENTSEEDS_HPP
