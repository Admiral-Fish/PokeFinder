/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish and bumba
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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

#include "PokeSpotModel.hpp"

PokeSpotModel::PokeSpotModel(QObject *parent) : QAbstractTableModel(parent)
{
}

void PokeSpotModel::setModel(vector<Frame3> frames)
{
    model = frames;
}

int PokeSpotModel::rowCount(const QModelIndex &parent) const
{
    (void) parent;
    return (int)model.size();
}

int PokeSpotModel::columnCount(const QModelIndex &parent) const
{
    (void) parent;
    return 7;
}

QVariant PokeSpotModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        int row = index.row();
        int column = index.column();
        Frame3 frame = model[row];
        switch (column)
        {
            case 0:
                return frame.frame;
            case 1:
                return QString::number(frame.pid, 16).toUpper().rightJustified(8, '0');
            case 2:
                return frame.getShiny();
            case 3:
                return frame.lockReason;
            case 4:
                return frame.getNature();
            case 5:
                return frame.ability;
            case 6:
                return frame.getGender();
        }
    }
    return QVariant();
}

QVariant PokeSpotModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
                case 0:
                    return tr("Frame");
                case 1:
                    return tr("PID");
                case 2:
                    return "!!!";
                case 3:
                    return tr("Type");
                case 4:
                    return tr("Nature");
                case 5:
                    return tr("Ability");
                case 6:
                    return tr("Gender");
            }
        }
    }
    return QVariant();
}
