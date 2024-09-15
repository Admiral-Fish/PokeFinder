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

#ifndef CHAINEDSID_HPP
#define CHAINEDSID_HPP

#include <QWidget>

class ChainedSIDCalc;
class QStandardItemModel;

namespace Ui
{
    class ChainedSID;
}

/**
 * @brief Allows computing SID from chained shiny Pokemon
 */
class ChainedSID : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new ChainedSID object
     *
     * @param parent Parent widget, which takes memory ownership
     */
    ChainedSID(QWidget *parent = nullptr);

    /**
     * @brief Destroy the ChainedSID object
     */
    ~ChainedSID() override;

private:
    Ui::ChainedSID *ui;

    ChainedSIDCalc *chainedCalc;
    QStandardItemModel *model;

private slots:
    /**
     * @brief Starts a new search for the selected pokemon and TID
     */
    void calculate();

    /**
     * @brief Clears and resets the current search
     */
    void clear();

    /**
     * @brief Updates ability and gender for the selected Pokemon
     *
     * @param index Pokemon index
     */
    void pokemonIndexChanged(int index);
};

#endif // CHAINEDSID_HPP
