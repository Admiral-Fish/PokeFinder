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

#include "StaticModel8.hpp"
#include <Core/Util/Translator.hpp>

StaticModel8::StaticModel8(QObject *parent) : TableModel(parent), showStats(false)
{
}

int StaticModel8::columnCount(const QModelIndex &parent) const
{
    return 14;
}

QVariant StaticModel8::data(const QModelIndex &index, int role) const
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
            return QString::number(state.getEC(), 16).toUpper().rightJustified(8, '0');
        case 2:
            return QString::number(state.getPID(), 16).toUpper().rightJustified(8, '0');
        case 3:
        {
            u8 shiny = state.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 4:
            return QString::fromStdString(Translator::getNature(state.getNature()));
        case 5:
            if (state.getAbility() == 0 || state.getAbility() == 1)
            {
                return QString("%1: %2")
                    .arg(state.getAbility())
                    .arg(QString::fromStdString(Translator::getAbility(state.getAbilityIndex())));
            }
            else
            {
                return QString("H (%2)").arg(QString::fromStdString(Translator::getAbility(state.getAbilityIndex())));
            }
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            return showStats ? state.getStat(column - 6) : state.getIV(column - 6);
        case 12:
            return QString::fromStdString(Translator::getGender(state.getGender()));
        case 13:
            return QString::fromStdString(Translator::getCharacteristic(state.getCharacteristic()));
        }
    }

    return QVariant();
}

QVariant StaticModel8::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

void StaticModel8::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, 6), index(rowCount(), 11), { Qt::DisplayRole });
}
