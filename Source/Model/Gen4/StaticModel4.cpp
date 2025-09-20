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

#include "StaticModel4.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Util/Translator.hpp>
#include <Core/Util/Utilities.hpp>

StaticGeneratorModel4::StaticGeneratorModel4(QObject *parent, Method method) : TableModel(parent), showStats(false), method(method)
{
}

int StaticGeneratorModel4::columnCount(const QModelIndex &parent) const
{
    switch (method)
    {
    case Method::Method1:
    case Method::MethodK:
        return 17;
    case Method::MethodJ:
        return 16;
    default:
        return 0;
    }
}

QVariant StaticGeneratorModel4::data(const QModelIndex &index, int role) const
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
            return state.getCall() == 0 ? "E" : state.getCall() == 1 ? "K" : "P";
        case 2:
            return QString::fromStdString(Utilities4::getChatot(state.getChatot()));
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

QVariant StaticGeneratorModel4::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        section = getColumn(section);

        return header[section];
    }
    return QVariant();
}

void StaticGeneratorModel4::setMethod(Method method)
{
    this->method = method;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

void StaticGeneratorModel4::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, getColumn(7)), index(rowCount(), getColumn(12)), { Qt::DisplayRole });
}

int StaticGeneratorModel4::getColumn(int column) const
{
    switch (method)
    {
    case Method::Method1:
    case Method::MethodK:
    default:
        return column;
    case Method::MethodJ:
        return column > 0 ? column + 1 : column;
    }
}

StaticSearcherModel4::StaticSearcherModel4(QObject *parent) : TableModel(parent), showStats(false)
{
}

int StaticSearcherModel4::columnCount(const QModelIndex &parent) const
{
    return 16;
}

QVariant StaticSearcherModel4::data(const QModelIndex &index, int role) const
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
            return QString::number(state.getPID(), 16).toUpper().rightJustified(8, '0');
        case 3:
        {
            u8 shiny = state.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 4:
            return QString::fromStdString(Translator::getNature(state.getNature()));
        case 5:
            return QString("%1: %2").arg(state.getAbility()).arg(QString::fromStdString(Translator::getAbility(state.getAbilityIndex())));
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            return showStats ? state.getStat(column - 6) : state.getIV(column - 6);
        case 12:
            return QString::fromStdString(Translator::getHiddenPower(state.getHiddenPower()));
        case 13:
            return state.getHiddenPowerStrength();
        case 14:
            return QString::fromStdString(Translator::getGender(state.getGender()));
        case 15:
            return QString::fromStdString(Translator::getCharacteristic(state.getCharacteristic()));
        }
    }
    return QVariant();
}

QVariant StaticSearcherModel4::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

void StaticSearcherModel4::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, 6), index(rowCount(), 11), { Qt::DisplayRole });
}
