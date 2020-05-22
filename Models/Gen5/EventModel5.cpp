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

EventGeneratorModel5::EventGeneratorModel5(QObject *parent) : TableModel<Frame>(parent)
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
        const auto &frame = model.at(index.row());
        int column = index.column();
        switch (column)
        {
        case 0:
            return frame.getFrame();
        case 1:
            return "TODO";
        case 2:
            return QString::number(frame.getPID(), 16).toUpper().rightJustified(8, '0');
        case 3:
        {
            u8 shiny = frame.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 4:
            return Translator::getNature(frame.getNature());
        case 5:
        {
            u8 ability = frame.getAbility();
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
            return frame.getIV(static_cast<u8>(column - 6));
        case 12:
            return Translator::getHiddenPower(frame.getHidden());
        case 13:
            return frame.getPower();
        case 14:
            return Translator::getGender(frame.getGender());
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

EventSearcherModel5::EventSearcherModel5(QObject *parent) : TableModel<EventFrame5>(parent)
{
}

void EventSearcherModel5::sort(int column, Qt::SortOrder order)
{
    // TODO
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
        const auto &frame = display.getFrame();
        int column = index.column();
        switch (column)
        {
        case 0:
            return QString::number(display.getInitialSeed(), 16).toUpper().rightJustified(16, '0');
        case 1:
            return frame.getFrame();
        case 2:
            return QString::number(frame.getPID(), 16).toUpper().rightJustified(8, '0');
        case 3:
        {
            u8 shiny = frame.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 4:
            return Translator::getNature(frame.getNature());
        case 5:
        {
            u8 ability = frame.getAbility();
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
            return frame.getIV(static_cast<u8>(column - 6));
        case 12:
            return Translator::getHiddenPower(frame.getHidden());
        case 13:
            return frame.getPower();
        case 14:
            return Translator::getGender(frame.getGender());
        case 15:
            return display.getDateTime().toString("yyyy-MM-dd hh:mm:ss");
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
