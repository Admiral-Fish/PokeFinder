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

#ifndef WILD4_HPP
#define WILD4_HPP

#include <PokeFinderCore/Gen4/Frame4.hpp>
#include <PokeFinderCore/Gen4/Generator4.hpp>
#include <PokeFinderCore/Gen4/Searcher4.hpp>
#include <PokeFinderCore/Objects/FrameCompare.hpp>
#include <PokeFinderCore/Objects/Nature.hpp>
#include <PokeFinderCore/Objects/Power.hpp>
#include <PokeFinderCore/Gen4/Profile4.hpp>
#include <Forms/Gen4/ProfileManager4.hpp>
#include <PokeFinderCore/Gen4/EncounterArea4.hpp>
#include <PokeFinderCore/Translator.hpp>
#include <Models/Gen4/Wild4Model.hpp>
#include <Models/Gen4/Searcher4Model.hpp>
#include <QMenu>
#include <thread>
#include <QFileDialog>
#include <QSettings>

namespace Ui
{
    class Wild4;
}

class Wild4 : public QMainWindow
{
    Q_OBJECT

protected:
    void changeEvent(QEvent *);

signals:
    void updateView(vector<Frame4>);
    void alertProfiles(int);
    void updateProgress();

private:
    Ui::Wild4 *ui;
    vector<Profile4> profiles;
    bool isSearching = false;
    bool cancel = false;
    u32 progress;
    Searcher4Model *s = new Searcher4Model(this, Method1);
    Wild4Model *g = new Wild4Model(this, MethodJ);
    //QMenu *contextMenu = new QMenu(this);
    //QModelIndex lastIndex;
    //QModelIndex targetFrame;
    vector<EncounterArea4> encounterGenerator;
    vector<EncounterArea4> encounterSearcher;

    void setupModels();
    void search();
    void updateSearch();
    void updateLocationsSearcher();
    void updatePokemonSearcher();
    void updateLocationsGenerator();
    void updatePokemonGenerator();

private slots:
    void on_generate_clicked();
    void refreshProfiles();
    void on_anyNatureGenerator_clicked();
    void on_anyHiddenPowerGenerator_clicked();
    void on_search_clicked();
    void on_anyNatureSearcher_clicked();
    void on_anyHiddenPowerSearcher_clicked();
    void updateViewSearcher(vector<Frame4> frames);
    void on_comboBoxProfiles_currentIndexChanged(int index);
    //void on_tableViewGenerator_customContextMenuRequested(const QPoint &pos);
    //void setTargetFrameGenerator();
    //void jumpToTargetGenerator();
    //void centerFramesAndSetTargetGenerator(u32 centerFrames);
    //void outputToTxt();
    //void outputToCSV();
    void updateProgressBar();
    void on_pushButtonLeadGenerator_clicked();
    void on_pushButtonProfileManager_clicked();
    void on_comboBoxEncounterGenerator_currentIndexChanged(int index);
    void on_anySlotGenerator_clicked();
    void on_comboBoxEncounterSearcher_currentIndexChanged(int index);
    void on_anySlotSearcher_clicked();
    void on_comboBoxLocationSearcher_currentIndexChanged(int index);
    void on_comboBoxPokemonSearcher_currentIndexChanged(int index);
    void on_comboBoxLocationGenerator_currentIndexChanged(int index);
    void on_comboBoxPokemonGenerator_currentIndexChanged(int index);
    void on_comboBoxTimeGenerator_currentIndexChanged(int index);
    void on_comboBoxTimeSearcher_currentIndexChanged(int index);

public:
    explicit Wild4(QWidget *parent = 0);
    ~Wild4();
    void updateProfiles();

};

#endif // WILD4_HPP
