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

#include "WildModel3.hpp"
#include <Core/Enum/Lead.hpp>
#include <Core/Util/Translator.hpp>

WildGeneratorModel3::WildGeneratorModel3(QObject *parent) : TableModel<WildFrame>(parent)
{
}

int WildGeneratorModel3::columnCount(const QModelIndex & /*parent*/) const
{
    return 16;
}

QVariant WildGeneratorModel3::data(const QModelIndex &index, int role) const
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
            return frame.getEncounterSlot();
        case 2:
            return frame.getLevel();
        case 3:
            return QString::number(frame.getPID(), 16).toUpper().rightJustified(8, '0');
        case 4:
        {
            u8 shiny = frame.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 5:
            return Translator::getNature(frame.getNature());
        case 6:
            return frame.getAbility();
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
            return frame.getIV(static_cast<u8>(column - 7));
        case 13:
            return Translator::getHiddenPower(frame.getHidden());
        case 14:
            return frame.getPower();
        case 15:
            return Translator::getGender(frame.getGender());
        }
    }
    return QVariant();
}

QVariant WildGeneratorModel3::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header.at(section);
    }
    return QVariant();
}

WildSearcherModel3::WildSearcherModel3(QObject *parent) : TableModel<WildFrame>(parent)
{
}

void WildSearcherModel3::sort(int column, Qt::SortOrder order)
{
    if (!model.empty())
    {
        emit layoutAboutToBeChanged();
        bool flag = order == Qt::AscendingOrder;
        switch (column)
        {
        case 0:
            std::sort(model.begin(), model.end(), [flag](const WildFrame &frame1, const WildFrame &frame2) {
                return flag ? frame1.getSeed() < frame2.getSeed() : frame1.getSeed() > frame2.getSeed();
            });
            break;
        case 1:
            std::sort(model.begin(), model.end(), [flag](const WildFrame &frame1, const WildFrame &frame2) {
                return flag ? frame1.getLead() < frame2.getLead() : frame1.getLead() > frame2.getLead();
            });
            break;
        case 2:
            std::sort(model.begin(), model.end(), [flag](const WildFrame &frame1, const WildFrame &frame2) {
                return flag ? frame1.getEncounterSlot() < frame2.getEncounterSlot() : frame1.getEncounterSlot() > frame2.getEncounterSlot();
            });
            break;
        case 3:
            std::sort(model.begin(), model.end(), [flag](const WildFrame &frame1, const WildFrame &frame2) {
                return flag ? frame1.getLevel() < frame2.getLevel() : frame1.getLevel() > frame2.getLevel();
            });
            break;
        case 4:
            std::sort(model.begin(), model.end(), [flag](const WildFrame &frame1, const WildFrame &frame2) {
                return flag ? frame1.getPID() < frame2.getPID() : frame1.getPID() > frame2.getPID();
            });
            break;
        case 5:
            std::sort(model.begin(), model.end(), [flag](const WildFrame &frame1, const WildFrame &frame2) {
                return flag ? frame1.getShiny() < frame2.getShiny() : frame1.getShiny() > frame2.getShiny();
            });
            break;
        case 6:
            std::sort(model.begin(), model.end(), [flag](const WildFrame &frame1, const WildFrame &frame2) {
                return flag ? frame1.getNature() < frame2.getNature() : frame1.getNature() > frame2.getNature();
            });
            break;
        case 7:
            std::sort(model.begin(), model.end(), [flag](const WildFrame &frame1, const WildFrame &frame2) {
                return flag ? frame1.getAbility() < frame2.getAbility() : frame1.getAbility() > frame2.getAbility();
            });
            break;
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            std::sort(model.begin(), model.end(), [flag, column](const WildFrame &frame1, const WildFrame &frame2) {
                return flag ? frame1.getIV(static_cast<u8>(column - 8)) < frame2.getIV(static_cast<u8>(column - 8))
                            : frame1.getIV(static_cast<u8>(column - 8)) > frame2.getIV(static_cast<u8>(column - 8));
            });
            break;
        case 14:
            std::sort(model.begin(), model.end(), [flag](const WildFrame &frame1, const WildFrame &frame2) {
                return flag ? frame1.getHidden() < frame2.getHidden() : frame1.getHidden() > frame2.getHidden();
            });
            break;
        case 15:
            std::sort(model.begin(), model.end(), [flag](const WildFrame &frame1, const WildFrame &frame2) {
                return flag ? frame1.getPower() < frame2.getPower() : frame1.getPower() > frame2.getPower();
            });
            break;
        case 16:
            std::sort(model.begin(), model.end(), [flag](const WildFrame &frame1, const WildFrame &frame2) {
                return flag ? frame1.getGender() < frame2.getGender() : frame1.getGender() > frame2.getGender();
            });
            break;
        }

        emit layoutChanged();
    }
}

int WildSearcherModel3::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return 17;
}

QVariant WildSearcherModel3::data(const QModelIndex &index, int role) const
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
        {
            auto lead = frame.getLead();
            return lead == Lead::None ? tr("None") : lead == Lead::Synchronize ? tr("Synch") : tr("Cute Charm");
        }
        case 2:
            return frame.getEncounterSlot();
        case 3:
            return frame.getLevel();
        case 4:
            return QString::number(frame.getPID(), 16).toUpper().rightJustified(8, '0');
        case 5:
        {
            u8 shiny = frame.getShiny();
            return shiny == 2 ? tr("Square") : shiny == 1 ? tr("Star") : tr("No");
        }
        case 6:
            return Translator::getNature(frame.getNature());
        case 7:
            return frame.getAbility();
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            return frame.getIV(static_cast<u8>(column - 8));
        case 14:
            return Translator::getHiddenPower(frame.getHidden());
        case 15:
            return frame.getPower();
        case 16:
            return Translator::getGender(frame.getGender());
        }
    }
    return QVariant();
}

QVariant WildSearcherModel3::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        return header.at(section);
    }
    return QVariant();
}
