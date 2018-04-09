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

#include "Searcher4Model.hpp"

Searcher4Model::Searcher4Model(QObject *parent, Method method) : QAbstractTableModel(parent)
{
    this->method = method;
}

void Searcher4Model::setModel(vector<Frame4> frames)
{
    model = frames;
}

void Searcher4Model::addItems(vector<Frame4> frames)
{
    int i = rowCount();
    emit beginInsertRows(QModelIndex(), i, i + frames.size() - 1);
    model.insert(model.end(), frames.begin(), frames.end());
    emit endInsertRows();
}

void Searcher4Model::clear()
{
    if (model.empty())
        return;
    emit beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    model.clear();
    model.shrink_to_fit();
    emit endRemoveRows();
}

void Searcher4Model::setMethod(Method method)
{
    this->method = method;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

void Searcher4Model::sort(int column, Qt::SortOrder order)
{
    if (model.empty())
        return;

    emit layoutAboutToBeChanged();

    if (order == Qt::AscendingOrder)
    {
        switch (method)
        {
            case Method1:
            case ChainedShiny:
                switch (column)
                {
                    case 0:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.seed < frame2.seed;
                        });
                        break;
                    case 1:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.frame < frame2.frame;
                        });
                        break;
                    case 2:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.pid < frame2.pid;
                        });
                        break;
                    case 3:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.shiny < frame2.shiny;
                        });
                        break;
                    case 4:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.nature < frame2.nature;
                        });
                        break;
                    case 5:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ability < frame2.ability;
                        });
                        break;
                    case 6:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[0] < frame2.ivs[0];
                        });
                        break;
                    case 7:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[1] < frame2.ivs[1];
                        });
                        break;
                    case 8:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[2] < frame2.ivs[2];
                        });
                        break;
                    case 9:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[3] < frame2.ivs[3];
                        });
                        break;
                    case 10:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[4] < frame2.ivs[4];
                        });
                        break;
                    case 11:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[5] < frame2.ivs[5];
                        });
                        break;
                    case 12:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.hidden < frame2.hidden;
                        });
                        break;
                    case 13:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.power < frame2.power;
                        });
                        break;
                    case 14:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.gender < frame2.gender;
                        });
                        break;
                }
                break;
            case WondercardIVs:
                switch (column)
                {
                    case 0:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.seed < frame2.seed;
                        });
                        break;
                    case 1:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.frame < frame2.frame;
                        });
                        break;
                    case 2:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[0] < frame2.ivs[0];
                        });
                        break;
                    case 3:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[1] < frame2.ivs[1];
                        });
                        break;
                    case 4:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[2] < frame2.ivs[2];
                        });
                        break;
                    case 5:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[3] < frame2.ivs[3];
                        });
                        break;
                    case 6:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[4] < frame2.ivs[4];
                        });
                        break;
                    case 7:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[5] < frame2.ivs[5];
                        });
                        break;
                    case 8:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.hidden < frame2.hidden;
                        });
                        break;
                    case 9:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.power < frame2.power;
                        });
                        break;
                }
                break;
            case MethodJ:
            case MethodK:
                switch (column)
                {
                    case 0:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.seed < frame2.seed;
                        });
                        break;
                    case 1:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.frame < frame2.frame;
                        });
                        break;
                    case 2:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.leadType < frame2.leadType;
                        });
                        break;
                    case 3:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.pid < frame2.pid;
                        });
                        break;
                    case 4:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.encounterSlot < frame2.encounterSlot;
                        });
                        break;
                    case 5:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.shiny < frame2.shiny;
                        });
                        break;
                    case 6:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.nature < frame2.nature;
                        });
                        break;
                    case 7:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ability < frame2.ability;
                        });
                        break;
                    case 8:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[0] < frame2.ivs[0];
                        });
                        break;
                    case 9:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[1] < frame2.ivs[1];
                        });
                        break;
                    case 10:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[2] < frame2.ivs[2];
                        });
                        break;
                    case 11:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[3] < frame2.ivs[3];
                        });
                        break;
                    case 12:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[4] < frame2.ivs[4];
                        });
                        break;
                    case 13:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[5] < frame2.ivs[5];
                        });
                        break;
                    case 14:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.hidden < frame2.hidden;
                        });
                        break;
                    case 15:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.power < frame2.power;
                        });
                        break;
                    case 16:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
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
            case Method1:
            case ChainedShiny:
                switch (column)
                {
                    case 0:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.seed > frame2.seed;
                        });
                        break;
                    case 1:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.frame > frame2.frame;
                        });
                        break;
                    case 2:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.pid > frame2.pid;
                        });
                        break;
                    case 3:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.shiny > frame2.shiny;
                        });
                        break;
                    case 4:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.nature > frame2.nature;
                        });
                        break;
                    case 5:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ability > frame2.ability;
                        });
                        break;
                    case 6:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[0] > frame2.ivs[0];
                        });
                        break;
                    case 7:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[1] > frame2.ivs[1];
                        });
                        break;
                    case 8:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[2] > frame2.ivs[2];
                        });
                        break;
                    case 9:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[3] > frame2.ivs[3];
                        });
                        break;
                    case 10:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[4] > frame2.ivs[4];
                        });
                        break;
                    case 11:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[5] > frame2.ivs[5];
                        });
                        break;
                    case 12:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.hidden > frame2.hidden;
                        });
                        break;
                    case 13:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.power > frame2.power;
                        });
                        break;
                    case 14:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.gender > frame2.gender;
                        });
                        break;
                }
                break;
            case WondercardIVs:
                switch (column)
                {
                    case 0:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.seed > frame2.seed;
                        });
                        break;
                    case 1:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.frame > frame2.frame;
                        });
                        break;
                    case 2:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[0] > frame2.ivs[0];
                        });
                        break;
                    case 3:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[1] > frame2.ivs[1];
                        });
                        break;
                    case 4:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[2] > frame2.ivs[2];
                        });
                        break;
                    case 5:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[3] > frame2.ivs[3];
                        });
                        break;
                    case 6:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[4] > frame2.ivs[4];
                        });
                        break;
                    case 7:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[5] > frame2.ivs[5];
                        });
                        break;
                    case 8:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.hidden > frame2.hidden;
                        });
                        break;
                    case 9:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.power > frame2.power;
                        });
                        break;
                }
                break;
            case MethodJ:
            case MethodK:
                switch (column)
                {
                    case 0:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.seed > frame2.seed;
                        });
                        break;
                    case 1:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.frame > frame2.frame;
                        });
                        break;
                    case 2:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.leadType > frame2.leadType;
                        });
                        break;
                    case 3:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.pid > frame2.pid;
                        });
                        break;
                    case 4:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.encounterSlot > frame2.encounterSlot;
                        });
                        break;
                    case 5:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.shiny > frame2.shiny;
                        });
                        break;
                    case 6:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.nature > frame2.nature;
                        });
                        break;
                    case 7:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ability > frame2.ability;
                        });
                        break;
                    case 8:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[0] > frame2.ivs[0];
                        });
                        break;
                    case 9:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[1] > frame2.ivs[1];
                        });
                        break;
                    case 10:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[2] > frame2.ivs[2];
                        });
                        break;
                    case 11:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[3] > frame2.ivs[3];
                        });
                        break;
                    case 12:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[4] > frame2.ivs[4];
                        });
                        break;
                    case 13:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.ivs[5] > frame2.ivs[5];
                        });
                        break;
                    case 14:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.hidden > frame2.hidden;
                        });
                        break;
                    case 15:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
                        {
                            return frame1.power > frame2.power;
                        });
                        break;
                    case 16:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frame1, const Frame4 & frame2)
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

