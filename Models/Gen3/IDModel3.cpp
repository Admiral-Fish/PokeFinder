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

#include "IDModel3.hpp"

IDModel3::IDModel3(QObject *parent) : TableModel<IDFrame3>(parent)
{
}

int IDModel3::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return 4;
}

QVariant IDModel3::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &frame = model.at(index.row());
        switch (index.column())
        {
        case 0:
            return frame.getFrame();
        case 1:
            return frame.getTID();
        case 2:
            return frame.getSID();
        case 3:
            return frame.getTSV();
        }
    }

    return QVariant();
}

QVariant IDModel3::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header.at(section);
    }
    return QVariant();
}
