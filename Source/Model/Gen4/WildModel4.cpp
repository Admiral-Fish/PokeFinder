/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "WildModel4.hpp"
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Util/Translator.hpp>
#include <Core/Util/Utilities.hpp>

WildGeneratorModel4::WildGeneratorModel4(QObject *parent, Method method) :
    TableModel<WildGeneratorState4>(parent), showStats(false), method(method)
{
}

int WildGeneratorModel4::columnCount(const QModelIndex &parent) const
{
    switch (method)
    {
    case Method::MethodJ:
    case Method::PokeRadar:
        return 19;
    case Method::PokeRadarShiny:
    case Method::MethodK:
        return 20;
    default:
        return 0;
    }
}

QVariant WildGeneratorModel4::data(const QModelIndex &index, int role) const
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
            return state.getOccidentary();
        case 2:
            return QString::fromStdString(*Translator::getItem(state.getItem()));
        case 3:
            return state.getRadarShinyPatch();
        case 4:
            return state.getCall() == 0 ? "E" : state.getCall() == 1 ? "K" : "P";
        case 5:
            return QString::fromStdString(Utilities4::getChatot(state.getChatot()));
        case 6:
            return QString("%1 (%2)").arg(state.getEncounterSlot()).arg(QString::fromStdString(*Translator::getSpecie(state.getSpecie())));
        case 7:
            return state.getLevel();
        case 8:
            return QString::number(state.getPID(), 16).toUpper().rightJustified(8, '0');
        case 9:
        {
            u8 shiny = state.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 10:
            return QString::fromStdString(*Translator::getNature(state.getNature()));
        case 11:
            return QString("%1 (%2)").arg(state.getAbility()).arg(QString::fromStdString(*Translator::getAbility(state.getAbilityIndex())));
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
            return showStats ? state.getStat(column - 12) : state.getIV(column - 12);
        case 18:
            return QString::fromStdString(*Translator::getHiddenPower(state.getHiddenPower()));
        case 19:
            return state.getHiddenPowerStrength();
        case 20:
            return QString::fromStdString(*Translator::getGender(state.getGender()));
        }
    }
    return QVariant();
}

QVariant WildGeneratorModel4::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        section = getColumn(section);

        return header[section];
    }
    return QVariant();
}

void WildGeneratorModel4::setMethod(Method method)
{
    this->method = method;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

void WildGeneratorModel4::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, 11), index(rowCount(), 16), { Qt::DisplayRole });
}

int WildGeneratorModel4::getColumn(int column) const
{
    switch (method)
    {
    case Method::MethodJ:
    case Method::PokeRadar:
        return column > 2 ? column + 2 : column;
    case Method::PokeRadarShiny:
        return column > 3 ? column + 1 : column;
    case Method::MethodK:
        return column > 2 ? column + 1 : column;
    default:
        return column;
    }
}

WildSearcherModel4::WildSearcherModel4(QObject *parent) : TableModel<WildSearcherState4>(parent), showStats(false)
{
}

int WildSearcherModel4::columnCount(const QModelIndex &parent) const
{
    return 18;
}

QVariant WildSearcherModel4::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &state = model[index.row()];
        int column = index.column();
        switch (column)
        {
        case 0:
            return QString::number(state.getSeed(), 16).toUpper().rightJustified(8, '0');
        case 1:
            return state.getAdvances();
        case 2:
            return QString::fromStdString(*Translator::getItem(state.getItem()));
        case 3:
            return QString("%1 (%2)").arg(state.getEncounterSlot()).arg(QString::fromStdString(*Translator::getSpecie(state.getSpecie())));
        case 4:
            return state.getLevel();
        case 5:
            return QString::number(state.getPID(), 16).toUpper().rightJustified(8, '0');
        case 6:
        {
            u8 shiny = state.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 7:
            return QString::fromStdString(*Translator::getNature(state.getNature()));
        case 8:
            return QString("%1 (%2)").arg(state.getAbility()).arg(QString::fromStdString(*Translator::getAbility(state.getAbilityIndex())));
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
            return showStats ? state.getStat(column - 9) : state.getIV(column - 9);
        case 15:
            return QString::fromStdString(*Translator::getHiddenPower(state.getHiddenPower()));
        case 16:
            return state.getHiddenPowerStrength();
        case 17:
            return QString::fromStdString(*Translator::getGender(state.getGender()));
        }
    }
    return QVariant();
}

