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

#include <QMainWindow>
#include <QMenu>
#include <QFileDialog>
#include <QSettings>
#include <thread>
#include <Forms/Gen4/ProfileManager4.hpp>
#include <Forms/Gen4/SeedtoTime4.hpp>
#include <Models/Gen4/Searcher4Model.hpp>
#include <Models/Gen4/Wild4Model.hpp>
#include <Gen4/Encounters4.hpp>
#include <Gen4/Generator4.hpp>
#include <Gen4/Searcher4.hpp>
#include <Translator.hpp>

namespace Ui
{
    class Wild4;
}

class Wild4 : public QMainWindow
{
    Q_OBJECT

signals:
    void updateView(QVector<Frame4>);
    void alertProfiles(int);
    void updateProgress();

private:
    Ui::Wild4 *ui;
    QVector<Profile4> profiles;
    bool isSearching = false;
    bool cancel = false;
    int progress;
    Searcher4Model *s = new Searcher4Model(this, Method::Method1);
    Wild4Model *g = new Wild4Model(this, Method::MethodJ);
    QMenu *searcherMenu = new QMenu(this);
    QVector<EncounterArea4> encounterGenerator;
    QVector<EncounterArea4> encounterSearcher;

    void setupModels();
    void search();
    void updateSearch();
    void updateLocationsSearcher();
    void updatePokemonSearcher();
    void updateLocationsGenerator();
    void updatePokemonGenerator();
    void loadSettings();
    void saveSettings();

private slots:
    void on_pushButtonGenerate_clicked();
    void refreshProfiles();
    void on_anyNatureGenerator_clicked();
    void on_anyHiddenPowerGenerator_clicked();
    void on_pushButtonSearch_clicked();
    void on_anyNatureSearcher_clicked();
    void on_anyHiddenPowerSearcher_clicked();
    void updateViewSearcher(const QVector<Frame4> &frames);
    void on_comboBoxProfiles_currentIndexChanged(int index);
    void seedToTime();
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
    void on_tableViewSearcher_customContextMenuRequested(const QPoint &pos);

public:
    explicit Wild4(QWidget *parent = nullptr);
    ~Wild4() override;
    void updateProfiles();

};

#endif // WILD4_HPP
