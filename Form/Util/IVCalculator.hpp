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

#ifndef IVCALCULATOR_HPP
#define IVCALCULATOR_HPP

#include <Core/Global.hpp>
#include <QWidget>

class QLabel;

namespace Ui
{
    class IVCalculator;
}

/**
 * @brief Calculate IV ranges from a pokemon stats.
 */
class IVCalculator : public QWidget
{
    Q_OBJECT
signals:
    /**
     * @brief Emits calculated IVs
     */
    void ivsCalculated(std::array<std::vector<u8>, 6>);

public:
    /**
     * @brief Construct a new IVCalculator object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    IVCalculator(QWidget *parent = nullptr);

    /**
     * @brief Destroy the IVCalculator object
     */
    ~IVCalculator() override;

private:
    Ui::IVCalculator *ui;

    int rows;

    /**
     * @brief Updates label with string representation of IV range
     *
     * @param label QLabel to update
     * @param ivs IVs to display
     */
    void displayIVs(QLabel *label, const std::vector<u8> &ivs);

private slots:
    /**
     * @brief Adds a input entry row
     */
    void addEntry();

    /**
     * @brief Updates display base stats when altform changes
     *
     * @param index Altform index
     */
    void altformIndexChanged(int index);

    /**
     * @brief Computes the IV ranges
     */
    void findIVs();

    /**
     * @brief Updates the pokemon when the game changes
     *
     * @param index Game index
     */
    void gameIndexChanged(int index);

    /**
     * @brief Updates the altforms when the pokemon changes
     *
     * @param index Pokemon index
     */
    void pokemonIndexChanged(int index);

    /**
     * @brief Removes a input entry row
     */
    void removeEntry();
};

#endif // IVCALCULATOR_HPP
