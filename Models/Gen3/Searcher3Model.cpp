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

#include "Searcher3Model.hpp"

Searcher3Model::Searcher3Model(QObject *parent, Method method) : QAbstractTableModel(parent)
{
    this->method = method;
}

void Searcher3Model::setModel(vector<Frame3> frames)
{
    model = frames;
}

void Searcher3Model::addItems(vector<Frame3> frames)
{
    int i = rowCount();
    emit beginInsertRows(QModelIndex(), i, i + frames.size() - 1);
    model.insert(model.end(), frames.begin(), frames.end());
    emit endInsertRows();
}

void Searcher3Model::clear()
{
    if (model.empty())
        return;
    emit beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    model.clear();
    model.shrink_to_fit();
    emit endRemoveRows();
}

void Searcher3Model::setMethod(Method method)
{
    this->method = method;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

void Searcher3Model::sort(int column, Qt::SortOrder order)
{
    if (model.empty())
        return;

    emit layoutAboutToBeChanged();

    if (order == Qt::AscendingOrder)
    {
        switch (method)
        {
            case MethodH1:
            case MethodH2:
            case MethodH4:
                switch (column)
                {
                    case 0:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.seed < frame2.seed;
                        });
                        break;
                    case 1:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.leadType < frame2.leadType;
                        });
                        break;
                    case 2:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.encounterSlot < frame2.encounterSlot;
                        });
                        break;
                    case 3:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.pid < frame2.pid;
                        });
                        break;
                    case 4:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.shiny < frame2.shiny;
                        });
                        break;
                    case 5:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.nature < frame2.nature;
                        });
                        break;
                    case 6:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ability < frame2.ability;
                        });
                        break;
                    case 7:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[0] < frame2.ivs[0];
                        });
                        break;
                    case 8:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[1] < frame2.ivs[1];
                        });
                        break;
                    case 9:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[2] < frame2.ivs[2];
                        });
                        break;
                    case 10:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[3] < frame2.ivs[3];
                        });
                        break;
                    case 11:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[4] < frame2.ivs[4];
                        });
                        break;
                    case 12:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[5] < frame2.ivs[5];
                        });
                        break;
                    case 13:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.hidden < frame2.hidden;
                        });
                        break;
                    case 14:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.power < frame2.power;
                        });
                        break;
                    case 15:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.gender < frame2.gender;
                        });
                        break;
                }
                break;
            case XD:
            case Colo:
                switch (column)
                {
                    case 0:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.seed < frame2.seed;
                        });
                        break;
                    case 1:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.pid < frame2.pid;
                        });
                        break;
                    case 2:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.shiny < frame2.shiny;
                        });
                        break;
                    case 3:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.nature < frame2.nature;
                        });
                        break;
                    case 4:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ability < frame2.ability;
                        });
                        break;
                    case 5:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[0] < frame2.ivs[0];
                        });
                        break;
                    case 6:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[1] < frame2.ivs[1];
                        });
                        break;
                    case 7:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[2] < frame2.ivs[2];
                        });
                        break;
                    case 8:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[3] < frame2.ivs[3];
                        });
                        break;
                    case 9:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[4] < frame2.ivs[4];
                        });
                        break;
                    case 10:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[5] < frame2.ivs[5];
                        });
                        break;
                    case 11:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.hidden < frame2.hidden;
                        });
                        break;
                    case 12:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.power < frame2.power;
                        });
                        break;
                    case 13:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.gender < frame2.gender;
                        });
                        break;
                    case 14:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.lockReason < frame2.lockReason;
                        });
                        break;
                }
                break;
            case Method1:
            case Method1Reverse:
            case Method2:
            case Method4:
            case XDColo:
            case Channel:
                switch (column)
                {
                    case 0:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.seed < frame2.seed;
                        });
                        break;
                    case 1:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.pid < frame2.pid;
                        });
                        break;
                    case 2:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.shiny < frame2.shiny;
                        });
                        break;
                    case 3:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.nature < frame2.nature;
                        });
                        break;
                    case 4:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ability < frame2.ability;
                        });
                        break;
                    case 5:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[0] < frame2.ivs[0];
                        });
                        break;
                    case 6:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[1] < frame2.ivs[1];
                        });
                        break;
                    case 7:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[2] < frame2.ivs[2];
                        });
                        break;
                    case 8:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[3] < frame2.ivs[3];
                        });
                        break;
                    case 9:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[4] < frame2.ivs[4];
                        });
                        break;
                    case 10:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[5] < frame2.ivs[5];
                        });
                        break;
                    case 11:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.hidden < frame2.hidden;
                        });
                        break;
                    case 12:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.power < frame2.power;
                        });
                        break;
                    case 13:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.gender < frame2.gender;
                        });
                        break;
                }
                break;
        }
    }
    else
    {
        switch (method)
        {
            case MethodH1:
            case MethodH2:
            case MethodH4:
                switch (column)
                {
                    case 0:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.seed > frame2.seed;
                        });
                        break;
                    case 1:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.leadType > frame2.leadType;
                        });
                        break;
                    case 2:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.encounterSlot > frame2.encounterSlot;
                        });
                        break;
                    case 3:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.pid > frame2.pid;
                        });
                        break;
                    case 4:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.shiny > frame2.shiny;
                        });
                        break;
                    case 5:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.nature > frame2.nature;
                        });
                        break;
                    case 6:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ability > frame2.ability;
                        });
                        break;
                    case 7:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[0] > frame2.ivs[0];
                        });
                        break;
                    case 8:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[1] > frame2.ivs[1];
                        });
                        break;
                    case 9:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[2] > frame2.ivs[2];
                        });
                        break;
                    case 10:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[3] > frame2.ivs[3];
                        });
                        break;
                    case 11:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[4] > frame2.ivs[4];
                        });
                        break;
                    case 12:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[5] > frame2.ivs[5];
                        });
                        break;
                    case 13:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.hidden > frame2.hidden;
                        });
                        break;
                    case 14:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.power > frame2.power;
                        });
                        break;
                    case 15:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.gender > frame2.gender;
                        });
                        break;
                }
                break;
            case XD:
            case Colo:
                switch (column)
                {
                    case 0:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.seed > frame2.seed;
                        });
                        break;
                    case 1:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.pid > frame2.pid;
                        });
                        break;
                    case 2:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.shiny > frame2.shiny;
                        });
                        break;
                    case 3:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.nature > frame2.nature;
                        });
                        break;
                    case 4:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ability > frame2.ability;
                        });
                        break;
                    case 5:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[0] > frame2.ivs[0];
                        });
                        break;
                    case 6:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[1] > frame2.ivs[1];
                        });
                        break;
                    case 7:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[2] > frame2.ivs[2];
                        });
                        break;
                    case 8:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[3] > frame2.ivs[3];
                        });
                        break;
                    case 9:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[4] > frame2.ivs[4];
                        });
                        break;
                    case 10:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[5] > frame2.ivs[5];
                        });
                        break;
                    case 11:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.hidden > frame2.hidden;
                        });
                        break;
                    case 12:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.power > frame2.power;
                        });
                        break;
                    case 13:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.gender > frame2.gender;
                        });
                        break;
                    case 14:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.lockReason > frame2.lockReason;
                        });
                        break;
                }
                break;
            case Method1:
            case Method1Reverse:
            case Method2:
            case Method4:
            case XDColo:
            case Channel:
                switch (column)
                {
                    case 0:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.seed > frame2.seed;
                        });
                        break;
                    case 1:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.pid > frame2.pid;
                        });
                        break;
                    case 2:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.shiny > frame2.shiny;
                        });
                        break;
                    case 3:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.nature > frame2.nature;
                        });
                        break;
                    case 4:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ability > frame2.ability;
                        });
                        break;
                    case 5:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[0] > frame2.ivs[0];
                        });
                        break;
                    case 6:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[1] > frame2.ivs[1];
                        });
                        break;
                    case 7:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[2] > frame2.ivs[2];
                        });
                        break;
                    case 8:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[3] > frame2.ivs[3];
                        });
                        break;
                    case 9:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[4] > frame2.ivs[4];
                        });
                        break;
                    case 10:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.ivs[5] > frame2.ivs[5];
                        });
                        break;
                    case 11:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.hidden > frame2.hidden;
                        });
                        break;
                    case 12:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.power > frame2.power;
                        });
                        break;
                    case 13:
                        std::sort(model.begin(), model.end(), [] (const Frame3 & frame1, const Frame3 & frame2)
                        {
                            return frame1.gender > frame2.gender;
                        });
                        break;
                }
                break;
        }
    }

    emit layoutChanged();
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
            return 16;
        case XD:
        case Colo:
            return 15;
        case Method1:
        case Method1Reverse:
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
                        {
                            Lead type = frame.leadType;
                            return type == None ? tr("None") : type == Synchronize ? tr("Synch") : tr("Cute Charm");
                        }
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
                }
            case Method1:
            case Method1Reverse:
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
                        return frame.getShiny();
                    case 3:
                        return frame.getNature();
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
                        return frame.getPower();
                    case 12:
                        return frame.power;
                    case 13:
                        return frame.getGender();
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
                        return frame.getShiny();
                    case 3:
                        return frame.getNature();
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
                        return frame.getPower();
                    case 12:
                        return frame.power;
                    case 13:
                        return frame.getGender();
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
                            return tr("Lead");
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
                    }
                case Method1:
                case Method1Reverse:
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
