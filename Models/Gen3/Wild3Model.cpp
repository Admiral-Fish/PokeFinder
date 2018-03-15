/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "Wild3Model.hpp"

Wild3Model::Wild3Model(QObject *parent) : QAbstractTableModel(parent)
{
}

void Wild3Model::setModel(vector<Frame3> frames)
{
    if (frames.empty())
        return;
    int i = rowCount();
    emit beginInsertRows(QModelIndex(), i, i + frames.size() - 1);
    model.insert(model.end(), frames.begin(), frames.end());
    emit endInsertRows();
}

void Wild3Model::clear()
{
    if (model.empty())
        return;
    emit beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    model.clear();
    model.shrink_to_fit();
    emit endRemoveRows();
}

int Wild3Model::rowCount(const QModelIndex &parent) const
{
    (void) parent;
    return (int)model.size();
}

int Wild3Model::columnCount(const QModelIndex &parent) const
{
    (void) parent;
    return 17;
}

QVariant Wild3Model::data(const QModelIndex &index, int role) const
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
                return frame.occidentary;
            case 2:
                return frame.encounterSlot;
            case 3:
                return QString::number(frame.pid, 16).toUpper().rightJustified(8, '0');
            case 4:
                return frame.getShiny();
            case 5:
                return frame.getNature();
            case 6:
                return frame.ability;
            case 7:
                return frame.ivs[0];
            case 8:
                return frame.ivs[1];
            case 9:
                return frame.ivs[2];
            case 10:
                return frame.ivs[3];
            case 11:
                return frame.ivs[4];
            case 12:
                return frame.ivs[5];
            case 13:
                return frame.getPower();
            case 14:
                return frame.power;
            case 15:
                return frame.getGender();
            case 16:
                return frame.getTime();
        }
    }
    return QVariant();
}

QVariant Wild3Model::headerData(int section, Qt::Orientation orientation, int role) const
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
                    return tr("Occidentary");
                case 2:
                    return tr("Slot");
                case 3:
                    return tr("PID");
                case 4:
                    return "!!!";
                case 5:
                    return tr("Nature");
                case 6:
                    return tr("Ability");
                case 7:
                    return tr("HP");
                case 8:
                    return tr("Atk");
                case 9:
                    return tr("Def");
                case 10:
                    return tr("SpA");
                case 11:
                    return tr("SpD");
                case 12:
                    return tr("Spe");
                case 13:
                    return tr("Hidden");
                case 14:
                    return tr("Power");
                case 15:
                    return tr("Gender");
                case 16:
                    return tr("Time");
            }
        }
    }
    return QVariant();
}
