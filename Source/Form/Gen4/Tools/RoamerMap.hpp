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

#ifndef ROAMERMAP_HPP
#define ROAMERMAP_HPP

#include <QDialog>

namespace Ui
{
    class RoamerMap;
}

/**
 * @brief Shows a map of roamer locations in HGSS
 */
class RoamerMap : public QDialog
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new RoamerMap object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    RoamerMap(QWidget *parent = nullptr);

    /**
     * @brief Destroy the RoamerMap object
     */
    ~RoamerMap() override;

private:
    Ui::RoamerMap *ui;
};

#endif // ROAMERMAP_HPP