int Searcher4Model::rowCount(const QModelIndex &parent) const
{
    (void) parent;
    return (int)model.size();
}

int Searcher4Model::columnCount(const QModelIndex &parent) const
{
    (void) parent;
    switch (method)
    {
        case Method1:
        case ChainedShiny:
            return 15;
        case WondercardIVs:
            return 10;
        case MethodJ:
        case MethodK:
        default:
            return 17;
    }
}

QVariant Searcher4Model::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        int row = index.row();
        int column = index.column();
        Frame4 frame = model[row];
        switch (method)
        {
            case WondercardIVs:
                switch (column)
                {
                    case 0:
                        return QString::number(frame.seed, 16).toUpper().rightJustified(8, '0');
                    case 1:
                        return frame.frame;
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
                break;
            case Method1:
            case ChainedShiny:
                switch (column)
                {
                    case 0:
                        return QString::number(frame.seed, 16).toUpper().rightJustified(8, '0');
                    case 1:
                        return frame.frame;
                    case 2:
                        return QString::number(frame.pid, 16).toUpper().rightJustified(8, '0');
                    case 3:
                        return frame.getShiny();
                    case 4:
                        return frame.getNature();
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
                        return frame.getPower();
                    case 13:
                        return frame.power;
                    case 14:
                        return frame.getGender();

                }
                break;
            case MethodJ:
            case MethodK:
                switch (column)
                {
                    case 0:
                        return QString::number(frame.seed, 16).toUpper().rightJustified(8, '0');
                    case 1:
                        return frame.frame;
                    case 2:
                        return frame.leadType;
                    case 3:
                        return QString::number(frame.pid, 16).toUpper().rightJustified(8, '0');
                    case 4:
                        return frame.encounterSlot;
                    case 5:
                        return frame.getShiny();
                    case 6:
                        return frame.getNature();
                    case 7:
                        return frame.ability;
                    case 8:
                        return frame.ivs[0];
                    case 9:
                        return frame.ivs[1];
                    case 10:
                        return frame.ivs[2];
                    case 11:
                        return frame.ivs[3];
                    case 12:
                        return frame.ivs[4];
                    case 13:
                        return frame.ivs[5];
                    case 14:
                        return frame.getPower();
                    case 15:
                        return frame.power;
                    case 16:
                        return frame.getGender();
                }
                break;
        }
    }
    return QVariant();
}

QVariant Searcher4Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (method)
            {
                case Method1:
                case ChainedShiny:
                    switch (section)
                    {
                        case 0:
                            return tr("Seed");
                        case 1:
                            return tr("Frame");
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
                    break;
                case WondercardIVs:
                    switch (section)
                    {
                        case 0:
                            return tr("Seed");
                        case 1:
                            return tr("Frame");
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
                    break;
                case MethodJ:
                case MethodK:
                    switch (section)
                    {
                        case 0:
                            return tr("Seed");
                        case 1:
                            return tr("Frame");
                        case 2:
                            return tr("Lead");
                        case 3:
                            return tr("PID");
                        case 4:
                            return tr("Slot");
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
                    }
            }
        }
    }
    return QVariant();
}
