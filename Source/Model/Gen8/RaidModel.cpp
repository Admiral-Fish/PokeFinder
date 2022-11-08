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

RaidModel::RaidModel(QObject *parent) : TableModel<GeneratorState8>(parent), showStats(false)
{
}

int RaidModel::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return 12;
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
            return QString::number(state.getPID(), 16).toUpper();
        case 2:
        {
            u8 shiny = state.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 3:
            return QString::fromStdString(*Translator::getNature(state.getNature()));
        case 4:
            if (state.getAbility() == 0 || state.getAbility() == 1)
            {
                return QString("%1 (%2)")
                    .arg(state.getAbility())
                    .arg(QString::fromStdString(*Translator::getAbility(state.getAbilityIndex())));
            }
            else
            {
                return QString("H (%2)").arg(QString::fromStdString(*Translator::getAbility(state.getAbilityIndex())));
            }
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            return showStats ? state.getStat(column - 5) : state.getIV(column - 5);
        case 11:
            return QString::fromStdString(*Translator::getGender(state.getGender()));
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

void RaidModel::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, 5), index(rowCount(), 10), { Qt::DisplayRole });
}
