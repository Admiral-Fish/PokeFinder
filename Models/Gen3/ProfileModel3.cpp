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

#include "ProfileModel3.hpp"
#include <Core/Enum/Game.hpp>

ProfileModel3::ProfileModel3(QObject *parent) : TableModel<Profile3>(parent)
{
}

int ProfileModel3::columnCount(const QModelIndex & /*parent*/) const
{
    return 5;
}

QVariant ProfileModel3::data(const QModelIndex &index, int role) const
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
            if (profile.getVersion() & Game::RS)
            {
                return profile.getDeadBattery() ? tr("Yes") : tr("No");
            }
            else
            {
                return "-";
            }
        }
    }
    return QVariant();
}

QVariant ProfileModel3::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header.at(section);
    }
    return QVariant();
}
