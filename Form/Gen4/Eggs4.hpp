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

#ifndef EGGS4_HPP
#define EGGS4_HPP

#include <QWidget>

class EggGeneratorModel4;
class EggSearcherModel4;
class Profile4;
class SortFilterProxyModel;

namespace Ui
{
    class Eggs4;
}

/**
 * @brief Provides settings and filters to RNG egg encounters in Gen 4 games
 */
class Eggs4 : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the profiles have been changed
     */
    void profilesChanged(int);

public:
    /**
     * @brief Construct a new Eggs4 object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    Eggs4(QWidget *parent = nullptr);

    /**
     * @brief Destroy the Eggs4 object
     */
    ~Eggs4() override;

public slots:
    /**
     * @brief Reloads profiles
     */
    void updateProfiles();

private:
    Ui::Eggs4 *ui;

    QAction *poketch;
    EggGeneratorModel4 *generatorModel;
    EggSearcherModel4 *searcherModel;
    const Profile4 *currentProfile;
    SortFilterProxyModel *proxyModel;

private slots:
    /**
     * @brief Computes number of interactions with the Poketch apps for IV advances
     */
    void calcPoketch();

    /**
     * @brief Generates egg encounters from a starting seed
     */
    void generate();

    /**
     * @brief Searches egg encounters
     */
    void search();

    /**
     * @brief Updates showing profile related information
     *
     * @param profile Selected profile
     */
    void profileChanged(const Profile4 &profile);

    /**
     * @brief Opens seed to time. Provides current game version and seed from the selected tableview row.
     */
    void seedToTime();

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

    /**
     * @brief Opens the advance finder dialog
     */
    void openAdvanceFinder();
};

#endif // EGGS4_HPP
