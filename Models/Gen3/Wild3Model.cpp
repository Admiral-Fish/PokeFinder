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

void Wild3Model::setModel(QVector<Frame3> frames)
{
    if (frames.empty())
        return;
    int i = rowCount();
    emit beginInsertRows(QModelIndex(), i, i + frames.size() - 1);
    model.append(frames);
    emit endInsertRows();
}

void Wild3Model::clear()
{
    if (model.empty())
        return;
    emit beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    model.clear();
    model.squeeze();
    emit endRemoveRows();
}

int Wild3Model::rowCount(const QModelIndex &parent) const
{
    (void) parent;
    return model.size();
}

int Wild3Model::columnCount(const QModelIndex &parent) const
{
    (void) parent;
    return 18;
}

QVariant Wild3Model::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        Frame3 frame = model[index.row()];
        switch (index.column())
        {
            case 0:
                return frame.getFrame();
            case 1:
                return frame.getOccidentary();
            case 2:
                return frame.getEncounterSlot();
            case 3:
                return frame.getLevel();
            case 4:
                return QString::number(frame.getPid(), 16).toUpper().rightJustified(8, '0');
            case 5:
                return frame.getShinyString();
            case 6:
                return frame.getNatureString();
            case 7:
                return frame.getAbility();
            case 8:
                return frame.getIV(0);
            case 9:
                return frame.getIV(1);
            case 10:
                return frame.getIV(2);
            case 11:
                return frame.getIV(3);
            case 12:
                return frame.getIV(4);
            case 13:
                return frame.getIV(5);
            case 14:
                return frame.getPowerString();
            case 15:
                return frame.getPower();
            case 16:
                return frame.getGenderString();
            case 17:
                return frame.getTime();
        }
    }
    return QVariant();
}

QVariant Wild3Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
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
                return tr("Level");
            case 4:
                return tr("PID");
            case 5:
                return "!!!";
            case 6:
                return tr("Nature");
            case 7:
                return tr("Ability");
            case 8:
                return tr("HP");
            case 9:
                return tr("Atk");
            case 10:
                return tr("Def");
            case 11:
                return tr("SpA");
            case 12:
                return tr("SpD");
            case 13:
                return tr("Spe");
            case 14:
                return tr("Hidden");
            case 15:
                return tr("Power");
            case 16:
                return tr("Gender");
            case 17:
                return tr("Time");
        }
    }
    return QVariant();
}
