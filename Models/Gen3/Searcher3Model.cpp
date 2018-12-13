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

#include "Searcher3Model.hpp"

Searcher3Model::Searcher3Model(QObject *parent, Method method) : QAbstractTableModel(parent)
{
    this->method = method;
}

void Searcher3Model::setModel(const QVector<Frame3> &frames)
{
    model = frames;
}

void Searcher3Model::addItems(const QVector<Frame3> &frames)
{
    int i = rowCount();
    emit beginInsertRows(QModelIndex(), i, i + frames.size() - 1);
    model.append(frames);
    emit endInsertRows();
}

void Searcher3Model::clear()
{
    if (model.isEmpty())
    {
        return;
    }

    emit beginRemoveRows(QModelIndex(), 0, rowCount() - 1);
    model.clear();
    model.squeeze();
    emit endRemoveRows();
}

void Searcher3Model::setMethod(Method method)
{
    this->method = method;
    emit headerDataChanged(Qt::Horizontal, 0, columnCount());
}

void Searcher3Model::sort(int column, Qt::SortOrder order)
{
    if (model.empty())
    {
        return;
    }

    emit layoutAboutToBeChanged();
    bool flag = order == Qt::AscendingOrder;

    switch (method)
    {
        case Method::MethodH1:
        case Method::MethodH2:
        case Method::MethodH4:
            switch (column)
            {
                case 0:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getSeed() < frame2.getSeed() : frame1.getSeed() > frame2.getSeed();
                    });
                    break;
                case 1:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getLeadType() < frame2.getLeadType(): frame1.getLeadType() > frame2.getLeadType();
                    });
                    break;
                case 2:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getEncounterSlot() < frame2.getEncounterSlot(): frame1.getEncounterSlot() > frame2.getEncounterSlot();
                    });
                    break;
                case 3:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getLevel() < frame2.getLevel(): frame1.getLevel() > frame2.getLevel();
                    });
                    break;
                case 4:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getPID() < frame2.getPID(): frame1.getPID() > frame2.getPID();
                    });
                    break;
                case 5:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getShiny() < frame2.getShiny(): frame1.getShiny() > frame2.getShiny();
                    });
                    break;
                case 6:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getNature() < frame2.getNature() : frame1.getNature() > frame2.getNature();
                    });
                    break;
                case 7:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getAbility() < frame2.getAbility(): frame1.getAbility() > frame2.getAbility();
                    });
                    break;
                case 8:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getIV(0) < frame2.getIV(0): frame1.getIV(0) > frame2.getIV(0);
                    });
                    break;
                case 9:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getIV(1) < frame2.getIV(1): frame1.getIV(1) > frame2.getIV(1);
                    });
                    break;
                case 10:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getIV(2) < frame2.getIV(2): frame1.getIV(2) > frame2.getIV(2);
                    });
                    break;
                case 11:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getIV(3) < frame2.getIV(3): frame1.getIV(3) > frame2.getIV(3);
                    });
                    break;
                case 12:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getIV(4) < frame2.getIV(4): frame1.getIV(4) > frame2.getIV(4);
                    });
                    break;
                case 13:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getIV(5) < frame2.getIV(5): frame1.getIV(5) > frame2.getIV(5);
                    });
                    break;
                case 14:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getHidden() < frame2.getHidden() : frame1.getHidden() > frame2.getHidden();
                    });
                    break;
                case 15:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getPower() < frame2.getPower() : frame1.getPower() > frame2.getPower();
                    });
                    break;
                case 16:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getGender() < frame2.getGender() : frame1.getGender() > frame2.getGender();
                    });
                    break;
            }
            break;
        case Method::XD:
        case Method::Colo:
            switch (column)
            {
                case 0:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getSeed() < frame2.getSeed() : frame1.getSeed() > frame2.getSeed();
                    });
                    break;
                case 1:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getPID() < frame2.getPID() : frame1.getPID() > frame2.getPID();
                    });
                    break;
                case 2:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getShiny() < frame2.getShiny() : frame1.getShiny() > frame2.getShiny();
                    });
                    break;
                case 3:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getNature() < frame2.getNature() : frame1.getNature() > frame2.getNature();
                    });
                    break;
                case 4:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getAbility() < frame2.getAbility() : frame1.getAbility() > frame2.getAbility();
                    });
                    break;
                case 5:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getIV(0) < frame2.getIV(0): frame1.getIV(0) > frame2.getIV(0);
                    });
                    break;
                case 6:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getIV(1) < frame2.getIV(1): frame1.getIV(1) > frame2.getIV(1);
                    });
                    break;
                case 7:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getIV(2) < frame2.getIV(2): frame1.getIV(2) > frame2.getIV(2);
                    });
                    break;
                case 8:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getIV(3) < frame2.getIV(3): frame1.getIV(3) > frame2.getIV(3);
                    });
                    break;
                case 9:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getIV(4) < frame2.getIV(4): frame1.getIV(4) > frame2.getIV(4);
                    });
                    break;
                case 10:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getIV(5) < frame2.getIV(5): frame1.getIV(5) > frame2.getIV(5);
                    });
                    break;
                case 11:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getHidden() < frame2.getHidden() : frame1.getHidden() > frame2.getHidden();
                    });
                    break;
                case 12:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getPower() < frame2.getPower() : frame1.getPower() > frame2.getPower();
                    });
                    break;
                case 13:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getGender() < frame2.getGender() : frame1.getGender() > frame2.getGender();
                    });
                    break;
                case 14:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getLockReason() < frame2.getLockReason() : frame1.getLockReason() > frame2.getLockReason();
                    });
                    break;
            }
            break;
        case Method::Method1:
        case Method::Method1Reverse:
        case Method::Method2:
        case Method::Method4:
        case Method::XDColo:
        case Method::Channel:
            switch (column)
            {
                case 0:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getSeed() < frame2.getSeed() : frame1.getSeed() > frame2.getSeed();
                    });
                    break;
                case 1:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getPID() < frame2.getPID() : frame1.getPID() > frame2.getPID();
                    });
                    break;
                case 2:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getShiny() < frame2.getShiny() : frame1.getShiny() > frame2.getShiny();
                    });
                    break;
                case 3:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getNature() < frame2.getNature() : frame1.getNature() > frame2.getNature();
                    });
                    break;
                case 4:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getAbility() < frame2.getAbility() : frame1.getAbility() > frame2.getAbility();
                    });
                    break;
                case 5:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getIV(0) < frame2.getIV(0): frame1.getIV(0) > frame2.getIV(0);
                    });
                    break;
                case 6:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getIV(1) < frame2.getIV(1): frame1.getIV(1) > frame2.getIV(1);
                    });
                    break;
                case 7:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getIV(2) < frame2.getIV(2): frame1.getIV(2) > frame2.getIV(2);
                    });
                    break;
                case 8:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getIV(3) < frame2.getIV(3): frame1.getIV(3) > frame2.getIV(3);
                    });
                    break;
                case 9:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getIV(4) < frame2.getIV(4): frame1.getIV(4) > frame2.getIV(4);
                    });
                    break;
                case 10:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getIV(5) < frame2.getIV(5): frame1.getIV(5) > frame2.getIV(5);
                    });
                    break;
                case 11:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getHidden() < frame2.getHidden() : frame1.getHidden() > frame2.getHidden();
                    });
                    break;
                case 12:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getPower() < frame2.getPower() : frame1.getPower() > frame2.getPower();
                    });
                    break;
                case 13:
                    std::sort(model.begin(), model.end(), [flag] (const Frame3 & frame1, const Frame3 & frame2)
                    {
                        return flag ? frame1.getGender() < frame2.getGender() : frame1.getGender() > frame2.getGender();
                    });
                    break;
            }
            break;
        default:
            break;
    }

    emit layoutChanged();
}

