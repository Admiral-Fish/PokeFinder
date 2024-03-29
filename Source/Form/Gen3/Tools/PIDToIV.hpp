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

#ifndef PIDTOIVS_HPP
#define PIDTOIVS_HPP

#include <Core/Global.hpp>
#include <QWidget>

class PIDToIVModel;

namespace Ui
{
    class PIDToIV;
}

/**
 * @brief Calculates potentional IVs from the given PID for various methods
 */
class PIDToIV : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new PIDToIV object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    PIDToIV(QWidget *parent = nullptr);

    /**
     * @brief Destroy the PIDToIV object
     */
    ~PIDToIV() override;

private:
    Ui::PIDToIV *ui;

    PIDToIVModel *model;

private slots:
    /**
     * @brief Computes IVs for the given PID
     */
    void generate();
};

#endif // PIDTOIVS_HPP
