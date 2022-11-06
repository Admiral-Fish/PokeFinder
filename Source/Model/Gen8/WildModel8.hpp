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

#ifndef WILDMODEL8_HPP
#define WILDMODEL8_HPP

#include <Core/Gen8/States/WildState8.hpp>
#include <Model/TableModel.hpp>

/**
 * @brief Provides a table model implementation to show wild encounter information for Gen 8
 */
class WildModel8 : public TableModel<WildGeneratorState8>
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new WildModel8 object
     *
     * @param parent Parent object, which takes memory ownership
     */
    WildModel8(QObject *parent);

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
    QVariant data(const QModelIndex &index, int role) const override;

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

public slots:
    /**
     * @brief Sets flag that controls whether the model display stats or IVs
     *
     * @param flag Whether to show stats or not
     */
    void setShowStats(bool flag);

private:
    QStringList header = { tr("Advances"), tr("Item"), tr("Slot"), tr("Level"), tr("PID"), tr("Shiny"), tr("Nature"), tr("Ability"),
                           tr("HP"),       tr("Atk"),  tr("Def"),  tr("SpA"),   tr("SpD"), tr("Spe"),   tr("Hidden"), tr("Gender") };
    bool showStats;
};

#endif // WILDMODEL8_HPP
