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

#ifndef DENMAP_HPP
#define DENMAP_HPP

#include <QWidget>

namespace Ui
{
    class DenMap;
}

/**
 * @brief Allows seeing the locations of dens on the map
 */
class DenMap : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new DenMap object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    DenMap(QWidget *parent = nullptr);

    /**
     * @brief Destroy the DenMap object
     */
    ~DenMap() override;

private:
    Ui::DenMap *ui;

private slots:
    /**
     * @brief Updates the map image with the location of the den
     *
     * @param index Den index
     */
    void denIndexChanged(int index);

    /**
     * @brief Updates the dens for the regional location
     *
     * @param index Region location index
     */
    void locationIndexChanged(int index);
};

#endif // DENMAP_HPP
