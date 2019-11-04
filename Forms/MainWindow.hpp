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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QActionGroup>
#include <QMainWindow>

namespace PokeFinderForms
{
    namespace Ui
    {
        class MainWindow;
    }

    class Eggs3;
    class GameCube;
    class IDs3;
    class Stationary3;
    class Wild3;
    class Eggs4;
    class IDs4;
    class Stationary4;
    class Wild4;

    class MainWindow : public QMainWindow
    {
        Q_OBJECT
    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow() override;

    private:
        Ui::MainWindow *ui;
        QActionGroup *langGroup {};
        QActionGroup *styleGroup {};
        QString currentLanguage;
        QString currentStyle;

        Stationary3 *stationary3 {};
        Wild3 *wild3 {};
        GameCube *gamecube {};
        Eggs3 *egg3 {};
        IDs3 *ids3 {};
        Stationary4 *stationary4 {};
        Wild4 *wild4 {};
        Eggs4 *egg4 {};
        IDs4 *ids4 {};

        void setupLanguage();
        void setupStyle();
        void checkUpdates();

    private slots:
        void slotLanguageChanged(QAction *action);
        void slotStyleChanged(QAction *action);
        void updateProfiles(int num);
        void on_pushButtonStationary3_clicked();
        void on_pushButtonWild3_clicked();
        void on_pushButtonGameCube_clicked();
        void on_pushButtonEgg3_clicked();
        void on_pushButtonIDs3_clicked();
        void on_actionGameCubeRTC_triggered();
        void on_actionGameCube_Seed_Finder_triggered();
        void on_actionIVtoPID3_triggered();
        void on_actionJirachiPattern_triggered();
        void on_actionPIDtoIV_triggered();
        void on_actionPokeSpot_triggered();
        void on_actionSeedtoTime3_triggered();
        void on_actionSpinda_Painter_triggered();
        void on_pushButtonStationary4_clicked();
        void on_pushButtonWild4_clicked();
        void on_pushButtonEgg4_clicked();
        void on_pushButtonIDs4_clicked();
        void on_actionIVtoPID4_triggered();
        void on_actionSeedtoTime4_triggered();
        void on_actionSID_from_Chained_Shiny_triggered();
        void on_actionEncounter_Lookup_triggered();
        void on_actionIV_Calculator_triggered();
        void on_actionResearcher_triggered();
    };
}

#endif // MAINWINDOW_HPP
