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
    MainWindow(bool profile, QWidget *parent = nullptr);

    /**
     * @brief Destroy the MainWindow object
     */
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;

    // Gen 3
    Eggs3 *egg3 = nullptr;
    GameCube *gamecube = nullptr;
    IDs3 *ids3 = nullptr;
    Static3 *static3 = nullptr;
    Wild3 *wild3 = nullptr;

    // Gen 4
    Eggs4 *egg4 = nullptr;
    IDs4 *ids4 = nullptr;
    Static4 *static4 = nullptr;
    Wild4 *wild4 = nullptr;

    // Gen 5
    DreamRadar *dreamRadar = nullptr;
    Eggs5 *egg5 = nullptr;
    Event5 *event5 = nullptr;
    HiddenGrotto *hiddenGrotto = nullptr;
    IDs5 *ids5 = nullptr;
    Static5 *static5 = nullptr;

    // Gen 8
    Eggs8 *egg8 = nullptr;
    Event8 *event8 = nullptr;
    IDs8 *ids8 = nullptr;
    Raids *raids = nullptr;
    Static8 *static8 = nullptr;
    Wild8 *wild8 = nullptr;

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
    // Gen 3
    /**
     * @brief Opens the Gen 3 IDs window
     */
    void openIDs3();

    /**
     * @brief Opens the GameCube window
     */
    void openGameCube();
    /**
     * @brief Opens the GameCube Seed Finder window
     */
    void openGameCubeSeedFinder();

    /**
     * @brief Opens the Gen 3 Profile Manager window
     */
    void openProfileManager3() const;

    /**
     * @brief Opens the Gen 3 Static window
     */
    void openStatic3();

    /**
     * @brief Opens the Gen 3 Wild window
     */
    void openWild3();

    // void openEgg3();
    // void openIVtoPID();
    // void openPIDtoIV();
    // void openPokeSpot();
    // void openSeedtoTime3();
    // void openSpindaPainter();

    // Gen 4
    /**
     * @brief Opens the Gen 4 IDs window
     */
    void openIDs4();

    /**
     * @brief Opens the Gen 3 Profile Manager window
     */
    void openProfileManager4() const;

    /**
     * @brief Opens the Gen 4 Static window
     */
    void openStatic4();

    /**
     * @brief Opens the Gen 4 Wild window
     */
    void openWild4();

    /**
     * @brief Opens the Gen 4 Seed to Time window
     */
    void openSeedToTime4() const;

    // void openEgg4();
    // void openSIDFromChainedShiny();

    // Gen 5
    // void openStatic5();
    // void openEvent5();
    // void openDreamRadar();
    // void openHiddenGrotto();
    // void openEgg5();
    // void openIDs5();
    // void openProfileCalibrator();
    // void openProfileManager5();

    // Gen 8
    // void downloadEventData();

    /**
     * @brief Opens the Gen 8 Egg window
     */
    void openEgg8();

    /**
     * @brief Opens the Gen 8 Event window
     */
    void openEvent8();

    /**
     * @brief Opens the Gen 8 Static window
     */
    void openStatic8();

    /**
     * @brief Opens the Gen 8 Wild window
     */
    void openWild8();

    // void openRaids();

    /**
     * @brief Opens the Gen 8 IDs window
     */
    void openIDs8();

    // void openDenMap();

    /**
     * @brief Opens the Gen 8 Profile Manager window
     */
    void openProfileManager8();

    // General tools and settings

    /**
     * @brief Open the About window
     */
    void openAbout() const;

    /**
     * @brief Opens the IV Calculator
     */
    void openIVCalculator() const;

    /**
     * @brief Opens the Researcher window
     */
    void openResearcher() const;

    /**
     * @brief Opens the Settings window
     */
    void openSettings() const;

    /**
     * @brief Informs open windows that they need to refresh profiles
     *
     * @param num Which generation profiles were updated
     */
    void updateProfiles(int num);

    // void openEncounterLookup();
};

#endif // MAINWINDOW_HPP
