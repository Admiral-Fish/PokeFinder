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

#include "HiddenGrottoModel.hpp"
#include <Core/Util/Translator.hpp>
#include <Core/Util/Utilities.hpp>

HiddenGrottoSlotGeneratorModel5::HiddenGrottoSlotGeneratorModel5(QObject *parent) : TableModel(parent)
{
}

int HiddenGrottoSlotGeneratorModel5::columnCount(const QModelIndex &parent) const
{
    return 4;
}

QVariant HiddenGrottoSlotGeneratorModel5::data(const QModelIndex &index, int role) const
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
            return state.getGroup();
        case 3:
            if (state.getItem())
            {
                return QString("%1: %2").arg(state.getSlot()).arg(QString::fromStdString(Translator::getItem(state.getData())));
            }
            else
            {
                return QString("%1 (%2 %3)")
                    .arg(state.getSlot())
                    .arg(QString::fromStdString(Translator::getSpecie(state.getData())),
                         QString::fromStdString(Translator::getGender(state.getGender())));
            }
        }
    }
    return QVariant();
}

QVariant HiddenGrottoSlotGeneratorModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

HiddenGrottoSlotSearcherModel5::HiddenGrottoSlotSearcherModel5(QObject *parent) : TableModel(parent)
{
}

int HiddenGrottoSlotSearcherModel5::columnCount(const QModelIndex &parent) const
{
    return 7;
}

QVariant HiddenGrottoSlotSearcherModel5::data(const QModelIndex &index, int role) const
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
            return state.getGroup();
        case 3:
            if (state.getItem())
            {
                return QString("%1: %2").arg(state.getSlot()).arg(QString::fromStdString(Translator::getItem(state.getData())));
            }
            else
            {
                return QString("%1 (%2 %3)")
                    .arg(state.getSlot())
                    .arg(QString::fromStdString(Translator::getSpecie(state.getData())),
                         QString::fromStdString(Translator::getGender(state.getGender())));
            }
        case 4:
            return QString::fromStdString(display.getDateTime().toString());
        case 5:
            return QString::number(display.getTimer0(), 16).toUpper();
        case 6:
            return QString::fromStdString(Translator::getKeypresses(display.getButtons()));
        }
    }
    return QVariant();
}

QVariant HiddenGrottoSlotSearcherModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

void HiddenGrottoSlotSearcherModel5::sort(int column, Qt::SortOrder order)
{
    if (!model.empty())
    {
        emit layoutAboutToBeChanged();
        bool flag = order == Qt::AscendingOrder;
        switch (column)
        {
        case 0:
            std::sort(model.begin(), model.end(),
                      [flag](const SearcherState5<HiddenGrottoState> &state1, const SearcherState5<HiddenGrottoState> &state2) {
                          return flag ? state1.getInitialSeed() < state2.getInitialSeed()
                                      : state1.getInitialSeed() > state2.getInitialSeed();
                      });
            break;
        case 1:
            std::sort(model.begin(), model.end(),
                      [flag](const SearcherState5<HiddenGrottoState> &state1, const SearcherState5<HiddenGrottoState> &state2) {
                          return flag ? state1.getState().getAdvances() < state2.getState().getAdvances()
                                      : state1.getState().getAdvances() > state2.getState().getAdvances();
                      });
            break;
        case 2:
            std::sort(model.begin(), model.end(),
                      [flag](const SearcherState5<HiddenGrottoState> &state1, const SearcherState5<HiddenGrottoState> &state2) {
                          return flag ? state1.getState().getGroup() < state2.getState().getGroup()
                                      : state1.getState().getGroup() > state2.getState().getGroup();
                      });
            break;
        case 3:
            std::sort(model.begin(), model.end(),
                      [flag](const SearcherState5<HiddenGrottoState> &state1, const SearcherState5<HiddenGrottoState> &state2) {
                          return flag ? state1.getState().getSlot() < state2.getState().getSlot()
                                      : state1.getState().getSlot() > state2.getState().getSlot();
                      });
            break;
        case 4:
            std::sort(model.begin(), model.end(),
                      [flag](const SearcherState5<HiddenGrottoState> &state1, const SearcherState5<HiddenGrottoState> &state2) {
                          return flag ? state1.getDateTime() < state2.getDateTime() : state1.getDateTime() > state2.getDateTime();
                      });
            break;
        case 5:
            std::sort(model.begin(), model.end(),
                      [flag](const SearcherState5<HiddenGrottoState> &state1, const SearcherState5<HiddenGrottoState> &state2) {
                          return flag ? state1.getTimer0() < state2.getTimer0() : state1.getTimer0() > state2.getTimer0();
                      });
            break;
        case 6:
            std::sort(model.begin(), model.end(),
                      [flag](const SearcherState5<HiddenGrottoState> &state1, const SearcherState5<HiddenGrottoState> &state2) {
                          return flag ? state1.getButtons() < state2.getButtons() : state1.getButtons() > state2.getButtons();
                      });
            break;
        }
    }
}

HiddenGrottoGeneratorModel5::HiddenGrottoGeneratorModel5(QObject *parent) : TableModel(parent), showStats(false)
{
}

int HiddenGrottoGeneratorModel5::columnCount(const QModelIndex &parent) const
{
    return 17;
}

