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

#include "GameCubeModel.hpp"
#include <Core/Util/Translator.hpp>

GameCubeGeneratorModel::GameCubeGeneratorModel(QObject *parent) : TableModel<GameCubeState>(parent)
{
}

int GameCubeGeneratorModel::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return 14;
}

QVariant GameCubeGeneratorModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &state = model.at(index.row());
        int column = index.column();
        switch (column)
        {
        case 0:
            return state.getAdvances();
        case 1:
            return QString::number(state.getPID(), 16).toUpper().rightJustified(8, '0');
        case 2:
        {
            u8 shiny = state.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 3:
            return Translator::getNature(state.getNature());
        case 4:
            return state.getAbility();
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            return state.getIV(static_cast<u8>(column - 5));
        case 11:
            return Translator::getHiddenPower(state.getHidden());
        case 12:
            return state.getPower();
        case 13:
            return Translator::getGender(state.getGender());
        }
    }
    return QVariant();
}

QVariant GameCubeGeneratorModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header.at(section);
    }
    return QVariant();
}

GameCubeSearcherModel::GameCubeSearcherModel(QObject *parent, Method type) : TableModel<GameCubeState>(parent), type(type)
{
}

void GameCubeSearcherModel::setMethod(Method type)
{
    this->type = type;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

void GameCubeSearcherModel::sort(int column, Qt::SortOrder order)
{
    if (!model.isEmpty())
    {
        emit layoutAboutToBeChanged();
        bool flag = order == Qt::AscendingOrder;
        switch (column)
        {
        case 0:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &state1, const GameCubeState &state2) {
                return flag ? state1.getSeed() < state2.getSeed() : state1.getSeed() > state2.getSeed();
            });
            break;
        case 1:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &state1, const GameCubeState &state2) {
                return flag ? state1.getPID() < state2.getPID() : state1.getPID() > state2.getPID();
            });
            break;
        case 2:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &state1, const GameCubeState &state2) {
                return flag ? state1.getShiny() < state2.getShiny() : state1.getShiny() > state2.getShiny();
            });
            break;
        case 3:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &state1, const GameCubeState &state2) {
                return flag ? state1.getNature() < state2.getNature() : state1.getNature() > state2.getNature();
            });
            break;
        case 4:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &state1, const GameCubeState &state2) {
                return flag ? state1.getAbility() < state2.getAbility() : state1.getAbility() > state2.getAbility();
            });
            break;
        case 5:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &state1, const GameCubeState &state2) {
                return flag ? state1.getIV(0) < state2.getIV(0) : state1.getIV(0) > state2.getIV(0);
            });
            break;
        case 6:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &state1, const GameCubeState &state2) {
                return flag ? state1.getIV(1) < state2.getIV(1) : state1.getIV(1) > state2.getIV(1);
            });
            break;
        case 7:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &state1, const GameCubeState &state2) {
                return flag ? state1.getIV(2) < state2.getIV(2) : state1.getIV(2) > state2.getIV(2);
            });
            break;
        case 8:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &state1, const GameCubeState &state2) {
                return flag ? state1.getIV(3) < state2.getIV(3) : state1.getIV(3) > state2.getIV(3);
            });
            break;
        case 9:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &state1, const GameCubeState &state2) {
                return flag ? state1.getIV(4) < state2.getIV(4) : state1.getIV(4) > state2.getIV(4);
            });
            break;
        case 10:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &state1, const GameCubeState &state2) {
                return flag ? state1.getIV(5) < state2.getIV(5) : state1.getIV(5) > state2.getIV(5);
            });
            break;
        case 11:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &state1, const GameCubeState &state2) {
                return flag ? state1.getHidden() < state2.getHidden() : state1.getHidden() > state2.getHidden();
            });
            break;
        case 12:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &state1, const GameCubeState &state2) {
                return flag ? state1.getPower() < state2.getPower() : state1.getPower() > state2.getPower();
            });
            break;
        case 13:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &state1, const GameCubeState &state2) {
                return flag ? state1.getGender() < state2.getGender() : state1.getGender() > state2.getGender();
            });
            break;
        case 14:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &state1, const GameCubeState &state2) {
                return flag ? state1.getInfo() < state2.getInfo() : state1.getInfo() > state2.getInfo();
            });
            break;
        }

        emit layoutChanged();
    }
}

int GameCubeSearcherModel::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    if (type == Method::XD || type == Method::Colo)
    {
        return 15;
    }
    return 14;
}

QVariant GameCubeSearcherModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &state = model.at(index.row());
        switch (index.column())
        {
        case 0:
            return QString::number(state.getSeed(), 16).toUpper().rightJustified(8, '0');
        case 1:
            return QString::number(state.getPID(), 16).toUpper().rightJustified(8, '0');
        case 2:
        {
            u8 shiny = state.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 3:
            return Translator::getNature(state.getNature());
        case 4:
            return state.getAbility();
        case 5:
            return state.getIV(0);
        case 6:
            return state.getIV(1);
        case 7:
            return state.getIV(2);
        case 8:
            return state.getIV(3);
        case 9:
            return state.getIV(4);
        case 10:
            return state.getIV(5);
        case 11:
            return Translator::getHiddenPower(state.getHidden());
        case 12:
            return state.getPower();
        case 13:
            return Translator::getGender(state.getGender());
        case 14:
            switch (state.getInfo())
            {
            case 0:
                return tr("Pass NL");
            case 1:
                return tr("First shadow unset");
            case 2:
                return tr("First shadow set");
            }
        }
    }
    return QVariant();
}

QVariant GameCubeSearcherModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header.at(section);
    }
    return QVariant();
}
