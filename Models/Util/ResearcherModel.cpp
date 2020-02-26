/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <functional>

ResearcherModel::ResearcherModel(QObject *parent, bool flag) : TableModel<ResearcherFrame>(parent), flag(flag)
{
}

void ResearcherModel::setFlag(bool flag)
{
    this->flag = flag;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

void ResearcherModel::setHex(const QVector<bool> &hex)
{
    this->hex = hex;
}

int ResearcherModel::columnCount(const QModelIndex &index) const
{
    (void)index;
    return flag ? 22 : 20;
}

QVariant ResearcherModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &frame = model.at(index.row());
        int column = getColumn(index.column());
        switch (column)
        {
        case 0:
            return frame.getFrame();
        case 1:
            return QString::number(frame.getFull64(), 16).toUpper().rightJustified(16, '0');
        case 2:
            return QString::number(frame.getHigh32(), 16).toUpper().rightJustified(8, '0');
        case 3:
            return QString::number(frame.getLow32(), 16).toUpper().rightJustified(8, '0');
        case 4:
            return QString::number(frame.getFull32(), 16).toUpper().rightJustified(8, '0');
        case 5:
            return QString::number(frame.getHigh16(), 16).toUpper().rightJustified(4, '0');
        case 6:
            return QString::number(frame.getLow16(), 16).toUpper().rightJustified(4, '0');
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
            return QString::number(frame.getCustom(static_cast<u8>(column - 7)), hex.at(column - 7) ? 16 : 10).toUpper();
        case 17:
            return frame.getMod3();
        case 18:
            return frame.getMod25();
        case 19:
            return frame.getMod100();
        case 20:
            return frame.getDiv656();
        case 21:
            return frame.getHighBit();
        case 22:
            return frame.getLowBit();
        }
    }
    return QVariant();
}

QVariant ResearcherModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header.at(getColumn(section));
    }
    return QVariant();
}

QModelIndex ResearcherModel::search(const QString &string, u64 result, int row)
{
    int column = 0;
    std::function<u64(const ResearcherFrame &)> getResult;
    if (string == tr("64Bit"))
    {
        column = 1;
        getResult = [](const ResearcherFrame &frame) { return frame.getFull64(); };
    }
    else if (string == tr("32Bit High"))
    {
        column = 2;
        getResult = [](const ResearcherFrame &frame) { return frame.getHigh32(); };
    }
    else if (string == tr("32Bit Low"))
    {
        column = 3;
        getResult = [](const ResearcherFrame &frame) { return frame.getLow32(); };
    }
    else if (string == tr("32Bit"))
    {
        column = 1;
        getResult = [](const ResearcherFrame &frame) { return frame.getFull32(); };
    }
    else if (string == tr("16Bit High"))
    {
        column = flag ? 4 : 2;
        getResult = [](const ResearcherFrame &frame) { return frame.getHigh16(); };
    }
    else if (string == tr("16Bit Low"))
    {
        column = flag ? 5 : 3;
        getResult = [](const ResearcherFrame &frame) { return frame.getLow16(); };
    }

    for (; row < rowCount(); row++)
    {
        u64 value = getResult(model.at(row));
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
