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

#ifndef STATIONARY3_H
#define STATIONARY3_H

#include <QClipboard>
#include <QFileDialog>
#include <QMainWindow>
#include <QMenu>
#include <QMutex>
#include <QSettings>
#include <QThread>
#include <QTimer>
#include <Core/Gen3/Generator3.hpp>
#include <Core/Gen3/Searcher3.hpp>
#include <Core/Translator.hpp>
#include <Forms/Gen3/ProfileManager3.hpp>
#include <Forms/Gen3/SeedToTime3.hpp>
#include <Models/Gen3/Searcher3Model.hpp>
#include <Models/Gen3/Stationary3Model.hpp>

namespace Ui
{
    class Stationary3;
}

class Stationary3 : public QMainWindow
{
    Q_OBJECT

signals:
    void alertProfiles(int);

public:
    explicit Stationary3(QWidget *parent = nullptr);
    ~Stationary3() override;
    void updateProfiles();

private:
    Ui::Stationary3 *ui;
    Searcher3Model *searcherModel;
    Stationary3Model *generatorModel;
    QVector<Profile3> profiles;
    QMenu *generatorMenu;
    QMenu *searcherMenu;
    QModelIndex lastIndex;
    QModelIndex targetFrame;

    void setupModels();
    void updateView(const QVector<Frame3> &frames, int progress);

public slots:
    void moveResults(const QString &seed, const QString &method, u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe);

private slots:
    void refreshProfiles();
    void on_comboBoxProfiles_currentIndexChanged(int index);
    void on_pushButtonGenerate_clicked();
    void on_pushButtonSearch_clicked();
    void on_comboBoxSearcherMethod_currentIndexChanged(int index);
    void on_tableViewGenerator_customContextMenuRequested(const QPoint &pos);
    void on_tableViewSearcher_customContextMenuRequested(const QPoint &pos);
    void setTargetFrameGenerator();
    void jumpToTargetGenerator();
    void centerFramesAndSetTargetGenerator(u32 centerFrames);
    void seedToTime();
    void copySeedToClipboard();
    void on_pushButtonProfileManager_clicked();

};

class StationarySearcher3 : public QThread
{
    Q_OBJECT

public:
    StationarySearcher3(const Searcher3 &searcher, const QVector<u8> &min, const QVector<u8> &max);
    void run() override;
    int currentProgress() const;
    QVector<Frame3> getResults();

public slots:
    void cancelSearch();

private:
    Searcher3 searcher;
    QVector<u8> min;
    QVector<u8> max;

    QMutex mutex;
    QVector<Frame3> results;
    bool cancel;
    int progress;

};

#endif // STATIONARY3_H
