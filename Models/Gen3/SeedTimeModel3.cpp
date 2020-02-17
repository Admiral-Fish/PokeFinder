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

#include "SeedTimeModel3.hpp"

SeedTimeModel3::SeedTimeModel3(QObject *parent) : TableModel<SeedTimeFrame3>(parent)
{
}

int SeedTimeModel3::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return 2;
}

QVariant SeedTimeModel3::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &frame = model.at(index.row());
        switch (index.column())
        {
        case 0:
            return frame.getDateTime();
        case 1:
            return frame.getFrame();
        }
    }
    return QVariant();
}

QVariant SeedTimeModel3::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header.at(section);
    }
    return QVariant();
}
