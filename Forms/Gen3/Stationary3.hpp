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

#ifndef STATIONARY3_H
#define STATIONARY3_H

#include <QMenu>
#include <QAction>
#include <QModelIndex>
#include <QFileDialog>
#include <QClipboard>
#include <QSettings>
#include <QVector>
#include <thread>
#include <PokeFinderCore/Gen3/Frame3.hpp>
#include <PokeFinderCore/Gen3/Generator3.hpp>
#include <PokeFinderCore/Gen3/Searcher3.hpp>
#include <PokeFinderCore/Objects/FrameCompare.hpp>
#include <PokeFinderCore/Objects/Nature.hpp>
#include <PokeFinderCore/Objects/Power.hpp>
#include <PokeFinderCore/Gen3/Profile3.hpp>
#include <Forms/Gen3/ProfileManager3.hpp>
#include <Models/Gen3/Stationary3Model.hpp>
#include <Models/Gen3/Searcher3Model.hpp>
#include <PokeFinderCore/Translator.hpp>
#include <Forms/Gen3/SeedToTime3.hpp>

namespace Ui
{
    class Stationary3;
}

class Stationary3 : public QMainWindow
{
    Q_OBJECT

protected:
    void changeEvent(QEvent *event) override;

signals:
    void updateView(QVector<Frame3>);
    void alertProfiles(int);
    void updateProgress();

private:
    Ui::Stationary3 *ui;
    Searcher3Model *s = new Searcher3Model(this, Method::Method1);
    Stationary3Model *g = new Stationary3Model(this);
    bool isSearching = false;
    bool cancel = false;
    int progress;
    QVector<Profile3> profiles;
    QMenu *generatorMenu = new QMenu();
    QMenu *searcherMenu = new QMenu();
    QModelIndex lastIndex;
    QModelIndex targetFrame;

    void setupModels();
    void search();
    void updateSearch();

public slots:
    void moveResults(const QString &seed, const QString &method, u32 hp, u32 atk, u32 def, u32 spa, u32 spd, u32 spe);

private slots:
    void on_generate_clicked();
    void refreshProfiles();
    void on_comboBoxProfiles_currentIndexChanged(int index);
    void on_anyNatureGenerator_clicked();
    void on_anyHiddenPowerGenerator_clicked();
    void on_checkBoxDelayGenerator_clicked();
    void on_search_clicked();
    void on_anyNatureSearcher_clicked();
    void on_anyHiddenPowerSearcher_clicked();
    void updateViewSearcher(QVector<Frame3> frames);
    void on_comboBoxMethodSearcher_currentIndexChanged(int index);
    void on_tableViewGenerator_customContextMenuRequested(const QPoint &pos);
    void setTargetFrameGenerator();
    void jumpToTargetGenerator();
    void centerFramesAndSetTargetGenerator(u32 centerFrames);
    void seedToTime();
    void outputToTxt();
    void outputToCSV();
    void on_tableViewSearcher_customContextMenuRequested(const QPoint &pos);
    void copySeedToClipboard();
    void updateProgressBar();
    void on_pushButtonProfileManager_clicked();

public:
    explicit Stationary3(QWidget *parent = nullptr);
    ~Stationary3() override;
    void updateProfiles();

};

#endif // STATIONARY3_H
