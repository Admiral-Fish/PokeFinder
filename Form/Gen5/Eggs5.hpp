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

#ifndef EGGS5_HPP
#define EGGS5_HPP

#include <QWidget>

class EggGeneratorModel5;
class EggSearcherModel5;
class Profile5;
class SortFilterProxyModel;

namespace Ui
{
    class Eggs5;
}

/**
 * @brief Provides settings and filters to RNG eggs in Gen 5 games
 */
class Eggs5 : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the profiles have been modified
     */
    void profilesModified(int);

public:
    /**
     * @brief Construct a new Eggs5 object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    Eggs5(QWidget *parent = nullptr);

    /**
     * @brief Destroy the Eggs5 object
     */
    ~Eggs5() override;

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
    Ui::Eggs5 *ui;

    EggGeneratorModel5 *generatorModel;
    EggSearcherModel5 *searcherModel;
    Profile5 *currentProfile;
    SortFilterProxyModel *proxyModel;
    std::vector<Profile5> profiles;

private slots:
    /**
     * @brief Generates egg encounters from a starting seed
     */
    void generate();

    /**
     * @brief Searches egg encounters from date range
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

#endif // EGGS5_HPP
