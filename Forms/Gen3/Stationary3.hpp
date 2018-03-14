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

#ifndef STATIONARY3_H
#define STATIONARY3_H

#include <libPokeFinder/Gen3/Frame3.hpp>
#include <libPokeFinder/Gen3/Generator3.hpp>
#include <libPokeFinder/Gen3/Searcher3.hpp>
#include <libPokeFinder/Objects/FrameCompare.hpp>
#include <libPokeFinder/Objects/Nature.hpp>
#include <libPokeFinder/Objects/Power.hpp>
#include <libPokeFinder/Gen3/Profile3.hpp>
#include <Forms/ProfileManager/ProfileManager3.hpp>
#include <Models/Gen3/Stationary3Model.hpp>
#include <Models/Gen3/Searcher3Model.hpp>
#include <thread>
#include <QMenu>
#include <QAction>
#include <QModelIndex>
#include <QFileDialog>
#include <QClipboard>

namespace Ui {
class Stationary3;
}

class Stationary3 : public QMainWindow
{
    Q_OBJECT

protected:
    void changeEvent(QEvent*);

signals:
    void updateView(vector<Frame3>);
    void alertProfiles(int);

private:
    Ui::Stationary3 *ui;
    Searcher3Model *s = new Searcher3Model(this, Method1);
    Stationary3Model *g = new Stationary3Model(this);
    bool isSearching = false;
    vector<Profile3> profiles;
    QMenu *generatorMenu = new QMenu();
    QMenu *searcherMenu = new QMenu();
    QModelIndex lastIndex;
    QModelIndex targetFrame;
    QClipboard *clipboard;

    void setupModels();
    void search();

private slots:
    void on_generate_clicked();
    void refreshProfiles();
    void on_saveProfileGenerator_clicked();
    void on_comboBoxProfiles_currentIndexChanged(int index);
    void on_anyNatureGenerator_clicked();
    void on_anyHiddenPowerGenerator_clicked();
    void on_checkBoxDelayGenerator_clicked();
    void on_saveSearcher_clicked();
    void on_search_clicked();
    void on_anyNatureSearcher_clicked();
    void on_anyHiddenPowerSearcher_clicked();
    void updateViewSearcher(vector<Frame3> frames);
    void on_comboBoxMethodSearcher_currentIndexChanged(int index);
    void on_tableViewGenerator_customContextMenuRequested(const QPoint &pos);
    void setTargetFrameGenerator();
    void jumpToTargetGenerator();
    void centerFramesAndSetTargetGenerator(u32 centerFrames);
    void outputToTxt();
    void outputToCSV();
    void on_tableViewSearcher_customContextMenuRequested(const QPoint &pos);
    void copySeedToClipboard();

public:
    explicit Stationary3(QWidget *parent = 0);
    ~Stationary3();
    void updateProfiles();

};

#endif // STATIONARY3_H
