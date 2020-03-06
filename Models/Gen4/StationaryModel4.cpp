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

#include "StationaryModel4.hpp"
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Util/Translator.hpp>

StationaryGeneratorModel4::StationaryGeneratorModel4(QObject *parent, Method method) : TableModel<Frame>(parent), method(method)
{
}

void StationaryGeneratorModel4::setMethod(Method method)
{
    this->method = method;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

int StationaryGeneratorModel4::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    switch (method)
    {
    case Method::Method1:
    case Method::MethodK:
        return 16;
    case Method::MethodJ:
        return 15;
    case Method::WondercardIVs:
        return 11;
    default:
        return 0;
    }
}

QVariant StationaryGeneratorModel4::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &frame = model.at(index.row());
        int column = getColumn(index.column());
        switch (column)
        {
        case 0:
            return frame.getFrame();
        case 1:
        {
            u8 call = frame.getSeed() % 3;
            return call == 0 ? "E" : call == 1 ? "K" : "P";
        }
        case 2:
        {
            u8 val = ((frame.getSeed() & 0x1fff) * 100) >> 13;
            QString pitch;
            if (val < 20)
            {
                pitch = tr("Low");
            }
            else if (val < 40)
            {
                pitch = tr("Mid-Low");
            }
            else if (val < 60)
            {
                pitch = tr("Mid");
            }
            else if (val < 80)
            {
                pitch = tr("Mid-High");
            }
            else
            {
                pitch = tr("High");
            }

            pitch += " (" + QString::number(val) + ")";
            return pitch;
        }
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

QVariant StationaryGeneratorModel4::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        section = getColumn(section);

        return header.at(section);
    }
    return QVariant();
}

int StationaryGeneratorModel4::getColumn(int column) const
{
    switch (method)
    {
    case Method::Method1:
    case Method::MethodK:
    default:
        return column;
    case Method::MethodJ:
        return column > 0 ? column + 1 : column;
    case Method::WondercardIVs:
        return column > 2 ? column + 4 : column;
    }
}

StationarySearcherModel4::StationarySearcherModel4(QObject *parent, Method method) : TableModel<StationaryFrame>(parent), method(method)
{
}

void StationarySearcherModel4::setMethod(Method method)
{
    this->method = method;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

void StationarySearcherModel4::sort(int column, Qt::SortOrder order)
{
    if (!model.isEmpty())
    {
        emit layoutAboutToBeChanged();
        bool flag = order == Qt::AscendingOrder;

        column = getColumn(column);
        switch (column)
        {
        case 0:
            std::sort(model.begin(), model.end(), [flag](const StationaryFrame &frame1, const StationaryFrame &frame2) {
                return flag ? frame1.getSeed() < frame2.getSeed() : frame1.getSeed() > frame2.getSeed();
            });
            break;
        case 1:
            std::sort(model.begin(), model.end(), [flag](const StationaryFrame &frame1, const StationaryFrame &frame2) {
                return flag ? frame1.getFrame() < frame2.getFrame() : frame1.getFrame() > frame2.getFrame();
            });
            break;
        case 2:
            std::sort(model.begin(), model.end(), [flag](const StationaryFrame &frame1, const StationaryFrame &frame2) {
                return flag ? frame1.getLead() < frame2.getLead() : frame1.getLead() > frame2.getLead();
            });
            break;
        case 3:
            std::sort(model.begin(), model.end(), [flag](const StationaryFrame &frame1, const StationaryFrame &frame2) {
                return flag ? frame1.getPID() < frame2.getPID() : frame1.getPID() > frame2.getPID();
            });
            break;
        case 4:
            std::sort(model.begin(), model.end(), [flag](const StationaryFrame &frame1, const StationaryFrame &frame2) {
                return flag ? frame1.getShiny() < frame2.getShiny() : frame1.getShiny() > frame2.getShiny();
            });
            break;
        case 5:
            std::sort(model.begin(), model.end(), [flag](const StationaryFrame &frame1, const StationaryFrame &frame2) {
                return flag ? frame1.getNature() < frame2.getNature() : frame1.getNature() > frame2.getNature();
            });
            break;
        case 6:
            std::sort(model.begin(), model.end(), [flag](const StationaryFrame &frame1, const StationaryFrame &frame2) {
                return flag ? frame1.getAbility() < frame2.getAbility() : frame1.getAbility() > frame2.getAbility();
            });
            break;
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
            std::sort(model.begin(), model.end(), [flag, column](const StationaryFrame &frame1, const StationaryFrame &frame2) {
                return flag ? frame1.getIV(static_cast<u8>(column - 7)) < frame2.getIV(static_cast<u8>(column - 7))
                            : frame1.getIV(static_cast<u8>(column - 7)) > frame2.getIV(static_cast<u8>(column - 7));
            });
            break;
        case 13:
            std::sort(model.begin(), model.end(), [flag](const StationaryFrame &frame1, const StationaryFrame &frame2) {
                return flag ? frame1.getHidden() < frame2.getHidden() : frame1.getHidden() > frame2.getHidden();
            });
            break;
        case 14:
            std::sort(model.begin(), model.end(), [flag](const StationaryFrame &frame1, const StationaryFrame &frame2) {
                return flag ? frame1.getPower() < frame2.getPower() : frame1.getPower() > frame2.getPower();
            });
            break;
        case 15:
            std::sort(model.begin(), model.end(), [flag](const StationaryFrame &frame1, const StationaryFrame &frame2) {
                return flag ? frame1.getGender() < frame2.getGender() : frame1.getGender() > frame2.getGender();
            });
            break;
        }

        emit layoutChanged();
    }
}

int StationarySearcherModel4::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    switch (method)
    {
    case Method::MethodJ:
    case Method::MethodK:
        return 16;
    case Method::Method1:
        return 15;
    case Method::WondercardIVs:
        return 10;
    default:
        return 0;
    }
}

QVariant StationarySearcherModel4::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        const auto &frame = model.at(index.row());
        int column = getColumn(index.column());
        switch (column)
        {
        case 0:
            return QString::number(frame.getSeed(), 16).toUpper().rightJustified(8, '0');
        case 1:
            return frame.getFrame();
        case 2:
            switch (frame.getLead())
            {
            case Lead::None:
                return tr("None");
            case Lead::Synchronize:
                return tr("Synchronize");
            case Lead::SuctionCups:
                return tr("Suction Cups");
            case Lead::CuteCharmFemale:
                return tr("Cute Charm (♀)");
            case Lead::CuteCharm25M:
                return tr("Cute Charm (25% ♂)");
            case Lead::CuteCharm50M:
                return tr("Cute Charm (50% ♂)");
            case Lead::CuteCharm75M:
                return tr("Cute Charm (75% ♂)");
            case Lead::CuteCharm875M:
            default:
                return tr("Cute Charm (87.5% ♂)");
            }
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

QVariant StationarySearcherModel4::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        section = getColumn(section);
        return header.at(section);
    }
    return QVariant();
}

int StationarySearcherModel4::getColumn(int column) const
{
    switch (method)
    {
    case Method::MethodJ:
    case Method::MethodK:
    default:
        return column;
    case Method::Method1:
        return column > 1 ? column + 1 : column;
    case Method::WondercardIVs:
        return column > 1 ? column + 5 : column;
    }
}
