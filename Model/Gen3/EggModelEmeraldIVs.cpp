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

#include "EggModelEmeraldIVs.hpp"

#include <Core/Util/Translator.hpp>

EggModelEmeraldIVs::EggModelEmeraldIVs(QObject *parent)
    : TableModel(parent)
    , showInheritance(false)
    , showStats(false)
{
}

int EggModelEmeraldIVs::columnCount(const QModelIndex &parent) const
{
    return static_cast<int>(Column::Count);
}

QVariant EggModelEmeraldIVs::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &state = model[index.row()];
        int column = index.column();
        switch (static_cast<Column>(column))
        {
        case Column::Advances:
            return state.getPickupAdvances();
        case Column::HP:
        case Column::Attack:
        case Column::Defense:
        case Column::SpAttack:
        case Column::SpDefense:
        case Column::Speed:
            if (showInheritance)
            {
                u8 inh = state.getInheritance(column - static_cast<int>(Column::HP));
                if (inh)
                {
                    return inh == 1 ? "A" : "B";
                }
            }
            return showStats
                ? state.getStat(column - static_cast<int>(Column::HP))
                : state.getIV(column - static_cast<int>(Column::HP));
        case Column::HiddenPowerType:
            return QString::fromStdString(Translator::getHiddenPower(state.getHiddenPower()));
        case Column::HiddenPowerStrength:
            return state.getHiddenPowerStrength();
        default:
            break;
        }
    }

    return {};
}
QVariant EggModelEmeraldIVs::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return getColumnName(section);
    }
    return {};
}

QString EggModelEmeraldIVs::getColumnName(int columnIndex)
{
    switch (static_cast<Column>(columnIndex))
    {
    case Column::Advances:
        return tr("Pickup Advances");
    case Column::HP:
        return tr("HP");
    case Column::Attack:
        return tr("Atk");
    case Column::Defense:
        return tr("Def");
    case Column::SpAttack:
        return tr("SpA");
    case Column::SpDefense:
        return tr("SpD");
    case Column::Speed:
        return tr("Spe");
    case Column::HiddenPowerType:
        return tr("Hidden");
    case Column::HiddenPowerStrength:
        return tr("Power");
    default:
        return {};
    }
}

void EggModelEmeraldIVs::setShowInheritance(bool flag)
{
    showInheritance = flag;
    emit dataChanged(index(0, static_cast<int>(Column::HP)), index(rowCount() - 1, static_cast<int>(Column::Speed)), { Qt::DisplayRole });
}

void EggModelEmeraldIVs::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, static_cast<int>(Column::HP)), index(rowCount() - 1, static_cast<int>(Column::Speed)), { Qt::DisplayRole });

}