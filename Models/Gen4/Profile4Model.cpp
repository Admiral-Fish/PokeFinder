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

#include "Profile4Model.hpp"

namespace PokeFinderModels
{
    Profile4Model::Profile4Model(QObject *parent)
        : TableModel<PokeFinderCore::Profile4>(parent)
    {
    }

    int Profile4Model::columnCount(const QModelIndex & /*parent*/) const { return 9; }

    QVariant Profile4Model::data(const QModelIndex &index, int role) const
    {
        if (role == Qt::DisplayRole)
        {
            auto profile = model.at(index.row());
            switch (index.column())
            {
            case 0:
                return profile.getProfileName();
            case 1:
                return profile.getVersionString();
            case 2:
                return profile.getLanguageString();
            case 3:
                return profile.getTID();
            case 4:
                return profile.getSID();
            case 5:
                return profile.getDualSlotString();
            case 6:
                return profile.getRadioString();
            case 7:
                return profile.getRadar() ? tr("True") : tr("False");
            case 8:
                return profile.getSwarm() ? tr("True") : tr("False");
            }
        }
        return QVariant();
    }

    QVariant Profile4Model::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        {
            return header.at(section);
        }
        return QVariant();
    }
}
