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

#include "StationaryModel3.hpp"
#include <Core/Util/Translator.hpp>

StationaryGeneratorModel3::StationaryGeneratorModel3(QObject *parent) : TableModel<Frame>(parent)
{
}

int StationaryGeneratorModel3::columnCount(const QModelIndex & /*parent*/) const
{
    return 14;
}

QVariant StationaryGeneratorModel3::data(const QModelIndex &index, int role) const
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
            return QString::number(frame.getPID(), 16).toUpper().rightJustified(8, '0');
        case 2:
        {
            u8 shiny = frame.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 3:
            return Translator::getNature(frame.getNature());
        case 4:
            return frame.getAbility();
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            return frame.getIV(static_cast<u8>(column - 5));
        case 11:
            return Translator::getHiddenPower(frame.getHidden());
        case 12:
            return frame.getPower();
        case 13:
            return Translator::getGender(frame.getGender());
        }
    }
    return QVariant();
}

QVariant StationaryGeneratorModel3::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header.at(section);
    }
    return QVariant();
}

StationarySearcherModel3::StationarySearcherModel3(QObject *parent) : TableModel<Frame>(parent)
{
}

void StationarySearcherModel3::sort(int column, Qt::SortOrder order)
{
    if (!model.isEmpty())
    {
        emit layoutAboutToBeChanged();
        bool flag = order == Qt::AscendingOrder;
        switch (column)
        {
        case 0:
            std::sort(model.begin(), model.end(), [flag](const Frame &frame1, const Frame &frame2) {
                return flag ? frame1.getSeed() < frame2.getSeed() : frame1.getSeed() > frame2.getSeed();
            });
            break;
        case 1:
            std::sort(model.begin(), model.end(), [flag](const Frame &frame1, const Frame &frame2) {
                return flag ? frame1.getPID() < frame2.getPID() : frame1.getPID() > frame2.getPID();
            });
            break;
        case 2:
            std::sort(model.begin(), model.end(), [flag](const Frame &frame1, const Frame &frame2) {
                return flag ? frame1.getShiny() < frame2.getShiny() : frame1.getShiny() > frame2.getShiny();
            });
            break;
        case 3:
            std::sort(model.begin(), model.end(), [flag](const Frame &frame1, const Frame &frame2) {
                return flag ? frame1.getNature() < frame2.getNature() : frame1.getNature() > frame2.getNature();
            });
            break;
        case 4:
            std::sort(model.begin(), model.end(), [flag](const Frame &frame1, const Frame &frame2) {
                return flag ? frame1.getAbility() < frame2.getAbility() : frame1.getAbility() > frame2.getAbility();
            });
            break;
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            std::sort(model.begin(), model.end(), [flag, column](const Frame &frame1, const Frame &frame2) {
                return flag ? frame1.getIV(static_cast<u8>(column - 5)) < frame2.getIV(static_cast<u8>(column - 5))
                            : frame1.getIV(static_cast<u8>(column - 5)) > frame2.getIV(static_cast<u8>(column - 5));
            });
            break;
        case 11:
            std::sort(model.begin(), model.end(), [flag](const Frame &frame1, const Frame &frame2) {
                return flag ? frame1.getHidden() < frame2.getHidden() : frame1.getHidden() > frame2.getHidden();
            });
            break;
        case 12:
            std::sort(model.begin(), model.end(), [flag](const Frame &frame1, const Frame &frame2) {
                return flag ? frame1.getPower() < frame2.getPower() : frame1.getPower() > frame2.getPower();
            });
            break;
        case 13:
            std::sort(model.begin(), model.end(), [flag](const Frame &frame1, const Frame &frame2) {
                return flag ? frame1.getGender() < frame2.getGender() : frame1.getGender() > frame2.getGender();
            });
            break;
        }

        emit layoutChanged();
    }
}

int StationarySearcherModel3::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return 14;
}

QVariant StationarySearcherModel3::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &frame = model.at(index.row());
        int column = index.column();
        switch (column)
        {
        case 0:
            return QString::number(frame.getSeed(), 16).toUpper().rightJustified(8, '0');
        case 1:
            return QString::number(frame.getPID(), 16).toUpper().rightJustified(8, '0');
        case 2:
        {
            u8 shiny = frame.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 3:
            return Translator::getNature(frame.getNature());
        case 4:
            return frame.getAbility();
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
            return frame.getIV(static_cast<u8>(column - 5));
        case 11:
            return Translator::getHiddenPower(frame.getHidden());
        case 12:
            return frame.getPower();
        case 13:
            return Translator::getGender(frame.getGender());
        }
    }
    return QVariant();
}

QVariant StationarySearcherModel3::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header.at(section);
    }
    return QVariant();
}
