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

#include "HiddenGrottoModel.hpp"
#include <Core/Util/Translator.hpp>
#include <Core/Util/Utilities.hpp>
#include <QStringList>

namespace
{
    QString getPokemonSlotDisplay(const HiddenGrottoState &state)
    {
        constexpr u16 nidoranF = 29;
        constexpr u16 nidoranM = 32;
        constexpr u16 ditto = 132;
        constexpr u16 metang = 375;
        constexpr u16 bronzor = 436;

        u16 specie = state.getData();
        QString name = QString::fromStdString(Translator::getSpecie(specie));
        if (specie == nidoranF || specie == nidoranM || specie == ditto || specie == metang || specie == bronzor)
        {
            return QString("%1 (%2)").arg(state.getSlot()).arg(name);
        }

        QString gender = QString::fromStdString(Translator::getGender(state.getGender()));
        return QString("%1 (%2 %3)").arg(state.getSlot()).arg(name, gender);
    }
}

HiddenGrottoSlotGeneratorModel5::HiddenGrottoSlotGeneratorModel5(QObject *parent) : TableModel(parent)
{
}

int HiddenGrottoSlotGeneratorModel5::columnCount(const QModelIndex &parent) const
{
    return 5;
}

QVariant HiddenGrottoSlotGeneratorModel5::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &state = model[index.row()];
        int column = index.column();
        switch (column)
        {
        case 0:
            return state.getAdvances();
        case 1:
            return QString::fromStdString(Utilities5::getChatot(state.getChatot()));
        case 2:
            return QString::fromStdString(Translator::getNeedle(state.getNeedle()));
        case 3:
            return state.getGroup();
        case 4:
            if (state.getItem())
            {
                return QString("%1: %2").arg(state.getSlot()).arg(QString::fromStdString(Translator::getItem(state.getData())));
            }
            else
            {
                return getPokemonSlotDisplay(state);
            }
        }
    }
    return QVariant();
}

QVariant HiddenGrottoSlotGeneratorModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

HiddenGrottoSlotSearcherModel5::HiddenGrottoSlotSearcherModel5(QObject *parent) : TableModel(parent)
{
}

int HiddenGrottoSlotSearcherModel5::columnCount(const QModelIndex &parent) const
{
    return 8;
}

QVariant HiddenGrottoSlotSearcherModel5::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &display = model[index.row()];
        const auto &state = display.getState();
        int column = index.column();
        switch (column)
        {
        case 0:
            return QString::number(display.getInitialSeed(), 16).toUpper().rightJustified(16, '0');
        case 1:
        {
            const auto &itemAdvances = state.getItemAdvances();
            if (!itemAdvances.empty())
            {
                QStringList advances;
                for (u32 advance : itemAdvances)
                {
                    advances.append(QString::number(advance));
                }
                return advances.join(", ");
            }
            return state.getAdvances();
        }
        case 2:
            return state.getGroup();
        case 3:
            if (state.getItem())
            {
                return QString("%1: %2").arg(state.getSlot()).arg(QString::fromStdString(Translator::getItem(state.getData())));
            }
            else
            {
                return getPokemonSlotDisplay(state);
            }
        case 4:
            return state.getAmount();
        case 5:
            return QString::fromStdString(display.getDateTime().toString());
        case 6:
            return QString::number(display.getTimer0(), 16).toUpper();
        case 7:
            return QString::fromStdString(Translator::getKeypresses(display.getButtons()));
        }
    }
    else if (role == Qt::UserRole)
    {
        const auto &display = model[index.row()];
        const auto &state = display.getState();
        int column = index.column();
        switch (column)
        {
        case 0:
            return static_cast<qulonglong>(display.getInitialSeed());
        case 1:
            return state.getAdvances();
        case 2:
            return state.getGroup();
        case 3:
            return state.getSlot();
        case 4:
            return state.getAmount();
        case 5:
            return QString::fromStdString(display.getDateTime().toString());
        case 6:
            return display.getTimer0();
        case 7:
            return QString::fromStdString(Translator::getKeypresses(display.getButtons()));
        }
    }
    return QVariant();
}

QVariant HiddenGrottoSlotSearcherModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

HiddenGrottoGeneratorModel5::HiddenGrottoGeneratorModel5(QObject *parent) : TableModel(parent), showStats(false)
{
}

int HiddenGrottoGeneratorModel5::columnCount(const QModelIndex &parent) const
{
    return 18;
}

QVariant HiddenGrottoGeneratorModel5::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &state = model[index.row()];
        int column = index.column();
        switch (column)
        {
        case 0:
            return state.getAdvances();
        case 1:
            return QString::fromStdString(Utilities5::getChatot(state.getChatot()));
        case 2:
            return QString::fromStdString(Translator::getNeedle(state.getNeedle()));
        case 3:
            return state.getLevel();
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
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            return showStats ? state.getStat(column - 8) : state.getIV(column - 8);
        case 14:
            return QString::fromStdString(Translator::getHiddenPower(state.getHiddenPower()));
        case 15:
            return state.getHiddenPowerStrength();
        case 16:
            return QString::fromStdString(Translator::getGender(state.getGender()));
        case 17:
            return QString::fromStdString(Translator::getCharacteristic(state.getCharacteristic(), CharacteristicGeneration::Gen5));
        }
    }

    return QVariant();
}

QVariant HiddenGrottoGeneratorModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

void HiddenGrottoGeneratorModel5::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, 8), index(rowCount() - 1, 13), { Qt::DisplayRole });
}

HiddenGrottoSearcherModel5::HiddenGrottoSearcherModel5(QObject *parent) : TableModel(parent), showStats(false)
{
}

int HiddenGrottoSearcherModel5::columnCount(const QModelIndex &parent) const
{
    return 21;
}

QVariant HiddenGrottoSearcherModel5::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &display = model[index.row()];
        const auto &state = display.getState();
        int column = index.column();
        switch (column)
        {
        case 0:
            return QString::number(display.getInitialSeed(), 16).toUpper().rightJustified(16, '0');
        case 1:
            return state.getAdvances();
        case 2:
            return state.getIVAdvances();
        case 3:
            return state.getLevel();
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
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            return showStats ? state.getStat(column - 8) : state.getIV(column - 8);
        case 14:
            return QString::fromStdString(Translator::getHiddenPower(state.getHiddenPower()));
        case 15:
            return state.getHiddenPowerStrength();
        case 16:
            return QString::fromStdString(Translator::getGender(state.getGender()));
        case 17:
            return QString::fromStdString(Translator::getCharacteristic(state.getCharacteristic(), CharacteristicGeneration::Gen5));
        case 18:
            return QString::fromStdString(display.getDateTime().toString());
        case 19:
            return QString::number(display.getTimer0(), 16).toUpper();
        case 20:
            return QString::fromStdString(Translator::getKeypresses(display.getButtons()));
        }
    }

    return QVariant();
}

QVariant HiddenGrottoSearcherModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

void HiddenGrottoSearcherModel5::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, 8), index(rowCount() - 1, 13), { Qt::DisplayRole });
}
