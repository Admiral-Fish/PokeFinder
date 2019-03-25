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

#ifndef EGGS4_HPP
#define EGGS4_HPP

#include <QMenu>
#include <QMutex>
#include <QSettings>
#include <QThread>
#include <QTimer>
#include <Core/Gen4/Egg4.hpp>
#include <Forms/Gen4/SeedtoTime4.hpp>
#include <Forms/Gen4/ProfileManager4.hpp>
#include <Models/Gen4/Egg4Model.hpp>

namespace Ui
{
    class Eggs4;
}

class Eggs4 : public QWidget
{
    Q_OBJECT

signals:
    void alertProfiles(int);

public:
    explicit Eggs4(QWidget *parent = nullptr);
    ~Eggs4() override;
    void updateProfiles();

private:
    Ui::Eggs4 *ui;
    QVector<Profile4> profiles;
    Egg4GeneratorModel *generatorModel;
    Egg4SearcherModel *searcherIVs;
    Egg4SearcherModel *searcherPID;
    QMenu *searcherMenu;
    bool flag;

    void setupModels();
    void updatePID(const QVector<Frame4> &frames, int progress);
    void updateIVs(const QVector<Frame4> &frames, int progress);

private slots:
    void refreshProfiles();
    void on_pushButtonGenerate_clicked();
    void on_pushButtonSearchPID_clicked();
    void on_pushButtonSearchIVs_clicked();
    void on_comboBoxProfiles_currentIndexChanged(int index);
    void on_tableViewPID_customContextMenuRequested(const QPoint &pos);
    void on_tableViewIVs_customContextMenuRequested(const QPoint &pos);
    void seedToTime();
    void on_pushButtonProfileManager_clicked();

};

class PIDSearcher : public QThread
{
    Q_OBJECT

public:
    PIDSearcher(const Egg4 &generator, const FrameCompare &compare, u32 minDelay, u32 maxDelay);
    void run() override;
    int currentProgress() const;
    QVector<Frame4> getResults();

public slots:
    void cancelSearch();

private:
    Egg4 generator;
    FrameCompare compare;
    u32 minDelay;
    u32 maxDelay;

    QVector<Frame4> results;
    QMutex mutex;
    bool cancel;
    int progress;

};

class IVSearcher : public QThread
{
    Q_OBJECT

public:
    IVSearcher(const Egg4 &generator, const FrameCompare &compare, u32 minDelay, u32 maxDelay);
    void run() override;
    int currentProgress() const;
    QVector<Frame4> getResults();

public slots:
    void cancelSearch();

private:
    Egg4 generator;
    FrameCompare compare;
    u32 minDelay;
    u32 maxDelay;

    QMutex mutex;
    QVector<Frame4> results;
    bool cancel;
    int progress;

};

#endif // EGGS4_HPP
