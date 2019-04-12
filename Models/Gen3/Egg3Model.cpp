/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2019 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "Egg3Model.hpp"

Egg3Model::Egg3Model(QObject *parent, Method method) :
    TableModel<Frame3>(parent)
{
    this->method = method;
    showInheritance = false;
}

void Egg3Model::setMethod(Method method)
{
    this->method = method;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

int Egg3Model::columnCount(const QModelIndex &parent) const
{
    (void) parent;
    switch (method)
    {
        case Method::EBred:
        case Method::EBredAlternate:
        case Method::EBredSplit:
            return 10;
        case Method::EBredPID:
            return 8;
        case Method::RSBred:
        case Method::FRLGBred:
            return 17;
        default:
            return 0;
    }
}

QVariant Egg3Model::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        auto frame = model.at(index.row());
        switch (method)
        {
            case Method::EBred:
            case Method::EBredAlternate:
            case Method::EBredSplit:
                switch (index.column())
                {
                    case 0:
                        return frame.getFrame();
                    case 1:
                        return frame.getTime();
                    case 2:
                        if (showInheritance)
                        {
                            QChar inh = frame.getInheritance(0);
                            if (!inh.isNull())
                            {
                                return inh;
                            }
                        }
                        return frame.getIV(0);
                    case 3:
                        if (showInheritance)
                        {
                            QChar inh = frame.getInheritance(1);
                            if (!inh.isNull())
                            {
                                return inh;
                            }
                        }
                        return frame.getIV(1);
                    case 4:
                        if (showInheritance)
                        {
                            QChar inh = frame.getInheritance(2);
                            if (!inh.isNull())
                            {
                                return inh;
                            }
                        }
                        return frame.getIV(2);
                    case 5:
                        if (showInheritance)
                        {
                            QChar inh = frame.getInheritance(3);
                            if (!inh.isNull())
                            {
                                return inh;
                            }
                        }
                        return frame.getIV(3);
                    case 6:
                        if (showInheritance)
                        {
                            QChar inh = frame.getInheritance(4);
                            if (!inh.isNull())
                            {
                                return inh;
                            }
                        }
                        return frame.getIV(4);
                    case 7:
                        if (showInheritance)
                        {
                            QChar inh = frame.getInheritance(5);
                            if (!inh.isNull())
                            {
                                return inh;
                            }
                        }
                        return frame.getIV(5);
                    case 8:
                        return frame.getPowerString();
                    case 9:
                        return frame.getPower();
                    default:
                        return QVariant();
                }
            case Method::EBredPID:
                switch (index.column())
                {
                    case 0:
                        return frame.getFrame();
                    case 1:
                        return frame.getTime();
                    case 2:
                        return frame.getOccidentary();
                    case 3:
                        return QString::number(frame.getPID(), 16).toUpper().rightJustified(8, '0');
                    case 4:
                        return frame.getShinyString();
                    case 5:
                        return frame.getNatureString();
                    case 6:
                        return frame.getAbility();
                    case 7:
                        return frame.getGenderString();
                }
            case Method::RSBred:
            case Method::FRLGBred:
                switch (index.column())
                {
                    case 0:
                        return frame.getFrame();
                    case 1:
                        return frame.getTime();
                    case 2:
                        return frame.getOccidentary();
                    case 3:
                        return frame.getTimeEgg();
                    case 4:
                        return QString::number(frame.getPID(), 16).toUpper().rightJustified(8, '0');
                    case 5:
                        return frame.getShinyString();
                    case 6:
                        return frame.getNatureString();
                    case 7:
                        return frame.getAbility();
                    case 8:
                        if (showInheritance)
                        {
                            QChar inh = frame.getInheritance(0);
                            if (!inh.isNull())
                            {
                                return inh;
                            }
                        }
                        return frame.getIV(0);
                    case 9:
                        if (showInheritance)
                        {
                            QChar inh = frame.getInheritance(1);
                            if (!inh.isNull())
                            {
                                return inh;
                            }
                        }
                        return frame.getIV(1);
                    case 10:
                        if (showInheritance)
                        {
                            QChar inh = frame.getInheritance(2);
                            if (!inh.isNull())
                            {
                                return inh;
                            }
                        }
                        return frame.getIV(2);
                    case 11:
                        if (showInheritance)
                        {
                            QChar inh = frame.getInheritance(3);
                            if (!inh.isNull())
                            {
                                return inh;
                            }
                        }
                        return frame.getIV(3);
                    case 12:
                        if (showInheritance)
                        {
                            QChar inh = frame.getInheritance(4);
                            if (!inh.isNull())
                            {
                                return inh;
                            }
                        }
                        return frame.getIV(4);
                    case 13:
                        if (showInheritance)
                        {
                            QChar inh = frame.getInheritance(5);
                            if (!inh.isNull())
                            {
                                return inh;
                            }
                        }
                        return frame.getIV(5);
                    case 14:
                        return frame.getPowerString();
                    case 15:
                        return frame.getPower();
                    case 16:
                        return frame.getGenderString();
                }
            default:
                break;
        }
    }
    else if (role == Qt::FontRole)
    {
        auto frame = model.at(index.row());
        switch (method)
        {
            case Method::EBred:
            case Method::EBredSplit:
            case Method::EBredAlternate:
                switch (index.column())
                {
                    case 2:
                        if (!showInheritance || frame.getInheritance(0).isNull())
                        {
                            return TableUtility::getBold(frame.getIV(0));
                        }
                        break;
                    case 3:
                        if (!showInheritance || frame.getInheritance(1).isNull())
                        {
                            return TableUtility::getBold(frame.getIV(1));
                        }
                        break;
                    case 4:
                        if (!showInheritance || frame.getInheritance(2).isNull())
                        {
                            return TableUtility::getBold(frame.getIV(2));
                        }
                        break;
                    case 5:
                        if (!showInheritance || frame.getInheritance(3).isNull())
                        {
                            return TableUtility::getBold(frame.getIV(3));
                        }
                        break;
                    case 6:
                        if (!showInheritance || frame.getInheritance(4).isNull())
                        {
                            return TableUtility::getBold(frame.getIV(4));
                        }
                        break;
                    case 7:
                        if (!showInheritance || frame.getInheritance(5).isNull())
                        {
                            return TableUtility::getBold(frame.getIV(5));
                        }
                        break;
                }
                break;
            case Method::RSBred:
            case Method::FRLGBred:
                switch (index.column())
                {
                    case 8:
                        if (!showInheritance || frame.getInheritance(0).isNull())
                        {
                            return TableUtility::getBold(frame.getIV(0));
                        }
                        break;
                    case 9:
                        if (!showInheritance || frame.getInheritance(1).isNull())
                        {
                            return TableUtility::getBold(frame.getIV(1));
                        }
                        break;
                    case 10:
                        if (!showInheritance || frame.getInheritance(2).isNull())
                        {
                            return TableUtility::getBold(frame.getIV(2));
                        }
                        break;
                    case 11:
                        if (!showInheritance || frame.getInheritance(3).isNull())
                        {
                            return TableUtility::getBold(frame.getIV(3));
                        }
                        break;
                    case 12:
                        if (!showInheritance || frame.getInheritance(4).isNull())
                        {
                            return TableUtility::getBold(frame.getIV(4));
                        }
                        break;
                    case 13:
                        if (!showInheritance || frame.getInheritance(5).isNull())
                        {
                            return TableUtility::getBold(frame.getIV(5));
                        }
                        break;
                }
            default:
                break;
        }
    }
    return QVariant();
}

QVariant Egg3Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (method)
        {
            case Method::EBred:
            case Method::EBredSplit:
            case Method::EBredAlternate:
                return header1.at(section);
            case Method::EBredPID:
                return header2.at(section);
            case Method::RSBred:
            case Method::FRLGBred:
                return header3.at(section);
            default:
                break;
        }
    }
    return QVariant();
}

void Egg3Model::toggleInheritance(bool flag)
{
    beginResetModel();
    showInheritance = flag;
    endResetModel();
}
