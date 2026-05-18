/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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

EggModel3::EggModel3(QObject *parent, bool emerald) :
    TableModel(parent), emerald(emerald), showInheritance(false), showStats(false), showSeeds(false)
{
}

int EggModel3::columnCount(const QModelIndex &parent) const
{
    if (showSeeds)
    {
        return 17;
    }
    else if (emerald)
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
        const auto &state = model[index.row()];

        // 搜索模式下的种子列
        if (showSeeds)
        {
            switch (index.column())
            {
            case 0:
                return QString::number(state.getSeedHeld(), 16).toUpper().rightJustified(4, '0');
            case 1:
                return QString::number(state.getSeedPickup(), 16).toUpper().rightJustified(4, '0');
            default:
                break;
            }
        }

        int column = getColumn(index.column());

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
            return QString("%1: %2").arg(state.getAbility()).arg(QString::fromStdString(Translator::getAbility(state.getAbilityIndex())));
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
        if (showSeeds)
        {
            switch (section)
            {
            case 0:
                return tr("Held Seed");
            case 1:
                return tr("Pickup Seed");
            default:
                break;
            }
        }
        section = getColumn(section);
        return header[section];
    }
    return QVariant();
}

void EggModel3::setShowInheritance(bool flag)
{
    showInheritance = flag;
    emit dataChanged(index(0, 7), index(rowCount() - 1, 12), { Qt::DisplayRole });
}

void EggModel3::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, 7), index(rowCount() - 1, 12), { Qt::DisplayRole });
}

void EggModel3::setShowSeeds(bool flag)
{
    showSeeds = flag;
    emit layoutChanged();
}

int EggModel3::getColumn(int column) const
{
    // Search mode: first two columns are seedHeld/seedPickup, rest shift by 2
    if (showSeeds)
    {
        column -= 2;
    }

    if (emerald)
    {
        return column;
    }
    else
    {
        // Non-Emerald: skip header[2] (Redraws)
        return column > 1 ? column + 1 : column;
    }
}
