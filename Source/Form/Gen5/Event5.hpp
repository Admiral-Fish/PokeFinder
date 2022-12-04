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

#ifndef EVENT5_HPP
#define EVENT5_HPP

#include <QWidget>

class EventGeneratorModel5;
class EventSearcherModel5;
class PGF;
class Profile5;

namespace Ui
{
    class Event5;
}

/**
 * @brief Provides settings and filters to RNG event encounters in Gen 5 games
 */
class Event5 : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the profiles have been modified
     */
    void profilesModified(int);

public:
    /**
     * @brief Construct a new Event5 object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    Event5(QWidget *parent = nullptr);

    /**
     * @brief Destroy the Event5 object
     */
    ~Event5() override;

    /**
     * @brief Determines if any profiles exist
     *
     * @return true At least 1 profile exists
     * @return false 0 profiles exist
     */
    bool hasProfiles() const;

    /**
     * @brief Reloads profiles
     */
    void updateProfiles();

private:
    Ui::Event5 *ui;

    EventGeneratorModel5 *generatorModel;
    EventSearcherModel5 *searcherModel;
    Profile5 *currentProfile;
    std::vector<Profile5> profiles;

    /**
     * @brief Loads UI settings for a wondercard
     *
     * @return Wondercard template
     */
    PGF getGeneratorParameters() const;

    /**
     * @brief Loads UI settings for a wondercard
     *
     * @return Wondercard template
     */
    PGF getSearcherParameters() const;

private slots:
    /**
     * @brief Generates static encounters from a starting seed
     */
    void generate();

    /**
     * @brief Reads in settings from a imported wondercard file
     */
    void generatorImportEvent();

    /**
     * @brief Searches static encounters from date range
     */
    void search();

    /**
     * @brief Reads in settings from a imported wondercard file
     */
    void searcherImportEvent();

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
