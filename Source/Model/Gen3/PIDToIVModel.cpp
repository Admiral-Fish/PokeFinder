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

#include "PIDToIVModel.hpp"
#include <Core/Enum/Method.hpp>

PIDToIVModel::PIDToIVModel(QObject *parent) : TableModel(parent)
{
}

int PIDToIVModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QVariant PIDToIVModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        auto const &state = model[index.row()];
        switch (index.column())
        {
        case 0:
            return QString::number(state.getSeed(), 16).toUpper().rightJustified(8, '0');
        case 1:
            switch (state.getMethod())
            {
            case Method::Method1:
                return tr("Method 1");
            case Method::Method2:
                return tr("Method 2");
            case Method::Method4:
                return tr("Method 4");
            case Method::XDColo:
                return tr("XD/Colo");
            case Method::Channel:
                return tr("Channel");
            default:
                return QVariant();
            }
        case 2:
            return QString("%1.%2.%3.%4.%5.%6")
                .arg(state.getIV(0))
                .arg(state.getIV(1))
                .arg(state.getIV(2))
                .arg(state.getIV(3))
                .arg(state.getIV(4))
                .arg(state.getIV(5));
        }
    }
    return QVariant();
}

QVariant PIDToIVModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}
