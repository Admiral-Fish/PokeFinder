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

constexpr int generatorPhenomenonColumn = 3;
constexpr int searcherPhenomenonColumn = 2;

WildGeneratorModel5::WildGeneratorModel5(QObject *parent) : TableModel(parent), showStats(false), showPhenomenon(false)
{
}

int WildGeneratorModel5::columnCount(const QModelIndex &parent) const
{
    return showPhenomenon ? header.size() : header.size() - 1;
}

QVariant WildGeneratorModel5::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &state = model[index.row()];
        int column = !showPhenomenon && index.column() >= generatorPhenomenonColumn ? index.column() + 1 : index.column();
        bool item = state.getPhenomenonItem();
        switch (column)
        {
        case 0:
            return state.getAdvances();
        case 1:
            return QString::fromStdString(Utilities5::getChatot(state.getChatot()));
        case 2:
            return QString::fromStdString(Translator::getNeedle(state.getNeedle()));
        case 3:
            return state.getPhenomenon() ? tr("Yes") : tr("No");
        case 4:
            return QString::fromStdString(Translator::getItem(state.getItem()));
        case 5:
            if (item)
            {
                return "-";
            }
            return QString("%1: %2")
                .arg(state.getEncounterSlot())
                .arg(QString::fromStdString(Translator::getSpecie(state.getSpecie(), state.getForm())));
        case 6:
            if (item)
            {
                return "-";
            }
            return state.getLevel();
        case 7:
            if (item)
            {
                return "-";
            }
            return QString::number(state.getPID(), 16).toUpper().rightJustified(8, '0');
        case 8:
        {
            if (item)
            {
                return "-";
            }
            u8 shiny = state.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 9:
            if (item)
            {
                return "-";
            }
            return QString::fromStdString(Translator::getNature(state.getNature()));
        case 10:
            if (item)
            {
                return "-";
            }
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
            if (item)
            {
                return "-";
            }
            return showStats ? state.getStat(column - 11) : state.getIV(column - 11);
        case 17:
            if (item)
            {
                return "-";
            }
            return QString::fromStdString(Translator::getHiddenPower(state.getHiddenPower()));
        case 18:
            if (item)
            {
                return "-";
            }
            return state.getHiddenPowerStrength();
        case 19:
            if (item)
            {
                return "-";
            }
            return QString::fromStdString(Translator::getGender(state.getGender()));
        case 20:
            if (item)
            {
                return "-";
            }
            return QString::fromStdString(Translator::getCharacteristic(state.getCharacteristic(), CharacteristicGeneration::Gen5));
        }
    }

    return QVariant();
}

QVariant WildGeneratorModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        int column = !showPhenomenon && section >= generatorPhenomenonColumn ? section + 1 : section;
        return header[column];
    }
    return QVariant();
}

void WildGeneratorModel5::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1), { Qt::DisplayRole });
}

WildSearcherModel5::WildSearcherModel5(QObject *parent) : TableModel(parent), showStats(false), showPhenomenon(false)
{
}

int WildSearcherModel5::columnCount(const QModelIndex &parent) const
{
    return showPhenomenon ? header.size() : header.size() - 1;
}

QVariant WildSearcherModel5::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &display = model[index.row()];
        const auto &state = display.getState();
        int column = !showPhenomenon && index.column() >= searcherPhenomenonColumn ? index.column() + 1 : index.column();
        bool item = state.getPhenomenonItem();
        switch (column)
        {
        case 0:
            return QString::number(display.getInitialSeed(), 16).toUpper().rightJustified(16, '0');
        case 1:
            return state.getAdvances();
        case 2:
            return state.getPhenomenon() ? tr("Yes") : tr("No");
        case 3:
            return state.getIVAdvances();
        case 4:
            return QString::fromStdString(Translator::getItem(state.getItem()));
        case 5:
            if (item)
            {
                return "-";
            }
            return QString("%1: %2")
                .arg(state.getEncounterSlot())
                .arg(QString::fromStdString(Translator::getSpecie(state.getSpecie(), state.getForm())));
        case 6:
            if (item)
            {
                return "-";
            }
            return state.getLevel();
        case 7:
            if (item)
            {
                return "-";
            }
            return QString::number(state.getPID(), 16).toUpper().rightJustified(8, '0');
        case 8:
        {
            if (item)
            {
                return "-";
            }
            u8 shiny = state.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 9:
            if (item)
            {
                return "-";
            }
            return QString::fromStdString(Translator::getNature(state.getNature()));
        case 10:
            if (item)
            {
                return "-";
            }
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
            if (item)
            {
                return "-";
            }
            return showStats ? state.getStat(column - 11) : state.getIV(column - 11);
        case 17:
            if (item)
            {
                return "-";
            }
            return QString::fromStdString(Translator::getHiddenPower(state.getHiddenPower()));
        case 18:
            if (item)
            {
                return "-";
            }
            return state.getHiddenPowerStrength();
        case 19:
            if (item)
            {
                return "-";
            }
            return QString::fromStdString(Translator::getGender(state.getGender()));
        case 20:
            if (item)
            {
                return "-";
            }
            return QString::fromStdString(Translator::getCharacteristic(state.getCharacteristic(), CharacteristicGeneration::Gen5));
        case 21:
            return QString::fromStdString(display.getDateTime().toString());
        case 22:
            return QString::number(display.getTimer0(), 16).toUpper();
        case 23:
            return QString::fromStdString(Translator::getKeypresses(display.getButtons()));
        }
    }

    return QVariant();
}

QVariant WildSearcherModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        int column = !showPhenomenon && section >= searcherPhenomenonColumn ? section + 1 : section;
        return header[column];
    }
    return QVariant();
}

void WildSearcherModel5::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1), { Qt::DisplayRole });
}