QVariant WildSearcherModel4::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

void WildSearcherModel4::sort(int column, Qt::SortOrder order)
{
    if (!model.empty())
    {
        emit layoutAboutToBeChanged();
        bool flag = order == Qt::AscendingOrder;
        switch (column)
        {
        case 0:
            std::sort(model.begin(), model.end(), [flag](const WildSearcherState4 &state1, const WildSearcherState4 &state2) {
                return flag ? state1.getSeed() < state2.getSeed() : state1.getSeed() > state2.getSeed();
            });
            break;
        case 1:
            std::sort(model.begin(), model.end(), [flag](const WildSearcherState4 &state1, const WildSearcherState4 &state2) {
                return flag ? state1.getAdvances() < state2.getAdvances() : state1.getAdvances() > state2.getAdvances();
            });
            break;
        case 2:
            std::sort(model.begin(), model.end(), [flag](const WildSearcherState4 &state1, const WildSearcherState4 &state2) {
                return flag ? state1.getItem() > state2.getItem() : state1.getItem() < state2.getItem();
            });
            break;
        case 3:
            std::sort(model.begin(), model.end(), [flag](const WildSearcherState4 &state1, const WildSearcherState4 &state2) {
                return flag ? state1.getEncounterSlot() < state2.getEncounterSlot() : state1.getEncounterSlot() > state2.getEncounterSlot();
            });
            break;
        case 4:
            std::sort(model.begin(), model.end(), [flag](const WildSearcherState4 &state1, const WildSearcherState4 &state2) {
                return flag ? state1.getLevel() < state2.getLevel() : state1.getLevel() > state2.getLevel();
            });
            break;
        case 5:
            std::sort(model.begin(), model.end(), [flag](const WildSearcherState4 &state1, const WildSearcherState4 &state2) {
                return flag ? state1.getPID() < state2.getPID() : state1.getPID() > state2.getPID();
            });
            break;
        case 6:
            std::sort(model.begin(), model.end(), [flag](const WildSearcherState4 &state1, const WildSearcherState4 &state2) {
                return flag ? state1.getShiny() < state2.getShiny() : state1.getShiny() > state2.getShiny();
            });
            break;
        case 7:
            std::sort(model.begin(), model.end(), [flag](const WildSearcherState4 &state1, const WildSearcherState4 &state2) {
                return flag ? state1.getNature() < state2.getNature() : state1.getNature() > state2.getNature();
            });
            break;
        case 8:
            std::sort(model.begin(), model.end(), [flag](const WildSearcherState4 &state1, const WildSearcherState4 &state2) {
                return flag ? state1.getAbility() < state2.getAbility() : state1.getAbility() > state2.getAbility();
            });
            break;
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
            std::sort(model.begin(), model.end(), [flag, column](const WildSearcherState4 &state1, const WildSearcherState4 &state2) {
                return flag ? state1.getIV(column - 9) < state2.getIV(column - 9) : state1.getIV(column - 9) > state2.getIV(column - 9);
            });
            break;
        case 15:
            std::sort(model.begin(), model.end(), [flag](const WildSearcherState4 &state1, const WildSearcherState4 &state2) {
                return flag ? state1.getHiddenPower() < state2.getHiddenPower() : state1.getHiddenPower() > state2.getHiddenPower();
            });
            break;
        case 16:
            std::sort(model.begin(), model.end(), [flag](const WildSearcherState4 &state1, const WildSearcherState4 &state2) {
                return flag ? state1.getHiddenPowerStrength() < state2.getHiddenPowerStrength()
                            : state1.getHiddenPowerStrength() > state2.getHiddenPowerStrength();
            });
            break;
        case 17:
            std::sort(model.begin(), model.end(), [flag](const WildSearcherState4 &state1, const WildSearcherState4 &state2) {
                return flag ? state1.getGender() < state2.getGender() : state1.getGender() > state2.getGender();
            });
            break;
        }

        emit layoutChanged();
    }
}

void WildSearcherModel4::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, 9), index(rowCount(), 14), { Qt::DisplayRole });
}
