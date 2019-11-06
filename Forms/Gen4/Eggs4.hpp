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

#ifndef EGGS4_HPP
#define EGGS4_HPP

#include <Core/Gen4/Profile4.hpp>
#include <QMenu>

namespace PokeFinderCore
{
    class Frame4;
}

namespace PokeFinderModels
{
    class Egg4GeneratorModel;
    class Egg4SearcherModel;
}

namespace PokeFinderForms
{
    namespace Ui
    {
        class Eggs4;
    }

    class Eggs4 : public QWidget
    {
        Q_OBJECT
    signals:
        void alertProfiles(int);

    public:
        explicit Eggs4(QWidget *parent = nullptr);
        ~Eggs4() override;
        void updateProfiles();

    private:
        Ui::Eggs4 *ui;
        QVector<PokeFinderCore::Profile4> profiles;
        PokeFinderModels::Egg4GeneratorModel *generatorModel {};
        PokeFinderModels::Egg4SearcherModel *searcherModel {};
        QMenu *searcherMenu {};

        void setupModels();

    private slots:
        void updateProgress(const QVector<PokeFinderCore::Frame4> &frames, int progress);
        void refreshProfiles();
        void generate();
        void search();
        void profilesIndexChanged(int index);
        void tableViewSearcherContextMenu(QPoint pos);
        void seedToTime();
        void profileManager();
    };
}

#endif // EGGS4_HPP
