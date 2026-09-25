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

#include "PhenomenonModel.hpp"
#include <Core/Util/Translator.hpp>
#include <Core/Util/Utilities.hpp>
#include <QColor>
#include <QFont>

PhenomenonGeneratorModel5::PhenomenonGeneratorModel5(QObject *parent) : TableModel(parent)
{
}

int PhenomenonGeneratorModel5::columnCount(const QModelIndex &parent) const
{
    return 5;
}

QVariant PhenomenonGeneratorModel5::data(const QModelIndex &index, int role) const
{
    const auto &state = model[index.row()];
    if (role == Qt::FontRole)
    {
        if (!state.isValid())
        {
            QFont font;
            font.setItalic(true);
            return font;
        }
    }
    else if (role == Qt::ForegroundRole)
    {
        if (!state.isValid())
        {
            return QColor(128, 128, 128);
        }
    }
    else if (role == Qt::DisplayRole)
    {
        int column = index.column();
        if (!state.isValid() && column > 3)
        {
            return "-";
        }

        switch (column)
        {
        case 0:
            return state.getAdvances();
        case 1:
            return QString::fromStdString(Utilities5::getChatot(state.getChatot()));
        case 2:
            return QString::fromStdString(Translator::getNeedle(state.getNeedle()));
        case 3:
            return state.getPhenomenon() ? tr("Yes") : tr("No");
        case 4:
            return QString::fromStdString(Translator::getItem(state.getItem()));
        }
    }
    return QVariant();
}

QVariant PhenomenonGeneratorModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

PhenomenonSearcherModel5::PhenomenonSearcherModel5(QObject *parent) : TableModel(parent)
{
}

int PhenomenonSearcherModel5::columnCount(const QModelIndex &parent) const
{
    return 6;
}

QVariant PhenomenonSearcherModel5::data(const QModelIndex &index, int role) const
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
            return QString::fromStdString(Translator::getItem(state.getItem()));
        case 3:
            return QString::fromStdString(display.getDateTime().toString());
        case 4:
            return QString::number(display.getTimer0(), 16).toUpper();
        case 5:
            return QString::fromStdString(Translator::getKeypresses(display.getButtons()));
        }
    }
    return QVariant();
}

QVariant PhenomenonSearcherModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}
