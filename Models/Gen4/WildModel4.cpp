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

#include "WildModel4.hpp"
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Util/Translator.hpp>

WildGeneratorModel4::WildGeneratorModel4(QObject *parent, Method method) : TableModel<WildState>(parent), method(method)
{
}

void WildGeneratorModel4::setMethod(Method method)
{
    this->method = method;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

int WildGeneratorModel4::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    switch (method)
    {
    case Method::MethodJ:
        return 17;
    case Method::MethodK:
        return 18;
    case Method::ChainedShiny:
        return 14;
    default:
        return 0;
    }
}

QVariant WildGeneratorModel4::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &currentState = model.at(index.row());
        int column = getColumn(index.column());
        switch (column)
        {
        case 0:
            return currentState.getAdvance();
        case 1:
        {
            u8 call = currentState.getSeed() % 3;
            return call == 0 ? "E" : call == 1 ? "K" : "P";
        }
        case 2:
        {
            u8 val = ((currentState.getSeed() & 0x1fff) * 100) >> 13;
            QString pitch;
            if (val < 20)
            {
                pitch = tr("Low");
            }
            else if (val < 40)
            {
                pitch = tr("Mid-Low");
            }
            else if (val < 60)
            {
                pitch = tr("Mid");
            }
            else if (val < 80)
            {
                pitch = tr("Mid-High");
            }
            else
            {
                pitch = tr("High");
            }

            pitch += " (" + QString::number(val) + ")";
            return pitch;
        }
        case 3:
            return currentState.getEncounterSlot();
        case 4:
            return currentState.getLevel();
        case 5:
            return QString::number(currentState.getPID(), 16).toUpper().rightJustified(8, '0');
        case 6:
        {
            u8 shiny = currentState.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 7:
            return Translator::getNature(currentState.getNature());
        case 8:
            return currentState.getAbility();
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
            return currentState.getIV(static_cast<u8>(column - 9));
        case 15:
            return Translator::getHiddenPower(currentState.getHidden());
        case 16:
            return currentState.getPower();
        case 17:
            return Translator::getGender(currentState.getGender());
        }
    }
    return QVariant();
}

QVariant WildGeneratorModel4::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        section = getColumn(section);

        return header.at(section);
    }
    return QVariant();
}

int WildGeneratorModel4::getColumn(int column) const
{
    switch (method)
    {
    case Method::MethodJ:
        return column > 0 ? column + 1 : column;
    case Method::MethodK:
    default:
        return column;
    case Method::ChainedShiny:
        return column > 0 ? column + 4 : column;
    }
}

WildSearcherModel4::WildSearcherModel4(QObject *parent, Method method) : TableModel<WildState>(parent), method(method)
{
}

