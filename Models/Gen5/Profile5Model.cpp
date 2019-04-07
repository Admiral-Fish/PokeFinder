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

#include "Profile5Model.hpp"

Profile5Model::Profile5Model(QObject *parent) :
    QAbstractTableModel(parent)
{
}

void Profile5Model::setModel(const QVector<Profile5> &profiles)
{
    if (!profiles.isEmpty())
    {
        int i = rowCount();
        emit beginInsertRows(QModelIndex(), i, i + profiles.size() - 1);
        model.append(profiles);
        emit endInsertRows();
    }
}

void Profile5Model::addItem(const Profile5 &profile)
{
    int i = rowCount();
    emit beginInsertRows(QModelIndex(), i, i);
    model.push_back(profile);
    emit endInsertRows();
}

void Profile5Model::updateProfile(const Profile5 &profile, int row)
{
    model[row] = profile;
    emit dataChanged(index(row, 0), index(row, columnCount()));
}

int Profile5Model::rowCount(const QModelIndex &parent) const
{
    (void) parent;
    return model.size();
}

int Profile5Model::columnCount(const QModelIndex &parent) const
{
    (void) parent;
    return 14;
}

QVariant Profile5Model::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        auto profile = model.at(index.row());
        switch (index.column())
        {
            case 0:
                return profile.getProfileName();
            case 1:
                return profile.getVersionString();
            case 2:
                return profile.getLanguageString();
            case 3:
                return QString::number(profile.getTID());
            case 4:
                return QString::number(profile.getSID());
            case 5:
                return QString::number(profile.getMac(), 16);
            case 6:
                return profile.getDSTypeString();
            case 7:
                return QString::number(profile.getVCount(), 16);
            case 8:
                return QString::number(profile.getTimer0Min(), 16) + "/" + QString::number(profile.getTimer0Max(), 16);
            case 9:
                return profile.getGxStat();
            case 10:
                return profile.getVFrame();
            case 11:
                {
                    QStringList presses;
                    QVector<bool> keys = profile.getKeypresses();
                    for (u8 i = 0; i < keys.size(); i++)
                    {
                        if (i == 0 && keys.at(0))
                        {
                            presses.append(tr("None"));
                        }
                        else if (keys.at(i))
                        {
                            presses.append(QString::number(i));
                        }
                    }
                    return presses.join(", ");
                }
            case 12:
                return profile.getSkipLR() ? tr("True") : tr("False");
            case 13:
                return profile.getSoftReset() ? tr("True") : tr("False");
        }
    }
    return QVariant();
}

QVariant Profile5Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
    {
        switch (section)
        {
            case 0:
                return tr("Profile Name");
            case 1:
                return tr("Version");
            case 2:
                return tr("Language");
            case 3:
                return tr("TID");
            case 4:
                return tr("SID");
            case 5:
                return tr("MAC Address");
            case 6:
                return tr("DS Type");
            case 7:
                return tr("VCount");
            case 8:
                return tr("Timer0");
            case 9:
                return tr("GxStat");
            case 10:
                return tr("VFrame");
            case 11:
                return tr("Keypresses");
            case 12:
                return tr("Skip L/R");
            case 13:
                return tr("Soft Reset");

        }
    }
    return QVariant();
}

Profile5 Profile5Model::getProfile(int index)
{
    return model.at(index);
}

void Profile5Model::removeProfile(int index)
{
    emit beginRemoveRows(QModelIndex(), index, index);
    model.erase(model.begin() + index);
    model.squeeze();
    emit endRemoveRows();
}
