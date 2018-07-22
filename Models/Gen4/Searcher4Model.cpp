/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "Searcher4Model.hpp"

Searcher4Model::Searcher4Model(QObject *parent, Method method) : QAbstractTableModel(parent)
{
    this->method = method;
}

void Searcher4Model::setModel(QVector<Frame4> frames)
{
    model = frames;
}

void Searcher4Model::addItems(QVector<Frame4> frames)
{
    int i = rowCount();
    emit beginInsertRows(QModelIndex(), i, i + frames.size() - 1);
    model.append(frames);
    emit endInsertRows();
}

void Searcher4Model::clear()
{
    if (model.empty())
        return;
    emit beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    model.clear();
    model.squeeze();
    emit endRemoveRows();
}

void Searcher4Model::setMethod(Method method)
{
    this->method = method;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

void Searcher4Model::sort(int column, Qt::SortOrder order)
{
    if (model.empty())
        return;

    emit layoutAboutToBeChanged();

    if (order == Qt::AscendingOrder)
    {
        switch (method)
        {
            case Method1:
            case ChainedShiny:
                switch (column)
                {
                    case 0:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getSeed() < frameB.getSeed();
                        });
                        break;
                    case 1:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getFrame() < frameB.getFrame();
                        });
                        break;
                    case 2:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getPid() < frameB.getPid();
                        });
                        break;
                    case 3:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getShiny() < frameB.getShiny();
                        });
                        break;
                    case 4:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getNature() < frameB.getNature();
                        });
                        break;
                    case 5:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getAbility() < frameB.getAbility();
                        });
                        break;
                    case 6:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(0) < frameB.getIV(1);
                        });
                        break;
                    case 7:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(1) < frameB.getIV(1);
                        });
                        break;
                    case 8:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(2) < frameB.getIV(2);
                        });
                        break;
                    case 9:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(3) < frameB.getIV(3);
                        });
                        break;
                    case 10:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(4) < frameB.getIV(4);
                        });
                        break;
                    case 11:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(5) < frameB.getIV(5);
                        });
                        break;
                    case 12:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getHidden() < frameB.getHidden();
                        });
                        break;
                    case 13:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getPower() < frameB.getPower();
                        });
                        break;
                    case 14:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getGender() < frameB.getGender();
                        });
                        break;
                }
                break;
            case WondercardIVs:
                switch (column)
                {
                    case 0:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getSeed() < frameB.getSeed();
                        });
                        break;
                    case 1:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getFrame() < frameB.getFrame();
                        });
                        break;
                    case 2:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(0) < frameB.getIV(0);
                        });
                        break;
                    case 3:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(1) < frameB.getIV(1);
                        });
                        break;
                    case 4:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(2) < frameB.getIV(2);
                        });
                        break;
                    case 5:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(3) < frameB.getIV(3);
                        });
                        break;
                    case 6:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(4) < frameB.getIV(4);
                        });
                        break;
                    case 7:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(5) < frameB.getIV(5);
                        });
                        break;
                    case 8:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getHidden() < frameB.getHidden();
                        });
                        break;
                    case 9:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getPower() < frameB.getPower();
                        });
                        break;
                }
                break;
            case MethodJ:
            case MethodK:
                switch (column)
                {
                    case 0:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getSeed() < frameB.getSeed();
                        });
                        break;
                    case 1:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getFrame() < frameB.getFrame();
                        });
                        break;
                    case 2:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getLeadType() < frameB.getLeadType();
                        });
                        break;
                    case 3:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getPid() < frameB.getPid();
                        });
                        break;
                    case 4:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getEncounterSlot() < frameB.getEncounterSlot();
                        });
                        break;
                    case 5:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getShiny() < frameB.getShiny();
                        });
                        break;
                    case 6:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getNature() < frameB.getNature();
                        });
                        break;
                    case 7:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getAbility() < frameB.getAbility();
                        });
                        break;
                    case 8:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(0) < frameB.getIV(0);
                        });
                        break;
                    case 9:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(1) < frameB.getIV(1);
                        });
                        break;
                    case 10:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(2) < frameB.getIV(2);
                        });
                        break;
                    case 11:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(3) < frameB.getIV(3);
                        });
                        break;
                    case 12:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(4) < frameB.getIV(4);
                        });
                        break;
                    case 13:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(5) < frameB.getIV(5);
                        });
                        break;
                    case 14:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getHidden() < frameB.getHidden();
                        });
                        break;
                    case 15:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getPower() < frameB.getPower();
                        });
                        break;
                    case 16:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getGender() < frameB.getGender();
                        });
                        break;
                }
                break;
            default:
                break;
        }
    }
    else
    {
        switch (method)
        {
            case Method1:
            case ChainedShiny:
                switch (column)
                {
                    case 0:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getSeed() > frameB.getSeed();
                        });
                        break;
                    case 1:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getFrame() > frameB.getFrame();
                        });
                        break;
                    case 2:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getPid() > frameB.getPid();
                        });
                        break;
                    case 3:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getShiny() > frameB.getShiny();
                        });
                        break;
                    case 4:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getNature() > frameB.getNature();
                        });
                        break;
                    case 5:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getAbility() > frameB.getAbility();
                        });
                        break;
                    case 6:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(0) > frameB.getIV(0);
                        });
                        break;
                    case 7:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(1) > frameB.getIV(1);
                        });
                        break;
                    case 8:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(2) > frameB.getIV(2);
                        });
                        break;
                    case 9:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(3) > frameB.getIV(3);
                        });
                        break;
                    case 10:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(4) > frameB.getIV(4);
                        });
                        break;
                    case 11:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(5) > frameB.getIV(5);
                        });
                        break;
                    case 12:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getHidden() > frameB.getHidden();
                        });
                        break;
                    case 13:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getPower() > frameB.getPower();
                        });
                        break;
                    case 14:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getGender() > frameB.getGender();
                        });
                        break;
                }
                break;
            case WondercardIVs:
                switch (column)
                {
                    case 0:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getSeed() > frameB.getSeed();
                        });
                        break;
                    case 1:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getFrame() > frameB.getFrame();
                        });
                        break;
                    case 2:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(0) > frameB.getIV(0);
                        });
                        break;
                    case 3:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(1) > frameB.getIV(1);
                        });
                        break;
                    case 4:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(2) > frameB.getIV(2);
                        });
                        break;
                    case 5:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(3) > frameB.getIV(3);
                        });
                        break;
                    case 6:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(4) > frameB.getIV(4);
                        });
                        break;
                    case 7:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(5) > frameB.getIV(5);
                        });
                        break;
                    case 8:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getHidden() > frameB.getHidden();
                        });
                        break;
                    case 9:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getPower() > frameB.getPower();
                        });
                        break;
                }
                break;
            case MethodJ:
            case MethodK:
                switch (column)
                {
                    case 0:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getSeed() > frameB.getSeed();
                        });
                        break;
                    case 1:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getFrame() > frameB.getFrame();
                        });
                        break;
                    case 2:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getLeadType() > frameB.getLeadType();
                        });
                        break;
                    case 3:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getPid() > frameB.getPid();
                        });
                        break;
                    case 4:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getEncounterSlot() > frameB.getEncounterSlot();
                        });
                        break;
                    case 5:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getShiny() > frameB.getShiny();
                        });
                        break;
                    case 6:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getNature() > frameB.getNature();
                        });
                        break;
                    case 7:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getAbility() > frameB.getAbility();
                        });
                        break;
                    case 8:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(0) > frameB.getIV(0);
                        });
                        break;
                    case 9:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(1) > frameB.getIV(1);
                        });
                        break;
                    case 10:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(2) > frameB.getIV(2);
                        });
                        break;
                    case 11:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(3) > frameB.getIV(3);
                        });
                        break;
                    case 12:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(4) > frameB.getIV(4);
                        });
                        break;
                    case 13:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getIV(5) > frameB.getIV(5);
                        });
                        break;
                    case 14:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getHidden() > frameB.getHidden();
                        });
                        break;
                    case 15:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getPower() > frameB.getPower();
                        });
                        break;
                    case 16:
                        std::sort(model.begin(), model.end(), [] (const Frame4 & frameA, const Frame4 & frameB)
                        {
                            return frameA.getGender() > frameB.getGender();
                        });
                        break;
                }
                break;
            default:
                break;
        }
    }

    emit layoutChanged();
}

