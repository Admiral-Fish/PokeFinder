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

#include "EggModel3.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Util/Translator.hpp>

EggModel3::EggModel3(QObject *parent, Method method) : TableModel<EggFrame3>(parent)
{
    this->method = method;
    showInheritance = false;
}

void EggModel3::setMethod(Method method)
{
    this->method = method;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

int EggModel3::columnCount(const QModelIndex & /*parent*/) const
{
    switch (method)
    {
    case Method::RSBred:
    case Method::FRLGBred:
        return 15;
    case Method::EBred:
    case Method::EBredAlternate:
    case Method::EBredSplit:
        return 9;
    case Method::EBredPID:
        return 7;
    default:
        return 0;
    }
}

QVariant EggModel3::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        int column = getColumn(index.column());

        const auto &frame = model.at(index.row());
        switch (column)
        {
        case 0:
        case 1:
            return frame.getFrame();
        case 2:
            return frame.getPickupFrame();
        case 3:
            return frame.getRedraw();
        case 4:
            return QString::number(frame.getPID(), 16).toUpper().rightJustified(8, '0');
        case 5:
        {
            u8 shiny = frame.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 6:
            return Translator::getNature(frame.getNature());
        case 7:
            return frame.getAbility();
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            if (showInheritance)
            {
                u8 inh = frame.getInheritance(static_cast<u8>(column - 8));
                if (inh)
                {
                    return inh == 1 ? "A" : "B";
                }
            }
            return frame.getIV(static_cast<u8>(column - 8));
        case 14:
            return Translator::getHiddenPower(frame.getHidden());
        case 15:
            return frame.getPower();
        case 16:
            return Translator::getGender(frame.getGender());
        }
    }

    return QVariant();
}

QVariant EggModel3::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        section = getColumn(section);
        return header.at(section);
    }
    return QVariant();
}

void EggModel3::toggleInheritance(bool flag)
{
    beginResetModel();
    showInheritance = flag;
    endResetModel();
}

int EggModel3::getColumn(int column) const
{
    switch (method)
    {
    case Method::RSBred:
    case Method::FRLGBred:
        return column > 1 ? column + 2 : column + 1;
    case Method::EBred:
    case Method::EBredAlternate:
    case Method::EBredSplit:
        return column > 0 ? column + 7 : column;
    case Method::EBredPID:
        return column == 6 ? column + 10 : column > 0 ? column + 2 : column;
    default:
        return column;
    }
}
