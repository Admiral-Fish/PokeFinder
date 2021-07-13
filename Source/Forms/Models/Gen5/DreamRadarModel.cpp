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

#include "DreamRadarModel.hpp"
#include <Core/Util/Translator.hpp>

DreamRadarGeneratorModel5::DreamRadarGeneratorModel5(QObject *parent) : TableModel<DreamRadarState>(parent)
{
}

int DreamRadarGeneratorModel5::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return 16;
}

QVariant DreamRadarGeneratorModel5::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &state = model[index.row()];
        int column = index.column();
        switch (column)
        {
        case 0:
            return state.getKeyAdvances();
        case 1:
            return state.getAdvances();
        case 2:
            return state.getSeed();
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
        {
            u8 ability = state.getAbility();
            if (ability == 0 || ability == 1)
            {
                return ability;
            }
            return "H";
        }
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

QVariant DreamRadarGeneratorModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

DreamRadarSearcherModel5::DreamRadarSearcherModel5(QObject *parent) : TableModel<SearcherState5<DreamRadarState>>(parent)
{
}

void DreamRadarSearcherModel5::sort(int column, Qt::SortOrder order)
{
    if (!model.empty())
    {
        emit layoutAboutToBeChanged();
        bool flag = order == Qt::AscendingOrder;
        switch (column)
        {
        case 0:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<DreamRadarState> &state1, const SearcherState5<DreamRadarState> &state2) {
                return flag ? state1.getInitialSeed() < state2.getInitialSeed() : state1.getInitialSeed() > state2.getInitialSeed();
            });
            break;
        case 1:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<DreamRadarState> &state1, const SearcherState5<DreamRadarState> &state2) {
                return flag ? state1.getState().getKeyAdvances() < state2.getState().getKeyAdvances()
                            : state1.getState().getKeyAdvances() > state2.getState().getKeyAdvances();
            });
            break;
        case 2:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<DreamRadarState> &state1, const SearcherState5<DreamRadarState> &state2) {
                return flag ? state1.getState().getAdvances() < state2.getState().getAdvances()
                            : state1.getState().getAdvances() > state2.getState().getAdvances();
            });
            break;
        case 3:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<DreamRadarState> &state1, const SearcherState5<DreamRadarState> &state2) {
                return flag ? state1.getState().getPID() < state2.getState().getPID()
                            : state1.getState().getPID() > state2.getState().getPID();
            });
            break;
        case 4:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<DreamRadarState> &state1, const SearcherState5<DreamRadarState> &state2) {
                return flag ? state1.getState().getShiny() < state2.getState().getShiny()
                            : state1.getState().getShiny() > state2.getState().getShiny();
            });
            break;
        case 5:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<DreamRadarState> &state1, const SearcherState5<DreamRadarState> &state2) {
                return flag ? state1.getState().getNature() < state2.getState().getNature()
                            : state1.getState().getNature() > state2.getState().getNature();
            });
            break;
        case 6:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<DreamRadarState> &state1, const SearcherState5<DreamRadarState> &state2) {
                return flag ? state1.getState().getAbility() < state2.getState().getAbility()
                            : state1.getState().getAbility() > state2.getState().getAbility();
            });
            break;
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
            std::sort(model.begin(), model.end(), [flag, column](const SearcherState5<DreamRadarState> &state1, const SearcherState5<DreamRadarState> &state2) {
                return flag ? state1.getState().getIV(static_cast<u8>(column - 7)) < state2.getState().getIV(static_cast<u8>(column - 7))
                            : state1.getState().getIV(static_cast<u8>(column - 7)) > state2.getState().getIV(static_cast<u8>(column - 7));
            });
            break;
        case 13:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<DreamRadarState> &state1, const SearcherState5<DreamRadarState> &state2) {
                return flag ? state1.getState().getHidden() < state2.getState().getHidden()
                            : state1.getState().getHidden() > state2.getState().getHidden();
            });
            break;
        case 14:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<DreamRadarState> &state1, const SearcherState5<DreamRadarState> &state2) {
                return flag ? state1.getState().getPower() < state2.getState().getPower()
                            : state1.getState().getPower() > state2.getState().getPower();
            });
            break;
        case 15:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<DreamRadarState> &state1, const SearcherState5<DreamRadarState> &state2) {
                return flag ? state1.getState().getGender() < state2.getState().getGender()
                            : state1.getState().getGender() > state2.getState().getGender();
            });
            break;
        case 16:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<DreamRadarState> &state1, const SearcherState5<DreamRadarState> &state2) {
                return flag ? state1.getDateTime() < state2.getDateTime() : state1.getDateTime() > state2.getDateTime();
            });
            break;
        case 17:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<DreamRadarState> &state1, const SearcherState5<DreamRadarState> &state2) {
                return flag ? state1.getTimer0() < state2.getTimer0() : state1.getTimer0() > state2.getTimer0();
            });
            break;
        case 18:
            std::sort(model.begin(), model.end(), [flag](const SearcherState5<DreamRadarState> &state1, const SearcherState5<DreamRadarState> &state2) {
                return flag ? state1.getButtons() < state2.getButtons() : state1.getButtons() > state2.getButtons();
            });
            break;
        }
    }
}

int DreamRadarSearcherModel5::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return 19;
}

QVariant DreamRadarSearcherModel5::data(const QModelIndex &index, int role) const
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
            return state.getKeyAdvances();
        case 2:
            return state.getAdvances();
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
        {
            u8 ability = state.getAbility();
            if (ability == 0 || ability == 1)
            {
                return ability;
            }
            return "H";
        }
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
        case 16:
            return QString::fromStdString(display.getDateTime().toString());
        case 17:
            return QString::number(display.getTimer0(), 16).toUpper();
        case 18:
            return QString::fromStdString(Translator::getKeypresses(display.getButtons()));
        }
    }
    return QVariant();
}

QVariant DreamRadarSearcherModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}
