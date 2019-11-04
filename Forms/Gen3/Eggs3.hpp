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

#ifndef EGGS3_HPP
#define EGGS3_HPP

#include <Core/Gen3/Profile3.hpp>
#include <QWidget>

namespace PokeFinderCore
{
    class Frame3;
}

namespace PokeFinderModels
{
    class Egg3Model;
}

namespace PokeFinderForms
{
    namespace Ui
    {
        class Eggs3;
    }

    class Eggs3 : public QWidget
    {
        Q_OBJECT
    signals:
        void alertProfiles(int);

    public:
        explicit Eggs3(QWidget *parent = nullptr);
        ~Eggs3() override;
        void updateProfiles();

    private:
        Ui::Eggs3 *ui;
        QVector<PokeFinderCore::Profile3> profiles;
        PokeFinderModels::Egg3Model *emeraldIVs {};
        PokeFinderModels::Egg3Model *emeraldPID {};
        PokeFinderModels::Egg3Model *rs {};
        PokeFinderModels::Egg3Model *frlg {};

        void setupModels();

    private slots:
        void refreshProfiles();
        void on_pushButtonEmeraldPIDGenerate_clicked();
        void on_pushButtonEmeraldIVsGenerate_clicked();
        void on_pushButtonRSGenerate_clicked();
        void on_pushButtonFRLGGenerate_clicked();
        void on_comboBoxProfiles_currentIndexChanged(int index);
        void on_pushButtonEmeraldAnyAbility_clicked();
        void on_pushButtonRSAnyAbility_clicked();
        void on_pushButtonFRLGAnyAbility_clicked();
        void on_pushButtonProfileManager_clicked();
    };
}

#endif // EGGS3_HPP
