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

#include "WildModel5.hpp"
#include <Core/Util/Translator.hpp>
#include <Core/Util/Utilities.hpp>
#include <QColor>
#include <QFont>

WildGeneratorModel5::WildGeneratorModel5(QObject *parent) : TableModel(parent), showStats(false), showMovingTrigger(false)
{
}

int WildGeneratorModel5::columnCount(const QModelIndex &parent) const
{
    return showMovingTrigger ? 21 : 20;
}

QVariant WildGeneratorModel5::data(const QModelIndex &index, int role) const
{
    const auto &state = model[index.row()];
    if (showMovingTrigger && !state.isValid())
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
        int column = index.column();
        if (!showMovingTrigger && column >= 3)
        {
            column++;
        }

        if (showMovingTrigger && !state.isValid() && column > 3)
        {
            return "-";
        }

        switch (column)
        {
        case 0:
            return state.getAdvances();
        case 1:
            return QString::fromStdString(Utilities5::getChatot(state.getChatot()));
        case 2:
            return QString::fromStdString(Translator::getNeedle(state.getNeedle()));
        case 3:
            if (state.getMovingSteps() == 255)
            {
                return "-";
            }
            return state.getMovingSteps();
        case 4:
            return QString::fromStdString(Translator::getItem(state.getItem()));
        case 5:
            return QString("%1: %2")
                .arg(state.getEncounterSlot())
                .arg(QString::fromStdString(Translator::getSpecie(state.getSpecie(), state.getForm())));
        case 6:
            return state.getLevel();
        case 7:
            return QString::number(state.getPID(), 16).toUpper().rightJustified(8, '0');
        case 8:
        {
            u8 shiny = state.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 9:
            return QString::fromStdString(Translator::getNature(state.getNature()));
        case 10:
            if (state.getAbility() == 0 || state.getAbility() == 1)
            {
                return QString("%1: %2")
                    .arg(state.getAbility())
                    .arg(QString::fromStdString(Translator::getAbility(state.getAbilityIndex())));
            }
            else
            {
                return QString("H (%2)").arg(QString::fromStdString(Translator::getAbility(state.getAbilityIndex())));
            }
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
            return showStats ? state.getStat(column - 11) : state.getIV(column - 11);
        case 17:
            return QString::fromStdString(Translator::getHiddenPower(state.getHiddenPower()));
        case 18:
            return state.getHiddenPowerStrength();
        case 19:
            return QString::fromStdString(Translator::getGender(state.getGender()));
        case 20:
            return QString::fromStdString(Translator::getCharacteristic(state.getCharacteristic(), CharacteristicGeneration::Gen5));
        }
    }

    return QVariant();
}

QVariant WildGeneratorModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        if (!showMovingTrigger && section >= 3)
        {
            section++;
        }

        return header[section];
    }
    return QVariant();
}

void WildGeneratorModel5::setShowStats(bool flag)
{
    showStats = flag;
    int offset = showMovingTrigger ? 0 : -1;
    emit dataChanged(index(0, 11 + offset), index(rowCount() - 1, 16 + offset), { Qt::DisplayRole });
}

void WildGeneratorModel5::setShowMovingTrigger(bool flag)
{
    showMovingTrigger = flag;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
    emit layoutChanged();
}

WildSearcherModel5::WildSearcherModel5(QObject *parent) : TableModel(parent), showStats(false), showMovingTrigger(false)
{
}

int WildSearcherModel5::columnCount(const QModelIndex &parent) const
{
    return showMovingTrigger ? 24 : 23;
}

QVariant WildSearcherModel5::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &display = model[index.row()];
        const auto &state = display.getState();
        int column = index.column();
        if (showMovingTrigger && column >= 2)
        {
            column++;
        }
        else if (!showMovingTrigger && column >= 2)
        {
            column += 2;
        }

        switch (column)
        {
        case 0:
            return QString::number(display.getInitialSeed(), 16).toUpper().rightJustified(16, '0');
        case 1:
            return state.getAdvances();
        case 2:
            return state.getMovingTrigger();
        case 3:
            if (state.getMovingSteps() == 255)
            {
                return "-";
            }
            return state.getMovingSteps();
        case 4:
            return state.getIVAdvances();
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
            if (state.getAbility() == 0 || state.getAbility() == 1)
            {
                return QString("%1: %2")
                    .arg(state.getAbility())
                    .arg(QString::fromStdString(Translator::getAbility(state.getAbilityIndex())));
            }
            else
            {
                return QString("H (%2)").arg(QString::fromStdString(Translator::getAbility(state.getAbilityIndex())));
            }
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
            return QString::fromStdString(Translator::getCharacteristic(state.getCharacteristic(), CharacteristicGeneration::Gen5));
        case 22:
            return QString::fromStdString(display.getDateTime().toString());
        case 23:
            return QString::number(display.getTimer0(), 16).toUpper();
        case 24:
            return QString::fromStdString(Translator::getKeypresses(display.getButtons()));
        }
    }

    return QVariant();
}

QVariant WildSearcherModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        if (!showMovingTrigger && section >= 2)
        {
            section++;
        }

        return header[section];
    }
    return QVariant();
}

void WildSearcherModel5::setShowStats(bool flag)
{
    showStats = flag;
    int offset = showMovingTrigger ? -1 : -2;
    emit dataChanged(index(0, 12 + offset), index(rowCount() - 1, 17 + offset), { Qt::DisplayRole });
}

void WildSearcherModel5::setShowMovingTrigger(bool flag)
{
    showMovingTrigger = flag;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
    emit layoutChanged();
}
