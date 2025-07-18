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

#ifndef IDMODEL5_HPP
#define IDMODEL5_HPP

#include <Core/Gen5/States/SearcherState5.hpp>
#include <Core/Parents/States/IDState.hpp>
#include <Model/TableModel.hpp>

enum class Game : u32;

/**
 * @brief Provides a table model implementation to show TID/SID information for Gen 5
 */
class IDModel5 : public TableModel<SearcherState5<IDState>>
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new IDModel5 object
     *
     * @param parent Parent object, which takes memory ownership
     */
    IDModel5(QObject *parent);

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

    /**
     * @brief Sets the game version
     *
     * @param version Game version
     */
    void setGame(Game version);

private:
    QStringList header = { tr("Seed"), tr("Initial Advances"), tr("Advances"), tr("TID"),    tr("SID"),
                           tr("TSV"),  tr("Date/Time"),        tr("Timer0"),   tr("Buttons") };
    Game version;
};

#endif // IDMODEL5_HPP
