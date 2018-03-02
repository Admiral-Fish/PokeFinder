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

#include "Egg3Model.hpp"

Egg3Model::Egg3Model(QObject *parent, Method method) : QAbstractTableModel(parent)
{
    this->method = method;
}

void Egg3Model::setModel(vector<Frame3> frames)
{
    model = frames;
}

void Egg3Model::addItems(vector<Frame3> frames)
{
    int i = rowCount();
    emit beginInsertRows(QModelIndex(), i, i + frames.size() - 1);
    model.insert(model.end(), frames.begin(), frames.end());
    emit endInsertRows();
}

int Egg3Model::rowCount(const QModelIndex &parent) const
{
    (void) parent;
    return (int)model.size();
}

int Egg3Model::columnCount(const QModelIndex &parent) const
{
    (void) parent;
    switch (method)
    {
        case EBred:
        case EBredAlternate:
        case EBredSplit:
            return 10;
        case EBredPID:
            return 8;
        case RSBred:
        case FRLGBred:
        default:
            return 18;
    }
}

QVariant Egg3Model::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        int row = index.row();
        int column = index.column();
        Frame3 frame = model[row];
        switch (method)
        {
            case EBred:
            case EBredAlternate:
            case EBredSplit:
                switch (column)
                {
                    case 0:
                        return frame.frame;
                    case 1:
                        return frame.getTime();
                    case 2:
                        return frame.ivs[0];
                    case 3:
                        return frame.ivs[1];
                    case 4:
                        return frame.ivs[2];
                    case 5:
                        return frame.ivs[3];
                    case 6:
                        return frame.ivs[4];
                    case 7:
                        return frame.ivs[5];
                    case 8:
                        return frame.getPower();
                    case 9:
                        return frame.power;
                }
            case EBredPID:
                switch (column)
                {
                    case 0:
                        return frame.frame;
                    case 1:
                        return frame.getTime();
                    case 2:
                        return frame.occidentary;
                    case 3:
                        return QString::number(frame.pid, 16).toUpper().rightJustified(8, '0');
                    case 4:
                        return frame.getShiny();
                    case 5:
                        return frame.getNature();
                    case 6:
                        return frame.ability;
                    case 7:
                        return frame.getGender();
                }
        }
    }

    return QVariant();
}

QVariant Egg3Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (method)
            {
                case EBred:
                case EBredSplit:
                case EBredAlternate:
                    switch (section)
                    {
                        case 0:
                            return tr("Frame");
                        case 1:
                            return tr("Time");
                        case 2:
                            return tr("HP");
                        case 3:
                            return tr("Atk");
                        case 4:
                            return tr("Def");
                        case 5:
                            return tr("SpA");
                        case 6:
                            return tr("SpD");
                        case 7:
                            return tr("Spe");
                        case 8:
                            return tr("Hidden");
                        case 9:
                            return tr("Power");
                    }
                case EBredPID:
                    switch (section)
                    {
                        case 0:
                            return tr("Frame");
                        case 1:
                            return tr("Time");
                        case 2:
                            return tr("Redraws");
                        case 3:
                            return tr("PID");
                        case 4:
                            return "!!!";
                        case 5:
                            return tr("Nature");
                        case 6:
                            return tr("Ability");
                        case 7:
                            return tr("Gender");
                    }
            }
        }
    }

    return QVariant();
}
