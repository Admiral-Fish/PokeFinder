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

#include "SeedTimeModel4.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Gen4/HGSSRoamer.hpp>
#include <Core/Util/Utilities.hpp>

SeedTimeModel4::SeedTimeModel4(QObject *parent) : TableModel<SeedTime4>(parent)
{
}

int SeedTimeModel4::columnCount(const QModelIndex & /*parent*/) const
{
    return 2;
}

QVariant SeedTimeModel4::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &state = model[index.row()];
        switch (index.column())
        {
        case 0:
            return QString::fromStdString(state.getDateTime().toString());
        case 1:
            return state.getDelay();
        }
    }
    return QVariant();
}

QVariant SeedTimeModel4::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

SeedTimeCalibrateModel4::SeedTimeCalibrateModel4(QObject *parent, Game version) : TableModel<SeedTimeCalibrate4>(parent), version(version)
{
}

int SeedTimeCalibrateModel4::columnCount(const QModelIndex &parent) const
{
    return (version & Game::HGSS) != Game::None ? 5 : 4;
}

QVariant SeedTimeCalibrateModel4::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &state = model[index.row()];
        int column = getColumn(index.column());
        switch (column)
        {
        case 0:
            return QString::number(state.getSeed(), 16).toUpper().toUpper().rightJustified(8, '0');
        case 1:
            return QString::fromStdString(state.getDateTime().toString());
        case 2:
            return state.getDelay();
        case 3:
        case 4:
            return QString::fromStdString(state.getSequence());
        case 5:
        {
            std::string str = state.getRoamer()->getRouteString();
            return str.empty() ? tr("No roamers") : QString::fromStdString(str);
        }
        }
    }
    return QVariant();
}

QVariant SeedTimeCalibrateModel4::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

int SeedTimeCalibrateModel4::getColumn(int column) const
{
    switch (version)
    {
    case Game::HGSS:
        return column > 2 ? column + 1 : column;
    case Game::DPPt:
    default:
        return column;
    }
}
