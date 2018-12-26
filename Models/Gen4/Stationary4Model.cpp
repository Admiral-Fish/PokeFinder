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

#include "Stationary4Model.hpp"

Stationary4Model::Stationary4Model(QObject *parent, Method method) : QAbstractTableModel(parent)
{
    this->method = method;
}

void Stationary4Model::setModel(const QVector<Frame4> &frames)
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

void Stationary4Model::clear()
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

void Stationary4Model::setMethod(Method method)
{
    this->method = method;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

int Stationary4Model::rowCount(const QModelIndex &parent) const
{
    (void) parent;
    return model.size();
}

int Stationary4Model::columnCount(const QModelIndex &parent) const
{
    (void) parent;
    switch (method)
    {
        case Method::Method1:
        case Method::MethodJ:
            return 16;
        case Method::MethodK:
            return 17;
        case Method::WondercardIVs:
            return 11;
        default:
            return 0;
    }
}

QVariant Stationary4Model::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        auto frame = model.at(index.row());
        switch (method)
        {
            case Method::Method1:
                switch (index.column())
                {
                    case 0:
                        return frame.getFrame();
                    case 1:
                        return frame.getCall();
                    case 2:
                        return frame.chatotPitch();
                    case 3:
                        return QString::number(frame.getPID(), 16).toUpper().rightJustified(8, '0');
                    case 4:
                        return frame.getShinyString();
                    case 5:
                        return frame.getNatureString();
                    case 6:
                        return frame.getAbility();
                    case 7:
                        return frame.getIV(0);
                    case 8:
                        return frame.getIV(1);
                    case 9:
                        return frame.getIV(2);
                    case 10:
                        return frame.getIV(3);
                    case 11:
                        return frame.getIV(4);
                    case 12:
                        return frame.getIV(5);
                    case 13:
                        return frame.getPowerString();
                    case 14:
                        return frame.getPower();
                    case 15:
                        return frame.getGenderString();
                }
            case Method::MethodJ:
                switch (index.column())
                {
                    case 0:
                        return frame.getFrame();
                    case 1:
                        return frame.getOccidentary();
                    case 2:
                        return frame.chatotPitch();
                    case 3:
                        return QString::number(frame.getPID(), 16).toUpper().rightJustified(8, '0');
                    case 4:
                        return frame.getShinyString();
                    case 5:
                        return frame.getNatureString();
                    case 6:
                        return frame.getAbility();
                    case 7:
                        return frame.getIV(0);
                    case 8:
                        return frame.getIV(1);
                    case 9:
                        return frame.getIV(2);
                    case 10:
                        return frame.getIV(3);
                    case 11:
                        return frame.getIV(4);
                    case 12:
                        return frame.getIV(5);
                    case 13:
                        return frame.getPowerString();
                    case 14:
                        return frame.getPower();
                    case 15:
                        return frame.getGenderString();
                }
            case Method::MethodK:
                switch (index.column())
                {
                    case 0:
                        return frame.getFrame();
                    case 1:
                        return frame.getOccidentary();
                    case 2:
                        return frame.getCall();
                    case 3:
                        return frame.chatotPitch();
                    case 4:
                        return QString::number(frame.getPID(), 16).toUpper().rightJustified(8, '0');
                    case 5:
                        return frame.getShinyString();
                    case 6:
                        return frame.getNatureString();
                    case 7:
                        return frame.getAbility();
                    case 8:
                        return frame.getIV(0);
                    case 9:
                        return frame.getIV(1);
                    case 10:
                        return frame.getIV(2);
                    case 11:
                        return frame.getIV(3);
                    case 12:
                        return frame.getIV(4);
                    case 13:
                        return frame.getIV(5);
                    case 14:
                        return frame.getPowerString();
                    case 15:
                        return frame.getPower();
                    case 16:
                        return frame.getGenderString();
                }
            case Method::WondercardIVs:
                switch (index.column())
                {
                    case 0:
                        return frame.getFrame();
                    case 1:
                        return frame.getCall();
                    case 2:
                        return frame.chatotPitch();
                    case 3:
                        return frame.getIV(0);
                    case 4:
                        return frame.getIV(1);
                    case 5:
                        return frame.getIV(2);
                    case 6:
                        return frame.getIV(3);
                    case 7:
                        return frame.getIV(4);
                    case 8:
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
            case Method::Method1:
            case Method::MethodJ:
                switch (index.column())
                {
                    case 7:
                        return TableUtility::getBold(frame.getIV(0));
                    case 8:
                        return TableUtility::getBold(frame.getIV(1));
                    case 9:
                        return TableUtility::getBold(frame.getIV(2));
                    case 10:
                        return TableUtility::getBold(frame.getIV(3));
                    case 11:
                        return TableUtility::getBold(frame.getIV(4));
                    case 12:
                        return TableUtility::getBold(frame.getIV(5));
                }
                break;
            case Method::MethodK:
                switch (index.column())
                {
                    case 8:
                        return TableUtility::getBold(frame.getIV(0));
                    case 9:
                        return TableUtility::getBold(frame.getIV(1));
                    case 10:
                        return TableUtility::getBold(frame.getIV(2));
                    case 11:
                        return TableUtility::getBold(frame.getIV(3));
                    case 12:
                        return TableUtility::getBold(frame.getIV(4));
                    case 13:
                        return TableUtility::getBold(frame.getIV(5));
                }
                break;
            case Method::WondercardIVs:
                switch (index.column())
                {
                    case 3:
                        return TableUtility::getBold(frame.getIV(0));
                    case 4:
                        return TableUtility::getBold(frame.getIV(1));
                    case 5:
                        return TableUtility::getBold(frame.getIV(2));
                    case 6:
                        return TableUtility::getBold(frame.getIV(3));
                    case 7:
                        return TableUtility::getBold(frame.getIV(4));
                    case 8:
                        return TableUtility::getBold(frame.getIV(5));
                }
            default:
                break;
        }
    }
    return QVariant();
}

QVariant Stationary4Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (method)
        {
            case Method::Method1:
                switch (section)
                {
                    case 0:
                        return tr("Frame");
                    case 1:
                        return tr("Call");
                    case 2:
                        return tr("Chatot");
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
            case Method::MethodJ:
                switch (section)
                {
                    case 0:
                        return tr("Frame");
                    case 1:
                        return tr("Occidentary");
                    case 2:
                        return tr("Chatot");
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
            case Method::MethodK:
                switch (section)
                {
                    case 0:
                        return tr("Frame");
                    case 1:
                        return tr("Occidentary");
                    case 2:
                        return tr("Call");
                    case 3:
                        return tr("Chatot");
                    case 4:
                        return tr("PID");
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
            case Method::WondercardIVs:
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
