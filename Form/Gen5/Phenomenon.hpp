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

#ifndef PHENOMENON_HPP
#define PHENOMENON_HPP

#include <QWidget>

class PhenomenonArea;
class PhenomenonGeneratorModel5;
class PhenomenonSearcherModel5;
class Profile5;
class SortFilterProxyModel;

namespace Ui
{
    class Phenomenon;
}

/**
 * @brief Provides settings and filters to RNG phenomenon in Gen 5
 */
class Phenomenon : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the profiles have been modified
     */
    void profilesModified(int);

public:
    /**
     * @brief Construct a new Phenomenon object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    Phenomenon(QWidget *parent = nullptr);

    /**
     * @brief Destroy the Phenomenon object
     */
    ~Phenomenon() override;

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
    Ui::Phenomenon *ui;

    PhenomenonGeneratorModel5 *generatorModel;
    PhenomenonSearcherModel5 *searcherModel;
    Profile5 *currentProfile;
    SortFilterProxyModel *proxyModel;
    std::vector<PhenomenonArea> encounter;
    std::vector<Profile5> profiles;

private slots:
    /**
     * @brief Generates phenomenon encounters from a starting seed
     */
    void generate();

    /**
     * @brief Updates the phenomenon listed
     *
     * @param index Location index
     */
    void generatorLocationIndexChanged(int index);

    /**
     * @brief Searches phenomenon encounters from date range
     */
    void search();

    /**
     * @brief Updates the phenomenon listed
     *
     * @param index Location index
     */
    void searcherLocationIndexChanged(int index);

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
     * @brief Transfers the filters from the active tab to the inactive tab
     * 
     * @param index Which tab widget to copy from
     */
    void transferFilters(int index);

    /**
     * @brief Transfers the settings from the active tab to the inactive tab
     * 
     * @param index Which tab widget to copy from
     */
    void transferSettings(int index);
};

#endif // PHENOMENON_HPP
