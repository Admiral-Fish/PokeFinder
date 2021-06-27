/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

WildGeneratorModel3::WildGeneratorModel3(QObject *parent) : TableModel<WildState>(parent)
{
}

int WildGeneratorModel3::columnCount(const QModelIndex & /*parent*/) const
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
            return state.getEncounterSlot();
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
            return state.getAbility();
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
            return state.getIV(static_cast<u8>(column - 7));
        case 13:
            return QString::fromStdString(Translator::getHiddenPower(state.getHidden()));
        case 14:
            return state.getPower();
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

WildSearcherModel3::WildSearcherModel3(QObject *parent) : TableModel<WildState>(parent)
{
}

void WildSearcherModel3::sort(int column, Qt::SortOrder order)
{
    if (!model.empty())
    {
        emit layoutAboutToBeChanged();
        bool flag = order == Qt::AscendingOrder;
        switch (column)
        {
        case 0:
            std::sort(model.begin(), model.end(), [flag](const WildState &state1, const WildState &state2) {
                return flag ? state1.getSeed() < state2.getSeed() : state1.getSeed() > state2.getSeed();
            });
            break;
        case 1:
            std::sort(model.begin(), model.end(), [flag](const WildState &state1, const WildState &state2) {
                return flag ? state1.getLead() < state2.getLead() : state1.getLead() > state2.getLead();
            });
            break;
        case 2:
            std::sort(model.begin(), model.end(), [flag](const WildState &state1, const WildState &state2) {
                return flag ? state1.getEncounterSlot() < state2.getEncounterSlot() : state1.getEncounterSlot() > state2.getEncounterSlot();
            });
            break;
        case 3:
            std::sort(model.begin(), model.end(), [flag](const WildState &state1, const WildState &state2) {
                return flag ? state1.getLevel() < state2.getLevel() : state1.getLevel() > state2.getLevel();
            });
            break;
        case 4:
            std::sort(model.begin(), model.end(), [flag](const WildState &state1, const WildState &state2) {
                return flag ? state1.getPID() < state2.getPID() : state1.getPID() > state2.getPID();
            });
            break;
        case 5:
            std::sort(model.begin(), model.end(), [flag](const WildState &state1, const WildState &state2) {
                return flag ? state1.getShiny() < state2.getShiny() : state1.getShiny() > state2.getShiny();
            });
            break;
        case 6:
            std::sort(model.begin(), model.end(), [flag](const WildState &state1, const WildState &state2) {
                return flag ? state1.getNature() < state2.getNature() : state1.getNature() > state2.getNature();
            });
            break;
        case 7:
            std::sort(model.begin(), model.end(), [flag](const WildState &state1, const WildState &state2) {
                return flag ? state1.getAbility() < state2.getAbility() : state1.getAbility() > state2.getAbility();
            });
            break;
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            std::sort(model.begin(), model.end(), [flag, column](const WildState &state1, const WildState &state2) {
                return flag ? state1.getIV(static_cast<u8>(column - 8)) < state2.getIV(static_cast<u8>(column - 8))
                            : state1.getIV(static_cast<u8>(column - 8)) > state2.getIV(static_cast<u8>(column - 8));
            });
            break;
        case 14:
            std::sort(model.begin(), model.end(), [flag](const WildState &state1, const WildState &state2) {
                return flag ? state1.getHidden() < state2.getHidden() : state1.getHidden() > state2.getHidden();
            });
            break;
        case 15:
            std::sort(model.begin(), model.end(), [flag](const WildState &state1, const WildState &state2) {
                return flag ? state1.getPower() < state2.getPower() : state1.getPower() > state2.getPower();
            });
            break;
        case 16:
            std::sort(model.begin(), model.end(), [flag](const WildState &state1, const WildState &state2) {
                return flag ? state1.getGender() < state2.getGender() : state1.getGender() > state2.getGender();
            });
            break;
        }

        emit layoutChanged();
    }
}

int WildSearcherModel3::columnCount(const QModelIndex &parent) const
{
    (void)parent;
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
        {
            auto lead = state.getLead();
            return lead == Lead::None ? tr("None") : lead == Lead::Synchronize ? tr("Synch") : tr("Cute Charm");
        }
        case 2:
            return state.getEncounterSlot();
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
            return state.getAbility();
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            return state.getIV(static_cast<u8>(column - 8));
        case 14:
            return QString::fromStdString(Translator::getHiddenPower(state.getHidden()));
        case 15:
            return state.getPower();
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
