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

#include "EggModelEmeraldPID.hpp"

#include <Core/Util/Translator.hpp>

int EggModelEmeraldPID::columnCount(const QModelIndex &parent) const
{
    return static_cast<int>(Column::Count);
}

QVariant EggModelEmeraldPID::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &state = model[index.row()];
        switch (static_cast<Column>(index.column()))
        {
        case Column::Advances:
            return state.getAdvances();
        case Column::Redraws:
            return state.getRedraws();
        case Column::PID:
            return QString::number(state.getPID(), 16).toUpper().rightJustified(8, '0');
        case Column::Shiny:
        {
            u8 shiny = state.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case Column::Nature:
            return QString::fromStdString(Translator::getNature(state.getNature()));
        case Column::Ability:
            return QString("%1: %2").arg(state.getAbility()).arg(QString::fromStdString(Translator::getAbility(state.getAbilityIndex())));
        case Column::Gender:
            return QString::fromStdString(Translator::getGender(state.getGender()));
        default:
            break;
        }
    }

    return {};
}

QVariant EggModelEmeraldPID::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return getColumnName(section);
    }
    return {};
}

QString EggModelEmeraldPID::getColumnName(int columnIndex)
{
    switch (static_cast<Column>(columnIndex))
    {
    case Column::Advances:
        return tr("Held Advances");
    case Column::Redraws:
        return tr("Redraws");
    case Column::PID:
        return tr("PID");
    case Column::Shiny:
        return tr("Shiny");
    case Column::Nature:
        return tr("Nature");
    case Column::Ability:
        return tr("Ability");
    case Column::Gender:
        return tr("Gender");
    default:
        return {};
    }
}