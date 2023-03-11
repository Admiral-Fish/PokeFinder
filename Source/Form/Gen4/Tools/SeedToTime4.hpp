/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef SEEDTOTIME4_HPP
#define SEEDTOTIME4_HPP

#include <Core/Global.hpp>
#include <QWidget>

class SeedTime4;
class SeedTimeCalibrate4;
class SeedToTimeCalibrateModel4;
class SeedToTimeModel4;
enum class Game : u32;

namespace Ui
{
    class SeedToTime4;
}

/**
 * @brief Allows converted seeds to date/times and calibrating what date/time/delay was hit
 */
class SeedToTime4 : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new SeedToTime4 object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    SeedToTime4(QWidget *parent = nullptr);

    /**
     * @brief Construct a new SeedToTime4 object
     *
     * @param seed PRNG state
     * @param version Game version
     * @param parent Parent widget, which takes memory ownership
     */
    SeedToTime4(u32 seed, Game version, QWidget *parent = nullptr);

    /**
     * @brief Destroy the SeedToTime4 object
     */
    ~SeedToTime4() override;

private:
    Ui::SeedToTime4 *ui;

    SeedToTimeCalibrateModel4 *dpptCalibrateModel;
    SeedToTimeModel4 *dpptModel;
    SeedToTimeCalibrateModel4 *hgssCalibrateModel;
    SeedToTimeModel4 *hgssModel;

private slots:
    /**
     * @brief Generates calibration date/times for DPPt
     */
    void dpptCalibrate();

    /**
     * @brief Generates date/times for DPPt
     */
    void dpptGenerate();

    /**
     * @brief Generates calibration date/times for HGSS
     */
    void hgssCalibrate();

    /**
     * @brief Generates date/times for HGSS
     */
    void hgssGenerate();

    /**
     * @brief Opens the roamer map
     */
    void map() const;

    /**
     * @brief Searches chatot calls to narrow down potentional results
     */
    void searchCalls();

    /**
     * @brief Searches coin flips to narrow down potentional results
     */
    void searchFlips();
};

#endif // SEEDTOTIME4_HPP
