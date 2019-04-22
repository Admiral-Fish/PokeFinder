/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2019 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "SeedtoTime4Model.hpp"
#include <Core/Util/Utilities.hpp>

SeedtoTime4Model::SeedtoTime4Model(QObject *parent, bool flag, Game version) :
    TableModel<DateTime>(parent)
{
    calibrate = flag;
    this->version = version;
}

void SeedtoTime4Model::setFlags(bool flag, Game version)
{
    calibrate = flag;
    this->version = version;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}
int SeedtoTime4Model::columnCount(const QModelIndex &parent) const
{
    (void) parent;
    if (calibrate)
    {
        return version & Game::HGSS ? 6 : 5;
    }
    return 3;
}

QVariant SeedtoTime4Model::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        auto frame = model.at(index.row());
        if (calibrate)
        {
            switch (index.column())
            {
                case 0:
                    return QString::number(frame.getSeed(), 16).toUpper().toUpper().rightJustified(8, '0');
                case 1:
                    return frame.getDate();
                case 2:
                    return frame.getTime();
                case 3:
                    return frame.getDelay();
                case 4:
                    return version & Game::HGSS ? Utilities::getCalls(frame.getSeed(), 15, frame.getInfo()) : Utilities::coinFlips(frame.getSeed(), 15);
                case 5:
                    {
                        QString str = frame.getInfo().getRoutes();
                        return str.isEmpty() ? tr("No roamers") : str;
                    }
            }
        }
        else
        {
            switch (index.column())
            {
                case 0:
                    return frame.getDate();
                case 1:
                    return frame.getTime();
                case 2:
                    return frame.getDelay();
            }
        }
    }
    return QVariant();
}

QVariant SeedtoTime4Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        if (calibrate)
        {
            return version & Game::HGSS ? header1.at(section) : header2.at(section);
        }
        else
        {
            return header3.at(section);
        }
    }
    return QVariant();
}
