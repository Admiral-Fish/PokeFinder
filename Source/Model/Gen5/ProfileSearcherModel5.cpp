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

#include "ProfileSearcherModel5.hpp"

ProfileSearcherModel5::ProfileSearcherModel5(QObject *parent) : TableModel(parent)
{
}

int ProfileSearcherModel5::columnCount(const QModelIndex &parent) const
{
    return 6;
}

QVariant ProfileSearcherModel5::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &state = model[index.row()];
        switch (index.column())
        {
        case 0:
            return QString::number(state.getSeed(), 16);
        case 1:
            return state.getSecond();
        case 2:
            return QString::number(state.getVCount(), 16);
        case 3:
            return QString::number(state.getTimer0(), 16);
        case 4:
            return QString::number(state.getGxStat(), 16);
        case 5:
            return QString::number(state.getVFrame(), 16);
        }
    }
    return QVariant();
}

QVariant ProfileSearcherModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}
