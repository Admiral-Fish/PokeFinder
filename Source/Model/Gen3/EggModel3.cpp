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

#include "EggModel3.hpp"
#include <Core/Util/Translator.hpp>

EggModel3::EggModel3(QObject *parent, bool emerald) : TableModel(parent), emerald(emerald), showInheritance(false), showStats(false)
{
}

int EggModel3::columnCount(const QModelIndex &parent) const
{
    if (emerald)
    {
        return 16;
    }
    else
    {
        return 15;
    }
}

QVariant EggModel3::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        int column = getColumn(index.column());

        const auto &state = model[index.row()];
        switch (column)
        {
        case 0:
            return state.getAdvances();
        case 1:
            return state.getPickupAdvances();
        case 2:
            return state.getRedraws();
        case 3:
            return QString::number(state.getPID(), 16).toUpper().rightJustified(8, '0');
        case 4:
        {
            u8 shiny = state.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 5:
            return QString::fromStdString(Translator::getNature(state.getNature()));
        case 6:
            return QString("%1 (%2)").arg(state.getAbility()).arg(QString::fromStdString(Translator::getAbility(state.getAbilityIndex())));
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
            if (showInheritance)
            {
                u8 inh = state.getInheritance(column - 7);
                if (inh)
                {
                    return inh == 1 ? "A" : "B";
                }
            }
            return showStats ? state.getStat(column - 7) : state.getIV(column - 7);
        case 13:
            return QString::fromStdString(Translator::getHiddenPower(state.getHiddenPower()));
        case 14:
            return state.getHiddenPowerStrength();
        case 15:
            return QString::fromStdString(Translator::getGender(state.getGender()));
        }
    }

    return QVariant();
}

QVariant EggModel3::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        section = getColumn(section);
        return header[section];
    }
    return QVariant();
}

void EggModel3::setShowInheritance(bool flag)
{
    showInheritance = flag;
    emit dataChanged(index(0, 7), index(rowCount(), 12), { Qt::DisplayRole });
}

void EggModel3::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, 7), index(rowCount(), 12), { Qt::DisplayRole });
}

int EggModel3::getColumn(int column) const
{
    if (emerald)
    {
        return column;
    }
    else
    {
        return column > 1 ? column + 1 : column;
    }
}
