/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef SEEDTIME4MODEL_HPP
#define SEEDTIME4MODEL_HPP

#include <Core/Gen4/SeedTime4.hpp>
#include <Model/TableModel.hpp>

enum class Game : u32;

/**
 * @brief Provides a table model implementation to show Seed to Time information for Gen 4
 */
class SeedTimeModel4 : public TableModel<SeedTime4>
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new SeedTimeModel4 object
     *
     * @param parent Parent object, which takes memory ownership
     */
    SeedTimeModel4(QObject *parent);

    /**
     * @brief Returns the number of columns in the model
     *
     * @param parent Unused parent index
     *
     * @return Number of columns
     */
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief Returns data at the \p index with \p role
     *
     * @param index Row/column index
     * @param role Model data role
     *
     * @return Data at index
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /**
     * @brief Returns header text at the \p section, \p orientation, and \p role
     *
     * @param section Column index
     * @param orientation Header position
     * @param role Model data role
     *
     * @return Header text at column
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QStringList header = { tr("Seed"), tr("Date/Time") };
};

/**
 * @brief Provides a table model implementation to show Seed to Time Calibration information for Gen 4
 */
class SeedTimeCalibrateModel4 : public TableModel<SeedTimeCalibrate4>
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new SeedTimeCalibrateModel4 object
     *
     * @param parent Parent object, which takes memory ownership
     */
    SeedTimeCalibrateModel4(QObject *parent, Game version);

    /**
     * @brief Returns the number of columns in the model
     *
     * @param parent Unused parent index
     *
     * @return Number of columns
     */
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief Returns data at the \p index with \p role
     *
     * @param index Row/column index
     * @param role Model data role
     *
     * @return Data at index
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    /**
     * @brief Returns header text at the \p section, \p orientation, and \p role
     *
     * @param section Column index
     * @param orientation Header position
     * @param role Model data role
     *
     * @return Header text at column
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QStringList header = { tr("Seed"), tr("Date/Time"), tr("Delay"), tr("Coin flips"), tr("Calls"), tr("Roamer locations") };
    Game version;

    /**
     * @brief Gets modified column index based on model method
     *
     * @param column Original column
     *
     * @return Modified column
     */
    int getColumn(int column) const;
};

#endif // SEEDTIME4MODEL_HPP
