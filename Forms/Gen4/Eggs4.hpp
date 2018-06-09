/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
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

#include <QMainWindow>
#include <PokeFinderCore/Gen4/Frame4.hpp>
#include <Forms/Gen4/ProfileManager4.hpp>
#include <Models/Gen4/Egg4Model.hpp>
#include <PokeFinderCore/Objects/FrameCompare.hpp>
#include <PokeFinderCore/Gen4/Egg4.hpp>
#include <QSettings>
#include <thread>

namespace Ui
{
    class Eggs4;
}

class Eggs4 : public QMainWindow
{
    Q_OBJECT

protected:
    void changeEvent(QEvent *);

signals:
    void updatePID(vector<Frame4>);
    void updateIVs(vector<Frame4>);
    void alertProfiles(int);
    void updateProgress();

private:
    Ui::Eggs4 *ui;
    vector<Profile4> profiles;
    bool isSearching[2] = {false, false};
    bool cancel[2] = { false, false };
    Egg4GeneratorModel *generatorModel = new Egg4GeneratorModel(this, DPPtIVs);
    Egg4SearcherModel *searcherIVs = new Egg4SearcherModel(this, DPPtIVs);
    Egg4SearcherModel *searcherPID = new Egg4SearcherModel(this, Gen4Normal);
    u32 progressPID;
    u32 progressIVs;

    void setupModels();
    void searchPID();
    void searchIVs();
    void updateSearch(int i);

private slots:
    void refreshProfiles();
    void on_pushButtonAnyNatureGenerator_clicked();
    void on_pushButtonAnyHiddenPowerGenerator_clicked();
    void on_pushButtonAnyNatureSearcher_clicked();
    void on_comboBoxProfiles_currentIndexChanged(int index);
    void on_pushButtonProfileManager_clicked();
    void on_pushButtonGenerate_clicked();
    void on_pushButtonGeneratePID_clicked();
    void on_pushButtonGenerateIVs_clicked();
    void updateViewPID(vector<Frame4> frames);
    void updateViewIVs(vector<Frame4> frames);
    void updateProgressPID();
    void updateProgressIVs();

public:
    explicit Eggs4(QWidget *parent = 0);
    ~Eggs4();
    void updateProfiles();

};

#endif // EGGS4_HPP
