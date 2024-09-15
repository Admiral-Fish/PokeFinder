/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "EggModel4.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Util/Translator.hpp>
#include <Core/Util/Utilities.hpp>

EggGeneratorModel4::EggGeneratorModel4(QObject *parent, Game version) :
    TableModel(parent), dppt((version & Game::DPPt) != Game::None), showInheritance(false), showStats(false)
{
}

int EggGeneratorModel4::columnCount(const QModelIndex &parent) const
{
    if (dppt)
    {
        return 17;
    }
    else
    {
        return 18;
    }
}

QVariant EggGeneratorModel4::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &state = model[index.row()];
        int column = getColumn(index.column());
        switch (column)
        {
        case 0:
            return state.getAdvances();
        case 1:
            return state.getPickupAdvances();
        case 2:
            return state.getCall() == 0 ? "E" : state.getCall() == 1 ? "K" : "P";
        case 3:
            return QString::fromStdString(Utilities4::getChatot(state.getChatot()));
        case 4:
            return QString::number(state.getPID(), 16).toUpper().rightJustified(8, '0');
        case 5:
        {
            u8 shiny = state.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 6:
            return QString::fromStdString(Translator::getNature(state.getNature()));
        case 7:
            return QString("%1: %2").arg(state.getAbility()).arg(QString::fromStdString(Translator::getAbility(state.getAbilityIndex())));
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            if (showInheritance)
            {
                u8 inh = state.getInheritance(column - 8);
                if (inh)
                {
                    return inh == 1 ? "A" : "B";
                }
            }
            return showStats ? state.getStat(column - 8) : state.getIV(column - 8);
        case 14:
            return QString::fromStdString(Translator::getHiddenPower(state.getHiddenPower()));
        case 15:
            return state.getHiddenPowerStrength();
        case 16:
            return QString::fromStdString(Translator::getGender(state.getGender()));
        case 17:
            return QString::fromStdString(Translator::getCharacteristic(state.getCharacteristic()));
        }
    }

    return QVariant();
}

QVariant EggGeneratorModel4::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        section = getColumn(section);

        return header[section];
    }
    return QVariant();
}

void EggGeneratorModel4::setVersion(Game version)
{
    dppt = (version & Game::DPPt) != Game::None;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

void EggGeneratorModel4::setShowInheritance(bool flag)
{
    showInheritance = flag;
    emit dataChanged(index(0, 7), index(rowCount(), 12), { Qt::DisplayRole });
}

void EggGeneratorModel4::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, 7), index(rowCount(), 12), { Qt::DisplayRole });
}

int EggGeneratorModel4::getColumn(int column) const
{
    if (dppt)
    {
        return column > 1 ? column + 1 : column;
    }
    else
    {
        return column;
    }
}

EggSearcherModel4::EggSearcherModel4(QObject *parent) : TableModel(parent), showInheritance(false), showStats(false)
{
}

int EggSearcherModel4::columnCount(const QModelIndex &parent) const
{
    return 16;
}

QVariant EggSearcherModel4::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &display = model[index.row()];
        const auto &state = display.getState();
        int column = index.column();
        switch (column)
        {
        case 0:
            return QString::number(display.getSeed(), 16).toUpper().rightJustified(8, '0');
        case 1:
            return state.getAdvances();
        case 2:
            return state.getPickupAdvances();
        case 3:
            return QString::number(state.getPID(), 16).toUpper().rightJustified(8, '0');
        case 4:
        {
            u8 shiny = state.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 5:
            return QString::fromStdString(Translator::getNature(state.getNature()));
        case 6:
            return QString("%1: %2").arg(state.getAbility()).arg(QString::fromStdString(Translator::getAbility(state.getAbilityIndex())));
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
            if (showInheritance)
            {
                u8 inh = state.getInheritance(column - 7);
                if (inh)
                {
                    return inh == 1 ? "A" : "B";
                }
            }
            return showStats ? state.getStat(column - 7) : state.getIV(column - 7);
        case 13:
            return QString::fromStdString(Translator::getHiddenPower(state.getHiddenPower()));
        case 14:
            return state.getHiddenPowerStrength();
        case 15:
            return QString::fromStdString(Translator::getGender(state.getGender()));
        case 16:
            return QString::fromStdString(Translator::getCharacteristic(state.getCharacteristic()));
        }
    }
    return QVariant();
}

QVariant EggSearcherModel4::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

