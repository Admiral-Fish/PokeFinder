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

#include <QDate>
#include <QDesktopServices>
#include <QJsonDocument>
#include <QMainWindow>
#include <QMessageBox>
#include <QtNetwork>
#include <Forms/Gen3/Eggs3.hpp>
#include <Forms/Gen3/GameCube.hpp>
#include <Forms/Gen3/GameCubeRTC.hpp>
#include <Forms/Gen3/IDs3.hpp>
#include <Forms/Gen3/JirachiPattern.hpp>
#include <Forms/Gen3/PIDtoIVs.hpp>
#include <Forms/Gen3/SeedToTime3.hpp>
#include <Forms/Gen3/SpindaPainter.hpp>
#include <Forms/Gen3/Stationary3.hpp>
#include <Forms/Gen3/Wild3.hpp>
#include <Forms/Gen3/PokeSpot.hpp>
#include <Forms/Gen4/ChainedSID.hpp>
#include <Forms/Gen4/Eggs4.hpp>
#include <Forms/Gen4/IDs4.hpp>
#include <Forms/Gen4/SeedtoTime4.hpp>
#include <Forms/Gen4/Stationary4.hpp>
#include <Forms/Gen4/Wild4.hpp>
#include <Forms/Util/EncounterLookup.hpp>
#include <Forms/Util/IVCalculator.hpp>
#include <Forms/Util/IVtoPID.hpp>
#include <Forms/Util/Researcher.hpp>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QTranslator translator;
    QActionGroup *langGroup;
    QActionGroup *styleGroup;
    const QString VERSION = "v2.3.1";

    Stationary3 *stationary3 = nullptr;
    Wild3 *wild3 = nullptr;
    GameCube *gamecube = nullptr;
    Eggs3 *egg3 = nullptr;
    IDs3 *ids3 = nullptr;
    Stationary4 *stationary4 = nullptr;
    Wild4 *wild4 = nullptr;
    Eggs4 *egg4 = nullptr;
    IDs4 *ids4 = nullptr;

    void setupLanguage();
    void setupStyle();
    void checkProfileJson();
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

#endif // MAINWINDOW_HPP
