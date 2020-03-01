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

#include "ProfileModel5.hpp"

ProfileModel5::ProfileModel5(QObject *parent) : TableModel<Profile5>(parent)
{
}

int ProfileModel5::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return 14;
}

QVariant ProfileModel5::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        auto profile = model.at(index.row());
        switch (index.column())
        {
        case 0:
            return profile.getName();
        case 1:
            return profile.getVersionString();
        case 2:
            return profile.getLanguageString();
        case 3:
            return QString::number(profile.getTID());
        case 4:
            return QString::number(profile.getSID());
        case 5:
            return QString::number(profile.getMac(), 16);
        case 6:
            return profile.getDSTypeString();
        case 7:
            return QString::number(profile.getVCount(), 16);
        case 8:
            return QString::number(profile.getTimer0Min(), 16) + "/" + QString::number(profile.getTimer0Max(), 16);
        case 9:
            return profile.getGxStat();
        case 10:
            return profile.getVFrame();
        case 11:
            return profile.getKeypressesString();
        case 12:
            return profile.getSkipLR() ? tr("True") : tr("False");
        case 13:
            return profile.getSoftReset() ? tr("True") : tr("False");
        }
    }
    return QVariant();
}

QVariant ProfileModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header.at(section);
    }
    return QVariant();
}
