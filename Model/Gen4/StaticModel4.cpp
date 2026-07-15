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
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Util/Translator.hpp>
#include <Core/Util/Utilities.hpp>
#include <QCoreApplication>
#include <QStringList>

static QString getSynchronizeLeadNameStatic4(Lead lead, u32 flags, u8 targetNature)
{
    constexpr u32 allNatures = (1 << 25) - 1;
    u32 nonTargetNatures = allNatures & ~(1 << targetNature);
    if ((flags & allNatures) == allNatures || (flags & nonTargetNatures) != 0)
    {
        return QString("%1: %2").arg(QCoreApplication::translate("StaticSearcherModel4", "Synchronize"),
                                     QCoreApplication::translate("StaticSearcherModel4", "Any Nature"));
    }

    if (lead <= Lead::SynchronizeEnd)
    {
        return QString("%1: %2")
            .arg(QCoreApplication::translate("StaticSearcherModel4", "Synchronize"),
                 QString::fromStdString(Translator::getNature(toInt(lead))));
    }

    return QCoreApplication::translate("StaticSearcherModel4", "Synchronize");
}

static QString getLeadNameStatic4(Lead lead, Lead synchronizeLead, u32 synchronizeFlags, u8 flags, u8 targetNature)
{
    if (lead == Lead::None)
    {
        return QCoreApplication::translate("StaticSearcherModel4", "None");
    }

    if (flags != 0)
    {
        QStringList leads;
        if ((flags & (1 << 0)) != 0)
        {
            leads.emplace_back(getSynchronizeLeadNameStatic4(synchronizeLead, synchronizeFlags, targetNature));
        }
        bool cuteCharmM = (flags & (1 << 1)) != 0;
        bool cuteCharmF = (flags & (1 << 2)) != 0;
        if (cuteCharmM && cuteCharmF)
        {
            leads.emplace_back(QCoreApplication::translate("StaticSearcherModel4", "Cute Charm: ♂ or ♀ Lead"));
        }
        else if (cuteCharmM)
        {
            leads.emplace_back(QCoreApplication::translate("StaticSearcherModel4", "Cute Charm: ♂ Lead"));
        }
        else if (cuteCharmF)
        {
            leads.emplace_back(QCoreApplication::translate("StaticSearcherModel4", "Cute Charm: ♀ Lead"));
        }

        return leads.join(" / ");
    }

    if (lead <= Lead::SynchronizeEnd)
    {
        return getSynchronizeLeadNameStatic4(lead, synchronizeFlags, targetNature);
    }

    switch (lead)
    {
    case Lead::CuteCharmM:
        return QCoreApplication::translate("StaticSearcherModel4", "Cute Charm: ♂ Lead");
    case Lead::CuteCharmF:
        return QCoreApplication::translate("StaticSearcherModel4", "Cute Charm: ♀ Lead");
    default:
        return QString();
    }
}

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
            return QString::fromStdString(Translator::getCharacteristic(state.getCharacteristic(), CharacteristicGeneration::Gen4));
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
    emit dataChanged(index(0, getColumn(7)), index(rowCount() - 1, getColumn(12)), { Qt::DisplayRole });
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
    return 19;
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
            return state.getSeed() & 0xffff;
        case 2:
            return (state.getSeed() >> 16) & 0xff;
        case 3:
            return getLeadNameStatic4(state.getLead(), state.getSynchronizeLead(), state.getSynchronizeLeadFlags(), state.getLeadFlags(),
                                      state.getNature());
        case 4:
            return state.getAdvances();
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
            return QString("%1: %2").arg(state.getAbility()).arg(QString::fromStdString(Translator::getAbility(state.getAbilityIndex())));
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
            return QString::fromStdString(Translator::getCharacteristic(state.getCharacteristic(), CharacteristicGeneration::Gen4));
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
    emit dataChanged(index(0, 9), index(rowCount() - 1, 14), { Qt::DisplayRole });
}
