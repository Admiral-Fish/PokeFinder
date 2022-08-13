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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

class Static3;
class Wild3;
class GameCube;
class Eggs3;
class IDs3;
class Static4;
class Wild4;
class Eggs4;
class IDs4;
class Static5;
class Event5;
class DreamRadar;
class HiddenGrotto;
class Eggs5;
class IDs5;
class Static8;
class Wild8;
class Event8;
class Raids;
class Eggs8;
class IDs8;
class QActionGroup;

namespace Ui
{
    class MainWindow;
}

/**
 * @brief Provides a central hub to access the various windows for RNG
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief Construct a new MainWindow object
     *
     * @param profile True if profiles file is located, false otherwise
     * @param parent Parent widget, which takes memory ownership
     */
    explicit MainWindow(bool profile, QWidget *parent = nullptr);

    /**
     * @brief Destroy the MainWindow object
     */
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;

    Static3 *static3 = nullptr;
    Wild3 *wild3 = nullptr;
    GameCube *gamecube = nullptr;
    Eggs3 *egg3 = nullptr;
    IDs3 *ids3 = nullptr;
    Static4 *static4 = nullptr;
    Wild4 *wild4 = nullptr;
    Eggs4 *egg4 = nullptr;
    IDs4 *ids4 = nullptr;
    Static5 *static5 = nullptr;
    Event5 *event5 = nullptr;
    DreamRadar *dreamRadar = nullptr;
    HiddenGrotto *hiddenGrotto = nullptr;
    Eggs5 *egg5 = nullptr;
    IDs5 *ids5 = nullptr;
    Static8 *static8 = nullptr;
    Wild8 *wild8 = nullptr;
    Event8 *event8 = nullptr;
    Raids *raids = nullptr;
    Eggs8 *egg8 = nullptr;
    IDs8 *ids8 = nullptr;

    /**
     * @brief Checks if any application updates are available
     */
    void checkUpdates() const;

    /**
     * @brief Downloads webpage from \p url
     *
     * @param url Link to download from
     *
     * @return Byte array of downloaded \p url
     */
    QByteArray downloadFile(const QString &url) const;

private slots:
    /**
     * @brief Informs open windows that they need to refresh profiles
     *
     * @param num Which generation profiles were updated
     */
    void updateProfiles(int num);

    // void openStatic3();
    // void openWild3();
    // void openGameCube();
    // void openEgg3();

    /**
     * @brief Opens the Gen 3 TID/SID window
     */
    void openIDs3();

    // void openGameCubeRTC();
    // void openGameCubeSeedFinder();
    // void openIVtoPID();
    // void openJirachiPattern();
    // void openPIDtoIV();
    // void openPokeSpot();

    /**
     * @brief Opens the Gen 3 Profile Manager window
     */
    void openProfileManager3();

    // void openSeedtoTime3();
    // void openSpindaPainter();
    // void openStatic4();
    // void openWild4();
    // void openEgg4();
    // void openIDs4();
    // void openProfileManager4();
    // void openSeedtoTime4();
    // void openSIDFromChainedShiny();
    // void openStatic5();
    // void openEvent5();
    // void openDreamRadar();
    // void openHiddenGrotto();
    // void openEgg5();
    // void openIDs5();
    // void openProfileCalibrator();
    // void openProfileManager5();
    // void openStatic8();
    // void openWild8();
    // void openEvent8();
    // void openRaids();
    // void openEgg8();
    // void openIDs8();
    // void openDenMap();
    // void downloadEventData();
    // void openProfileManager8();

    /**
     * @brief Open the About window
     */
    void openAbout() const;

    // void openEncounterLookup();
    // void openIVCalculator();
    // void openResearcher();
    // void openSettings();
};

#endif // MAINWINDOW_HPP
