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

#ifndef PROFILECALIBRATOR5_HPP
#define PROFILECALIBRATOR5_HPP

#include <Core/Global.hpp>
#include <QWidget>

class ProfileSearcherModel5;
class QAbstractButton;

namespace Ui
{
    class ProfileCalibrator5;
}

/**
 * @brief Provides interface to calibrate and create new profiles
 */
class ProfileCalibrator5 : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits that the profiles have been modified
     */
    void profilesModified(int);

public:
    /**
     * @brief Construct a new ProfileCalibrator5 object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    ProfileCalibrator5(QWidget *parent = nullptr);

    /**
     * @brief Destroy the ProfileCalibrator5 object
     */
    ~ProfileCalibrator5() override;

private:
    Ui::ProfileCalibrator5 *ui;

    ProfileSearcherModel5 *model;

    /**
     * @brief Updates default parameter ranges based on DS type and game
     */
    void updateParameters();

private slots:
    /**
     * @brief Adds select needle to the needle string
     *
     * @param button Needle button that was pressed
     */

    void addNeedle(QAbstractButton *button);

    /**
     * @brief Reset the selected needles
     */
    void clearNeedles();

    /**
     * @brief Opens interface to create profile from selected calibration row
     */
    void createProfile();

    /**
     * @brief Updates default parameters when DS type changes
     *
     * @param index DS type index
     */
    void dsTypeIndexChanged(int index);

    /**
     * @brief Opens IV Calculator for IV search method
     */
    void openIVCalculator() const;

    /**
     * @brief Removes the last added needle
     */
    void removeNeedle();

    /**
     * @brief Searches for possible profile calibration matches
     */
    void search();

    /**
     * @brief Updates IV ranges with what was calculated from the IV Calculator
     *
     * @param ivs IV ranges
     */
    void updateIVs(const std::array<std::vector<u8>, 6> &ivs);

    /**
     * @brief Updates default parameters when game changes
     *
     * @param index Game index
     */
    void versionIndexChanged(int index);
};

#endif // PROFILECALIBRATOR5_HPP
