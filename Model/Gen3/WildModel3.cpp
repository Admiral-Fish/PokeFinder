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

#include "WildModel3.hpp"
#include <Core/Enum/Lead.hpp>
#include <Core/Util/Translator.hpp>
#include <QCoreApplication>
#include <QStringList>

static QString getSynchronizeLeadName3(Lead lead, u32 flags, u8 targetNature)
{
    constexpr u32 allNatures = (1 << 25) - 1;
    u32 nonTargetNatures = allNatures & ~(1 << targetNature);
    if ((flags & allNatures) == allNatures || (flags & nonTargetNatures) != 0)
    {
        return QString("%1: %2").arg(QCoreApplication::translate("WildSearcherModel3", "Synchronize"),
                                     QCoreApplication::translate("WildSearcherModel3", "Any Nature"));
    }

    if (lead <= Lead::SynchronizeEnd)
    {
        return QString("%1: %2")
            .arg(QCoreApplication::translate("WildSearcherModel3", "Synchronize"),
                 QString::fromStdString(Translator::getNature(toInt(lead))));
    }

    return QCoreApplication::translate("WildSearcherModel3", "Synchronize");
}

static QString getLeadName3(Lead lead, Lead synchronizeLead, u32 synchronizeFlags, u8 flags, u8 targetNature)
{
    if (lead == Lead::None)
    {
        return QCoreApplication::translate("WildSearcherModel3", "None");
    }

    if (flags != 0)
    {
        QStringList leads;
        if ((flags & (1 << 0)) != 0)
        {
            leads.emplace_back(getSynchronizeLeadName3(synchronizeLead, synchronizeFlags, targetNature));
        }
        bool cuteCharmM = (flags & (1 << 1)) != 0;
        bool cuteCharmF = (flags & (1 << 2)) != 0;
        if (cuteCharmM && cuteCharmF)
        {
            leads.emplace_back(QCoreApplication::translate("WildSearcherModel3", "Cute Charm: ♂ or ♀ Lead"));
        }
        else if (cuteCharmM)
        {
            leads.emplace_back(QCoreApplication::translate("WildSearcherModel3", "Cute Charm: ♂ Lead"));
        }
        else if (cuteCharmF)
        {
            leads.emplace_back(QCoreApplication::translate("WildSearcherModel3", "Cute Charm: ♀ Lead"));
        }
        if ((flags & (1 << 3)) != 0)
        {
            leads.emplace_back(QCoreApplication::translate("WildSearcherModel3", "Magnet Pull"));
        }
        if ((flags & (1 << 4)) != 0)
        {
            leads.emplace_back(QCoreApplication::translate("WildSearcherModel3", "Static"));
        }
        if ((flags & (1 << 5)) != 0)
        {
            leads.emplace_back(QCoreApplication::translate("WildSearcherModel3", "Hustle / Pressure / Vital Spirit"));
        }

        return leads.join(" / ");
    }

    if (lead <= Lead::SynchronizeEnd)
    {
        return getSynchronizeLeadName3(lead, synchronizeFlags, targetNature);
    }

    switch (lead)
    {
    case Lead::CuteCharmM:
        return QCoreApplication::translate("WildSearcherModel3", "Cute Charm: ♂ Lead");
    case Lead::CuteCharmF:
        return QCoreApplication::translate("WildSearcherModel3", "Cute Charm: ♀ Lead");
    case Lead::MagnetPull:
        return QCoreApplication::translate("WildSearcherModel3", "Magnet Pull");
    case Lead::Static:
        return QCoreApplication::translate("WildSearcherModel3", "Static");
    case Lead::Pressure:
        return QCoreApplication::translate("WildSearcherModel3", "Hustle / Pressure / Vital Spirit");
    default:
        return QString();
    }
}

WildGeneratorModel3::WildGeneratorModel3(QObject *parent) : TableModel(parent), showStats(false)
{
}

int WildGeneratorModel3::columnCount(const QModelIndex &parent) const
{
    return 16;
}

QVariant WildGeneratorModel3::data(const QModelIndex &index, int role) const
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
            return QString("%1: %2")
                .arg(state.getEncounterSlot())
                .arg(QString::fromStdString(Translator::getSpecie(state.getSpecie(), state.getForm())));
        case 2:
            return state.getLevel();
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
        }
    }
    return QVariant();
}

QVariant WildGeneratorModel3::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

void WildGeneratorModel3::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, 7), index(rowCount() - 1, 12), { Qt::DisplayRole });
}

WildSearcherModel3::WildSearcherModel3(QObject *parent) : TableModel(parent), showStats(false)
{
}

int WildSearcherModel3::columnCount(const QModelIndex &parent) const
{
    return 17;
}

QVariant WildSearcherModel3::data(const QModelIndex &index, int role) const
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
            return getLeadName3(state.getLead(), state.getSynchronizeLead(), state.getSynchronizeLeadFlags(), state.getLeadFlags(),
                                state.getNature());
        case 2:
            return QString("%1: %2")
                .arg(state.getEncounterSlot())
                .arg(QString::fromStdString(Translator::getSpecie(state.getSpecie(), state.getForm())));
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
            return QString("%1: %2").arg(state.getAbility()).arg(QString::fromStdString(Translator::getAbility(state.getAbilityIndex())));
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
        }
    }
    return QVariant();
}

QVariant WildSearcherModel3::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

void WildSearcherModel3::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, 8), index(rowCount() - 1, 13), { Qt::DisplayRole });
}
