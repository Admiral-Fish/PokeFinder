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

#include "Wild3Model.hpp"
#include <Core/Gen3/Frame3.hpp>
#include <Core/Util/TableUtility.hpp>

namespace PokeFinderModels
{
    Wild3Model::Wild3Model(QObject *parent) :
        TableModel<PokeFinderCore::Frame3>(parent)
    {
    }

    int Wild3Model::columnCount(const QModelIndex & /*parent*/) const
    {
        return 17;
    }

    QVariant Wild3Model::data(const QModelIndex &index, int role) const
    {
        if (role == Qt::DisplayRole)
        {
            auto frame = model.at(index.row());
            switch (index.column())
            {
                case 0:
                    return frame.getFrame();
                case 1:
                    return frame.getEncounterSlot();
                case 2:
                    return frame.getLevel();
                case 3:
                    return QString::number(frame.getPID(), 16).toUpper().rightJustified(8, '0');
                case 4:
                    return frame.getShinyString();
                case 5:
                    return frame.getNatureString();
                case 6:
                    return frame.getAbility();
                case 7:
                    return frame.getIV(0);
                case 8:
                    return frame.getIV(1);
                case 9:
                    return frame.getIV(2);
                case 10:
                    return frame.getIV(3);
                case 11:
                    return frame.getIV(4);
                case 12:
                    return frame.getIV(5);
                case 13:
                    return frame.getPowerString();
                case 14:
                    return frame.getPower();
                case 15:
                    return frame.getGenderString();
                case 16:
                    return frame.getTime();
            }
        }
        else if (role == Qt::FontRole)
        {
            auto frame = model.at(index.row());
            switch (index.column())
            {
                case 8:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(0));
                case 9:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(1));
                case 10:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(2));
                case 11:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(3));
                case 12:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(4));
                case 13:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(5));
            }
        }
        return QVariant();
    }

    QVariant Wild3Model::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        {
            return header.at(section);
        }
        return QVariant();
    }
}
