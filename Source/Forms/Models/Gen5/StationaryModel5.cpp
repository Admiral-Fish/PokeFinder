/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "StationaryModel5.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Util/Translator.hpp>
#include <Core/Util/Utilities.hpp>

StationaryGeneratorModel5::StationaryGeneratorModel5(QObject *parent, Method method) : TableModel<StationaryState>(parent), method(method)
{
}

void StationaryGeneratorModel5::setMethod(Method method)
{
    this->method = method;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

int StationaryGeneratorModel5::columnCount(const QModelIndex &parent) const
{
    switch (method)
    {
    case Method::Method5IVs:
    case Method::Method5CGear:
        return 9;
    case Method::Method5:
        return 7;
    default:
        return 0;
    }
}

QVariant StationaryGeneratorModel5::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &state = model[index.row()];
        int column = getColumn(index.column());
        switch (column)
        {
        case 0:
            return state.getAdvances();
        case 1:
        {
            return QString::fromStdString(Utilities::getChatot(state.getSeed()));
        }
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
            return state.getAbility();
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            return state.getIV(static_cast<u8>(column - 6));
        case 12:
            return QString::fromStdString(Translator::getHiddenPower(state.getHidden()));
        case 13:
            return state.getPower();
        case 14:
            return QString::fromStdString(Translator::getGender(state.getGender()));
        }
    }
    return QVariant();
}

QVariant StationaryGeneratorModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        section = getColumn(section);

        return header[section];
    }
    return QVariant();
}

int StationaryGeneratorModel5::getColumn(int column) const
{
    switch (method)
    {
    case Method::Method5IVs:
    case Method::Method5CGear:
        return column > 1 ? column + 4 : column;
    case Method::Method5:
        return column > 5 ? column + 8 : column;
    default:
        return column;
    }
}

StationarySearcherModel5::StationarySearcherModel5(QObject *parent, Method method) :
    TableModel<SearcherState5<StationaryState>>(parent), method(method)
{
}

void StationarySearcherModel5::setMethod(Method method)
{
    this->method = method;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

void StationarySearcherModel5::sort(int column, Qt::SortOrder order)
{
    // TODO
}

int StationarySearcherModel5::columnCount(const QModelIndex &parent) const
{
    return 0;
}

QVariant StationarySearcherModel5::data(const QModelIndex &index, int role) const
{
    return QVariant();
}

QVariant StationarySearcherModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}
