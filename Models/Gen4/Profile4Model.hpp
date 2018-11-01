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

#ifndef PROFILE4MODEL_HPP
#define PROFILE4MODEL_HPP

#include <QAbstractTableModel>
#include <QVector>
#include <PokeFinderCore/Gen4/Profile4.hpp>

class Profile4Model : public QAbstractTableModel
{
    Q_OBJECT

private:
    QVector<Profile4> model;

public:
    Profile4Model(QObject *parent);
    void setModel(const QVector<Profile4> &profiles);
    void addItem(const Profile4 &profile);
    void updateProfile(const Profile4 &profile, int row);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Profile4 getProfile(int index);
    void removeProfile(int index);

};

#endif // PROFILE4MODEL_HPP
