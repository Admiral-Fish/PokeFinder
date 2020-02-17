/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "PIDIVModel.hpp"
#include <Core/Enum/Method.hpp>

PIDIVModel::PIDIVModel(QObject *parent) : TableModel<PIDIVFrame>(parent)
{
}

int PIDIVModel::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return 3;
}

QVariant PIDIVModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        auto const &frame = model.at(index.row());
        switch (index.column())
        {
        case 0:
            return QString::number(frame.getSeed(), 16).toUpper().rightJustified(8, '0');
        case 1:
            switch (frame.getMethod())
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
        {
            QString result;
            for (u8 i = 0; i < 6; i++)
            {
                result += QString::number(frame.getIV(i));
                if (i != 5)
                {
                    result += ".";
                }
            }
            return result;
        }
        }
    }
    return QVariant();
}

QVariant PIDIVModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header.at(section);
    }
    return QVariant();
}
