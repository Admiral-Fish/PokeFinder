/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef STATICMODEL5_HPP
#define STATICMODEL5_HPP

#include <Core/Gen5/States/SearcherState5.hpp>
#include <Core/Parents/States/StaticState.hpp>
#include <Forms/Models/TableModel.hpp>

enum class Method : u8;

class StaticGeneratorModel5 : public TableModel<StaticState>
{
    Q_OBJECT
public:
    explicit StaticGeneratorModel5(QObject *parent, Method method);
    void setMethod(Method method);
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    Method method;
    QStringList header = { tr("Advances"), tr("Chatot"), tr("PID"), tr("Shiny"), tr("Nature"), tr("Ability"), tr("HP"),    tr("Atk"),
                           tr("Def"),      tr("SpA"),    tr("SpD"), tr("Spe"),   tr("Hidden"), tr("Power"),   tr("Gender") };

    int getColumn(int column) const;
};

class StaticSearcherModel5 : public TableModel<SearcherState5<StaticState>>
{
    Q_OBJECT
public:
    explicit StaticSearcherModel5(QObject *parent, Method method);
    void setMethod(Method method);
    void sort(int column, Qt::SortOrder order) override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    Method method;
    QStringList header = { tr("Seed"),  tr("Advances"), tr("Lead"),      tr("PID"),    tr("Shiny"),  tr("Nature"), tr("Ability"),
                           tr("HP"),    tr("Atk"),      tr("Def"),       tr("SpA"),    tr("SpD"),    tr("Spe"),    tr("Hidden"),
                           tr("Power"), tr("Gender"),   tr("Date/Time"), tr("Timer0"), tr("Buttons") };

    int getColumn(int column) const;
};

#endif // STATICMODEL5_HPP
