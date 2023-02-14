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

#ifndef SEEDTOTIME3_HPP
#define SEEDTOTIME3_HPP

#include <Core/Global.hpp>
#include <QWidget>

class SeedToTimeModel3;

namespace Ui
{
    class SeedToTime3;
}

/**
 * @brief Allows converted seeds to date/times
 */
class SeedToTime3 : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new SeedToTime3 object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    SeedToTime3(QWidget *parent = nullptr);

    /**
     * @brief Construct a new SeedToTime3 object
     *
     * @param seed PRNG state
     * @param parent Parent widget, which takes memory ownership
     */
    SeedToTime3(u32 seed, QWidget *parent = nullptr);

    /**
     * @brief Destroy the SeedToTime3 object
     */
    ~SeedToTime3() override;

private:
    Ui::SeedToTime3 *ui;

    SeedToTimeModel3 *model;

private slots:
    /**
     * @brief Searches for date/times that generate the specified seed
     */
    void find();
};

#endif // SEEDTOTIME3_HPP
