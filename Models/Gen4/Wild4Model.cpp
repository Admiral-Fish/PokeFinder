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
#include <Core/Util/TableUtility.hpp>

namespace PokeFinderModels
{
    Wild4Model::Wild4Model(QObject *parent, PokeFinderCore::Method method)
        : TableModel<PokeFinderCore::Frame4>(parent)
    {
        this->method = method;
    }

    void Wild4Model::setMethod(PokeFinderCore::Method method)
    {
        this->method = method;
        emit headerDataChanged(Qt::Horizontal, 0, columnCount());
    }

    int Wild4Model::columnCount(const QModelIndex & /*parent*/) const
    {
        switch (method)
        {
        case PokeFinderCore::Method::MethodJ:
            return 18;
        case PokeFinderCore::Method::MethodK:
            return 19;
        case PokeFinderCore::Method::ChainedShiny:
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
            case PokeFinderCore::Method::MethodJ:
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
            case PokeFinderCore::Method::MethodK:
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
            case PokeFinderCore::Method::ChainedShiny:
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
            case PokeFinderCore::Method::MethodJ:
                switch (index.column())
                {
                case 9:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(0));
                case 10:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(1));
                case 11:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(2));
                case 12:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(3));
                case 13:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(4));
                case 14:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(5));
                }
                break;
            case PokeFinderCore::Method::MethodK:
                switch (index.column())
                {
                case 10:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(0));
                case 11:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(1));
                case 12:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(2));
                case 13:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(3));
                case 14:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(4));
                case 15:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(5));
                }
            case PokeFinderCore::Method::ChainedShiny:
                switch (index.column())
                {
                case 6:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(0));
                case 7:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(1));
                case 8:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(2));
                case 9:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(3));
                case 10:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(4));
                case 11:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(5));
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
            case PokeFinderCore::Method::MethodJ:
                return header1.at(section);
            case PokeFinderCore::Method::MethodK:
                return header2.at(section);
            case PokeFinderCore::Method::ChainedShiny:
                return header3.at(section);
            default:
                break;
            }
        }
        return QVariant();
    }

}
