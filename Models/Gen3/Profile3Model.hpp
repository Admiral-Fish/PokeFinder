/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish and bumba
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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

#ifndef PROFILE3MODEL_HPP
#define PROFILE3MODEL_HPP

#include <QAbstractTableModel>
#include <libPokeFinder/Gen3/Profile3.hpp>

class Profile3Model : public QAbstractTableModel
{

    Q_OBJECT

private:
    vector<Profile3> model;

public:
    Profile3Model(QObject *parent);
    void setModel(vector<Profile3> profiles);
    void addItem(Profile3 profile);
    void updateProfile(Profile3 profile, int row);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Profile3 getProfile(int index);
    void removeProfile(int index);

};

#endif // PROFILE3MODEL_HPP
