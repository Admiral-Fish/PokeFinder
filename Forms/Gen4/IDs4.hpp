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

#ifndef IDS4_HPP
#define IDS4_HPP

#include <QMainWindow>
#include <QMessageBox>
#include <QMutex>
#include <QStandardItemModel>
#include <QThread>
#include <QTimer>
#include <Core/Objects/Utilities.hpp>
#include <Core/RNG/LCRNG.hpp>

namespace Ui
{
    class IDs4;
}

class IDs4 : public QMainWindow
{
    Q_OBJECT

signals:
    void updateProgress();

public:
    explicit IDs4(QWidget *parent = nullptr);
    ~IDs4() override;

private:
    Ui::IDs4 *ui;
    QStandardItemModel *model = new QStandardItemModel(this);

    void setupModels();
    void updateModel(QVector<QList<QStandardItem *>> frames, int progress);

private slots:
    void on_pushButtonSearchShinyPID_clicked();
    void on_pushButtonSearchTIDSID_clicked();
    void on_pushButtonSearchSeedFinder_clicked();
    void on_checkBoxSearchTIDShinyPID_toggled(bool checked);
    void on_checkBoxInfiniteSearchShinyPID_toggled(bool checked);
    void on_checkBoxSearchSID_toggled(bool checked);
    void on_checkBoxInfiniteSearchTIDSID_toggled(bool checked);

};

class ShinyPIDSearcher : public QThread
{
    Q_OBJECT

signals:
    void resultReady(QVector <QList<QStandardItem *>> frames);

public:
    ShinyPIDSearcher(u32 pid, bool useTID, u16 tid, u32 year, u32 minDelay, u32 maxDelay, bool infinite);
    void run() override;
    int currentProgress();
    QVector<QList<QStandardItem *>> getResults();

public slots:
    void cancelSearch();

private:
    u32 pid;
    bool useTID;
    u16 tid;
    u32 year;
    u32 minDelay;
    u32 maxDelay;
    bool infinite;

    QMutex mutex;
    QVector<QList<QStandardItem *>> results;
    bool cancel;
    int progress;

};

class TIDSIDSearcher : public QThread
{
    Q_OBJECT

signals:
    void resultReady(QVector<QList<QStandardItem *>> frames);

public:
    TIDSIDSearcher(u16 tid, bool useSID, u16 searchSID, u32 year, u32 minDelay, u32 maxDelay, bool infinite);
    void run() override;
    int currentProgress();
    QVector<QList<QStandardItem *>> getResults();

public slots:
    void cancelSearch();

private:
    u16 tid;
    bool useSID;
    u16 searchSID;
    u32 year;
    u32 minDelay;
    u32 maxDelay;
    bool infinite;

    QMutex mutex;
    QVector<QList<QStandardItem *>> results;
    bool cancel;
    int progress;

};

#endif // IDS4_HPP
