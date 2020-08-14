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

#ifndef SEEDTOTIME4MODEL_HPP
#define SEEDTOTIME4MODEL_HPP

#include <Core/Enum/Game.hpp>
#include <Core/Util/DateTime.hpp>
#include <Models/TableModel.hpp>

class SeedtoTimeModel4 : public TableModel<DateTime>
{
    Q_OBJECT
public:
    explicit SeedtoTimeModel4(QObject *parent, bool flag = false, Game version = DPPt);
    void setFlags(bool flag = false, Game version = Diamond);
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    bool calibrate;
    Game version;

    QStringList header1 = { tr("Seed"), tr("Date"), tr("Time"), tr("Delay"), tr("Calls"), tr("Roamer locations") };
    QStringList header2 = { tr("Seed"), tr("Date"), tr("Time"), tr("Delay"), tr("Coin flips"), tr("Roamer locations") };
    QStringList header3 = { tr("Date"), tr("Time"), tr("Delay") };
};

#endif // SEEDTOTIME4MODEL_HPP
