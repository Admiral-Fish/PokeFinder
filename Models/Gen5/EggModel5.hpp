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

#ifndef EGGMODEL5_HPP
#define EGGMODEL5_HPP

#include <Models/TableModel.hpp>

template <typename Type>
class SearcherState5;
class EggState;

class EggGeneratorModel5 : public TableModel<EggState>
{
    Q_OBJECT
public:
    explicit EggGeneratorModel5(QObject *parent);
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QStringList header = { tr("Advances"), tr("Chatot"), tr("PID"), tr("Shiny"), tr("Nature"), tr("Ability"), tr("HP"),    tr("Atk"),
                           tr("Def"),      tr("SpA"),    tr("SpD"), tr("Spe"),   tr("Hidden"), tr("Power"),   tr("Gender") };
};

class EggSearcherModel5 : public TableModel<SearcherState5<EggState>>
{
    Q_OBJECT
public:
    explicit EggSearcherModel5(QObject *parent);
    void sort(int column, Qt::SortOrder order) override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
    QStringList header = { tr("Seed"),   tr("Advances"), tr("PID"),    tr("Shiny"),     tr("Nature"), tr("Ability"),
                           tr("HP"),     tr("Atk"),      tr("Def"),    tr("SpA"),       tr("SpD"),    tr("Spe"),
                           tr("Hidden"), tr("Power"),    tr("Gender"), tr("Date/Time"), tr("Timer0"), tr("Buttons") };
};

#endif // EGGMODEL5_HPP
