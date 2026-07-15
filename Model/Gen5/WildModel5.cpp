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
#include <Core/Enum/Lead.hpp>
#include <Core/Util/Translator.hpp>
#include <Core/Util/Utilities.hpp>
#include <QCoreApplication>
#include <QStringList>

static QString getLeadName(Lead lead, u8 flags)
{
    if (lead == Lead::None)
    {
        return QCoreApplication::translate("WildSearcherModel5", "None");
    }

    if (flags != 0)
    {
        QStringList leads;
        if ((flags & (1 << 0)) != 0)
        {
            leads.emplace_back(QCoreApplication::translate("WildSearcherModel5", "Synchronize"));
        }
        bool cuteCharmM = (flags & (1 << 1)) != 0;
        bool cuteCharmF = (flags & (1 << 2)) != 0;
        if (cuteCharmM && cuteCharmF)
        {
            leads.emplace_back(QCoreApplication::translate("WildSearcherModel5", "Cute Charm: ♂ or ♀ Lead"));
        }
        else if (cuteCharmM)
        {
            leads.emplace_back(QCoreApplication::translate("WildSearcherModel5", "Cute Charm: ♂ Lead"));
        }
        else if (cuteCharmF)
        {
            leads.emplace_back(QCoreApplication::translate("WildSearcherModel5", "Cute Charm: ♀ Lead"));
        }
        if ((flags & (1 << 3)) != 0)
        {
            leads.emplace_back(QCoreApplication::translate("WildSearcherModel5", "Magnet Pull"));
        }
        if ((flags & (1 << 4)) != 0)
        {
            leads.emplace_back(QCoreApplication::translate("WildSearcherModel5", "Static"));
        }
        if ((flags & (1 << 5)) != 0)
        {
            leads.emplace_back(QCoreApplication::translate("WildSearcherModel5", "Hustle / Pressure / Vital Spirit"));
        }
        if ((flags & (1 << 6)) != 0)
        {
            leads.emplace_back(QCoreApplication::translate("WildSearcherModel5", "Compound Eyes"));
        }

        return leads.join(" / ");
    }

    if (lead <= Lead::SynchronizeEnd)
    {
        return QCoreApplication::translate("WildSearcherModel5", "Synchronize");
    }

    switch (lead)
    {
    case Lead::None:
        return QCoreApplication::translate("WildSearcherModel5", "None");
    case Lead::CompoundEyes:
        return QCoreApplication::translate("WildSearcherModel5", "Compound Eyes");
    case Lead::CuteCharmM:
        return QCoreApplication::translate("WildSearcherModel5", "Cute Charm: ♂ Lead");
    case Lead::CuteCharmF:
        return QCoreApplication::translate("WildSearcherModel5", "Cute Charm: ♀ Lead");
    case Lead::MagnetPull:
        return QCoreApplication::translate("WildSearcherModel5", "Magnet Pull");
    case Lead::Static:
        return QCoreApplication::translate("WildSearcherModel5", "Static");
    case Lead::Pressure:
        return QCoreApplication::translate("WildSearcherModel5", "Hustle / Pressure / Vital Spirit");
    default:
        return QString();
    }
}

WildGeneratorModel5::WildGeneratorModel5(QObject *parent) : TableModel(parent), showStats(false)
{
}

int WildGeneratorModel5::columnCount(const QModelIndex &parent) const
{
    return 19;
}

QVariant WildGeneratorModel5::data(const QModelIndex &index, int role) const
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
            return QString::fromStdString(Translator::getItem(state.getItem()));
        case 3:
            return QString("%1: %2")
                .arg(state.getEncounterSlot())
                .arg(QString::fromStdString(Translator::getSpecie(state.getSpecie(), state.getForm())));
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
            return QString::fromStdString(Translator::getNature(state.getNature()));
        case 8:
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
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
            return showStats ? state.getStat(column - 9) : state.getIV(column - 9);
        case 15:
            return QString::fromStdString(Translator::getHiddenPower(state.getHiddenPower()));
        case 16:
            return state.getHiddenPowerStrength();
        case 17:
            return QString::fromStdString(Translator::getGender(state.getGender()));
        case 18:
            return QString::fromStdString(Translator::getCharacteristic(state.getCharacteristic(), CharacteristicGeneration::Gen5));
        }
    }

    return QVariant();
}

QVariant WildGeneratorModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

void WildGeneratorModel5::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, 9), index(rowCount() - 1, 14), { Qt::DisplayRole });
}

WildSearcherModel5::WildSearcherModel5(QObject *parent) : TableModel(parent), showStats(false)
{
}

int WildSearcherModel5::columnCount(const QModelIndex &parent) const
{
    return 24;
}

QVariant WildSearcherModel5::data(const QModelIndex &index, int role) const
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
            return getLeadName(state.getLead(), state.getLeadFlags());
        case 2:
            return state.getAdvances();
        case 3:
            return state.getIVAdvances();
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
            if (state.getVariableNature())
            {
                return QCoreApplication::translate("WildSearcherModel5", "Sync");
            }
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
        return header[section];
    }
    return QVariant();
}

void WildSearcherModel5::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, 11), index(rowCount() - 1, 16), { Qt::DisplayRole });
}