int Searcher4Model::rowCount(const QModelIndex &parent) const
{
    (void) parent;
    return model.size();
}

int Searcher4Model::columnCount(const QModelIndex &parent) const
{
    (void) parent;
    switch (method)
    {
        case Method1:
        case ChainedShiny:
            return 15;
        case WondercardIVs:
            return 10;
        case MethodJ:
        case MethodK:
            return 17;
        default:
            return 0;
    }
}

QVariant Searcher4Model::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        int column = index.column();
        Frame4 frame = model[index.row()];
        switch (method)
        {
            case WondercardIVs:
                switch (column)
                {
                    case 0:
                        return QString::number(frame.getSeed(), 16).toUpper().rightJustified(8, '0');
                    case 1:
                        return frame.getFrame();
                    case 2:
                        return frame.getIV(0);
                    case 3:
                        return frame.getIV(1);
                    case 4:
                        return frame.getIV(2);
                    case 5:
                        return frame.getIV(3);
                    case 6:
                        return frame.getIV(4);
                    case 7:
                        return frame.getIV(5);
                    case 8:
                        return frame.getPowerString();
                    case 9:
                        return frame.getPower();
                }
                break;
            case Method1:
            case ChainedShiny:
                switch (column)
                {
                    case 0:
                        return QString::number(frame.getSeed(), 16).toUpper().rightJustified(8, '0');
                    case 1:
                        return frame.getFrame();
                    case 2:
                        return QString::number(frame.getPid(), 16).toUpper().rightJustified(8, '0');
                    case 3:
                        return frame.getShinyString();
                    case 4:
                        return frame.getNatureString();
                    case 5:
                        return frame.getAbility();
                    case 6:
                        return frame.getIV(0);
                    case 7:
                        return frame.getIV(1);
                    case 8:
                        return frame.getIV(2);
                    case 9:
                        return frame.getIV(3);
                    case 10:
                        return frame.getIV(4);
                    case 11:
                        return frame.getIV(5);
                    case 12:
                        return frame.getPowerString();
                    case 13:
                        return frame.getPower();
                    case 14:
                        return frame.getGenderString();

                }
                break;
            case MethodJ:
            case MethodK:
                switch (column)
                {
                    case 0:
                        return QString::number(frame.getSeed(), 16).toUpper().rightJustified(8, '0');
                    case 1:
                        return frame.getFrame();
                    case 2:
                        switch (frame.getLeadType())
                        {
                            case None:
                                return tr("None");
                            case Synchronize:
                                return tr("Synchronize");
                            case SuctionCups:
                                return tr("Suction Cups");
                            case CuteCharmFemale:
                                return tr("Cute Charm (♀)");
                            case CuteCharm25M:
                                return tr("Cute Charm (25% ♂");
                            case CuteCharm50M:
                                return tr("Cute Charm (50% ♂");
                            case CuteCharm75M:
                                return tr("Cute Charm (75% ♂");
                            case CuteCharm875M:
                            default:
                                return tr("Cute Charm (87.5% ♂");
                        }
                    case 3:
                        return QString::number(frame.getPid(), 16).toUpper().rightJustified(8, '0');
                    case 4:
                        return frame.getEncounterSlot();
                    case 5:
                        return frame.getShinyString();
                    case 6:
                        return frame.getNatureString();
                    case 7:
                        return frame.getAbility();
                    case 8:
                        return frame.getIV(0);
                    case 9:
                        return frame.getIV(1);
                    case 10:
                        return frame.getIV(2);
                    case 11:
                        return frame.getIV(3);
                    case 12:
                        return frame.getIV(4);
                    case 13:
                        return frame.getIV(5);
                    case 14:
                        return frame.getPowerString();
                    case 15:
                        return frame.getPower();
                    case 16:
                        return frame.getGenderString();
                }
                break;
            default:
                break;
        }
    }
    return QVariant();
}

