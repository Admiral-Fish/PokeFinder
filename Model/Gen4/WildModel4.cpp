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
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Util/Translator.hpp>
#include <Core/Util/Utilities.hpp>
#include <QCoreApplication>
#include <QStringList>

static QString getSynchronizeLeadName4(Lead lead, u32 flags, u8 targetNature)
{
    constexpr u32 allNatures = (1 << 25) - 1;
    u32 nonTargetNatures = allNatures & ~(1 << targetNature);
    if ((flags & allNatures) == allNatures || (flags & nonTargetNatures) != 0)
    {
        return QString("%1: %2").arg(QCoreApplication::translate("WildSearcherModel4", "Synchronize"),
                                     QCoreApplication::translate("WildSearcherModel4", "Any Nature"));
    }

    if (lead <= Lead::SynchronizeEnd)
    {
        return QString("%1: %2")
            .arg(QCoreApplication::translate("WildSearcherModel4", "Synchronize"),
                 QString::fromStdString(Translator::getNature(toInt(lead))));
    }

    return QCoreApplication::translate("WildSearcherModel4", "Synchronize");
}

static QString getLeadName4(Lead lead, Lead synchronizeLead, u32 synchronizeFlags, u8 flags, u8 targetNature)
{
    if (lead == Lead::None)
    {
        return QCoreApplication::translate("WildSearcherModel4", "None");
    }

    if (flags != 0)
    {
        QStringList leads;
        if ((flags & (1 << 0)) != 0)
        {
            leads.emplace_back(getSynchronizeLeadName4(synchronizeLead, synchronizeFlags, targetNature));
        }
        bool cuteCharmM = (flags & (1 << 1)) != 0;
        bool cuteCharmF = (flags & (1 << 2)) != 0;
        if (cuteCharmM && cuteCharmF)
        {
            leads.emplace_back(QCoreApplication::translate("WildSearcherModel4", "Cute Charm: ♂ or ♀ Lead"));
        }
        else if (cuteCharmM)
        {
            leads.emplace_back(QCoreApplication::translate("WildSearcherModel4", "Cute Charm: ♂ Lead"));
        }
        else if (cuteCharmF)
        {
            leads.emplace_back(QCoreApplication::translate("WildSearcherModel4", "Cute Charm: ♀ Lead"));
        }
        if ((flags & (1 << 3)) != 0)
        {
            leads.emplace_back(QCoreApplication::translate("WildSearcherModel4", "Magnet Pull"));
        }
        if ((flags & (1 << 4)) != 0)
        {
            leads.emplace_back(QCoreApplication::translate("WildSearcherModel4", "Static"));
        }
        if ((flags & (1 << 5)) != 0)
        {
            leads.emplace_back(QCoreApplication::translate("WildSearcherModel4", "Hustle / Pressure / Vital Spirit"));
        }
        if ((flags & (1 << 6)) != 0)
        {
            leads.emplace_back(QCoreApplication::translate("WildSearcherModel4", "Compound Eyes"));
        }
        if ((flags & (1 << 7)) != 0)
        {
            leads.emplace_back(QCoreApplication::translate("WildSearcherModel4", "Arena Trap / Illuminate / No Guard"));
        }

        return leads.join(" / ");
    }

    if (lead <= Lead::SynchronizeEnd)
    {
        return getSynchronizeLeadName4(lead, synchronizeFlags, targetNature);
    }

    switch (lead)
    {
    case Lead::CompoundEyes:
        return QCoreApplication::translate("WildSearcherModel4", "Compound Eyes");
    case Lead::CuteCharmM:
        return QCoreApplication::translate("WildSearcherModel4", "Cute Charm: ♂ Lead");
    case Lead::CuteCharmF:
        return QCoreApplication::translate("WildSearcherModel4", "Cute Charm: ♀ Lead");
    case Lead::MagnetPull:
        return QCoreApplication::translate("WildSearcherModel4", "Magnet Pull");
    case Lead::Static:
        return QCoreApplication::translate("WildSearcherModel4", "Static");
    case Lead::Pressure:
        return QCoreApplication::translate("WildSearcherModel4", "Hustle / Pressure / Vital Spirit");
    case Lead::ArenaTrap:
        return QCoreApplication::translate("WildSearcherModel4", "Arena Trap / Illuminate / No Guard");
    case Lead::SuctionCups:
        return QCoreApplication::translate("WildSearcherModel4", "Sticky Hold / Suction Cups");
    default:
        return QString();
    }
}

WildGeneratorModel4::WildGeneratorModel4(QObject *parent, Method method) : TableModel(parent), showStats(false), method(method)
{
}

int WildGeneratorModel4::columnCount(const QModelIndex &parent) const
{
    switch (method)
    {
    case Method::MethodJ:
    case Method::HoneyTree:
    case Method::PokeRadar:
        return 20;
    case Method::MethodK:
        return 21;
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
            return state.getBattleAdvances();
        case 2:
            return state.getCall() == 0 ? "E" : state.getCall() == 1 ? "K" : "P";
        case 3:
            return QString::fromStdString(Utilities4::getChatot(state.getChatot()));
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
            return QString("%1: %2").arg(state.getAbility()).arg(QString::fromStdString(Translator::getAbility(state.getAbilityIndex())));
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
    this->method = method;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

void WildGeneratorModel4::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, 11), index(rowCount() - 1, 16), { Qt::DisplayRole });
}

int WildGeneratorModel4::getColumn(int column) const
{
    switch (method)
    {
    case Method::MethodJ:
    case Method::HoneyTree:
    case Method::PokeRadar:
        return column > 1 ? column + 1 : column;
    case Method::MethodK:
    default:
        return column;
    }
}

WildSearcherModel4::WildSearcherModel4(QObject *parent) : TableModel(parent), showStats(false)
{
}

int WildSearcherModel4::columnCount(const QModelIndex &parent) const
{
    return 20;
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
            return getLeadName4(state.getLead(), state.getSynchronizeLead(), state.getSynchronizeLeadFlags(), state.getLeadFlags(),
                                state.getNature());
        case 2:
            return state.getAdvances();
        case 3:
            return QString::fromStdString(Translator::getItem(state.getItem()));
        case 4:
            return QString("%1: %2")
                .arg(state.getEncounterSlot())
                .arg(QString::fromStdString(Translator::getSpecie(state.getSpecie(), state.getForm())));
        case 5:
            return state.getLevel();
        case 6:
            return QString::number(state.getPID(), 16).toUpper().rightJustified(8, '0');
        case 7:
        {
            u8 shiny = state.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 8:
            return QString::fromStdString(Translator::getNature(state.getNature()));
        case 9:
            return QString("%1: %2").arg(state.getAbility()).arg(QString::fromStdString(Translator::getAbility(state.getAbilityIndex())));
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            return showStats ? state.getStat(column - 10) : state.getIV(column - 10);
        case 16:
            return QString::fromStdString(Translator::getHiddenPower(state.getHiddenPower()));
        case 17:
            return state.getHiddenPowerStrength();
        case 18:
            return QString::fromStdString(Translator::getGender(state.getGender()));
        case 19:
            return QString::fromStdString(Translator::getCharacteristic(state.getCharacteristic()));
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

void WildSearcherModel4::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, 10), index(rowCount() - 1, 15), { Qt::DisplayRole });
}
