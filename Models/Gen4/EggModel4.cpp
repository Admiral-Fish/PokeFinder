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

#include "EggModel4.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Util/Translator.hpp>

EggGeneratorModel4::EggGeneratorModel4(QObject *parent, Method method) :
    TableModel<EggState4>(parent), method(method), showInheritance(false)
{
}

void EggGeneratorModel4::setMethod(Method method)
{
    this->method = method;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

int EggGeneratorModel4::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    switch (method)
    {
    case Method::Gen4Normal:
    case Method::Gen4Masuada:
        return 6;
    case Method::DPPtIVs:
        return 10;
    case Method::HGSSIVs:
        return 11;
    default:
        return 0;
    }
}

QVariant EggGeneratorModel4::data(const QModelIndex &index, int role) const
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
            if (showInheritance)
            {
                u8 inh = currentState.getInheritance(static_cast<u8>(column - 7));
                if (inh)
                {
                    return inh == 1 ? "A" : "B";
                }
            }
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

QVariant EggGeneratorModel4::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        section = getColumn(section);

        return header.at(section);
    }
    return QVariant();
}

void EggGeneratorModel4::toggleInheritance(bool flag)
{
    beginResetModel();
    showInheritance = flag;
    endResetModel();
}

int EggGeneratorModel4::getColumn(int column) const
{
    switch (method)
    {
    case Method::Gen4Normal:
    case Method::Gen4Masuada:
        return column == 5 ? column + 10 : column > 0 ? column + 2 : column;
    case Method::DPPtIVs:
        return column == 1 ? column + 1 : column > 1 ? column + 5 : column;
    case Method::HGSSIVs:
        return column > 2 ? column + 4 : column;
    default:
        return column;
    }
}

EggSearcherModel4::EggSearcherModel4(QObject *parent, Method method) : TableModel<EggState4>(parent), method(method), showInheritance(false)
{
}

void EggSearcherModel4::setMethod(Method method)
{
    this->method = method;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

void EggSearcherModel4::sort(int column, Qt::SortOrder order)
{
    if (!model.empty())
    {
        emit layoutAboutToBeChanged();
        bool flag = order == Qt::AscendingOrder;
        switch (column)
        {
        case 0:
            std::sort(model.begin(), model.end(), [flag](const EggState4 &currentState1, const EggState4 &currentState2) {
                return flag ? currentState1.getInitialSeed() < currentState2.getInitialSeed()
                            : currentState1.getInitialSeed() > currentState2.getInitialSeed();
            });
            break;
        case 1:
        case 2:
            std::sort(model.begin(), model.end(), [flag](const EggState4 &currentState1, const EggState4 &currentState2) {
                return flag ? currentState1.getAdvance() < currentState2.getAdvance()
                            : currentState1.getAdvance() > currentState2.getAdvance();
            });
            break;
        case 3:
            std::sort(model.begin(), model.end(), [flag](const EggState4 &currentState1, const EggState4 &currentState2) {
                return flag ? currentState1.getSecondaryAdvance() < currentState2.getSecondaryAdvance()
                            : currentState1.getSecondaryAdvance() > currentState2.getSecondaryAdvance();
            });
            break;
        case 4:
            std::sort(model.begin(), model.end(), [flag](const EggState4 &currentState1, const EggState4 &currentState2) {
                return flag ? currentState1.getPID() < currentState2.getPID() : currentState1.getPID() > currentState2.getPID();
            });
            break;
        case 5:
            std::sort(model.begin(), model.end(), [flag](const EggState4 &currentState1, const EggState4 &currentState2) {
                return flag ? currentState1.getShiny() < currentState2.getShiny() : currentState1.getShiny() > currentState2.getShiny();
            });
            break;
        case 6:
            std::sort(model.begin(), model.end(), [flag](const EggState4 &currentState1, const EggState4 &currentState2) {
                return flag ? currentState1.getNature() < currentState2.getNature() : currentState1.getNature() > currentState2.getNature();
            });
            break;
        case 7:
            std::sort(model.begin(), model.end(), [flag](const EggState4 &currentState1, const EggState4 &currentState2) {
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
            std::sort(model.begin(), model.end(), [flag, column](const EggState4 &currentState1, const EggState4 &currentState2) {
                return flag ? currentState1.getIV(static_cast<u8>(column - 8)) < currentState2.getIV(static_cast<u8>(column - 8))
                            : currentState1.getIV(static_cast<u8>(column - 8)) > currentState2.getIV(static_cast<u8>(column - 8));
            });
            break;
        case 14:
            std::sort(model.begin(), model.end(), [flag](const EggState4 &currentState1, const EggState4 &currentState2) {
                return flag ? currentState1.getHidden() < currentState2.getHidden() : currentState1.getHidden() > currentState2.getHidden();
            });
            break;
        case 15:
            std::sort(model.begin(), model.end(), [flag](const EggState4 &currentState1, const EggState4 &currentState2) {
                return flag ? currentState1.getPower() < currentState2.getPower() : currentState1.getPower() > currentState2.getPower();
            });
            break;
        case 16:
            std::sort(model.begin(), model.end(), [flag](const EggState4 &currentState1, const EggState4 &currentState2) {
                return flag ? currentState1.getGender() < currentState2.getGender() : currentState1.getGender() > currentState2.getGender();
            });
            break;
        }

        emit layoutChanged();
    }
}

int EggSearcherModel4::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    switch (method)
    {
    case Method::Gen4Normal:
    case Method::Gen4Masuada:
        return 7;
    case Method::DPPtIVs:
    case Method::HGSSIVs:
        return 10;
    case Method::Gen4Combined:
        return 16;
    default:
        return 0;
    }
}

QVariant EggSearcherModel4::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &currentState = model.at(index.row());
        int column = getColumn(index.column());
        switch (column)
        {
        case 0:
            return QString::number(currentState.getInitialSeed(), 16).toUpper().rightJustified(8, '0');
        case 1:
        case 2:
            return currentState.getAdvance();
        case 3:
            return currentState.getSecondaryAdvance();
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
            if (showInheritance)
            {
                u8 inh = currentState.getInheritance(static_cast<u8>(column - 8));
                if (inh)
                {
                    return inh == 1 ? "A" : "B";
                }
            }
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

QVariant EggSearcherModel4::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        section = getColumn(section);

        return header.at(section);
    }
    return QVariant();
}

void EggSearcherModel4::toggleInheritance(bool flag)
{
    beginResetModel();
    showInheritance = flag;
    endResetModel();
}

int EggSearcherModel4::getColumn(int column) const
{
    switch (method)
    {
    case Method::Gen4Normal:
    case Method::Gen4Masuada:
        return column == 6 ? column + 10 : column > 1 ? column + 2 : column;
    case Method::DPPtIVs:
    case Method::HGSSIVs:
        return column > 1 ? column + 6 : column;
    case Method::Gen4Combined:
        return column > 0 ? column + 1 : column;
    default:
        return column;
    }
}
