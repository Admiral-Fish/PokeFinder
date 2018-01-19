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

#include "Searcher3Model.hpp"

Searcher3Model::Searcher3Model(QObject *parent, Method method) : QAbstractTableModel(parent)
{
    this->method = method;
}

void Searcher3Model::SetModel(vector<Frame3> frames)
{
    model = frames;
}

void Searcher3Model::AddItems(vector<Frame3> frames)
{
    int i = rowCount();
    emit beginInsertRows(QModelIndex(), i, i + frames.size() - 1);
    model.insert(model.end(), frames.begin(), frames.end());
    emit endInsertRows();
}

int Searcher3Model::rowCount(const QModelIndex &parent) const
{
    (void) parent;
    return (int)model.size();
}

int Searcher3Model::columnCount(const QModelIndex &parent) const
{
    (void) parent;
    switch (method)
    {
        case MethodH1:
        case MethodH2:
        case MethodH4:
        case XD:
        case Colo:
            return 15;
        case Method1:
        case Method2:
        case Method4:
        case XDColo:
        case Channel:
        default:
            return 14;
    }
}

QVariant Searcher3Model::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        int row = index.row();
        int column = index.column();
        Frame3 frame = model[row];
        switch (method)
        {
            case MethodH1:
            case MethodH2:
            case MethodH4:
                switch (column)
                {
                    case 0:
                        return QString::number(frame.seed, 16).toUpper().rightJustified(8, '0');
                    case 1:
                        return frame.encounterSlot;
                    case 2:
                        return QString::number(frame.pid, 16).toUpper().rightJustified(8, '0');
                    case 3:
                        return frame.GetShiny();
                    case 4:
                        return frame.GetNature();
                    case 5:
                        return frame.ability;
                    case 6:
                        return frame.ivs[0];
                    case 7:
                        return frame.ivs[1];
                    case 8:
                        return frame.ivs[2];
                    case 9:
                        return frame.ivs[3];
                    case 10:
                        return frame.ivs[4];
                    case 11:
                        return frame.ivs[5];
                    case 12:
                        return frame.GetPower();
                    case 13:
                        return frame.power;
                    case 14:
                        return frame.GetFemale25();
                }
            case Method1:
            case Method2:
            case Method4:
            case XDColo:
            case Channel:
                switch (column)
                {
                    case 0:
                        return QString::number(frame.seed, 16).toUpper().rightJustified(8, '0');
                    case 1:
                        return QString::number(frame.pid, 16).toUpper().rightJustified(8, '0');
                    case 2:
                        return frame.GetShiny();
                    case 3:
                        return frame.GetNature();
                    case 4:
                        return frame.ability;
                    case 5:
                        return frame.ivs[0];
                    case 6:
                        return frame.ivs[1];
                    case 7:
                        return frame.ivs[2];
                    case 8:
                        return frame.ivs[3];
                    case 9:
                        return frame.ivs[4];
                    case 10:
                        return frame.ivs[5];
                    case 11:
                        return frame.GetPower();
                    case 12:
                        return frame.power;
                    case 13:
                        return frame.GetFemale25();
                }
            case XD:
            case Colo:
                switch (column)
                {
                    case 0:
                        return QString::number(frame.seed, 16).toUpper().rightJustified(8, '0');
                    case 1:
                        return QString::number(frame.pid, 16).toUpper().rightJustified(8, '0');
                    case 2:
                        return frame.GetShiny();
                    case 3:
                        return frame.GetNature();
                    case 4:
                        return frame.ability;
                    case 5:
                        return frame.ivs[0];
                    case 6:
                        return frame.ivs[1];
                    case 7:
                        return frame.ivs[2];
                    case 8:
                        return frame.ivs[3];
                    case 9:
                        return frame.ivs[4];
                    case 10:
                        return frame.ivs[5];
                    case 11:
                        return frame.GetPower();
                    case 12:
                        return frame.power;
                    case 13:
                        return frame.GetFemale25();
                    case 14:
                        return frame.lockReason;
                }
        }
    }
    return QVariant();
}

QVariant Searcher3Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (method)
            {
                case MethodH1:
                case MethodH2:
                case MethodH4:
                    switch (section)
                    {
                        case 0:
                            return tr("Seed");
                        case 1:
                            return tr("Slot");
                        case 2:
                            return tr("PID");
                        case 3:
                            return "!!!";
                        case 4:
                            return tr("Nature");
                        case 5:
                            return tr("Ability");
                        case 6:
                            return tr("HP");
                        case 7:
                            return tr("Atk");
                        case 8:
                            return tr("Def");
                        case 9:
                            return tr("SpA");
                        case 10:
                            return tr("SpD");
                        case 11:
                            return tr("Spe");
                        case 12:
                            return tr("Hidden");
                        case 13:
                            return tr("Power");
                        case 14:
                            return tr("Gender");
                    }
                case Method1:
                case Method2:
                case Method4:
                case XDColo:
                case Channel:
                    switch (section)
                    {
                        case 0:
                            return tr("Seed");
                        case 1:
                            return tr("PID");
                        case 2:
                            return "!!!";
                        case 3:
                            return tr("Nature");
                        case 4:
                            return tr("Ability");
                        case 5:
                            return tr("HP");
                        case 6:
                            return tr("Atk");
                        case 7:
                            return tr("Def");
                        case 8:
                            return tr("SpA");
                        case 9:
                            return tr("SpD");
                        case 10:
                            return tr("Spe");
                        case 11:
                            return tr("Hidden");
                        case 12:
                            return tr("Power");
                        case 13:
                            return tr("Gender");
                    }
                case XD:
                case Colo:
                    switch (section)
                    {
                        case 0:
                            return tr("Seed");
                        case 1:
                            return tr("PID");
                        case 2:
                            return "!!!";
                        case 3:
                            return tr("Nature");
                        case 4:
                            return tr("Ability");
                        case 5:
                            return tr("HP");
                        case 6:
                            return tr("Atk");
                        case 7:
                            return tr("Def");
                        case 8:
                            return tr("SpA");
                        case 9:
                            return tr("SpD");
                        case 10:
                            return tr("Spe");
                        case 11:
                            return tr("Hidden");
                        case 12:
                            return tr("Power");
                        case 13:
                            return tr("Gender");
                        case 14:
                            return tr("Reason");
                    }
            }
        }
    }
    return QVariant();
}
