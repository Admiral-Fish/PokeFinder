/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "EggModel5.hpp"
#include <Core/Util/Translator.hpp>
#include <Core/Util/Utilities.hpp>

EggGeneratorModel5::EggGeneratorModel5(QObject *parent) : TableModel<EggState5>(parent), showInheritance(false), showStats(false)
{
}

int EggGeneratorModel5::columnCount(const QModelIndex &parent) const
{
    return 15;
}

QVariant EggGeneratorModel5::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &state = model[index.row()];
        int column = index.column();
        switch (index.column())
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
            return QString::fromStdString(*Translator::getNature(state.getNature()));
        case 5:
            if (state.getAbility() == 0 || state.getAbility() == 1)
            {
                return QString("%1 (%2)")
                    .arg(state.getAbility())
                    .arg(QString::fromStdString(*Translator::getAbility(state.getAbilityIndex())));
            }
            else
            {
                return QString("H (%2)").arg(QString::fromStdString(*Translator::getAbility(state.getAbilityIndex())));
            }
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            if (showInheritance)
            {
                u8 inh = state.getInheritance(column - 6);
                if (inh)
                {
                    return inh == 1 ? "A" : "B";
                }
            }
            return showStats ? state.getStat(column - 6) : state.getIV(column - 6);
        case 12:
            return QString::fromStdString(*Translator::getHiddenPower(state.getHiddenPower()));
        case 13:
            return state.getHiddenPowerStrength();
        case 14:
            return QString::fromStdString(*Translator::getGender(state.getGender()));
        }
    }

    return QVariant();
}

QVariant EggGeneratorModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

void EggGeneratorModel5::setShowInheritance(bool flag)
{
    showInheritance = flag;
    emit dataChanged(index(0, 6), index(rowCount(), 11), { Qt::DisplayRole });
}

void EggGeneratorModel5::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, 6), index(rowCount(), 11), { Qt::DisplayRole });
}

EggSearcherModel5::EggSearcherModel5(QObject *parent) :
    TableModel<SearcherState5<EggState5>>(parent), showInheritance(false), showStats(false)
{
}
int EggSearcherModel5::columnCount(const QModelIndex &parent) const
{
    return 18;
}

QVariant EggSearcherModel5::data(const QModelIndex &index, int role) const
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
            return QString::number(state.getPID(), 16).toUpper().rightJustified(8, '0');
        case 3:
        {
            u8 shiny = state.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 4:
            return QString::fromStdString(*Translator::getNature(state.getNature()));
        case 5:
            if (state.getAbility() == 0 || state.getAbility() == 1)
            {
                return QString("%1 (%2)")
                    .arg(state.getAbility())
                    .arg(QString::fromStdString(*Translator::getAbility(state.getAbilityIndex())));
            }
            else
            {
                return QString("H (%2)").arg(QString::fromStdString(*Translator::getAbility(state.getAbilityIndex())));
            }
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            if (showInheritance)
            {
                u8 inh = state.getInheritance(column - 6);
                if (inh)
                {
                    return inh == 1 ? "A" : "B";
                }
            }
            return showStats ? state.getStat(column - 6) : state.getIV(column - 6);
        case 12:
            return QString::fromStdString(*Translator::getHiddenPower(state.getHiddenPower()));
        case 13:
            return state.getHiddenPowerStrength();
        case 14:
            return QString::fromStdString(*Translator::getGender(state.getGender()));
        case 15:
            return QString::fromStdString(display.getDateTime().toString());
        case 16:
            return QString::number(display.getTimer0(), 16).toUpper();
        case 17:
            return QString::fromStdString(Translator::getKeypresses(display.getButtons()));
        }
    }

    return QVariant();
}

QVariant EggSearcherModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

void EggSearcherModel5::sort(int column, Qt::SortOrder order)
{
    if (!model.empty())
    {
        emit layoutAboutToBeChanged();
        bool flag = order == Qt::AscendingOrder;
        switch (column)
        {
        case 0:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<EggState5> &state1, const SearcherState5<EggState5> &state2) {
                return flag ? state1.getInitialSeed() < state2.getInitialSeed() : state1.getInitialSeed() > state2.getInitialSeed();
            });
            break;
        case 1:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<EggState5> &state1, const SearcherState5<EggState5> &state2) {
                return flag ? state1.getState().getAdvances() < state2.getState().getAdvances()
                            : state1.getState().getAdvances() > state2.getState().getAdvances();
            });
            break;
        case 2:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<EggState5> &state1, const SearcherState5<EggState5> &state2) {
                return flag ? state1.getState().getPID() < state2.getState().getPID()
                            : state1.getState().getPID() > state2.getState().getPID();
            });
            break;
        case 3:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<EggState5> &state1, const SearcherState5<EggState5> &state2) {
                return flag ? state1.getState().getShiny() < state2.getState().getShiny()
                            : state1.getState().getShiny() > state2.getState().getShiny();
            });
            break;
        case 4:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<EggState5> &state1, const SearcherState5<EggState5> &state2) {
                return flag ? state1.getState().getNature() < state2.getState().getNature()
                            : state1.getState().getNature() > state2.getState().getNature();
            });
            break;
        case 5:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<EggState5> &state1, const SearcherState5<EggState5> &state2) {
                return flag ? state1.getState().getAbility() < state2.getState().getAbility()
                            : state1.getState().getAbility() > state2.getState().getAbility();
            });
            break;
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            std::sort(model.begin(), model.end(),
                      [flag, column](const SearcherState5<EggState5> &state1, const SearcherState5<EggState5> &state2) {
                          return flag ? state1.getState().getIV(column - 6) < state2.getState().getIV(column - 6)
                                      : state1.getState().getIV(column - 6) > state2.getState().getIV(column - 6);
                      });
            break;
        case 12:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<EggState5> &state1, const SearcherState5<EggState5> &state2) {
                return flag ? state1.getState().getHiddenPower() < state2.getState().getHiddenPower()
                            : state1.getState().getHiddenPower() > state2.getState().getHiddenPower();
            });
            break;
        case 13:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<EggState5> &state1, const SearcherState5<EggState5> &state2) {
                return flag ? state1.getState().getHiddenPowerStrength() < state2.getState().getHiddenPowerStrength()
                            : state1.getState().getHiddenPowerStrength() > state2.getState().getHiddenPowerStrength();
            });
            break;
        case 14:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<EggState5> &state1, const SearcherState5<EggState5> &state2) {
                return flag ? state1.getState().getGender() < state2.getState().getGender()
                            : state1.getState().getGender() > state2.getState().getGender();
            });
            break;
        case 15:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<EggState5> &state1, const SearcherState5<EggState5> &state2) {
                return flag ? state1.getDateTime() < state2.getDateTime() : state1.getDateTime() > state2.getDateTime();
            });
            break;
        case 16:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<EggState5> &state1, const SearcherState5<EggState5> &state2) {
                return flag ? state1.getTimer0() < state2.getTimer0() : state1.getTimer0() > state2.getTimer0();
            });
            break;
        case 17:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<EggState5> &state1, const SearcherState5<EggState5> &state2) {
                return flag ? state1.getButtons() < state2.getButtons() : state1.getButtons() > state2.getButtons();
            });
            break;
        }
    }
}

void EggSearcherModel5::setShowInheritance(bool flag)
{
    showInheritance = flag;
    emit dataChanged(index(0, 6), index(rowCount(), 11), { Qt::DisplayRole });
}

void EggSearcherModel5::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, 6), index(rowCount(), 11), { Qt::DisplayRole });
}
