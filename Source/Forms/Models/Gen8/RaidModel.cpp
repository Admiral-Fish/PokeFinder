/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "RaidModel.hpp"
#include <Core/Util/Translator.hpp>

RaidModel::RaidModel(QObject *parent) : TableModel<State>(parent)
{
}

int RaidModel::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return 14;
}

QVariant RaidModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &state = model[index.row()];
        int column = index.column();
        switch (column)
        {
        case 0:
            return state.getAdvances();
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            return state.getIV(static_cast<u8>(column - 1));
        case 7:
        {
            u8 shiny = state.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 8:
            return QString::fromStdString(*Translator::getNature(state.getNature()));
        case 9:
        {
            u8 ability = state.getAbility();
            if (ability == 0 || ability == 1)
            {
                return ability;
            }
            return "H";
        }
        case 10:
        {
            u8 gender = state.getGender();
            return gender == 0 ? "♂" : gender == 1 ? "♀" : "-";
        }
        case 11:
            //return QString::fromStdString(Translator::getCharacteristic(state.getCharacteristic()));
            return "-";
        case 12:
            //return QString::number(state.getEC(), 16).toUpper();
            return "-";
        case 13:
            return QString::number(state.getPID(), 16).toUpper();
        }
    }
    return QVariant();
}

QVariant RaidModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}
