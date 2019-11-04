/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2019 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Util/TableUtility.hpp>

namespace PokeFinderModels
{
    Searcher4Model::Searcher4Model(QObject *parent, PokeFinderCore::Method method, bool stationary)
        : TableModel<PokeFinderCore::Frame4>(parent)
    {
        this->method = method;
        this->stationary = stationary;
    }

    void Searcher4Model::setMethod(PokeFinderCore::Method method)
    {
        this->method = method;
        emit headerDataChanged(Qt::Horizontal, 0, columnCount());
    }

    void Searcher4Model::sort(int column, Qt::SortOrder order)
    {
        if (!model.empty())
        {
            emit layoutAboutToBeChanged();
            bool flag = order == Qt::AscendingOrder;
            bool stat = stationary;

            switch (method)
            {
            case PokeFinderCore::Method::Method1:
            case PokeFinderCore::Method::ChainedShiny:
                switch (column)
                {
                case 0:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getSeed() < frame2.getSeed() : frame1.getSeed() > frame2.getSeed();
                        });
                    break;
                case 1:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getFrame() < frame2.getFrame() : frame1.getFrame() > frame2.getFrame();
                        });
                    break;
                case 2:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getPID() < frame2.getPID() : frame1.getPID() > frame2.getPID();
                        });
                    break;
                case 3:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getShiny() < frame2.getShiny() : frame1.getShiny() > frame2.getShiny();
                        });
                    break;
                case 4:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getNature() < frame2.getNature()
                                        : frame1.getNature() > frame2.getNature();
                        });
                    break;
                case 5:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getAbility() < frame2.getAbility()
                                        : frame1.getAbility() > frame2.getAbility();
                        });
                    break;
                case 6:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getIV(0) < frame2.getIV(0) : frame1.getIV(0) > frame2.getIV(0);
                        });
                    break;
                case 7:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getIV(1) < frame2.getIV(1) : frame1.getIV(1) > frame2.getIV(1);
                        });
                    break;
                case 8:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getIV(2) < frame2.getIV(2) : frame1.getIV(2) > frame2.getIV(2);
                        });
                    break;
                case 9:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getIV(3) < frame2.getIV(3) : frame1.getIV(3) > frame2.getIV(3);
                        });
                    break;
                case 10:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getIV(4) < frame2.getIV(4) : frame1.getIV(4) > frame2.getIV(4);
                        });
                    break;
                case 11:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getIV(5) < frame2.getIV(5) : frame1.getIV(5) > frame2.getIV(5);
                        });
                    break;
                case 12:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getHidden() < frame2.getHidden()
                                        : frame1.getHidden() > frame2.getHidden();
                        });
                    break;
                case 13:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getPower() < frame2.getPower() : frame1.getPower() > frame2.getPower();
                        });
                    break;
                case 14:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getGender() < frame2.getGender()
                                        : frame1.getGender() > frame2.getGender();
                        });
                    break;
                }
                break;
            case PokeFinderCore::Method::WondercardIVs:
                switch (column)
                {
                case 0:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getSeed() < frame2.getSeed() : frame1.getSeed() > frame2.getSeed();
                        });
                    break;
                case 1:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getFrame() < frame2.getFrame() : frame1.getFrame() > frame2.getFrame();
                        });
                    break;
                case 2:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getIV(0) < frame2.getIV(0) : frame1.getIV(0) > frame2.getIV(0);
                        });
                    break;
                case 3:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getIV(1) < frame2.getIV(1) : frame1.getIV(1) > frame2.getIV(1);
                        });
                    break;
                case 4:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getIV(2) < frame2.getIV(2) : frame1.getIV(2) > frame2.getIV(2);
                        });
                    break;
                case 5:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getIV(3) < frame2.getIV(3) : frame1.getIV(3) > frame2.getIV(3);
                        });
                    break;
                case 6:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getIV(4) < frame2.getIV(4) : frame1.getIV(4) > frame2.getIV(4);
                        });
                    break;
                case 7:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getIV(5) < frame2.getIV(5) : frame1.getIV(5) > frame2.getIV(5);
                        });
                    break;
                case 8:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getHidden() < frame2.getHidden()
                                        : frame1.getHidden() > frame2.getHidden();
                        });
                    break;
                case 9:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getPower() < frame2.getPower() : frame1.getPower() > frame2.getPower();
                        });
                    break;
                }
                break;
            case PokeFinderCore::Method::MethodJ:
            case PokeFinderCore::Method::MethodK:
                switch (column)
                {
                case 0:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getSeed() < frame2.getSeed() : frame1.getSeed() > frame2.getSeed();
                        });
                    break;
                case 1:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getFrame() < frame2.getFrame() : frame1.getFrame() > frame2.getFrame();
                        });
                    break;
                case 2:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getLeadType() < frame2.getLeadType()
                                        : frame1.getLeadType() > frame2.getLeadType();
                        });
                    break;
                case 3:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getPID() < frame2.getPID() : frame1.getPID() > frame2.getPID();
                        });
                    break;
                case 4:
                    std::sort(model.begin(), model.end(),
                        [flag, stat](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return stat
                                ? flag ? frame1.getShiny() < frame2.getShiny() : frame1.getShiny() > frame2.getShiny()
                                : flag ? frame1.getEncounterSlot() < frame2.getEncounterSlot()
                                       : frame1.getEncounterSlot() > frame2.getEncounterSlot();
                        });
                    break;
                case 5:
                    std::sort(model.begin(), model.end(),
                        [flag, stat](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return stat
                                ? flag ? frame1.getNature() < frame2.getNature()
                                       : frame1.getNature() > frame2.getNature()
                                : flag ? frame1.getLevel() < frame2.getLevel() : frame1.getLevel() > frame2.getLevel();
                        });
                    break;
                case 6:
                    std::sort(model.begin(), model.end(),
                        [flag, stat](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return stat
                                ? flag ? frame1.getAbility() < frame2.getAbility()
                                       : frame1.getAbility() > frame2.getAbility()
                                : flag ? frame1.getShiny() < frame2.getShiny() : frame1.getShiny() > frame2.getShiny();
                        });
                    break;
                case 7:
                    std::sort(model.begin(), model.end(),
                        [flag, stat](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return stat ? flag ? frame1.getIV(0) < frame2.getIV(0) : frame1.getIV(0) > frame2.getIV(0)
                                        : flag ? frame1.getNature() < frame2.getNature()
                                               : frame1.getNature() > frame2.getNature();
                        });
                    break;
                case 8:
                    std::sort(model.begin(), model.end(),
                        [flag, stat](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return stat ? flag ? frame1.getIV(1) < frame2.getIV(1) : frame1.getIV(1) > frame2.getIV(1)
                                        : flag ? frame1.getAbility() < frame2.getAbility()
                                               : frame1.getAbility() > frame2.getAbility();
                        });
                    break;
                case 9:
                    std::sort(model.begin(), model.end(),
                        [flag, stat](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return stat ? flag ? frame1.getIV(2) < frame2.getIV(2) : frame1.getIV(2) > frame2.getIV(2)
                                        : flag ? frame1.getIV(0) < frame2.getIV(0) : frame1.getIV(0) > frame2.getIV(0);
                        });
                    break;
                case 10:
                    std::sort(model.begin(), model.end(),
                        [flag, stat](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return stat ? flag ? frame1.getIV(3) < frame2.getIV(3) : frame1.getIV(3) > frame2.getIV(3)
                                        : flag ? frame1.getIV(1) < frame2.getIV(1) : frame1.getIV(1) > frame2.getIV(1);
                        });
                    break;
                case 11:
                    std::sort(model.begin(), model.end(),
                        [flag, stat](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return stat ? flag ? frame1.getIV(4) < frame2.getIV(4) : frame1.getIV(4) > frame2.getIV(4)
                                        : flag ? frame1.getIV(2) < frame2.getIV(2) : frame1.getIV(2) > frame2.getIV(2);
                        });
                    break;
                case 12:
                    std::sort(model.begin(), model.end(),
                        [flag, stat](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return stat ? flag ? frame1.getIV(5) < frame2.getIV(5) : frame1.getIV(5) > frame2.getIV(5)
                                        : flag ? frame1.getIV(3) < frame2.getIV(3) : frame1.getIV(3) > frame2.getIV(3);
                        });
                    break;
                case 13:
                    std::sort(model.begin(), model.end(),
                        [flag, stat](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return stat ? flag ? frame1.getHidden() < frame2.getHidden()
                                               : frame1.getHidden() > frame2.getHidden()
                                        : flag ? frame1.getIV(4) < frame2.getIV(4) : frame1.getIV(4) > frame2.getIV(4);
                        });
                    break;
                case 14:
                    std::sort(model.begin(), model.end(),
                        [flag, stat](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return stat
                                ? flag ? frame1.getPower() < frame2.getPower() : frame1.getPower() > frame2.getPower()
                                : flag ? frame1.getIV(5) < frame2.getIV(5) : frame1.getIV(5) > frame2.getIV(5);
                        });
                    break;
                case 15:
                    std::sort(model.begin(), model.end(),
                        [flag, stat](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return stat ? flag ? frame1.getGender() < frame2.getGender()
                                               : frame1.getGender() > frame2.getGender()
                                        : flag ? frame1.getHidden() < frame2.getHidden()
                                               : frame1.getHidden() > frame2.getHidden();
                        });
                    break;
                case 16:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getPower() < frame2.getPower() : frame1.getPower() > frame2.getPower();
                        });
                    break;
                case 17:
                    std::sort(model.begin(), model.end(),
                        [flag](const PokeFinderCore::Frame4 &frame1, const PokeFinderCore::Frame4 &frame2) {
                            return flag ? frame1.getGender() < frame2.getGender()
                                        : frame1.getGender() > frame2.getGender();
                        });
                    break;
                }
                break;
            default:
                break;
            }

            emit layoutChanged();
        }
    }

    int Searcher4Model::columnCount(const QModelIndex & /*parent*/) const
    {
        switch (method)
        {
        case PokeFinderCore::Method::Method1:
        case PokeFinderCore::Method::ChainedShiny:
            return 15;
        case PokeFinderCore::Method::WondercardIVs:
            return 10;
        case PokeFinderCore::Method::MethodJ:
        case PokeFinderCore::Method::MethodK:
            return stationary ? 16 : 18;
        default:
            return 0;
        }
    }

    QVariant Searcher4Model::data(const QModelIndex &index, int role) const
    {
        if (role == Qt::DisplayRole)
        {
            auto frame = model.at(index.row());
            switch (method)
            {
            case PokeFinderCore::Method::WondercardIVs:
                switch (index.column())
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
            case PokeFinderCore::Method::Method1:
            case PokeFinderCore::Method::ChainedShiny:
                switch (index.column())
                {
                case 0:
                    return QString::number(frame.getSeed(), 16).toUpper().rightJustified(8, '0');
                case 1:
                    return frame.getFrame();
                case 2:
                    return QString::number(frame.getPID(), 16).toUpper().rightJustified(8, '0');
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
            case PokeFinderCore::Method::MethodJ:
            case PokeFinderCore::Method::MethodK:
                switch (index.column())
                {
                case 0:
                    return QString::number(frame.getSeed(), 16).toUpper().rightJustified(8, '0');
                case 1:
                    return frame.getFrame();
                case 2:
                    switch (frame.getLeadType())
                    {
                    case PokeFinderCore::Lead::None:
                        return tr("None");
                    case PokeFinderCore::Lead::Synchronize:
                        return tr("Synchronize");
                    case PokeFinderCore::Lead::SuctionCups:
                        return tr("Suction Cups");
                    case PokeFinderCore::Lead::CuteCharmFemale:
                        return tr("Cute Charm (♀)");
                    case PokeFinderCore::Lead::CuteCharm25M:
                        return tr("Cute Charm (25% ♂)");
                    case PokeFinderCore::Lead::CuteCharm50M:
                        return tr("Cute Charm (50% ♂)");
                    case PokeFinderCore::Lead::CuteCharm75M:
                        return tr("Cute Charm (75% ♂)");
                    case PokeFinderCore::Lead::CuteCharm875M:
                    default:
                        return tr("Cute Charm (87.5% ♂)");
                    }
                case 3:
                    return QString::number(frame.getPID(), 16).toUpper().rightJustified(8, '0');
                case 4:
                    if (stationary)
                        return frame.getShinyString();
                    return frame.getEncounterSlot();
                case 5:
                    if (stationary)
                        return frame.getNatureString();
                    return frame.getLevel();
                case 6:
                    if (stationary)
                        return frame.getAbility();
                    return frame.getShinyString();
                case 7:
                    if (stationary)
                        return frame.getIV(0);
                    return frame.getNatureString();
                case 8:
                    if (stationary)
                        return frame.getIV(1);
                    return frame.getAbility();
                case 9:
                    if (stationary)
                        return frame.getIV(2);
                    return frame.getIV(0);
                case 10:
                    if (stationary)
                        return frame.getIV(3);
                    return frame.getIV(1);
                case 11:
                    if (stationary)
                        return frame.getIV(4);
                    return frame.getIV(2);
                case 12:
                    if (stationary)
                        return frame.getIV(5);
                    return frame.getIV(3);
                case 13:
                    if (stationary)
                        return frame.getPowerString();
                    return frame.getIV(4);
                case 14:
                    if (stationary)
                        return frame.getPower();
                    return frame.getIV(5);
                case 15:
                    if (stationary)
                        return frame.getGenderString();
                    return frame.getPowerString();
                case 16:
                    return frame.getPower();
                case 17:
                    return frame.getGenderString();
                }
                break;
            default:
                break;
            }
        }
        else if (role == Qt::FontRole)
        {
            auto frame = model.at(index.row());
            switch (method)
            {
            case PokeFinderCore::Method::WondercardIVs:
                switch (index.column())
                {
                case 2:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(0));
                case 3:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(1));
                case 4:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(2));
                case 5:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(3));
                case 6:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(4));
                case 7:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(5));
                }
                break;
            case PokeFinderCore::Method::Method1:
            case PokeFinderCore::Method::ChainedShiny:
                switch (index.column())
                {
                case 6:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(0));
                case 7:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(1));
                case 8:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(2));
                case 9:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(3));
                case 10:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(4));
                case 11:
                    return PokeFinderCore::TableUtility::getBold(frame.getIV(5));
                }
                break;
            case PokeFinderCore::Method::MethodJ:
            case PokeFinderCore::Method::MethodK:
                if (stationary)
                {
                    switch (index.column())
                    {
                    case 7:
                        return PokeFinderCore::TableUtility::getBold(frame.getIV(0));
                    case 8:
                        return PokeFinderCore::TableUtility::getBold(frame.getIV(1));
                    case 9:
                        return PokeFinderCore::TableUtility::getBold(frame.getIV(2));
                    case 10:
                        return PokeFinderCore::TableUtility::getBold(frame.getIV(3));
                    case 11:
                        return PokeFinderCore::TableUtility::getBold(frame.getIV(4));
                    case 12:
                        return PokeFinderCore::TableUtility::getBold(frame.getIV(5));
                    }
                }
                else
                {
                    switch (index.column())
                    {
                    case 9:
                        return PokeFinderCore::TableUtility::getBold(frame.getIV(0));
                    case 10:
                        return PokeFinderCore::TableUtility::getBold(frame.getIV(1));
                    case 11:
                        return PokeFinderCore::TableUtility::getBold(frame.getIV(2));
                    case 12:
                        return PokeFinderCore::TableUtility::getBold(frame.getIV(3));
                    case 13:
                        return PokeFinderCore::TableUtility::getBold(frame.getIV(4));
                    case 14:
                        return PokeFinderCore::TableUtility::getBold(frame.getIV(5));
                    }
                }
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
            case PokeFinderCore::Method::Method1:
            case PokeFinderCore::Method::ChainedShiny:
                return header1.at(section);
            case PokeFinderCore::Method::WondercardIVs:
                return header2.at(section);
            case PokeFinderCore::Method::MethodJ:
            case PokeFinderCore::Method::MethodK:
                return stationary ? header4.at(section) : header3.at(section);
            default:
                break;
            }
        }
        return QVariant();
    }
}
