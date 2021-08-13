/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

namespace Ui
{
    class MainWindow;
}

class Stationary3;
class Wild3;
class GameCube;
class Eggs3;
class IDs3;
class Stationary4;
class Wild4;
class Eggs4;
class IDs4;
class Stationary5;
class Event5;
class DreamRadar;
class HiddenGrotto;
class Eggs5;
class IDs5;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(bool profile, QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;

    Stationary3 *stationary3 = nullptr;
    Wild3 *wild3 = nullptr;
    GameCube *gamecube = nullptr;
    Eggs3 *egg3 = nullptr;
    IDs3 *ids3 = nullptr;
    Stationary4 *stationary4 = nullptr;
    Wild4 *wild4 = nullptr;
    Eggs4 *egg4 = nullptr;
    IDs4 *ids4 = nullptr;
    Stationary5 *stationary5 = nullptr;
    Event5 *event5 = nullptr;
    DreamRadar *dreamRadar = nullptr;
    HiddenGrotto *hiddenGrotto = nullptr;
    Eggs5 *egg5 = nullptr;
    IDs5 *ids5 = nullptr;

    void setupModels();
    void checkUpdates();

private slots:
    void updateProfiles(int num);
    void openStationary3();
    void openWild3();
    void openGameCube();
    void openEgg3();
    void openIDs3();
    void openGameCubeRTC();
    void openGameCubeSeedFinder();
    void openIVtoPID();
    void openJirachiPattern();
    void openPIDtoIV();
    void openPokeSpot();
    void openProfileManager3();
    void openSeedtoTime3();
    void openSpindaPainter();
    void openStationary4();
    void openWild4();
    void openEgg4();
    void openIDs4();
    void openProfileManager4();
    void openSeedtoTime4();
    void openSIDFromChainedShiny();
    void openStationary5();
    void openEvent5();
    void openDreamRadar();
    void openHiddenGrotto();
    void openEgg5();
    void openIDs5();
    void openProfileCalibrator();
    void openProfileManager5();
    void openAbout();
    void openEncounterLookup();
    void openIVCalculator();
    void openResearcher();
    void openSettings();
};

#endif // MAINWINDOW_HPP
