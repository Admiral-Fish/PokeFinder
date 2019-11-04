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

#ifndef PROFILEMANAGER3_HPP
#define PROFILEMANAGER3_HPP

#include <QWidget>

namespace PokeFinderModels
{
    class Profile3Model;
}

namespace PokeFinderForms
{
    namespace Ui
    {
        class ProfileManager3;
    }

    class ProfileManager3 : public QWidget
    {
        Q_OBJECT
    signals:
        void updateProfiles();

    public:
        explicit ProfileManager3(QWidget *parent = nullptr);
        ~ProfileManager3() override;

    private:
        std::unique_ptr<Ui::ProfileManager3> ui;
        PokeFinderModels::Profile3Model *model {};

        void setupModels();

    private slots:
        void on_pushButtonNew_clicked();
        void on_pushButtonEdit_clicked();
        void on_pushButtonDelete_clicked();
    };
}

#endif // PROFILEMANAGER3_HPP
