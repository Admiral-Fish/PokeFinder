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

#ifndef EGG4MODEL_HPP
#define EGG4MODEL_HPP

#include <Core/Gen4/Frame4.hpp>
#include <Models/TableModel.hpp>

namespace PokeFinderModels
{

    class Egg4GeneratorModel : public TableModel<PokeFinderCore::Frame4>
    {
        Q_OBJECT

    public:
        Egg4GeneratorModel(QObject *parent, PokeFinderCore::Method method);
        void setMethod(PokeFinderCore::Method method);
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    public slots:
        void toggleInheritance(bool flag);

    private:
        PokeFinderCore::Method method;
        bool showInheritance;

        QStringList header1 =
        {
            tr("Frame"), tr("PID"), "!!!", tr("Nature"), tr("Ability"), tr("Gender")
        };
        QStringList header2 =
        {
            tr("Frame"), tr("Chatot"), tr("HP"), tr("Atk"), tr("Def"), tr("SpA"), tr("SpD"), tr("Spe"), tr("Hidden"), tr("Power")
        };
        QStringList header3 =
        {
            tr("Frame"), tr("Call"), tr("Chatot"), tr("HP"), tr("Atk"), tr("Def"), tr("SpA"), tr("SpD"), tr("Spe"), tr("Hidden"), tr("Power")
        };

    };

    class Egg4SearcherModel : public TableModel<PokeFinderCore::Frame4>
    {
        Q_OBJECT

    public:
        Egg4SearcherModel(QObject *parent, PokeFinderCore::Method method);
        void setMethod(PokeFinderCore::Method method);
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    public slots:
        void toggleInheritance(bool flag);

    private:
        PokeFinderCore::Method method;
        bool showInheritance;

        QStringList header1 =
        {
            tr("Seed"), tr("Frame"), tr("PID"), "!!!", tr("Nature"), tr("Ability"), tr("Gender")
        };
        QStringList header2 =
        {
            tr("Seed"), tr("Frame"), tr("HP"), tr("Atk"), tr("Def"), tr("SpA"), tr("SpD"), tr("Spe"), tr("Hidden"), tr("Power")
        };
        QStringList header3 =
        {
            tr("Seed"), tr("PID Frame"), tr("IV Frame"),  tr("PID"), "!!!", tr("Nature"), tr("Ability"),
            tr("HP"), tr("Atk"), tr("Def"), tr("SpA"), tr("SpD"), tr("Spe"), tr("Hidden"), tr("Power"), tr("Gender")
        };

    };

}

#endif // EGG4MODEL_HPP
