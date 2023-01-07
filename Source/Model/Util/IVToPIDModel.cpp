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

#include "IVToPIDModel.hpp"

IVToPIDModel::IVToPIDModel(QObject *parent) : TableModel(parent)
{
}

int IVToPIDModel::columnCount(const QModelIndex &parent) const
{
    return 9;
}

QVariant IVToPIDModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &state = model[index.row()];
        int column = index.column();
        switch (column)
        {
        case 0:
            return QString::number(state.getSeed(), 16).toUpper().rightJustified(8, '0');
        case 1:
            return QString::number(state.getPID(), 16).toUpper().rightJustified(8, '0');
        case 2:
            switch (state.getMethod())
            {
            case IVToPIDMethod::Method1:
                return tr("Method 1");
            case IVToPIDMethod::ReverseMethod1:
                return tr("Reverse Method 1");
            case IVToPIDMethod::Method2:
                return tr("Method 2");
            case IVToPIDMethod::Method4:
                return tr("Method 4");
            case IVToPIDMethod::CuteCharmDPPt:
                return tr("Cute Charm (DPPt)");
            case IVToPIDMethod::CuteCharmHGSS:
                return tr("Cute Charm (HGSS)");
            case IVToPIDMethod::XDColo:
                return tr("XD/Colo");
            case IVToPIDMethod::Channel:
                return tr("Channel");
            }
        case 3:
            return state.getPID() & 1;
        case 4:
            return (state.getPID() & 0xff) > 30 ? "♂" : "♀";
        case 5:
            return (state.getPID() & 0xff) > 63 ? "♂" : "♀";
        case 6:
            return (state.getPID() & 0xff) > 126 ? "♂" : "♀";
        case 7:
            return (state.getPID() & 0xff) > 190 ? "♂" : "♀";
        case 8:
            return state.getSID();
        }
    }
    return QVariant();
}

QVariant IVToPIDModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}
