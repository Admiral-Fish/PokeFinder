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

#include "AdjacentSeedModel.hpp"

AdjacentSeedModel::AdjacentSeedModel(QObject *parent) : TableModel(parent)
{
}

int AdjacentSeedModel::columnCount(const QModelIndex &parent) const
{
    return 10;
}

QVariant AdjacentSeedModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &state = model[index.row()];
        switch (index.column())
        {
        case 0:
            return QString::number(state.getSeed(), 16).toUpper().rightJustified(16, '0');
        case 1:
            return QString::fromStdString(state.getDateTime().toString());
        case 2:
            return QString::number(state.getTimer0(), 16).toUpper();
        case 3:
            return state.getIVAdvance();
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            return state.getIV(index.column() - 4);
        }
    }
    else if (role == Qt::UserRole)
    {
        return model[index.row()].isTarget();
    }
    return QVariant();
}

QVariant AdjacentSeedModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}
