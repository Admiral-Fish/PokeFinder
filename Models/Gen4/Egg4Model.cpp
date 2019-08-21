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

#include "Egg4Model.hpp"
#include <Util/TableUtility.hpp>

Egg4GeneratorModel::Egg4GeneratorModel(QObject *parent, Method method) :
    TableModel<Frame4>(parent)
{
    this->method = method;
    showInheritance = false;
}

void Egg4GeneratorModel::setMethod(Method method)
{
    this->method = method;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

int Egg4GeneratorModel::columnCount(const QModelIndex & /*parent*/) const
{
    switch (method)
    {
        case Method::Gen4Normal:
        case Method::Gen4Masuada:
            return 6;
        case Method::DPPtIVs:
            return 10;
        case Method::HGSSIVs:
            return 11;
        default:
            return 0;
    }
}

QVariant Egg4GeneratorModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        auto frame = model.at(index.row());
        switch (method)
        {
            case Method::Gen4Normal:
            case Method::Gen4Masuada:
                switch (index.column())
                {
                    case 0:
                        return frame.getFrame();
                    case 1:
                        return QString::number(frame.getPID(), 16).toUpper().rightJustified(8, '0');
                    case 2:
                        return frame.getShinyString();
                    case 3:
                        return frame.getNatureString();
                    case 4:
                        return frame.getAbility();
                    case 5:
                        return frame.getGenderString();
                }
            case Method::DPPtIVs:
                switch (index.column())
                {
                    case 0:
                        return frame.getFrame();
                    case 1:
                        return frame.chatotPitch();
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
            case Method::HGSSIVs:
                switch (index.column())
                {
                    case 0:
                        return frame.getFrame();
                    case 1:
                        return frame.getCall();
                    case 2:
                        return frame.chatotPitch();
                    case 3:
                        if (showInheritance)
                        {
                            QChar inh = frame.getInheritance(0);
                            if (!inh.isNull())
                            {
                                return inh;
                            }
                        }
                        return frame.getIV(0);
                    case 4:
                        if (showInheritance)
                        {
                            QChar inh = frame.getInheritance(1);
                            if (!inh.isNull())
                            {
                                return inh;
                            }
                        }
                        return frame.getIV(1);
                    case 5:
                        if (showInheritance)
                        {
                            QChar inh = frame.getInheritance(2);
                            if (!inh.isNull())
                            {
                                return inh;
                            }
                        }
                        return frame.getIV(2);
                    case 6:
                        if (showInheritance)
                        {
                            QChar inh = frame.getInheritance(3);
                            if (!inh.isNull())
                            {
                                return inh;
                            }
                        }
                        return frame.getIV(3);
                    case 7:
                        if (showInheritance)
                        {
                            QChar inh = frame.getInheritance(4);
                            if (!inh.isNull())
                            {
                                return inh;
                            }
                        }
                        return frame.getIV(4);
                    case 8:
                        if (showInheritance)
                        {
                            QChar inh = frame.getInheritance(5);
                            if (!inh.isNull())
                            {
                                return inh;
                            }
                        }
                        return frame.getIV(5);
                    case 9:
                        return frame.getPowerString();
                    case 10:
                        return frame.getPower();
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
            case Method::DPPtIVs:
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
            case Method::HGSSIVs:
                switch (index.column())
                {
                    case 3:
                        if (!showInheritance || frame.getInheritance(0).isNull())
                        {
                            return TableUtility::getBold(frame.getIV(0));
                        }
                        break;
                    case 4:
                        if (!showInheritance || frame.getInheritance(1).isNull())
                        {
                            return TableUtility::getBold(frame.getIV(1));
                        }
                        break;
                    case 5:
                        if (!showInheritance || frame.getInheritance(2).isNull())
                        {
                            return TableUtility::getBold(frame.getIV(2));
                        }
                        break;
                    case 6:
                        if (!showInheritance || frame.getInheritance(3).isNull())
                        {
                            return TableUtility::getBold(frame.getIV(3));
                        }
                        break;
                    case 7:
                        if (!showInheritance || frame.getInheritance(4).isNull())
                        {
                            return TableUtility::getBold(frame.getIV(4));
                        }
                        break;
                    case 8:
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

QVariant Egg4GeneratorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (method)
        {
            case Method::Gen4Normal:
            case Method::Gen4Masuada:
                return header1.at(section);
            case Method::DPPtIVs:
                return header2.at(section);
            case Method::HGSSIVs:
                return header3.at(section);
            default:
                break;
        }
    }
    return QVariant();
}

void Egg4GeneratorModel::toggleInheritance(bool flag)
{
    beginResetModel();
    showInheritance = flag;
    endResetModel();
}


Egg4SearcherModel::Egg4SearcherModel(QObject *parent, Method method) :
    TableModel<Frame4>(parent)
{
    this->method = method;
    showInheritance = false;
}

void Egg4SearcherModel::setMethod(Method method)
{
    this->method = method;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

int Egg4SearcherModel::columnCount(const QModelIndex & /*parent*/) const
{
    switch (method)
    {
        case Method::Gen4Normal:
        case Method::Gen4Masuada:
            return 7;
        case Method::DPPtIVs:
        case Method::HGSSIVs:
            return 10;
        case Method::Gen4Combined:
            return 16;
        default:
            return 0;
    }
}

QVariant Egg4SearcherModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        auto frame = model.at(index.row());
        switch (method)
        {
            case Method::Gen4Normal:
            case Method::Gen4Masuada:
                switch (index.column())
                {
                    case 0:
                        return QString::number(frame.getInitialSeed(), 16).toUpper().rightJustified(8, '0');
                    case 1:
                        return frame.getFrame();
                    case 2:
                        return QString::number(frame.getPID(), 16).toUpper().rightJustified(8, '0');
                    case 3:
                        return frame.getShinyString();
                    case 4:
                        return frame.getNatureString();
                    case 5:
                        return frame.getAbility();
                    case 6:
                        return frame.getGenderString();
                    default:
                        break;
                }
            case Method::DPPtIVs:
            case Method::HGSSIVs:
                switch (index.column())
                {
                    case 0:
                        return QString::number(frame.getInitialSeed(), 16).toUpper().rightJustified(8, '0');
                    case 1:
                        return frame.getFrame();
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
                        break;
                }
            case Method::Gen4Combined:
                switch (index.column())
                {
                    case 0:
                        return QString::number(frame.getInitialSeed(), 16).toUpper().rightJustified(8, '0');
                    case 1:
                        return frame.getFrame();
                    case 2:
                        return frame.getEggFrame();
                    case 3:
                        return QString::number(frame.getPID(), 16).toUpper().rightJustified(8, '0');
                    case 4:
                        return frame.getShinyString();
                    case 5:
                        return frame.getNatureString();
                    case 6:
                        return frame.getAbility();
                    case 7:
                        if (showInheritance)
                        {
                            QChar inh = frame.getInheritance(0);
                            if (!inh.isNull())
                            {
                                return inh;
                            }
                        }
                        return frame.getIV(0);
                    case 8:
                        if (showInheritance)
                        {
                            QChar inh = frame.getInheritance(1);
                            if (!inh.isNull())
                            {
                                return inh;
                            }
                        }
                        return frame.getIV(1);
                    case 9:
                        if (showInheritance)
                        {
                            QChar inh = frame.getInheritance(2);
                            if (!inh.isNull())
                            {
                                return inh;
                            }
                        }
                        return frame.getIV(2);
                    case 10:
                        if (showInheritance)
                        {
                            QChar inh = frame.getInheritance(3);
                            if (!inh.isNull())
                            {
                                return inh;
                            }
                        }
                        return frame.getIV(3);
                    case 11:
                        if (showInheritance)
                        {
                            QChar inh = frame.getInheritance(4);
                            if (!inh.isNull())
                            {
                                return inh;
                            }
                        }
                        return frame.getIV(4);
                    case 12:
                        if (showInheritance)
                        {
                            QChar inh = frame.getInheritance(5);
                            if (!inh.isNull())
                            {
                                return inh;
                            }
                        }
                        return frame.getIV(5);
                    case 13:
                        return frame.getPowerString();
                    case 14:
                        return frame.getPower();
                    case 15:
                        return frame.getGenderString();
                    default:
                        break;
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
            case Method::DPPtIVs:
            case Method::HGSSIVs:
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
            case Method::Gen4Combined:
                switch (index.column())
                {
                    case 7:
                        if (!showInheritance || frame.getInheritance(0).isNull())
                        {
                            return TableUtility::getBold(frame.getIV(0));
                        }
                        break;
                    case 8:
                        if (!showInheritance || frame.getInheritance(1).isNull())
                        {
                            return TableUtility::getBold(frame.getIV(1));
                        }
                        break;
                    case 9:
                        if (!showInheritance || frame.getInheritance(2).isNull())
                        {
                            return TableUtility::getBold(frame.getIV(2));
                        }
                        break;
                    case 10:
                        if (!showInheritance || frame.getInheritance(3).isNull())
                        {
                            return TableUtility::getBold(frame.getIV(3));
                        }
                        break;
                    case 11:
                        if (!showInheritance || frame.getInheritance(4).isNull())
                        {
                            return TableUtility::getBold(frame.getIV(4));
                        }
                        break;
                    case 12:
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

QVariant Egg4SearcherModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (method)
        {
            case Method::Gen4Normal:
            case Method::Gen4Masuada:
                return header1.at(section);
            case Method::DPPtIVs:
            case Method::HGSSIVs:
                return header2.at(section);
            case Method::Gen4Combined:
                return header3.at(section);
            default:
                break;
        }
    }
    return QVariant();
}

void Egg4SearcherModel::toggleInheritance(bool flag)
{
    beginResetModel();
    showInheritance = flag;
    endResetModel();
}
