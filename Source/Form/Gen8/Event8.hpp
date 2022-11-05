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

#ifndef EVENT8_HPP
#define EVENT8_HPP

#include <QWidget>

class Profile8;
class StaticModel8;
class WB8;

namespace Ui
{
    class Event8;
}

/**
 * @brief Provides settings and filters to RNG event encounters in Gen 8 games
 */
class Event8 : public QWidget
{
    Q_OBJECT
signals:
    void alertProfiles(int);

public:
    /**
     * @brief Construct a new Event8 object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    explicit Event8(QWidget *parent = nullptr);

    /**
     * @brief Destroy the Static3 object
     */
    ~Event8() override;

    /**
     * @brief Reloads profiles
     */
    void updateProfiles();

private:
    Ui::Event8 *ui;

    Profile8 *currentProfile;
    StaticModel8 *model;
    std::vector<Profile8> profiles;

    /**
     * @brief Loads UI settings for a wondercard
     *
     * @return Wondercard template
     */
    WB8 getParameters() const;

private slots:
    /**
     * @brief Reads in settings from a imported wondercard file
     */
    void importEvent();

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

#endif // EVENT5_HPP
