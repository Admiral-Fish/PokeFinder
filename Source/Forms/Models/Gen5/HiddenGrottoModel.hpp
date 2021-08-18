/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef HIDDENGROTTOMODEL_HPP
#define HIDDENGROTTOMODEL_HPP

#include <Core/Gen5/States/HiddenGrottoState.hpp>
#include <Core/Gen5/States/SearcherState5.hpp>
#include <Forms/Models/TableModel.hpp>

class HiddenGrottoSearcherModel5 : public TableModel<SearcherState5<HiddenGrottoState>>
{
    Q_OBJECT
public:
    explicit HiddenGrottoSearcherModel5(QObject *parent);
    void sort(int column, Qt::SortOrder order) override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QStringList header
        = { tr("Seed"), tr("Advances"), tr("Group"), tr("Slot"), tr("Gender"), tr("Date/Time"), tr("Timer0"), tr("Buttons") };
};

#endif // HIDDENGROTTOMODEL_HPP
