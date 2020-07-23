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

#ifndef WILD4MODEL_HPP
#define WILD4MODEL_HPP

#include <Core/Parents/States/WildState.hpp>
#include <Models/TableModel.hpp>

enum Method : u8;

class WildGeneratorModel4 : public TableModel<WildState>
{
    Q_OBJECT
public:
    explicit WildGeneratorModel4(QObject *parent, Method method);
    void setMethod(Method method);
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    Method method;
    QStringList header
        = { tr("Advances"), tr("Call"), tr("Chatot"), tr("Slot"), tr("Level"), tr("PID"), tr("Shiny"),  tr("Nature"), tr("Ability"),
            tr("HP"),       tr("Atk"),  tr("Def"),    tr("SpA"),  tr("SpD"),   tr("Spe"), tr("Hidden"), tr("Power"),  tr("Gender") };

    int getColumn(int column) const;
};

class WildSearcherModel4 : public TableModel<WildState>
{
    Q_OBJECT
public:
    explicit WildSearcherModel4(QObject *parent, Method method);
    void setMethod(Method method);
    void sort(int column, Qt::SortOrder order) override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    Method method;
    QStringList header
        = { tr("Seed"), tr("Advances"), tr("Lead"), tr("Slot"), tr("Level"), tr("PID"), tr("Shiny"),  tr("Nature"), tr("Ability"),
            tr("HP"),   tr("Atk"),      tr("Def"),  tr("SpA"),  tr("SpD"),   tr("Spe"), tr("Hidden"), tr("Power"),  tr("Gender") };

    int getColumn(int column) const;
};

#endif // WILD4MODEL_HPP
