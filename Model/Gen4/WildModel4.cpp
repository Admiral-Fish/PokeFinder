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

#include "WildModel4.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Util/Translator.hpp>
#include <Core/Util/Utilities.hpp>
#include <QColor>
#include <QFont>

WildGeneratorModel4::WildGeneratorModel4(QObject *parent, Method method) : TableModel(parent), showStats(false), showStepEncounter(false), method(method)
{
}

int WildGeneratorModel4::columnCount(const QModelIndex &parent) const
{
    int extra = showStepEncounter ? 1 : 0;
    switch (method)
    {
    case Method::MethodJ:
    case Method::HoneyTree:
    case Method::PokeRadar:
        return 20 + extra;
    case Method::MethodK:
        return 21 + extra;
    default:
        return 0;
    }
}

QVariant WildGeneratorModel4::data(const QModelIndex &index, int role) const
{
    const auto &state = model[index.row()];
    bool dpptStepEncounter = showStepEncounter && method != Method::MethodK;
    if (showStepEncounter && !state.getStepEncounter())
    {
        if (role == Qt::FontRole)
        {
            QFont font;
            font.setItalic(true);
            return font;
        }
        else if (role == Qt::ForegroundRole)
        {
            return QColor(128, 128, 128);
        }
    }

    if (role == Qt::DisplayRole)
    {
        int column = getColumn(index.column());
        if (dpptStepEncounter && !state.getStepEncounter() && column > 4)
        {
            return "-";
        }

        switch (column)
        {
        case 0:
            return state.getAdvances();
        case 1:
            return state.getBattleAdvances();
        case 2:
            return state.getCall() == 0 ? "E" : state.getCall() == 1 ? "K" : "P";
        case 3:
            return QString::fromStdString(Utilities4::getChatot(state.getChatot()));
        case 4:
            return state.getStepEncounter() ? QVariant(static_cast<int>(state.getMovements())) : QVariant("-");
        case 5:
            return QString::fromStdString(Translator::getItem(state.getItem()));
        case 6:
            return QString("%1: %2")
                .arg(state.getEncounterSlot())
                .arg(QString::fromStdString(Translator::getSpecie(state.getSpecie(), state.getForm())));
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
            return QString::fromStdString(Translator::getNature(state.getNature()));
        case 11:
            return QString("%1: %2").arg(state.getAbility()).arg(QString::fromStdString(Translator::getAbility(state.getAbilityIndex())));
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
            return showStats ? state.getStat(column - 12) : state.getIV(column - 12);
        case 18:
            return QString::fromStdString(Translator::getHiddenPower(state.getHiddenPower()));
        case 19:
            return state.getHiddenPowerStrength();
        case 20:
            return QString::fromStdString(Translator::getGender(state.getGender()));
        case 21:
            return QString::fromStdString(Translator::getCharacteristic(state.getCharacteristic()));
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
    beginResetModel();
    this->method = method;
    endResetModel();
}

void WildGeneratorModel4::setShowStepEncounter(bool flag)
{
    beginResetModel();
    showStepEncounter = flag;
    endResetModel();
}

void WildGeneratorModel4::setShowStats(bool flag)
{
    showStats = flag;
    int hp = showStepEncounter ? (method == Method::MethodK ? 12 : 11) : 11;
    emit dataChanged(index(0, hp), index(rowCount() - 1, hp + 5), { Qt::DisplayRole });
}

int WildGeneratorModel4::getColumn(int column) const
{
    switch (method)
    {
    case Method::MethodJ:
    case Method::HoneyTree:
    case Method::PokeRadar:
        column = column > 1 ? column + 1 : column;
        break;
    case Method::MethodK:
    default:
        break;
    }

    return !showStepEncounter && column >= 4 ? column + 1 : column;
}

WildSearcherModel4::WildSearcherModel4(QObject *parent) :
    TableModel(parent),
    showStats(false),
    showStepEncounter(false),
    showStepMovement(true),
    method(Method::MethodJ)
{
}

int WildSearcherModel4::columnCount(const QModelIndex &parent) const
{
    return showStepEncounter ? showStepMovement ? 22 : 21 : 19;
}

QVariant WildSearcherModel4::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &state = model[index.row()];
        int column = index.column();
        if (!showStepEncounter && column >= 2)
        {
            column += 3;
        }
        else if (showStepEncounter && !showStepMovement && column >= 3)
        {
            column++;
        }
        switch (column)
        {
        case 0:
            return QString::number(state.getSeed(), 16).toUpper().rightJustified(8, '0');
        case 1:
            return state.getAdvances();
        case 2:
            return static_cast<int>(state.getMovements());
        case 3:
            switch (state.getMovement())
            {
            case 1:
                return tr("Running");
            case 2:
                return tr("Biking");
            case 3:
                return method == Method::MethodK ? tr("Walking in Long Grass") : tr("Walking / Running in Long Grass");
            case 4:
                return tr("Running in Long Grass");
            case 5:
                return tr("Surfing");
            case 0:
            default:
                return method == Method::MethodK ? tr("Walking") : tr("Walking / Running");
            }
        case 4:
            switch (state.getStepModifier())
            {
            case 1:
                return tr("White Flute");
            case 2:
                return tr("Pokemon March");
            case 3:
                return tr("White Flute") + QString(" & ") + tr("Pokemon March");
            case 4:
                return tr("Pokemon Lullaby");
            case 5:
                return tr("White Flute") + QString(" & ") + tr("Pokemon Lullaby");
            case 6:
                return tr("Pokemon March") + QString(" / ") + tr("Lullaby");
            case 7:
                return tr("White Flute") + QString(" & ") + tr("Pokemon March") + QString(" / ") + tr("Lullaby");
            case 0:
            default:
                return QString("-");
            }
        case 5:
            return QString::fromStdString(Translator::getItem(state.getItem()));
        case 6:
            return QString("%1: %2")
                .arg(state.getEncounterSlot())
                .arg(QString::fromStdString(Translator::getSpecie(state.getSpecie(), state.getForm())));
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
            return QString::fromStdString(Translator::getNature(state.getNature()));
        case 11:
            return QString("%1: %2").arg(state.getAbility()).arg(QString::fromStdString(Translator::getAbility(state.getAbilityIndex())));
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
            return showStats ? state.getStat(column - 12) : state.getIV(column - 12);
        case 18:
            return QString::fromStdString(Translator::getHiddenPower(state.getHiddenPower()));
        case 19:
            return state.getHiddenPowerStrength();
        case 20:
            return QString::fromStdString(Translator::getGender(state.getGender()));
        case 21:
            return QString::fromStdString(Translator::getCharacteristic(state.getCharacteristic()));
        }
    }
    return QVariant();
}

QVariant WildSearcherModel4::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        if (!showStepEncounter && section >= 2)
        {
            section += 3;
        }
        else if (showStepEncounter && !showStepMovement && section >= 3)
        {
            section++;
        }
        return header[section];
    }
    return QVariant();
}

void WildSearcherModel4::setMethod(Method method)
{
    beginResetModel();
    this->method = method;
    endResetModel();
}

void WildSearcherModel4::setShowStats(bool flag)
{
    showStats = flag;
    int hp = showStepEncounter ? showStepMovement ? 12 : 11 : 9;
    emit dataChanged(index(0, hp), index(rowCount() - 1, hp + 5), { Qt::DisplayRole });
}

void WildSearcherModel4::setShowStepEncounter(bool flag)
{
    beginResetModel();
    showStepEncounter = flag;
    endResetModel();
}

void WildSearcherModel4::setShowStepMovement(bool flag)
{
    beginResetModel();
    showStepMovement = flag;
    endResetModel();
}
