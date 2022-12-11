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

#include "HiddenGrottoModel.hpp"
#include <Core/Util/Translator.hpp>
#include <Core/Util/Utilities.hpp>

HiddenGrottoGeneratorModel5::HiddenGrottoGeneratorModel5(QObject *parent) : TableModel<HiddenGrottoState>(parent)
{
}

int HiddenGrottoGeneratorModel5::columnCount(const QModelIndex &parent) const
{
    return 4;
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
            return state.getGroup();
        case 3:
            if (state.getItem())
            {
                return QString("%1 (%2)").arg(state.getSlot()).arg(QString::fromStdString(*Translator::getItem(state.getData())));
            }
            else
            {
                return QString("%1 (%2 %3)")
                    .arg(state.getSlot())
                    .arg(QString::fromStdString(*Translator::getSpecie(state.getData())),
                         QString::fromStdString(*Translator::getGender(state.getGender())));
            }
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

HiddenGrottoSearcherModel5::HiddenGrottoSearcherModel5(QObject *parent) : TableModel<SearcherState5<HiddenGrottoState>>(parent)
{
}

int HiddenGrottoSearcherModel5::columnCount(const QModelIndex &parent) const
{
    return 6;
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
            return state.getGroup();
        case 3:
            if (state.getItem())
            {
                return QString("%1 (%2)").arg(state.getSlot()).arg(QString::fromStdString(*Translator::getItem(state.getData())));
            }
            else
            {
                return QString("%1 (%2 %3)")
                    .arg(state.getSlot())
                    .arg(QString::fromStdString(*Translator::getSpecie(state.getData())),
                         QString::fromStdString(*Translator::getGender(state.getGender())));
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
