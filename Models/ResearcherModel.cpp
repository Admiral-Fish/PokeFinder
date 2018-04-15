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

#include "ResearcherModel.hpp"

ResearcherModel::ResearcherModel(QObject *parent, bool is64Bit) : QAbstractTableModel(parent)
{
    flag = is64Bit;
}

void ResearcherModel::setModel(vector<ResearcherFrame> frames)
{
    if (frames.empty())
        return;
    int i = rowCount();
    emit beginInsertRows(QModelIndex(), i, i + frames.size() - 1);
    model.insert(model.end(), frames.begin(), frames.end());
    emit endInsertRows();
}

void ResearcherModel::clear()
{
    if (model.empty())
        return;
    emit beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    model.clear();
    model.shrink_to_fit();
    emit endRemoveRows();
}

void ResearcherModel::setFlag(bool is64Bit)
{
    flag = is64Bit;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

void ResearcherModel::setHex(vector<bool> hex)
{
    this->hex = hex;
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
                    return frame.frame;
                case 1:
                    return QString::number(frame.full64, 16).toUpper().rightJustified(16, '0');
                case 2:
                    return QString::number(frame.high32(), 16).toUpper().rightJustified(8, '0');
                case 3:
                    return QString::number(frame.low32(), 16).toUpper().rightJustified(8, '0');
                case 4:
                    return QString::number(frame.high16(), 16).toUpper().rightJustified(4, '0');
                case 5:
                    return QString::number(frame.low16(), 16).toUpper().rightJustified(4, '0');
                case 6:
                    return QString::number(frame.custom[0], hex[0] ? 16 : 10).toUpper();
                case 7:
                    return QString::number(frame.custom[1], hex[1] ? 16 : 10).toUpper();
                case 8:
                    return QString::number(frame.custom[2], hex[2] ? 16 : 10).toUpper();
                case 9:
                    return QString::number(frame.custom[3], hex[3] ? 16 : 10).toUpper();
                case 10:
                    return QString::number(frame.custom[4], hex[4] ? 16 : 10).toUpper();
                case 11:
                    return QString::number(frame.custom[5], hex[5] ? 16 : 10).toUpper();
                case 12:
                    return QString::number(frame.custom[6], hex[6] ? 16 : 10).toUpper();
                case 13:
                    return QString::number(frame.custom[7], hex[7] ? 16 : 10).toUpper();
                case 14:
                    return QString::number(frame.custom[8], hex[8] ? 16 : 10).toUpper();
                case 15:
                    return QString::number(frame.custom[9], hex[9] ? 16 : 10).toUpper();
                case 16:
                    return frame.mod3();
                case 17:
                    return frame.mod25();
                case 18:
                    return frame.mod100();
                case 19:
                    return frame.div656();
                case 20:
                    return frame.lowBit();
                case 21:
                    return frame.highBit();
            }
        }
        else
        {
            switch (column)
            {
                case 0:
                    return frame.frame;
                case 1:
                    return QString::number(frame.full32, 16).toUpper().rightJustified(8, '0');
                case 2:
                    return QString::number(frame.high16(), 16).toUpper().rightJustified(4, '0');
                case 3:
                    return QString::number(frame.low16(), 16).toUpper().rightJustified(4, '0');
                case 4:
                    return QString::number(frame.custom[0], hex[0] ? 16 : 10).toUpper();
                case 5:
                    return QString::number(frame.custom[1], hex[1] ? 16 : 10).toUpper();
                case 6:
                    return QString::number(frame.custom[2], hex[2] ? 16 : 10).toUpper();
                case 7:
                    return QString::number(frame.custom[3], hex[3] ? 16 : 10).toUpper();
                case 8:
                    return QString::number(frame.custom[4], hex[4] ? 16 : 10).toUpper();
                case 9:
                    return QString::number(frame.custom[5], hex[5] ? 16 : 10).toUpper();
                case 10:
                    return QString::number(frame.custom[6], hex[6] ? 16 : 10).toUpper();
                case 11:
                    return QString::number(frame.custom[7], hex[7] ? 16 : 10).toUpper();
                case 12:
                    return QString::number(frame.custom[8], hex[8] ? 16 : 10).toUpper();
                case 13:
                    return QString::number(frame.custom[9], hex[9] ? 16 : 10).toUpper();
                case 14:
                    return frame.mod3();
                case 15:
                    return frame.mod25();
                case 16:
                    return frame.mod100();
                case 17:
                    return frame.div656();
                case 18:
                    return frame.lowBit();
                case 19:
                    return frame.highBit();
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
                        return tr("Frame");
                    case 1:
                        return tr("64Bit");
                    case 2:
                        return tr("32Bit High");
                    case 3:
                        return tr("32Bit Low");
                    case 4:
                        return tr("16Bit High");
                    case 5:
                        return tr("16Bit Low");
                    case 6:
                        return tr("Custom 1");
                    case 7:
                        return tr("Custom 2");
                    case 8:
                        return tr("Custom 3");
                    case 9:
                        return tr("Custom 4");
                    case 10:
                        return tr("Custom 5");
                    case 11:
                        return tr("Custom 6");
                    case 12:
                        return tr("Custom 7");
                    case 13:
                        return tr("Custom 8");
                    case 14:
                        return tr("Custom 9");
                    case 15:
                        return tr("Custom 10");
                    case 16:
                        return "%3";
                    case 17:
                        return "%25";
                    case 18:
                        return "%100";
                    case 19:
                        return "/656";
                    case 20:
                        return tr("LBit");
                    case 21:
                        return tr("HBit");
                }
            }
            else
            {
                switch (section)
                {
                    case 0:
                        return tr("Frame");
                    case 1:
                        return tr("32Bit");
                    case 2:
                        return tr("16Bit High");
                    case 3:
                        return tr("16Bit Low");
                    case 4:
                        return tr("Custom 1");
                    case 5:
                        return tr("Custom 2");
                    case 6:
                        return tr("Custom 3");
                    case 7:
                        return tr("Custom 4");
                    case 8:
                        return tr("Custom 5");
                    case 9:
                        return tr("Custom 6");
                    case 10:
                        return tr("Custom 7");
                    case 11:
                        return tr("Custom 8");
                    case 12:
                        return tr("Custom 9");
                    case 13:
                        return tr("Custom 10");
                    case 14:
                        return "%3";
                    case 15:
                        return "%25";
                    case 16:
                        return "%100";
                    case 17:
                        return "/656";
                    case 18:
                        return tr("LBit");
                    case 19:
                        return tr("HBit");
                }
            }
        }
    }
    return QVariant();
}

QModelIndex ResearcherModel::search(QString string, u64 result, int row)
{
    int column;
    u64 (*getResult)(ResearcherFrame);
    if (string == tr("64Bit"))
    {
        column = 1;
        getResult = &ResearcherModel::get64Bit;
    }
    else if (string == tr("32Bit High"))
    {
        column = 2;
        getResult = &ResearcherModel::get32BitHigh;
    }
    else if (string == tr("32Bit Low"))
    {
        column = 3;
        getResult = &ResearcherModel::get32BitLow;
    }
    else if (string == tr("32Bit"))
    {
        column = 1;
        getResult = &ResearcherModel::get32;
    }
    else if (string == tr("16Bit High"))
    {
        column = flag ? 4 : 2;
        getResult = &ResearcherModel::get16BitHigh;
    }
    else if (string == tr("16Bit Low"))
    {
        column = flag ? 5 : 3;
        getResult = &ResearcherModel::get16BitLow;
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
