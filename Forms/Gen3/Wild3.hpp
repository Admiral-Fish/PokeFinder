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

#ifndef WILD3_HPP
#define WILD3_HPP

#include <PokeFinderCore/Gen3/Frame3.hpp>
#include <PokeFinderCore/Gen3/Generator3.hpp>
#include <PokeFinderCore/Gen3/Searcher3.hpp>
#include <PokeFinderCore/Objects/FrameCompare.hpp>
#include <PokeFinderCore/Objects/Nature.hpp>
#include <PokeFinderCore/Objects/Power.hpp>
#include <PokeFinderCore/Gen3/Profile3.hpp>
#include <Forms/Gen3/ProfileManager3.hpp>
#include <PokeFinderCore/Gen3/EncounterArea3.hpp>
#include <PokeFinderCore/Translator.hpp>
#include <Models/Gen3/Wild3Model.hpp>
#include <Models/Gen3/Searcher3Model.hpp>
#include <Forms/Gen3/SeedToTime3.hpp>
#include <QMenu>
#include <thread>
#include <QFileDialog>
#include <QClipboard>
#include <QSettings>
#include <QVector>

namespace Ui
{
    class Wild3;
}

class Wild3 : public QMainWindow
{
    Q_OBJECT

protected:
    void changeEvent(QEvent *);

signals:
    void updateView(QVector<Frame3>);
    void alertProfiles(int);
    void updateProgress();

private:
    Ui::Wild3 *ui;
    QVector<Profile3> profiles;
    bool isSearching = false;
    bool cancel = false;
    int progress;
    Searcher3Model *s = new Searcher3Model(this, Method1);
    Wild3Model *g = new Wild3Model(this);
    QMenu *generatorMenu = new QMenu(this);
    QMenu *searcherMenu = new QMenu(this);
    QModelIndex lastIndex;
    QModelIndex targetFrame;
    QVector<EncounterArea3> encounterGenerator;
    QVector<EncounterArea3> encounterSearcher;

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
    void updateViewSearcher(QVector<Frame3> frames);
    void on_checkBoxDelayGenerator_clicked();
    void on_comboBoxProfiles_currentIndexChanged(int index);
    void on_tableViewGenerator_customContextMenuRequested(const QPoint &pos);
    void on_tableViewSearcher_customContextMenuRequested(const QPoint &pos);
    void copySeedToClipboard();
    void setTargetFrameGenerator();
    void jumpToTargetGenerator();
    void centerFramesAndSetTargetGenerator(u32 centerFrames);
    void seedToTime();
    void outputToTxt();
    void outputToCSV();
    void updateProgressBar();
    void on_pushButtonLeadGenerator_clicked();
    void on_pushButtonProfileManager_clicked();
    void on_comboBoxEncounterGenerator_currentIndexChanged(int index);
    void on_anySlotGenerator_clicked();
    void on_comboBoxEncounterSearcher_currentIndexChanged(int index);
    void on_pushButton_clicked();
    void on_comboBoxLocationSearcher_currentIndexChanged(int index);
    void on_comboBoxPokemonSearcher_currentIndexChanged(int index);
    void on_comboBoxLocationGenerator_currentIndexChanged(int index);
    void on_comboBoxPokemonGenerator_currentIndexChanged(int index);

public:
    explicit Wild3(QWidget *parent = nullptr);
    ~Wild3();
    void updateProfiles();

};

#endif // WILD3_HPP
