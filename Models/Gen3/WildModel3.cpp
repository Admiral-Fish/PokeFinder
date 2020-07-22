/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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
        const auto &currentState = model.at(index.row());
        int column = index.column();
        switch (column)
        {
        case 0:
            return currentState.getAdvance();
        case 1:
            return currentState.getEncounterSlot();
        case 2:
            return currentState.getLevel();
        case 3:
            return QString::number(currentState.getPID(), 16).toUpper().rightJustified(8, '0');
        case 4:
        {
            u8 shiny = currentState.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 5:
            return Translator::getNature(currentState.getNature());
        case 6:
            return currentState.getAbility();
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
            return currentState.getIV(static_cast<u8>(column - 7));
        case 13:
            return Translator::getHiddenPower(currentState.getHidden());
        case 14:
            return currentState.getPower();
        case 15:
            return Translator::getGender(currentState.getGender());
        }
    }
    return QVariant();
}

QVariant WildGeneratorModel3::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header.at(section);
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
            std::sort(model.begin(), model.end(), [flag](const WildState &currentState1, const WildState &currentState2) {
                return flag ? currentState1.getSeed() < currentState2.getSeed() : currentState1.getSeed() > currentState2.getSeed();
            });
            break;
        case 1:
            std::sort(model.begin(), model.end(), [flag](const WildState &currentState1, const WildState &currentState2) {
                return flag ? currentState1.getLead() < currentState2.getLead() : currentState1.getLead() > currentState2.getLead();
            });
            break;
        case 2:
            std::sort(model.begin(), model.end(), [flag](const WildState &currentState1, const WildState &currentState2) {
                return flag ? currentState1.getEncounterSlot() < currentState2.getEncounterSlot()
                            : currentState1.getEncounterSlot() > currentState2.getEncounterSlot();
            });
            break;
        case 3:
            std::sort(model.begin(), model.end(), [flag](const WildState &currentState1, const WildState &currentState2) {
                return flag ? currentState1.getLevel() < currentState2.getLevel() : currentState1.getLevel() > currentState2.getLevel();
            });
            break;
        case 4:
            std::sort(model.begin(), model.end(), [flag](const WildState &currentState1, const WildState &currentState2) {
                return flag ? currentState1.getPID() < currentState2.getPID() : currentState1.getPID() > currentState2.getPID();
            });
            break;
        case 5:
            std::sort(model.begin(), model.end(), [flag](const WildState &currentState1, const WildState &currentState2) {
                return flag ? currentState1.getShiny() < currentState2.getShiny() : currentState1.getShiny() > currentState2.getShiny();
            });
            break;
        case 6:
            std::sort(model.begin(), model.end(), [flag](const WildState &currentState1, const WildState &currentState2) {
                return flag ? currentState1.getNature() < currentState2.getNature() : currentState1.getNature() > currentState2.getNature();
            });
            break;
        case 7:
            std::sort(model.begin(), model.end(), [flag](const WildState &currentState1, const WildState &currentState2) {
                return flag ? currentState1.getAbility() < currentState2.getAbility()
                            : currentState1.getAbility() > currentState2.getAbility();
            });
            break;
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            std::sort(model.begin(), model.end(), [flag, column](const WildState &currentState1, const WildState &currentState2) {
                return flag ? currentState1.getIV(static_cast<u8>(column - 8)) < currentState2.getIV(static_cast<u8>(column - 8))
                            : currentState1.getIV(static_cast<u8>(column - 8)) > currentState2.getIV(static_cast<u8>(column - 8));
            });
            break;
        case 14:
            std::sort(model.begin(), model.end(), [flag](const WildState &currentState1, const WildState &currentState2) {
                return flag ? currentState1.getHidden() < currentState2.getHidden() : currentState1.getHidden() > currentState2.getHidden();
            });
            break;
        case 15:
            std::sort(model.begin(), model.end(), [flag](const WildState &currentState1, const WildState &currentState2) {
                return flag ? currentState1.getPower() < currentState2.getPower() : currentState1.getPower() > currentState2.getPower();
            });
            break;
        case 16:
            std::sort(model.begin(), model.end(), [flag](const WildState &currentState1, const WildState &currentState2) {
                return flag ? currentState1.getGender() < currentState2.getGender() : currentState1.getGender() > currentState2.getGender();
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
        const auto &currentState = model.at(index.row());
        int column = index.column();
        switch (column)
        {
        case 0:
            return QString::number(currentState.getSeed(), 16).toUpper().rightJustified(8, '0');
        case 1:
        {
            auto lead = currentState.getLead();
            return lead == Lead::None ? tr("None") : lead == Lead::Synchronize ? tr("Synch") : tr("Cute Charm");
        }
        case 2:
            return currentState.getEncounterSlot();
        case 3:
            return currentState.getLevel();
        case 4:
            return QString::number(currentState.getPID(), 16).toUpper().rightJustified(8, '0');
        case 5:
        {
            u8 shiny = currentState.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 6:
            return Translator::getNature(currentState.getNature());
        case 7:
            return currentState.getAbility();
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            return currentState.getIV(static_cast<u8>(column - 8));
        case 14:
            return Translator::getHiddenPower(currentState.getHidden());
        case 15:
            return currentState.getPower();
        case 16:
            return Translator::getGender(currentState.getGender());
        }
    }
    return QVariant();
}

QVariant WildSearcherModel3::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header.at(section);
    }
    return QVariant();
}
