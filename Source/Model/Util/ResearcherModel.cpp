/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

ResearcherModel::ResearcherModel(QObject *parent, bool flag) : TableModel(parent), flag(flag)
{
}

void ResearcherModel::setFlag(bool flag)
{
    this->flag = flag;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

void ResearcherModel::setHex(const std::array<bool, 10> &hex)
{
    this->hex = hex;
}

int ResearcherModel::columnCount(const QModelIndex &index) const
{
    return flag ? 16 : 14;
}

QVariant ResearcherModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &state = model[index.row()];
        int column = getColumn(index.column());
        switch (column)
        {
        case 0:
            return state.getAdvances();
        case 1:
            return QString::number(state.getPRNG(), 16).toUpper().rightJustified(16, '0');
        case 2:
            return QString::number(state.getHigh32(), 16).toUpper().rightJustified(8, '0');
        case 3:
            return QString::number(state.getLow32(), 16).toUpper().rightJustified(8, '0');
        case 4:
            return QString::number(state.getPRNG(), 16).toUpper().rightJustified(8, '0');
        case 5:
            return QString::number(state.getHigh16(), 16).toUpper().rightJustified(4, '0');
        case 6:
            return QString::number(state.getLow16(), 16).toUpper().rightJustified(4, '0');
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
            return QString::number(state.getCustom(column - 7), hex[column - 7] ? 16 : 10).toUpper();
        }
    }
    return QVariant();
}

QVariant ResearcherModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[getColumn(section)];
    }
    return QVariant();
}

QModelIndex ResearcherModel::search(Custom custom, u64 result, int row)
{
    int column = 0;
    u64 (*getResult)(const ResearcherState &);

    if (custom == Custom::Full64Bit)
    {
        column = 1;
        getResult = [](const ResearcherState &state) { return state.getPRNG(); };
    }
    else if (custom == Custom::High32Bit)
    {
        column = 2;
        getResult = [](const ResearcherState &state) { return static_cast<u64>(state.getHigh32()); };
    }
    else if (custom == Custom::Low32Bit)
    {
        column = 3;
        getResult = [](const ResearcherState &state) { return static_cast<u64>(state.getLow32()); };
    }
    else if (custom == Custom::Full32Bit)
    {
        column = 1;
        getResult = [](const ResearcherState &state) { return state.getPRNG(); };
    }
    else if (custom == Custom::High16Bit)
    {
        column = flag ? 4 : 2;
        getResult = [](const ResearcherState &state) { return static_cast<u64>(state.getHigh16()); };
    }
    else if (custom == Custom::Low16Bit)
    {
        column = flag ? 5 : 3;
        getResult = [](const ResearcherState &state) { return static_cast<u64>(state.getLow16()); };
    }
    else if (custom == Custom::Custom1)
    {
        column = flag ? 6 : 4;
        getResult = [](const ResearcherState &state) { return state.getCustom(0); };
    }
    else if (custom == Custom::Custom2)
    {
        column = flag ? 7 : 5;
        getResult = [](const ResearcherState &state) { return state.getCustom(1); };
    }
    else if (custom == Custom::Custom3)
    {
        column = flag ? 8 : 6;
        getResult = [](const ResearcherState &state) { return state.getCustom(2); };
    }
    else if (custom == Custom::Custom4)
    {
        column = flag ? 9 : 7;
        getResult = [](const ResearcherState &state) { return state.getCustom(3); };
    }
    else if (custom == Custom::Custom5)
    {
        column = flag ? 10 : 8;
        getResult = [](const ResearcherState &state) { return state.getCustom(4); };
    }
    else if (custom == Custom::Custom6)
    {
        column = flag ? 11 : 9;
        getResult = [](const ResearcherState &state) { return state.getCustom(5); };
    }
    else if (custom == Custom::Custom7)
    {
        column = flag ? 12 : 10;
        getResult = [](const ResearcherState &state) { return state.getCustom(6); };
    }
    else if (custom == Custom::Custom8)
    {
        column = flag ? 13 : 11;
        getResult = [](const ResearcherState &state) { return state.getCustom(7); };
    }
    else if (custom == Custom::Custom9)
    {
        column = flag ? 14 : 12;
        getResult = [](const ResearcherState &state) { return state.getCustom(8); };
    }
    else if (custom == Custom::Custom10)
    {
        column = flag ? 15 : 13;
        getResult = [](const ResearcherState &state) { return state.getCustom(9); };
    }

    for (; row < rowCount(); row++)
    {
        u64 value = getResult(model[row]);
        if (value == result)
        {
            return index(row, column, QModelIndex());
        }
    }

    return QModelIndex();
}

int ResearcherModel::getColumn(int column) const
{
    if (flag)
    {
        return column > 3 ? column + 1 : column;
    }
    return column > 0 ? column + 3 : column;
}