void EggSearcherModel4::sort(int column, Qt::SortOrder order)
{
    if (!model.empty())
    {
        emit layoutAboutToBeChanged();
        bool flag = order == Qt::AscendingOrder;
        switch (column)
        {
        case 0:
            std::sort(model.begin(), model.end(), [flag](const EggSearcherState4 &state1, const EggSearcherState4 &state2) {
                return flag ? state1.getSeed() < state2.getSeed() : state1.getSeed() > state2.getSeed();
            });
            break;
        case 1:
            std::sort(model.begin(), model.end(), [flag](const EggSearcherState4 &state1, const EggSearcherState4 &state2) {
                return flag ? state1.getState().getAdvances() < state2.getState().getAdvances()
                            : state1.getState().getAdvances() > state2.getState().getAdvances();
            });
            break;
        case 2:
            std::sort(model.begin(), model.end(), [flag](const EggSearcherState4 &state1, const EggSearcherState4 &state2) {
                return flag ? state1.getState().getPickupAdvances() < state2.getState().getPickupAdvances()
                            : state1.getState().getPickupAdvances() > state2.getState().getPickupAdvances();
            });
            break;
        case 3:
            std::sort(model.begin(), model.end(), [flag](const EggSearcherState4 &state1, const EggSearcherState4 &state2) {
                return flag ? state1.getState().getPID() < state2.getState().getPID()
                            : state1.getState().getPID() > state2.getState().getPID();
            });
            break;
        case 4:
            std::sort(model.begin(), model.end(), [flag](const EggSearcherState4 &state1, const EggSearcherState4 &state2) {
                return flag ? state1.getState().getShiny() < state2.getState().getShiny()
                            : state1.getState().getShiny() > state2.getState().getShiny();
            });
            break;
        case 5:
            std::sort(model.begin(), model.end(), [flag](const EggSearcherState4 &state1, const EggSearcherState4 &state2) {
                return flag ? state1.getState().getNature() < state2.getState().getNature()
                            : state1.getState().getNature() > state2.getState().getNature();
            });
            break;
        case 6:
            std::sort(model.begin(), model.end(), [flag](const EggSearcherState4 &state1, const EggSearcherState4 &state2) {
                return flag ? state1.getState().getAbility() < state2.getState().getAbility()
                            : state1.getState().getAbility() > state2.getState().getAbility();
            });
            break;
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
            std::sort(model.begin(), model.end(), [flag, column](const EggSearcherState4 &state1, const EggSearcherState4 &state2) {
                return flag ? state1.getState().getIV(column - 7) < state2.getState().getIV(column - 7)
                            : state1.getState().getIV(column - 7) > state2.getState().getIV(column - 7);
            });
            break;
        case 13:
            std::sort(model.begin(), model.end(), [flag](const EggSearcherState4 &state1, const EggSearcherState4 &state2) {
                return flag ? state1.getState().getHiddenPower() < state2.getState().getHiddenPower()
                            : state1.getState().getHiddenPower() > state2.getState().getHiddenPower();
            });
            break;
        case 14:
            std::sort(model.begin(), model.end(), [flag](const EggSearcherState4 &state1, const EggSearcherState4 &state2) {
                return flag ? state1.getState().getHiddenPowerStrength() < state2.getState().getHiddenPowerStrength()
                            : state1.getState().getHiddenPowerStrength() > state2.getState().getHiddenPowerStrength();
            });
            break;
        case 15:
            std::sort(model.begin(), model.end(), [flag](const EggSearcherState4 &state1, const EggSearcherState4 &state2) {
                return flag ? state1.getState().getGender() < state2.getState().getGender()
                            : state1.getState().getGender() > state2.getState().getGender();
            });
            break;
        case 16:
            std::sort(model.begin(), model.end(), [flag](const EggSearcherState4 &state1, const EggSearcherState4 &state2) {
                return flag ? state1.getState().getCharacteristic() < state2.getState().getCharacteristic()
                            : state1.getState().getCharacteristic() > state2.getState().getCharacteristic();
            });
            break;
        }

        emit layoutChanged();
    }
}

void EggSearcherModel4::setShowInheritance(bool flag)
{
    showInheritance = flag;
    emit dataChanged(index(0, 7), index(rowCount(), 12), { Qt::DisplayRole });
}

void EggSearcherModel4::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, 7), index(rowCount(), 12), { Qt::DisplayRole });
}