void WildSearcherModel4::setMethod(Method method)
{
    this->method = method;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

void WildSearcherModel4::sort(int column, Qt::SortOrder order)
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
                return flag ? currentState1.getAdvance() < currentState2.getAdvance()
                            : currentState1.getAdvance() > currentState2.getAdvance();
            });
            break;
        case 2:
            std::sort(model.begin(), model.end(), [flag](const WildState &currentState1, const WildState &currentState2) {
                return flag ? currentState1.getLead() < currentState2.getLead() : currentState1.getLead() > currentState2.getLead();
            });
            break;
        case 3:
            std::sort(model.begin(), model.end(), [flag](const WildState &currentState1, const WildState &currentState2) {
                return flag ? currentState1.getEncounterSlot() < currentState2.getEncounterSlot()
                            : currentState1.getEncounterSlot() > currentState2.getEncounterSlot();
            });
            break;
        case 4:
            std::sort(model.begin(), model.end(), [flag](const WildState &currentState1, const WildState &currentState2) {
                return flag ? currentState1.getLevel() < currentState2.getLevel() : currentState1.getLevel() > currentState2.getLevel();
            });
            break;
        case 5:
            std::sort(model.begin(), model.end(), [flag](const WildState &currentState1, const WildState &currentState2) {
                return flag ? currentState1.getPID() < currentState2.getPID() : currentState1.getPID() > currentState2.getPID();
            });
            break;
        case 6:
            std::sort(model.begin(), model.end(), [flag](const WildState &currentState1, const WildState &currentState2) {
                return flag ? currentState1.getShiny() < currentState2.getShiny() : currentState1.getShiny() > currentState2.getShiny();
            });
            break;
        case 7:
            std::sort(model.begin(), model.end(), [flag](const WildState &currentState1, const WildState &currentState2) {
                return flag ? currentState1.getNature() < currentState2.getNature() : currentState1.getNature() > currentState2.getNature();
            });
            break;
        case 8:
            std::sort(model.begin(), model.end(), [flag](const WildState &currentState1, const WildState &currentState2) {
                return flag ? currentState1.getAbility() < currentState2.getAbility()
                            : currentState1.getAbility() > currentState2.getAbility();
            });
            break;
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
            std::sort(model.begin(), model.end(), [flag, column](const WildState &currentState1, const WildState &currentState2) {
                return flag ? currentState1.getIV(static_cast<u8>(column - 9)) < currentState2.getIV(static_cast<u8>(column - 9))
                            : currentState1.getIV(static_cast<u8>(column - 9)) > currentState2.getIV(static_cast<u8>(column - 9));
            });
            break;
        case 15:
            std::sort(model.begin(), model.end(), [flag](const WildState &currentState1, const WildState &currentState2) {
                return flag ? currentState1.getHidden() < currentState2.getHidden() : currentState1.getHidden() > currentState2.getHidden();
            });
            break;
        case 16:
            std::sort(model.begin(), model.end(), [flag](const WildState &currentState1, const WildState &currentState2) {
                return flag ? currentState1.getPower() < currentState2.getPower() : currentState1.getPower() > currentState2.getPower();
            });
            break;
        case 17:
            std::sort(model.begin(), model.end(), [flag](const WildState &currentState1, const WildState &currentState2) {
                return flag ? currentState1.getGender() < currentState2.getGender() : currentState1.getGender() > currentState2.getGender();
            });
            break;
        }

        emit layoutChanged();
    }
}

int WildSearcherModel4::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    switch (method)
    {
    case Method::MethodJ:
        return 18;
    case Method::MethodK:
        return 18;
    case Method::ChainedShiny:
        return 15;
    default:
        return 0;
    }
}

QVariant WildSearcherModel4::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &currentState = model.at(index.row());
        int column = getColumn(index.column());
        switch (column)
        {
        case 0:
            return QString::number(currentState.getSeed(), 16).toUpper().rightJustified(8, '0');
        case 1:
            return currentState.getAdvance();
        case 2:
            switch (currentState.getLead())
            {
            case Lead::None:
                return tr("None");
            case Lead::Synchronize:
                return tr("Synchronize");
            case Lead::SuctionCups:
                return tr("Suction Cups");
            case Lead::CuteCharmFemale:
                return tr("Cute Charm (♀)");
            case Lead::CuteCharm25M:
                return tr("Cute Charm (25% ♂)");
            case Lead::CuteCharm50M:
                return tr("Cute Charm (50% ♂)");
            case Lead::CuteCharm75M:
                return tr("Cute Charm (75% ♂)");
            case Lead::CuteCharm875M:
            default:
                return tr("Cute Charm (87.5% ♂)");
            }
        case 3:
            return currentState.getEncounterSlot();
        case 4:
            return currentState.getLevel();
        case 5:
            return QString::number(currentState.getPID(), 16).toUpper().rightJustified(8, '0');
        case 6:
        {
            u8 shiny = currentState.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 7:
            return Translator::getNature(currentState.getNature());
        case 8:
            return currentState.getAbility();
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
            return currentState.getIV(static_cast<u8>(column - 9));
        case 15:
            return Translator::getHiddenPower(currentState.getHidden());
        case 16:
            return currentState.getPower();
        case 17:
            return Translator::getGender(currentState.getGender());
        }
    }
    return QVariant();
}

QVariant WildSearcherModel4::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        section = getColumn(section);

        return header.at(section);
    }
    return QVariant();
}

int WildSearcherModel4::getColumn(int column) const
{
    switch (method)
    {
    case Method::MethodJ:
    case Method::MethodK:
    default:
        return column;
    case Method::ChainedShiny:
        return column > 1 ? column + 3 : column;
    }
}
