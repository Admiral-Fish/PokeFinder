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

#include "ProfileModel4.hpp"

ProfileModel4::ProfileModel4(QObject *parent) : TableModel<Profile4>(parent)
{
}

int ProfileModel4::columnCount(const QModelIndex & /*parent*/) const
{
    return 8;
}

QVariant ProfileModel4::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &profile = model.at(index.row());
        switch (index.column())
        {
        case 0:
            return profile.getName();
        case 1:
            return profile.getVersionString();
        case 2:
            return profile.getTID();
        case 3:
            return profile.getSID();
        case 4:
            return profile.getDualSlotString();
        case 5:
            return profile.getRadioString();
        case 6:
            return profile.getRadar() ? tr("Yes") : tr("No");
        case 7:
            return profile.getSwarm() ? tr("Yes") : tr("No");
        }
    }
    return QVariant();
}

QVariant ProfileModel4::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header.at(section);
    }
    return QVariant();
}
