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

#ifndef EVENT4_HPP
#define EVENT4_HPP

#include <QWidget>

class EventGeneratorModel4;
class EventSearcherModel4;
class Profile4;
class SortFilterProxyModel;

namespace Ui
{
    class Event4;
}

/**
 * @brief Provides settings and filters to RNG event encounters in Gen 4 games
 */
class Event4 : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the profiles have been modified
     */
    void profilesModified(int);

public:
    /**
     * @brief Construct a new Event4 object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    Event4(QWidget *parent = nullptr);

    /**
     * @brief Destroy the Event4 object
     */
    ~Event4() override;

public slots:
    /**
     * @brief Reloads profiles
     */
    void updateProfiles();

private:
    Ui::Event4 *ui;

    EventGeneratorModel4 *generatorModel;
    EventSearcherModel4 *searcherModel;
    Profile4 *currentProfile;
    SortFilterProxyModel *proxyModel;
    std::vector<Profile4> profiles;

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

    /**
     * @brief Searches static encounters from the provided IVs
     */
    void search();

    /**
     * @brief Opens seed to time. Provides current game version and seed from the selected tableview row.
     */
    void seedToTime();

    /**
     * @brief Transfers the current Searcher filters to the Generator.
     */
    void transferFiltersToGenerator();

    /**
     * @brief Transfers the current Searcher settings to the Generator.
     */
    void transferSettingsToGenerator();
};

#endif // EVENT4_HPP
