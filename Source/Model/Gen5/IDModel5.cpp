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

#include "IDModel5.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Util/Translator.hpp>
#include <Core/Util/Utilities.hpp>

IDModel5::IDModel5(QObject *parent) : TableModel(parent), version(Game::Black)
{
}

int IDModel5::columnCount(const QModelIndex &parent) const
{
    return 9;
}

QVariant IDModel5::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &display = model[index.row()];
        const auto &state = display.getState();
        switch (index.column())
        {
        case 0:
            return QString::number(display.getInitialSeed(), 16).toUpper().rightJustified(16, '0');
        case 1:
            return Utilities5::initialAdvancesID(display.getInitialSeed(), version);
        case 2:
            return state.getAdvances();
        case 3:
            return state.getTID();
        case 4:
            return state.getSID();
        case 5:
            return state.getTSV();
        case 6:
            return QString::fromStdString(display.getDateTime().toString());
        case 7:
            return QString::number(display.getTimer0(), 16).toUpper();
        case 8:
            return QString::fromStdString(Translator::getKeypresses(display.getButtons()));
        }
    }
    return QVariant();
}

QVariant IDModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

void IDModel5::setGame(Game version)
{
    this->version = version;
}
