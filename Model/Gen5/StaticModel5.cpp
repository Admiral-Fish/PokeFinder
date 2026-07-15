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

#include "StaticModel5.hpp"
#include <Core/Enum/Lead.hpp>
#include <Core/Util/Translator.hpp>
#include <Core/Util/Utilities.hpp>
#include <QCoreApplication>
#include <QStringList>

static QString getLeadNameStatic5(Lead lead, u8 flags)
{
    if (lead == Lead::None)
    {
        return QCoreApplication::translate("StaticSearcherModel5", "None");
    }

    if (flags != 0)
    {
        QStringList leads;
        if ((flags & (1 << 0)) != 0)
        {
            leads.emplace_back(QCoreApplication::translate("StaticSearcherModel5", "Synchronize"));
        }
        bool cuteCharmM = (flags & (1 << 1)) != 0;
        bool cuteCharmF = (flags & (1 << 2)) != 0;
        if (cuteCharmM && cuteCharmF)
        {
            leads.emplace_back(QCoreApplication::translate("StaticSearcherModel5", "Cute Charm: ♂ or ♀ Lead"));
        }
        else if (cuteCharmM)
        {
            leads.emplace_back(QCoreApplication::translate("StaticSearcherModel5", "Cute Charm: ♂ Lead"));
        }
        else if (cuteCharmF)
        {
            leads.emplace_back(QCoreApplication::translate("StaticSearcherModel5", "Cute Charm: ♀ Lead"));
        }

        return leads.join(" / ");
    }

    if (lead <= Lead::SynchronizeEnd)
    {
        return QCoreApplication::translate("StaticSearcherModel5", "Synchronize");
    }

    switch (lead)
    {
    case Lead::CuteCharmM:
        return QCoreApplication::translate("StaticSearcherModel5", "Cute Charm: ♂ Lead");
    case Lead::CuteCharmF:
        return QCoreApplication::translate("StaticSearcherModel5", "Cute Charm: ♀ Lead");
    default:
        return QString();
    }
}

StaticGeneratorModel5::StaticGeneratorModel5(QObject *parent) : TableModel(parent), showStats(false)
{
}

int StaticGeneratorModel5::columnCount(const QModelIndex &parent) const
{
    return 16;
}

QVariant StaticGeneratorModel5::data(const QModelIndex &index, int role) const
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
            return QString::number(state.getPID(), 16).toUpper().rightJustified(8, '0');
        case 3:
        {
            u8 shiny = state.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 4:
            return QString::fromStdString(Translator::getNature(state.getNature()));
        case 5:
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
            return QString::fromStdString(Translator::getCharacteristic(state.getCharacteristic(), CharacteristicGeneration::Gen5));
        }
    }

    return QVariant();
}

QVariant StaticGeneratorModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

void StaticGeneratorModel5::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, 6), index(rowCount() - 1, 11), { Qt::DisplayRole });
}

StaticSearcherModel5::StaticSearcherModel5(QObject *parent) : TableModel(parent), showStats(false)
{
}

int StaticSearcherModel5::columnCount(const QModelIndex &parent) const
{
    return 21;
}

QVariant StaticSearcherModel5::data(const QModelIndex &index, int role) const
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
            return getLeadNameStatic5(state.getLead(), state.getLeadFlags());
        case 2:
            return state.getAdvances();
        case 3:
            return state.getIVAdvances();
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

QVariant StaticSearcherModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

void StaticSearcherModel5::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, 8), index(rowCount() - 1, 13), { Qt::DisplayRole });
}
