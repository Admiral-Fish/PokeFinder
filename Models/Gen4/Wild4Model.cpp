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

#include "Wild4Model.hpp"

Wild4Model::Wild4Model(QObject *parent, Method method) : QAbstractTableModel(parent)
{
    this->method = method;
}

void Wild4Model::setModel(const QVector<Frame4> &frames)
{
    if (!frames.isEmpty())
    {
        int i = rowCount();
        emit beginInsertRows(QModelIndex(), i, i + frames.size() - 1);
        model.append(frames);
        emit endInsertRows();
    }
}

void Wild4Model::clear()
{
    if (!model.isEmpty())
    {
        emit beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
        model.clear();
        model.squeeze();
        emit endRemoveRows();
    }
}

void Wild4Model::setMethod(Method method)
{
    this->method = method;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

int Wild4Model::rowCount(const QModelIndex &parent) const
{
    (void) parent;
    return model.size();
}

int Wild4Model::columnCount(const QModelIndex &parent) const
{
    (void) parent;
    switch (method)
    {
        case Method::MethodJ:
            return 18;
        case Method::MethodK:
            return 19;
        case Method::ChainedShiny:
            return 15;
        default:
            return 0;
    }
}

QVariant Wild4Model::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        auto frame = model.at(index.row());
        switch (method)
        {
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
                        return frame.getEncounterSlot();
                    case 4:
                        return frame.getLevel();
                    case 5:
                        return QString::number(frame.getPID(), 16).toUpper().rightJustified(8, '0');
                    case 6:
                        return frame.getShinyString();
                    case 7:
                        return frame.getNatureString();
                    case 8:
                        return frame.getAbility();
                    case 9:
                        return frame.getIV(0);
                    case 10:
                        return frame.getIV(1);
                    case 11:
                        return frame.getIV(2);
                    case 12:
                        return frame.getIV(3);
                    case 13:
                        return frame.getIV(4);
                    case 14:
                        return frame.getIV(5);
                    case 15:
                        return frame.getPowerString();
                    case 16:
                        return frame.getPower();
                    case 17:
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
                        return frame.getEncounterSlot();
                    case 5:
                        return frame.getLevel();
                    case 6:
                        return QString::number(frame.getPID(), 16).toUpper().rightJustified(8, '0');
                    case 7:
                        return frame.getShinyString();
                    case 8:
                        return frame.getNatureString();
                    case 9:
                        return frame.getAbility();
                    case 10:
                        return frame.getIV(0);
                    case 11:
                        return frame.getIV(1);
                    case 12:
                        return frame.getIV(2);
                    case 13:
                        return frame.getIV(3);
                    case 14:
                        return frame.getIV(4);
                    case 15:
                        return frame.getIV(5);
                    case 16:
                        return frame.getPowerString();
                    case 17:
                        return frame.getPower();
                    case 18:
                        return frame.getGenderString();
                }
            case Method::ChainedShiny:
                switch (index.column())
                {
                    case 0:
                        return frame.getFrame();
                    case 1:
                        return frame.chatotPitch();
                    case 2:
                        return QString::number(frame.getPID(), 16).toUpper().rightJustified(8, '0');
                    case 3:
                        return frame.getShinyString();
                    case 4:
                        return frame.getNatureString();
                    case 5:
                        return frame.getAbility();
                    case 6:
                        return frame.getIV(0);
                    case 7:
                        return frame.getIV(1);
                    case 8:
                        return frame.getIV(2);
                    case 9:
                        return frame.getIV(3);
                    case 10:
                        return frame.getIV(4);
                    case 11:
                        return frame.getIV(5);
                    case 12:
                        return frame.getPowerString();
                    case 13:
                        return frame.getPower();
                    case 14:
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
            case Method::MethodJ:
                switch (index.column())
                {
                    case 9:
                        return TableUtility::getBold(frame.getIV(0));
                    case 10:
                        return TableUtility::getBold(frame.getIV(1));
                    case 11:
                        return TableUtility::getBold(frame.getIV(2));
                    case 12:
                        return TableUtility::getBold(frame.getIV(3));
                    case 13:
                        return TableUtility::getBold(frame.getIV(4));
                    case 14:
                        return TableUtility::getBold(frame.getIV(5));
                }
                break;
                break;
            case Method::MethodK:
                switch (index.column())
                {
                    case 10:
                        return TableUtility::getBold(frame.getIV(0));
                    case 11:
                        return TableUtility::getBold(frame.getIV(1));
                    case 12:
                        return TableUtility::getBold(frame.getIV(2));
                    case 13:
                        return TableUtility::getBold(frame.getIV(3));
                    case 14:
                        return TableUtility::getBold(frame.getIV(4));
                    case 15:
                        return TableUtility::getBold(frame.getIV(5));
                }
            case Method::ChainedShiny:
                switch (index.column())
                {
                    case 6:
                        return TableUtility::getBold(frame.getIV(0));
                    case 7:
                        return TableUtility::getBold(frame.getIV(1));
                    case 8:
                        return TableUtility::getBold(frame.getIV(2));
                    case 9:
                        return TableUtility::getBold(frame.getIV(3));
                    case 10:
                        return TableUtility::getBold(frame.getIV(4));
                    case 11:
                        return TableUtility::getBold(frame.getIV(5));
                }
            default:
                break;
        }
    }
    return QVariant();
}

QVariant Wild4Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (method)
        {
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
                        return tr("Slot");
                    case 4:
                        return tr("Level");
                    case 5:
                        return tr("PID");
                    case 6:
                        return "!!!";
                    case 7:
                        return tr("Nature");
                    case 8:
                        return tr("Ability");
                    case 9:
                        return tr("HP");
                    case 10:
                        return tr("Atk");
                    case 11:
                        return tr("Def");
                    case 12:
                        return tr("SpA");
                    case 13:
                        return tr("SpD");
                    case 14:
                        return tr("Spe");
                    case 15:
                        return tr("Hidden");
                    case 16:
                        return tr("Power");
                    case 17:
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
                        return tr("Slot");
                    case 5:
                        return tr("Level");
                    case 6:
                        return tr("PID");
                    case 7:
                        return "!!!";
                    case 8:
                        return tr("Nature");
                    case 9:
                        return tr("Ability");
                    case 10:
                        return tr("HP");
                    case 11:
                        return tr("Atk");
                    case 12:
                        return tr("Def");
                    case 13:
                        return tr("SpA");
                    case 14:
                        return tr("SpD");
                    case 15:
                        return tr("Spe");
                    case 16:
                        return tr("Hidden");
                    case 17:
                        return tr("Power");
                    case 18:
                        return tr("Gender");
                }
            case Method::ChainedShiny:
                switch (section)
                {
                    case 0:
                        return tr("Frame");
                    case 1:
                        return tr("Chatot");
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
            default:
                break;
        }
    }
    return QVariant();
}