QVariant HiddenGrottoGeneratorModel5::data(const QModelIndex &index, int role) const
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
            return QString::fromStdString(Translator::getCharacteristic(state.getCharacteristic()));
        }
    }

    return QVariant();
}

QVariant HiddenGrottoGeneratorModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

void HiddenGrottoGeneratorModel5::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, 7), index(rowCount(), 12), { Qt::DisplayRole });
}

HiddenGrottoSearcherModel5::HiddenGrottoSearcherModel5(QObject *parent) : TableModel(parent), showStats(false)
{
}

int HiddenGrottoSearcherModel5::columnCount(const QModelIndex &parent) const
{
    return 20;
}

QVariant HiddenGrottoSearcherModel5::data(const QModelIndex &index, int role) const
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
            return state.getIVAdvances();
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
            return QString::fromStdString(display.getDateTime().toString());
        case 18:
            return QString::number(display.getTimer0(), 16).toUpper();
        case 19:
            return QString::fromStdString(Translator::getKeypresses(display.getButtons()));
        }
    }

    return QVariant();
}

QVariant HiddenGrottoSearcherModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

void HiddenGrottoSearcherModel5::sort(int column, Qt::SortOrder order)
{
    if (!model.empty())
    {
        emit layoutAboutToBeChanged();
        bool flag = order == Qt::AscendingOrder;
        switch (column)
        {
        case 0:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<State5> &state1, const SearcherState5<State5> &state2) {
                return flag ? state1.getInitialSeed() < state2.getInitialSeed() : state1.getInitialSeed() > state2.getInitialSeed();
            });
            break;
        case 1:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<State5> &state1, const SearcherState5<State5> &state2) {
                return flag ? state1.getState().getAdvances() < state2.getState().getAdvances()
                            : state1.getState().getAdvances() > state2.getState().getAdvances();
            });
            break;
        case 2:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<State5> &state1, const SearcherState5<State5> &state2) {
                return flag ? state1.getState().getIVAdvances() < state2.getState().getIVAdvances()
                            : state1.getState().getIVAdvances() > state2.getState().getIVAdvances();
            });
            break;
        case 3:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<State5> &state1, const SearcherState5<State5> &state2) {
                return flag ? state1.getState().getLevel() < state2.getState().getLevel()
                            : state1.getState().getLevel() > state2.getState().getLevel();
            });
            break;
        case 4:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<State5> &state1, const SearcherState5<State5> &state2) {
                return flag ? state1.getState().getPID() < state2.getState().getPID()
                            : state1.getState().getPID() > state2.getState().getPID();
            });
            break;
        case 5:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<State5> &state1, const SearcherState5<State5> &state2) {
                return flag ? state1.getState().getShiny() < state2.getState().getShiny()
                            : state1.getState().getShiny() > state2.getState().getShiny();
            });
            break;
        case 6:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<State5> &state1, const SearcherState5<State5> &state2) {
                return flag ? state1.getState().getNature() < state2.getState().getNature()
                            : state1.getState().getNature() > state2.getState().getNature();
            });
            break;
        case 7:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<State5> &state1, const SearcherState5<State5> &state2) {
                return flag ? state1.getState().getAbility() < state2.getState().getAbility()
                            : state1.getState().getAbility() > state2.getState().getAbility();
            });
            break;
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            std::sort(model.begin(), model.end(),
                      [flag, column](const SearcherState5<State5> &state1, const SearcherState5<State5> &state2) {
                          return flag ? state1.getState().getIV(column - 8) < state2.getState().getIV(column - 8)
                                      : state1.getState().getIV(column - 8) > state2.getState().getIV(column - 8);
                      });
            break;
        case 14:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<State5> &state1, const SearcherState5<State5> &state2) {
                return flag ? state1.getState().getHiddenPower() < state2.getState().getHiddenPower()
                            : state1.getState().getHiddenPower() > state2.getState().getHiddenPower();
            });
            break;
        case 15:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<State5> &state1, const SearcherState5<State5> &state2) {
                return flag ? state1.getState().getHiddenPowerStrength() < state2.getState().getHiddenPowerStrength()
                            : state1.getState().getHiddenPowerStrength() > state2.getState().getHiddenPowerStrength();
            });
            break;
        case 16:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<State5> &state1, const SearcherState5<State5> &state2) {
                return flag ? state1.getState().getGender() < state2.getState().getGender()
                            : state1.getState().getGender() > state2.getState().getGender();
            });
            break;
        case 17:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<State5> &state1, const SearcherState5<State5> &state2) {
                return flag ? state1.getDateTime() < state2.getDateTime() : state1.getDateTime() > state2.getDateTime();
            });
            break;
        case 18:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<State5> &state1, const SearcherState5<State5> &state2) {
                return flag ? state1.getTimer0() < state2.getTimer0() : state1.getTimer0() > state2.getTimer0();
            });
            break;
        case 19:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<State5> &state1, const SearcherState5<State5> &state2) {
                return flag ? state1.getButtons() < state2.getButtons() : state1.getButtons() > state2.getButtons();
            });
            break;
        }
    }
}

void HiddenGrottoSearcherModel5::setShowStats(bool flag)
{
    showStats = flag;
    emit dataChanged(index(0, 8), index(rowCount(), 13), { Qt::DisplayRole });
}
