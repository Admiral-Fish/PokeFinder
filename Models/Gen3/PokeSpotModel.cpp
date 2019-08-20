/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2019 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "PokeSpotModel.hpp"

PokeSpotModel::PokeSpotModel(QObject *parent) :
    TableModel<Frame3>(parent)
{
}

int PokeSpotModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 7;
}

QVariant PokeSpotModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        auto frame = model.at(index.row());
        switch (index.column())
        {
            case 0:
                return frame.getFrame();
            case 1:
                return QString::number(frame.getPID(), 16).toUpper().rightJustified(8, '0');
            case 2:
                return frame.getShinyString();
            case 3:
                return frame.getLockReason();
            case 4:
                return frame.getNatureString();
            case 5:
                return frame.getAbility();
            case 6:
                return frame.getGenderString();
        }
    }
    return QVariant();
}

QVariant PokeSpotModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header.at(section);
    }
    return QVariant();
}