int Searcher3Model::rowCount(const QModelIndex &parent) const
{
    (void) parent;
    return model.size();
}

int Searcher3Model::columnCount(const QModelIndex &parent) const
{
    (void) parent;
    switch (method)
    {
        case Method::MethodH1:
        case Method::MethodH2:
        case Method::MethodH4:
            return 17;
        case Method::XD:
        case Method::Colo:
            return 15;
        case Method::Method1:
        case Method::Method1Reverse:
        case Method::Method2:
        case Method::Method4:
        case Method::XDColo:
        case Method::Channel:
            return 14;
        default:
            return 0;
    }
}

QVariant Searcher3Model::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        auto frame = model.at(index.row());
        switch (method)
        {
            case Method::MethodH1:
            case Method::MethodH2:
            case Method::MethodH4:
                switch (index.column())
                {
                    case 0:
                        return QString::number(frame.getSeed(), 16).toUpper().rightJustified(8, '0');
                    case 1:
                        {
                            Lead type = frame.getLeadType();
                            return type == Lead::None ? tr("None") : type == Lead::Synchronize ? tr("Synch") : tr("Cute Charm");
                        }
                    case 2:
                        return frame.getEncounterSlot();
                    case 3:
                        return frame.getLevel();
                    case 4:
                        return QString::number(frame.getPID(), 16).toUpper().rightJustified(8, '0');
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
            case Method::Method1:
            case Method::Method1Reverse:
            case Method::Method2:
            case Method::Method4:
            case Method::XDColo:
            case Method::Channel:
                switch (index.column())
                {
                    case 0:
                        return QString::number(frame.getSeed(), 16).toUpper().rightJustified(8, '0');
                    case 1:
                        return QString::number(frame.getPID(), 16).toUpper().rightJustified(8, '0');
                    case 2:
                        return frame.getShinyString();
                    case 3:
                        return frame.getNatureString();
                    case 4:
                        return frame.getAbility();
                    case 5:
                        return frame.getIV(0);
                    case 6:
                        return frame.getIV(1);
                    case 7:
                        return frame.getIV(2);
                    case 8:
                        return frame.getIV(3);
                    case 9:
                        return frame.getIV(4);
                    case 10:
                        return frame.getIV(5);
                    case 11:
                        return frame.getPowerString();
                    case 12:
                        return frame.getPower();
                    case 13:
                        return frame.getGenderString();
                }
            case Method::XD:
            case Method::Colo:
                switch (index.column())
                {
                    case 0:
                        return QString::number(frame.getSeed(), 16).toUpper().rightJustified(8, '0');
                    case 1:
                        return QString::number(frame.getPID(), 16).toUpper().rightJustified(8, '0');
                    case 2:
                        return frame.getShinyString();
                    case 3:
                        return frame.getNatureString();
                    case 4:
                        return frame.getAbility();
                    case 5:
                        return frame.getIV(0);
                    case 6:
                        return frame.getIV(1);
                    case 7:
                        return frame.getIV(2);
                    case 8:
                        return frame.getIV(3);
                    case 9:
                        return frame.getIV(4);
                    case 10:
                        return frame.getIV(5);
                    case 11:
                        return frame.getPowerString();
                    case 12:
                        return frame.getPower();
                    case 13:
                        return frame.getGenderString();
                    case 14:
                        return frame.getLockReason();
                }
            default:
                break;
        }
    }
    return QVariant();
}