QVariant Searcher4Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (method)
        {
            case Method1:
            case ChainedShiny:
                switch (section)
                {
                    case 0:
                        return tr("Seed");
                    case 1:
                        return tr("Frame");
                    case 2:
                        return tr("PID");
                    case 3:
                        return "!!!";
                    case 4:
                        return tr("Nature");
                    case 5:
                        return tr("Ability");
                    case 6:
                        return tr("HP");
                    case 7:
                        return tr("Atk");
                    case 8:
                        return tr("Def");
                    case 9:
                        return tr("SpA");
                    case 10:
                        return tr("SpD");
                    case 11:
                        return tr("Spe");
                    case 12:
                        return tr("Hidden");
                    case 13:
                        return tr("Power");
                    case 14:
                        return tr("Gender");
                }
                break;
            case WondercardIVs:
                switch (section)
                {
                    case 0:
                        return tr("Seed");
                    case 1:
                        return tr("Frame");
                    case 2:
                        return tr("HP");
                    case 3:
                        return tr("Atk");
                    case 4:
                        return tr("Def");
                    case 5:
                        return tr("SpA");
                    case 6:
                        return tr("SpD");
                    case 7:
                        return tr("Spe");
                    case 8:
                        return tr("Hidden");
                    case 9:
                        return tr("Power");
                }
                break;
            case MethodJ:
            case MethodK:
                switch (section)
                {
                    case 0:
                        return tr("Seed");
                    case 1:
                        return tr("Frame");
                    case 2:
                        return tr("Lead");
                    case 3:
                        return tr("PID");
                    case 4:
                        return tr("Slot");
                    case 5:
                        return "!!!";
                    case 6:
                        return tr("Nature");
                    case 7:
                        return tr("Ability");
                    case 8:
                        return tr("HP");
                    case 9:
                        return tr("Atk");
                    case 10:
                        return tr("Def");
                    case 11:
                        return tr("SpA");
                    case 12:
                        return tr("SpD");
                    case 13:
                        return tr("Spe");
                    case 14:
                        return tr("Hidden");
                    case 15:
                        return tr("Power");
                    case 16:
                        return tr("Gender");
                }
            default:
                break;
        }
    }
    return QVariant();
}
