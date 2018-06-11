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

#include "Profile4Model.hpp"

Profile4Model::Profile4Model(QObject *parent) : QAbstractTableModel(parent)
{
}

void Profile4Model::setModel(vector<Profile4> profiles)
{
    if (profiles.empty())
        return;
    int i = rowCount();
    emit beginInsertRows(QModelIndex(), i, i + profiles.size() - 1);
    model.insert(model.end(), profiles.begin(), profiles.end());
    emit endInsertRows();
}

void Profile4Model::addItem(Profile4 profile)
{
    int i = rowCount();
    emit beginInsertRows(QModelIndex(), i, i);
    model.push_back(profile);
    emit endInsertRows();
}

void Profile4Model::updateProfile(Profile4 profile, int row)
{
    model[row] = profile;
    emit dataChanged(index(row, 0), index(row, columnCount()));
}

int Profile4Model::rowCount(const QModelIndex &parent) const
{
    (void) parent;
    return (int)model.size();
}

int Profile4Model::columnCount(const QModelIndex &parent) const
{
    (void) parent;
    return 5;
}

QVariant Profile4Model::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        int row = index.row();
        int column = index.column();
        Profile4 profile = model[row];
        switch (column)
        {
            case 0:
                return profile.getProfileName();
            case 1:
                return profile.getVersionString();
            case 2:
                return profile.getLanguageString();
            case 3:
                return profile.getTid();
            case 4:
                return profile.getSid();
        }
    }
    return QVariant();
}

QVariant Profile4Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
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
            }
        }
    }
    return QVariant();
}

Profile4 Profile4Model::getProfile(int index)
{
    return model[index];
}

void Profile4Model::removeProfile(int index)
{
    emit beginRemoveRows(QModelIndex(), index, index);
    model.erase(model.begin() + index);
    model.shrink_to_fit();
    emit endRemoveRows();
}
