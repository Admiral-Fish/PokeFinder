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

#include "Profile3Model.hpp"

Profile3Model::Profile3Model(QObject *parent) : QAbstractTableModel(parent)
{
}

void Profile3Model::setModel(const QVector<Profile3> &profiles)
{
    if (profiles.isEmpty())
    {
        return;
    }

    int i = rowCount();
    emit beginInsertRows(QModelIndex(), i, i + profiles.size() - 1);
    model.append(profiles);
    emit endInsertRows();
}

void Profile3Model::addItem(const Profile3 &profile)
{
    int i = rowCount();
    emit beginInsertRows(QModelIndex(), i, i);
    model.push_back(profile);
    emit endInsertRows();
}

void Profile3Model::updateProfile(const Profile3 &profile, int row)
{
    model[row] = profile;
    emit dataChanged(index(row, 0), index(row, columnCount()));
}

int Profile3Model::rowCount(const QModelIndex &parent) const
{
    (void) parent;
    return model.size();
}

int Profile3Model::columnCount(const QModelIndex &parent) const
{
    (void) parent;
    return 6;
}

QVariant Profile3Model::data(const QModelIndex &index, int role) const
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
                return profile.getTID();
            case 4:
                return profile.getSID();
            case 5:
                if (profile.getVersion() == Ruby || profile.getVersion() == Sapphire)
                    return profile.getDeadBattery() ? tr("Yes") : tr("No");
                else
                    return "N/A";
        }
    }
    return QVariant();
}

QVariant Profile3Model::headerData(int section, Qt::Orientation orientation, int role) const
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
                return tr("Dead Battery");
        }
    }
    return QVariant();
}

Profile3 Profile3Model::getProfile(int index)
{
    return model[index];
}

void Profile3Model::removeProfile(int index)
{
    emit beginRemoveRows(QModelIndex(), index, index);
    model.erase(model.begin() + index);
    model.squeeze();
    emit endRemoveRows();
}
