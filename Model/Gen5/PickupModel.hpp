/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef PICKUPMODEL_HPP
#define PICKUPMODEL_HPP

#include <Core/Gen5/States/PickupState.hpp>
#include <Core/Gen5/States/SearcherState5.hpp>
#include <Model/TableModel.hpp>

class PickupGeneratorModel5 : public TableModel<PickupState>
{
    Q_OBJECT
public:
    PickupGeneratorModel5(QObject *parent);

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

public slots:
    void setShowStats(bool flag);

private:
    QStringList header = { tr("Advances"), tr("Chatot"), tr("Item 1"), tr("Item 2"), tr("Item 3"),
                           tr("Item 4"),   tr("Item 5"), tr("Item 6"), tr("Held Item"), tr("Slot"),
                           tr("Level"),    tr("PID"),    tr("Shiny"),  tr("Nature"),    tr("Ability"),
                           tr("HP"),       tr("Atk"),    tr("Def"),    tr("SpA"),       tr("SpD"),
                           tr("Spe"),      tr("Hidden"), tr("Power"),  tr("Gender"),    tr("Characteristic") };
    bool showStats = false;
};

class PickupSearcherModel5 : public TableModel<SearcherState5<PickupState>>
{
    Q_OBJECT
public:
    PickupSearcherModel5(QObject *parent);

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QStringList header = { tr("Seed"),   tr("Advances"), tr("Item 1"), tr("Item 2"), tr("Item 3"),
                           tr("Item 4"), tr("Item 5"),   tr("Item 6"), tr("Date/Time"), tr("Timer0"), tr("Buttons") };
};

#endif // PICKUPMODEL_HPP
