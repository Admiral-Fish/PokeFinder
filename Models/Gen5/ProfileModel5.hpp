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

#ifndef PROFILE5MODEL_HPP
#define PROFILE5MODEL_HPP

#include <Core/Gen5/Profile5.hpp>
#include <Models/TableModel.hpp>

class ProfileModel5 : public TableModel<Profile5>
{
    Q_OBJECT
public:
    explicit ProfileModel5(QObject *parent);
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QStringList header
        = { tr("Profile Name"), tr("Version"), tr("Language"), tr("TID"),    tr("SID"),        tr("MAC Address"), tr("DS Type"),
            tr("VCount"),       tr("Timer0"),  tr("GxStat"),   tr("VFrame"), tr("Keypresses"), tr("Skip L/R"),    tr("Soft Reset") };
};

#endif // PROFILE5MODEL_HPP
