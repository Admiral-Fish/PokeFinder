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

#ifndef GAMECUBE_HPP
#define GAMECUBE_HPP

#include <QMenu>
#include <Core/Gen3/Profile3.hpp>

namespace PokeFinderCore
{
    class Frame3;
}

namespace PokeFinderModels
{
    class Searcher3Model;
    class Stationary3Model;
}

namespace PokeFinderForms
{
    namespace Ui
    {
        class GameCube;
    }

    class GameCube : public QWidget
    {
        Q_OBJECT
    signals:
        void alertProfiles(int);

    public:
        explicit GameCube(QWidget *parent = nullptr);
        ~GameCube() override;
        void updateProfiles();

    private:
        Ui::GameCube *ui;
        PokeFinderModels::Searcher3Model *searcherModel{};
        PokeFinderModels::Stationary3Model *generatorModel{};
        QVector<PokeFinderCore::Profile3> profiles;
        QMenu *generatorMenu{};
        QMenu *searcherMenu{};

        void setupModels();

    private slots:
        void updateProgress(const QVector<PokeFinderCore::Frame3> &frames, int progress);
        void refreshProfiles();
        void on_comboBoxProfiles_currentIndexChanged(int index);
        void on_pushButtonGenerate_clicked();
        void on_pushButtonSearch_clicked();
        void on_comboBoxGeneratorMethod_currentIndexChanged(int index);
        void on_comboBoxGeneratorShadow_currentIndexChanged(int index);
        void on_comboBoxSearcherMethod_currentIndexChanged(int index);
        void on_tableViewGenerator_customContextMenuRequested(const QPoint &pos);
        void on_tableViewSearcher_customContextMenuRequested(const QPoint &pos);
        void seedToTime();
        void copySeedToClipboard();
        void on_pushButtonProfileManager_clicked();
    };
}

#endif // GAMECUBE_HPP
