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

#include "PickupModel.hpp"
#include <Core/Util/Translator.hpp>
#include <Core/Util/Utilities.hpp>
#include <QtGui/QColor>
#include <QtGui/QFont>

static QVariant getItemText(const PickupState &state, u8 slot)
{
    if (!state.getActive(slot))
    {
        return "-";
    }

    return QString::fromStdString(Translator::getItem(state.getItem(slot)));
}

static bool shouldDimItemCell(const PickupState &state, int column, int firstItemColumn)
{
    if (column < firstItemColumn || column >= firstItemColumn + 6)
    {
        return false;
    }

    u8 slot = static_cast<u8>(column - firstItemColumn);
    return state.getActive(slot) && state.getItem(slot) == 0;
}

static QVariant dimmedFont()
{
    QFont font;
    font.setItalic(true);
    return font;
}

static QVariant dimmedColor()
{
    return QColor(128, 128, 128);
}

PickupGeneratorModel5::PickupGeneratorModel5(QObject *parent) : TableModel(parent)
{
}

int PickupGeneratorModel5::columnCount(const QModelIndex &parent) const
{
    return 25;
}

QVariant PickupGeneratorModel5::data(const QModelIndex &index, int role) const
{
    const auto &state = model[index.row()];
    int column = index.column();
    if (shouldDimItemCell(state, column, 2))
    {
        if (role == Qt::FontRole)
        {
            return dimmedFont();
        }
        else if (role == Qt::ForegroundRole)
        {
            return dimmedColor();
        }
    }

    if (role == Qt::DisplayRole)
    {
        switch (column)
        {
        case 0:
            return state.getAdvances();
        case 1:
            return QString::fromStdString(Utilities5::getChatot(state.getChatot()));
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
            return getItemText(state, column - 2);
        default:
        {
            const auto &wild = state.getWild();
            if (!wild)
            {
                return "-";
            }

            switch (column)
            {
            case 8:
                return QString::fromStdString(Translator::getItem(wild->getItem()));
            case 9:
                return QString("%1: %2")
                    .arg(wild->getEncounterSlot())
                    .arg(QString::fromStdString(Translator::getSpecie(wild->getSpecie(), wild->getForm())));
            case 10:
                return wild->getLevel();
            case 11:
                return QString::number(wild->getPID(), 16).toUpper().rightJustified(8, '0');
            case 12:
            {
                u8 shiny = wild->getShiny();
                return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
            }
            case 13:
                return QString::fromStdString(Translator::getNature(wild->getNature()));
            case 14:
                if (wild->getAbility() == 0 || wild->getAbility() == 1)
                {
                    return QString("%1: %2")
                        .arg(wild->getAbility())
                        .arg(QString::fromStdString(Translator::getAbility(wild->getAbilityIndex())));
                }
                return QString("H (%2)").arg(QString::fromStdString(Translator::getAbility(wild->getAbilityIndex())));
            case 15:
            case 16:
            case 17:
            case 18:
            case 19:
            case 20:
                return showStats ? wild->getStat(column - 15) : wild->getIV(column - 15);
            case 21:
                return QString::fromStdString(Translator::getHiddenPower(wild->getHiddenPower()));
            case 22:
                return wild->getHiddenPowerStrength();
            case 23:
                return QString::fromStdString(Translator::getGender(wild->getGender()));
            case 24:
                return QString::fromStdString(Translator::getCharacteristic(wild->getCharacteristic(), CharacteristicGeneration::Gen5));
            }
        }
        }
    }
    return QVariant();
}

void PickupGeneratorModel5::setShowStats(bool flag)
{
    showStats = flag;
    if (rowCount() > 0)
    {
        emit dataChanged(index(0, 15), index(rowCount() - 1, 20), { Qt::DisplayRole });
    }
}

QVariant PickupGeneratorModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}

PickupSearcherModel5::PickupSearcherModel5(QObject *parent) : TableModel(parent)
{
}

int PickupSearcherModel5::columnCount(const QModelIndex &parent) const
{
    return 11;
}

QVariant PickupSearcherModel5::data(const QModelIndex &index, int role) const
{
    const auto &display = model[index.row()];
    const auto &state = display.getState();
    int column = index.column();
    if (shouldDimItemCell(state, column, 2))
    {
        if (role == Qt::FontRole)
        {
            return dimmedFont();
        }
        else if (role == Qt::ForegroundRole)
        {
            return dimmedColor();
        }
    }

    if (role == Qt::DisplayRole)
    {
        switch (column)
        {
        case 0:
            return QString::number(display.getInitialSeed(), 16).toUpper().rightJustified(16, '0');
        case 1:
            return state.getAdvances();
        case 8:
            return QString::fromStdString(display.getDateTime().toString());
        case 9:
            return QString::number(display.getTimer0(), 16).toUpper();
        case 10:
            return QString::fromStdString(Translator::getKeypresses(display.getButtons()));
        default:
            return getItemText(state, column - 2);
        }
    }
    return QVariant();
}

QVariant PickupSearcherModel5::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header[section];
    }
    return QVariant();
}
