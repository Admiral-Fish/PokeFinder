/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish and bumba
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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
#include <Forms/Gen3/Stationary3.hpp>
#include <Forms/Gen3/Wild3.hpp>
#include <Forms/Gen3/Eggs3.hpp>
#include <Forms/Util/Researcher.hpp>
#include <Forms/Gen3/SeedToTime3.hpp>
#include <Forms/Gen3/JirachiGeneration.hpp>
#include <Forms/Gen3/PokeSpot.hpp>
#include <Forms/Util/IVtoPID.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void changeEvent(QEvent*);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonStationary3_clicked();
    void on_pushButtonWild3_clicked();
    void on_pushButtonEgg3_clicked();
    void on_actionResearcher_triggered();
    void slotLanguageChanged(QAction *action);
    void updateProfiles(int num);
    void on_pushButtonSeedToTime3_clicked();
    void on_pushButtonJirachiGeneration_clicked();
    void on_pushButtonPokeSpot_clicked();

    void on_pushButtonIVToPID_clicked();

private:
    Ui::MainWindow *ui;
    QTranslator translator;
    QString currLang;
    QString langPath = QApplication::applicationDirPath().append("/Languages/");
    Stationary3 stationary3;
    Wild3 wild3;
    Eggs3 egg3;
    SeedToTime3 seedtotime3;
    JirachiGeneration jirachiGeneration;
    PokeSpot pokeSpot;
    IVtoPID ivToPID;

    void setupLanguage();
    void setupModels();
    void loadLanguage(const QString &lang);
    void switchTranslator(QTranslator& translator, const QString& filename);

};

#endif // MAINWINDOW_HPP
