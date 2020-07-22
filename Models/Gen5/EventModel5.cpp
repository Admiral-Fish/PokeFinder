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

#include "EventModel5.hpp"
#include <Core/Util/Translator.hpp>

EventGeneratorModel5::EventGeneratorModel5(QObject *parent) : TableModel<State>(parent)
{
}

int EventGeneratorModel5::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return 15;
}

QVariant EventGeneratorModel5::data(const QModelIndex &index, int role) const
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
            return "TODO";
        case 2:
            return QString::number(currentState.getPID(), 16).toUpper().rightJustified(8, '0');
        case 3:
        {
            u8 shiny = currentState.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 4:
            return Translator::getNature(currentState.getNature());
        case 5:
        {
            u8 ability = currentState.getAbility();
            if (ability == 0 || ability == 1)
            {
                return ability;
            }
            else
            {
                return "H";
            }
        }
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            return currentState.getIV(static_cast<u8>(column - 6));
        case 12:
            return Translator::getHiddenPower(currentState.getHidden());
        case 13:
            return currentState.getPower();
        case 14:
            return Translator::getGender(currentState.getGender());
        }
    }

    return QVariant();
}

QVariant EventGeneratorModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header.at(section);
    }
    return QVariant();
}

EventSearcherModel5::EventSearcherModel5(QObject *parent) : TableModel<SearcherState5<State>>(parent)
{
}

void EventSearcherModel5::sort(int column, Qt::SortOrder order)
{
    if (!model.empty())
    {
        emit layoutAboutToBeChanged();
        bool flag = order == Qt::AscendingOrder;
        switch (column)
        {
        case 0:
            std::sort(model.begin(), model.end(),
                      [flag](const SearcherState5<State> &currentState1, const SearcherState5<State> &currentState2) {
                          return flag ? currentState1.getInitialSeed() < currentState2.getInitialSeed()
                                      : currentState1.getInitialSeed() > currentState2.getInitialSeed();
                      });
            break;
        case 1:
            std::sort(model.begin(), model.end(),
                      [flag](const SearcherState5<State> &currentState1, const SearcherState5<State> &currentState2) {
                          return flag ? currentState1.getState().getAdvance() < currentState2.getState().getAdvance()
                                      : currentState1.getState().getAdvance() > currentState2.getState().getAdvance();
                      });
            break;
        case 2:
            std::sort(model.begin(), model.end(),
                      [flag](const SearcherState5<State> &currentState1, const SearcherState5<State> &currentState2) {
                          return flag ? currentState1.getState().getPID() < currentState2.getState().getPID()
                                      : currentState1.getState().getPID() > currentState2.getState().getPID();
                      });
            break;
        case 3:
            std::sort(model.begin(), model.end(),
                      [flag](const SearcherState5<State> &currentState1, const SearcherState5<State> &currentState2) {
                          return flag ? currentState1.getState().getShiny() < currentState2.getState().getShiny()
                                      : currentState1.getState().getShiny() > currentState2.getState().getShiny();
                      });
            break;
        case 4:
            std::sort(model.begin(), model.end(),
                      [flag](const SearcherState5<State> &currentState1, const SearcherState5<State> &currentState2) {
                          return flag ? currentState1.getState().getNature() < currentState2.getState().getNature()
                                      : currentState1.getState().getNature() > currentState2.getState().getNature();
                      });
            break;
        case 5:
            std::sort(model.begin(), model.end(),
                      [flag](const SearcherState5<State> &currentState1, const SearcherState5<State> &currentState2) {
                          return flag ? currentState1.getState().getAbility() < currentState2.getState().getAbility()
                                      : currentState1.getState().getAbility() > currentState2.getState().getAbility();
                      });
            break;
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            std::sort(model.begin(), model.end(),
                      [flag, column](const SearcherState5<State> &currentState1, const SearcherState5<State> &currentState2) {
                          return flag ? currentState1.getState().getIV(static_cast<u8>(column - 6))
                                  < currentState2.getState().getIV(static_cast<u8>(column - 6))
                                      : currentState1.getState().getIV(static_cast<u8>(column - 6))
                                  > currentState2.getState().getIV(static_cast<u8>(column - 6));
                      });
            break;
        case 12:
            std::sort(model.begin(), model.end(),
                      [flag](const SearcherState5<State> &currentState1, const SearcherState5<State> &currentState2) {
                          return flag ? currentState1.getState().getHidden() < currentState2.getState().getHidden()
                                      : currentState1.getState().getHidden() > currentState2.getState().getHidden();
                      });
            break;
        case 13:
            std::sort(model.begin(), model.end(),
                      [flag](const SearcherState5<State> &currentState1, const SearcherState5<State> &currentState2) {
                          return flag ? currentState1.getState().getPower() < currentState2.getState().getPower()
                                      : currentState1.getState().getPower() > currentState2.getState().getPower();
                      });
            break;
        case 14:
            std::sort(model.begin(), model.end(),
                      [flag](const SearcherState5<State> &currentState1, const SearcherState5<State> &currentState2) {
                          return flag ? currentState1.getState().getGender() < currentState2.getState().getGender()
                                      : currentState1.getState().getGender() > currentState2.getState().getGender();
                      });
            break;
        case 15:
            std::sort(model.begin(), model.end(),
                      [flag](const SearcherState5<State> &currentState1, const SearcherState5<State> &currentState2) {
                          return flag ? currentState1.getDateTime() < currentState2.getDateTime()
                                      : currentState1.getDateTime() > currentState2.getDateTime();
                      });
            break;
        case 16:
            std::sort(model.begin(), model.end(),
                      [flag](const SearcherState5<State> &currentState1, const SearcherState5<State> &currentState2) {
                          return flag ? currentState1.getTimer0() < currentState2.getTimer0()
                                      : currentState1.getTimer0() > currentState2.getTimer0();
                      });
            break;
        case 17:
            std::sort(model.begin(), model.end(),
                      [flag](const SearcherState5<State> &currentState1, const SearcherState5<State> &currentState2) {
                          return flag ? currentState1.getButtons() < currentState2.getButtons()
                                      : currentState1.getButtons() > currentState2.getButtons();
                      });
            break;
        }
    }
}

int EventSearcherModel5::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return 18;
}

QVariant EventSearcherModel5::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &display = model.at(index.row());
        const auto &currentState = display.getState();
        int column = index.column();
        switch (column)
        {
        case 0:
            return QString::number(display.getInitialSeed(), 16).toUpper().rightJustified(16, '0');
        case 1:
            return currentState.getAdvance();
        case 2:
            return QString::number(currentState.getPID(), 16).toUpper().rightJustified(8, '0');
        case 3:
        {
            u8 shiny = currentState.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 4:
            return Translator::getNature(currentState.getNature());
        case 5:
        {
            u8 ability = currentState.getAbility();
            if (ability == 0 || ability == 1)
            {
                return ability;
            }
            else
            {
                return "H";
            }
        }
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
            return currentState.getIV(static_cast<u8>(column - 6));
        case 12:
            return Translator::getHiddenPower(currentState.getHidden());
        case 13:
            return currentState.getPower();
        case 14:
            return Translator::getGender(currentState.getGender());
        case 15:
            return display.getDateTime().toString("MM-dd-yyyy hh:mm:ss");
        case 16:
            return QString::number(display.getTimer0(), 16).toUpper();
        case 17:
            return Translator::getKeypresses(display.getButtons());
        }
    }

    return QVariant();
}

QVariant EventSearcherModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header.at(section);
    }
    return QVariant();
}
