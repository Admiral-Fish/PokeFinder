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

#ifndef IDS3_HPP
#define IDS3_HPP

#include <QWidget>

class IDModel3;

namespace Ui
{
    class IDs3;
}

/**
 * @brief Provides settings and filters to RNG TID/SID in Gen 3 games
 */
class IDs3 : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new IDs3 object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    IDs3(QWidget *parent = nullptr);

    /**
     * @brief Destroy the IDs3 object
     */
    ~IDs3() override;

private:
    Ui::IDs3 *ui;

    IDModel3 *model;

private slots:
    /**
     * @brief Generates and display results for XD/Colo
     */
    void generateXDColo();

    /**
     * @brief Generates and display results for FRLG/E
     */
    void generateFRLGE();

    /**
     * @brief Generates and display results for RS
     */
    void generateRS();

    /**
     * @brief Toggles whether controls related to dead battery are enabled or not
     *
     * @param checked True if dead battery is enabled, false otherwise
     */
    void toggleDeadBatteryRS(bool checked);

    /**
     * @brief Toggles whether date/time is enabled or not
     *
     * @param checked True if date/time is enabled, false otherwise
     */
    void toggleDateTimeRS(bool checked);

    /**
     * @brief Toggles whether seed is enabled or not
     *
     * @param checked True if seed is enabled, false otherwise
     */
    void toggleSeedRS(bool checked);

    /**
     * @brief Hides/Shows TID/PID filter setting
     *
     * @param index Tab index
     */
    void tabIndexChanged(int index);
};

#endif // IDS3_HPP
