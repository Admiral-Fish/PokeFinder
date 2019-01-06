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

#include "Egg4Model.hpp"

Egg4GeneratorModel::Egg4GeneratorModel(QObject *parent, Method method)
    : QAbstractTableModel(parent)
{
    this->method = method;
    showInheritance = false;
}

void Egg4GeneratorModel::setModel(const QVector<Frame4> &frames)
{
    if (frames.isEmpty())
    {
        return;
    }

    int i = rowCount();
    emit beginInsertRows(QModelIndex(), i, i + frames.size() - 1);
    model.append(frames);
    emit endInsertRows();
}

void Egg4GeneratorModel::clear()
{
    if (model.isEmpty())
    {
        return;
    }

    emit beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    model.clear();
    model.squeeze();
    emit endRemoveRows();
}

void Egg4GeneratorModel::setMethod(Method method)
{
    this->method = method;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

int Egg4GeneratorModel::rowCount(const QModelIndex &parent) const
{
    (void) parent;
    return model.size();
}

int Egg4GeneratorModel::columnCount(const QModelIndex &parent) const
{
    (void) parent;
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
                switch (section)
                {
                    case 0:
                        return tr("Frame");
                    case 1:
                        return tr("PID");
                    case 2:
                        return "!!!";
                    case 3:
                        return tr("Nature");
                    case 4:
                        return tr("Ability");
                    case 5:
                        return tr("Gender");
                }
            case Method::DPPtIVs:
                switch (section)
                {
                    case 0:
                        return tr("Frame");
                    case 1:
                        return tr("Chatot");
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
            case Method::HGSSIVs:
                switch (section)
                {
                    case 0:
                        return tr("Frame");
                    case 1:
                        return tr("Call");
                    case 2:
                        return tr("Chatot");
                    case 3:
                        return tr("HP");
                    case 4:
                        return tr("Atk");
                    case 5:
                        return tr("Def");
                    case 6:
                        return tr("SpA");
                    case 7:
                        return tr("SpD");
                    case 8:
                        return tr("Spe");
                    case 9:
                        return tr("Hidden");
                    case 10:
                        return tr("Power");
                }
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


Egg4SearcherModel::Egg4SearcherModel(QObject *parent, Method method)
    : QAbstractTableModel(parent)
{
    this->method = method;
    showInheritance = false;
}

void Egg4SearcherModel::setModel(const QVector<Frame4> &frames)
{
    if (frames.isEmpty())
    {
        return;
    }

    int i = rowCount();
    emit beginInsertRows(QModelIndex(), i, i + frames.size() - 1);
    model.append(frames);
    emit endInsertRows();
}

void Egg4SearcherModel::addItems(const QVector<Frame4> &frames)
{
    if (frames.isEmpty())
    {
        return;
    }

    int i = rowCount();
    emit beginInsertRows(QModelIndex(), i, i + frames.size() - 1);
    model.append(frames);
    emit endInsertRows();
}

void Egg4SearcherModel::clear()
{
    if (model.isEmpty())
    {
        return;
    }

    emit beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    model.clear();
    model.squeeze();
    emit endRemoveRows();
}

void Egg4SearcherModel::setMethod(Method method)
{
    this->method = method;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

int Egg4SearcherModel::rowCount(const QModelIndex &parent) const
{
    (void) parent;
    return model.size();
}

int Egg4SearcherModel::columnCount(const QModelIndex &parent) const
{
    (void) parent;
    switch (method)
    {
        case Method::Gen4Normal:
        case Method::Gen4Masuada:
            return 7;
        case Method::DPPtIVs:
        case Method::HGSSIVs:
            return 10;
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
                        return tr("Gender");
                }
            case Method::DPPtIVs:
            case Method::HGSSIVs:
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