QVariant Searcher3Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (method)
        {
            case Method::MethodH1:
            case Method::MethodH2:
            case Method::MethodH4:
                switch (section)
                {
                    case 0:
                        return tr("Seed");
                    case 1:
                        return tr("Lead");
                    case 2:
                        return tr("Slot");
                    case 3:
                        return tr("Level");
                    case 4:
                        return tr("PID");
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
            case Method::Method1:
            case Method::Method1Reverse:
            case Method::Method2:
            case Method::Method4:
            case Method::XDColo:
            case Method::Channel:
                switch (section)
                {
                    case 0:
                        return tr("Seed");
                    case 1:
                        return tr("PID");
                    case 2:
                        return "!!!";
                    case 3:
                        return tr("Nature");
                    case 4:
                        return tr("Ability");
                    case 5:
                        return tr("HP");
                    case 6:
                        return tr("Atk");
                    case 7:
                        return tr("Def");
                    case 8:
                        return tr("SpA");
                    case 9:
                        return tr("SpD");
                    case 10:
                        return tr("Spe");
                    case 11:
                        return tr("Hidden");
                    case 12:
                        return tr("Power");
                    case 13:
                        return tr("Gender");
                }
            case Method::XD:
            case Method::Colo:
                switch (section)
                {
                    case 0:
                        return tr("Seed");
                    case 1:
                        return tr("PID");
                    case 2:
                        return "!!!";
                    case 3:
                        return tr("Nature");
                    case 4:
                        return tr("Ability");
                    case 5:
                        return tr("HP");
                    case 6:
                        return tr("Atk");
                    case 7:
                        return tr("Def");
                    case 8:
                        return tr("SpA");
                    case 9:
                        return tr("SpD");
                    case 10:
                        return tr("Spe");
                    case 11:
                        return tr("Hidden");
                    case 12:
                        return tr("Power");
                    case 13:
                        return tr("Gender");
                    case 14:
                        return tr("Reason");
                }
            default:
                break;
        }
    }
    return QVariant();
}
