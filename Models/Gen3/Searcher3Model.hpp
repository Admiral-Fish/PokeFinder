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

#ifndef SEARCHER3MODEL_HPP
#define SEARCHER3MODEL_HPP

#include <Core/Gen3/Frame3.hpp>
#include <Models/TableModel.hpp>
#include <Util/TableUtility.hpp>

class Searcher3Model : public TableModel<Frame3>
{
    Q_OBJECT

public:
    Searcher3Model(QObject *parent, Method method);
    void setMethod(Method method);
    void sort(int column, Qt::SortOrder order) override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    Method method;

    QStringList header1 =
    {
        tr("Seed"), tr("Lead"), tr("Slot"), tr("Level"), tr("PID"), "!!!", tr("Nature"), tr("Ability"),
        tr("HP"), tr("Atk"), tr("Def"), tr("SpA"), tr("SpD"), tr("Spe"), tr("Hidden"), tr("Power"), tr("Gender")
    };
    QStringList header2 =
    {
        tr("Seed"), tr("PID"), "!!!", tr("Nature"), tr("Ability"), tr("HP"), tr("Atk"),
        tr("Def"), tr("SpA"), tr("SpD"), tr("Spe"), tr("Hidden"), tr("Power"), tr("Gender")
    };
    QStringList header3 =
    {
        tr("Seed"), tr("PID"), "!!!", tr("Nature"), tr("Ability"), tr("HP"), tr("Atk"), tr("Def"),
        tr("SpA"), tr("SpD"), tr("Spe"), tr("Hidden"), tr("Power"), tr("Gender"), tr("Reason")
    };

};

#endif // SEARCHER3MODEL_HPP
