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

#ifndef IDMODEL4_HPP
#define IDMODEL4_HPP

#include <Core/Gen4/Frames/IDFrame4.hpp>
#include <Models/TableModel.hpp>

class IDModel4 : public TableModel<IDFrame4>
{
    Q_OBJECT
public:
    explicit IDModel4(QObject *parent, bool flag);
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    bool showSeconds;
    QStringList header = { tr("Seed"), tr("TID"), tr("SID"), tr("TSV"), tr("Delay"), tr("Seconds") };
};

#endif // IDMODEL4_HPP
