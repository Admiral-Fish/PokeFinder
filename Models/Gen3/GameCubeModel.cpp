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
        const auto &currentState = model.at(index.row());
        int column = index.column();
        switch (column)
        {
        case 0:
            return currentState.getAdvance();
        case 1:
            return QString::number(currentState.getPID(), 16).toUpper().rightJustified(8, '0');
        case 2:
        {
            u8 shiny = currentState.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 3:
            return Translator::getNature(currentState.getNature());
        case 4:
            return currentState.getAbility();
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            return currentState.getIV(static_cast<u8>(column - 5));
        case 11:
            return Translator::getHiddenPower(currentState.getHidden());
        case 12:
            return currentState.getPower();
        case 13:
            return Translator::getGender(currentState.getGender());
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
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &currentState1, const GameCubeState &currentState2) {
                return flag ? currentState1.getSeed() < currentState2.getSeed() : currentState1.getSeed() > currentState2.getSeed();
            });
            break;
        case 1:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &currentState1, const GameCubeState &currentState2) {
                return flag ? currentState1.getPID() < currentState2.getPID() : currentState1.getPID() > currentState2.getPID();
            });
            break;
        case 2:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &currentState1, const GameCubeState &currentState2) {
                return flag ? currentState1.getShiny() < currentState2.getShiny() : currentState1.getShiny() > currentState2.getShiny();
            });
            break;
        case 3:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &currentState1, const GameCubeState &currentState2) {
                return flag ? currentState1.getNature() < currentState2.getNature() : currentState1.getNature() > currentState2.getNature();
            });
            break;
        case 4:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &currentState1, const GameCubeState &currentState2) {
                return flag ? currentState1.getAbility() < currentState2.getAbility()
                            : currentState1.getAbility() > currentState2.getAbility();
            });
            break;
        case 5:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &currentState1, const GameCubeState &currentState2) {
                return flag ? currentState1.getIV(0) < currentState2.getIV(0) : currentState1.getIV(0) > currentState2.getIV(0);
            });
            break;
        case 6:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &currentState1, const GameCubeState &currentState2) {
                return flag ? currentState1.getIV(1) < currentState2.getIV(1) : currentState1.getIV(1) > currentState2.getIV(1);
            });
            break;
        case 7:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &currentState1, const GameCubeState &currentState2) {
                return flag ? currentState1.getIV(2) < currentState2.getIV(2) : currentState1.getIV(2) > currentState2.getIV(2);
            });
            break;
        case 8:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &currentState1, const GameCubeState &currentState2) {
                return flag ? currentState1.getIV(3) < currentState2.getIV(3) : currentState1.getIV(3) > currentState2.getIV(3);
            });
            break;
        case 9:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &currentState1, const GameCubeState &currentState2) {
                return flag ? currentState1.getIV(4) < currentState2.getIV(4) : currentState1.getIV(4) > currentState2.getIV(4);
            });
            break;
        case 10:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &currentState1, const GameCubeState &currentState2) {
                return flag ? currentState1.getIV(5) < currentState2.getIV(5) : currentState1.getIV(5) > currentState2.getIV(5);
            });
            break;
        case 11:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &currentState1, const GameCubeState &currentState2) {
                return flag ? currentState1.getHidden() < currentState2.getHidden() : currentState1.getHidden() > currentState2.getHidden();
            });
            break;
        case 12:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &currentState1, const GameCubeState &currentState2) {
                return flag ? currentState1.getPower() < currentState2.getPower() : currentState1.getPower() > currentState2.getPower();
            });
            break;
        case 13:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &currentState1, const GameCubeState &currentState2) {
                return flag ? currentState1.getGender() < currentState2.getGender() : currentState1.getGender() > currentState2.getGender();
            });
            break;
        case 14:
            std::sort(model.begin(), model.end(), [flag](const GameCubeState &currentState1, const GameCubeState &currentState2) {
                return flag ? currentState1.getInfo() < currentState2.getInfo() : currentState1.getInfo() > currentState2.getInfo();
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
        const auto &currentState = model.at(index.row());
        switch (index.column())
        {
        case 0:
            return QString::number(currentState.getSeed(), 16).toUpper().rightJustified(8, '0');
        case 1:
            return QString::number(currentState.getPID(), 16).toUpper().rightJustified(8, '0');
        case 2:
        {
            u8 shiny = currentState.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 3:
            return Translator::getNature(currentState.getNature());
        case 4:
            return currentState.getAbility();
        case 5:
            return currentState.getIV(0);
        case 6:
            return currentState.getIV(1);
        case 7:
            return currentState.getIV(2);
        case 8:
            return currentState.getIV(3);
        case 9:
            return currentState.getIV(4);
        case 10:
            return currentState.getIV(5);
        case 11:
            return Translator::getHiddenPower(currentState.getHidden());
        case 12:
            return currentState.getPower();
        case 13:
            return Translator::getGender(currentState.getGender());
        case 14:
            switch (currentState.getInfo())
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
