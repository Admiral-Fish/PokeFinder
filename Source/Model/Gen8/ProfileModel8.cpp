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

#include "ProfileModel8.hpp"
#include <Core/Util/Translator.hpp>

ProfileModel8::ProfileModel8(QObject *parent) : TableModel<Profile8>(parent)
{
}

int ProfileModel8::columnCount(const QModelIndex &parent) const
{
    return 6;
}

QVariant ProfileModel8::data(const QModelIndex &index, int role) const
{
        if (role == Qt::DisplayRole)
    {
        const auto &profile = model[index.row()];
        switch (index.column())
        {
        case 0:
            return QString::fromStdString(profile.getName());
        case 1:
            return QString::fromStdString(Translator::getGame(profile.getVersion()));
        case 2:
            return profile.getTID();
        case 3:
            return profile.getSID();
        case 4:
            return profile.getShinyCharm() ? tr("Yes") : tr("No");
        case 5:
            return profile.getOvalCharm() ? tr("Yes") : tr("No");
        }
    }
    return QVariant();
}

QVariant ProfileModel8::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}
