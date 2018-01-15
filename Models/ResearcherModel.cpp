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

#include "ResearcherModel.hpp"

ResearcherModel::ResearcherModel(QObject *parent, bool is64Bit) : QAbstractTableModel(parent)
{
    flag = is64Bit;
}

void ResearcherModel::SetModel(vector<ResearcherFrame> frames)
{
    model = frames;
}

int ResearcherModel::rowCount(const QModelIndex &parent) const
{
    (void) parent;
    return (int)model.size();
}

int ResearcherModel::columnCount(const QModelIndex &parent) const
{
    (void) parent;
    return flag ? 22 : 20;
}

QVariant ResearcherModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        int row = index.row();
        int column = index.column();
        ResearcherFrame frame = model[row];
        if (flag)
        {
            switch (column)
            {
                case 0:
                    return frame.Frame;
                case 1:
                    return QString::number(frame.Full64, 16).toUpper().rightJustified(16,'0');
                case 2:
                    return QString::number(frame.High32(), 16).toUpper().rightJustified(8,'0');
                case 3:
                    return QString::number(frame.Low32(), 16).toUpper().rightJustified(8,'0');
                case 4:
                    return QString::number(frame.High16(), 16).toUpper().rightJustified(4,'0');
                case 5:
                    return QString::number(frame.Low16(), 16).toUpper().rightJustified(4,'0');
                case 6:
                    return frame.Custom[0];
                case 7:
                    return frame.Custom[1];
                case 8:
                    return frame.Custom[2];
                case 9:
                    return frame.Custom[3];
                case 10:
                    return frame.Custom[4];
                case 11:
                    return frame.Custom[5];
                case 12:
                    return frame.Custom[6];
                case 13:
                    return frame.Custom[7];
                case 14:
                    return frame.Custom[8];
                case 15:
                    return frame.Custom[9];
                case 16:
                    return frame.Mod3();
                case 17:
                    return frame.Mod25();
                case 18:
                    return frame.Mod100();
                case 19:
                    return frame.Div656();
                case 20:
                    return frame.LowBit();
                case 21:
                    return frame.HighBit();
            }
        }
        else
        {
            switch(column)
            {
                case 0:
                    return frame.Frame;
                case 1:
                    return QString::number(frame.Full32, 16).toUpper().rightJustified(8, '0');
                case 2:
                    return QString::number(frame.High16(), 16).toUpper().rightJustified(4, '0');
                case 3:
                    return QString::number(frame.Low16(), 16).toUpper().rightJustified(4, '0');
                case 4:
                    return frame.Custom[0];
                case 5:
                    return frame.Custom[1];
                case 6:
                    return frame.Custom[2];
                case 7:
                    return frame.Custom[3];
                case 8:
                    return frame.Custom[4];
                case 9:
                    return frame.Custom[5];
                case 10:
                    return frame.Custom[6];
                case 11:
                    return frame.Custom[7];
                case 12:
                    return frame.Custom[8];
                case 13:
                    return frame.Custom[9];
                case 14:
                    return frame.Mod3();
                case 15:
                    return frame.Mod25();
                case 16:
                    return frame.Mod100();
                case 17:
                    return frame.Div656();
                case 18:
                    return frame.LowBit();
                case 19:
                    return frame.HighBit();
            }
        }
    }
    return QVariant();
}

QVariant ResearcherModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (flag)
            {
                switch (section)
                {
                    case 0:
                        return "Frame";
                    case 1:
                        return "64Bit";
                    case 2:
                        return "32Bit High";
                    case 3:
                        return "32Bit Low";
                    case 4:
                        return "16Bit High";
                    case 5:
                        return "16Bit Low";
                    case 6:
                        return "Custom1";
                    case 7:
                        return "Custom2";
                    case 8:
                        return "Custom3";
                    case 9:
                        return "Custom4";
                    case 10:
                        return "Custom5";
                    case 11:
                        return "Custom6";
                    case 12:
                        return "Custom7";
                    case 13:
                        return "Custom8";
                    case 14:
                        return "Custom9";
                    case 15:
                        return "Custom10";
                    case 16:
                        return "%3";
                    case 17:
                        return "%25";
                    case 18:
                        return "%100";
                    case 19:
                        return "/656";
                    case 20:
                        return "LBit";
                    case 21:
                        return "HBit";
                }
            }
            else
            {
                switch (section)
                {
                    case 0:
                        return "Frame";
                    case 1:
                        return "32Bit";
                    case 2:
                        return "16Bit High";
                    case 3:
                        return "16Bit Low";
                    case 4:
                        return "Custom1";
                    case 5:
                        return "Custom2";
                    case 6:
                        return "Custom3";
                    case 7:
                        return "Custom4";
                    case 8:
                        return "Custom5";
                    case 9:
                        return "Custom6";
                    case 10:
                        return "Custom7";
                    case 11:
                        return "Custom8";
                    case 12:
                        return "Custom9";
                    case 13:
                        return "Custom10";
                    case 14:
                        return "%3";
                    case 15:
                        return "%25";
                    case 16:
                        return "%100";
                    case 17:
                        return "/656";
                    case 18:
                        return "LBit";
                    case 19:
                        return "HBit";
                }
            }
        }
    }
    return QVariant();
}

QModelIndex ResearcherModel::Search(QString string, u64 result, int row)
{
    int column;
    u64 (*getResult)(ResearcherFrame);
    if (string == "64Bit")
    {
        column = 1;
        getResult = &ResearcherModel::Get64Bit;
    }
    else if(string == "32Bit High")
    {
        column = 2;
        getResult = &ResearcherModel::Get32BitHigh;
    }
    else if(string == "32Bit Low")
    {
        column = 3;
        getResult = &ResearcherModel::Get32BitLow;
    }
    else if (string == "32Bit")
    {
        column = 1;
        getResult = &ResearcherModel::Get32;
    }
    else if (string == "16Bit High")
    {
        column = flag ? 4 : 2;
        getResult = &ResearcherModel::Get16BitHigh;
    }
    else if (string == "16Bit Low")
    {
        column = flag ? 5 : 3;
        getResult = &ResearcherModel::Get16BitLow;
    }

    int size = rowCount();

    for (; row < size; row++)
    {
        u64 value = getResult(model[row]);
        if (value == result)
            return index(row, column, QModelIndex());
    }

    return QModelIndex();
}
