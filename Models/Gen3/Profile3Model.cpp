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

#include "Profile3Model.hpp"

namespace PokeFinderModels
{

    Profile3Model::Profile3Model(QObject *parent) :
        TableModel<PokeFinderCore::Profile3>(parent)
    {
    }

    int Profile3Model::columnCount(const QModelIndex & /*parent*/) const
    {
        return 6;
    }

    QVariant Profile3Model::data(const QModelIndex &index, int role) const
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
                    if (profile.getVersion() & PokeFinderCore::Game::RS)
                    {
                        return profile.getDeadBattery() ? tr("Yes") : tr("No");
                    }
                    else
                    {
                        return "N/A";
                    }
            }
        }
        return QVariant();
    }

    QVariant Profile3Model::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        {
            return header.at(section);
        }
        return QVariant();
    }

}
