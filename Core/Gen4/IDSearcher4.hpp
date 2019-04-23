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

#ifndef IDSEARCHER4_HPP
#define IDSEARCHER4_HPP

#include <QMutex>
#include <QObject>
#include <QStandardItem>
#include <Core/Util/Global.hpp>

class ShinyPIDSearcher : public QObject
{
    Q_OBJECT

signals:
    void finished();
    void updateProgress(const QVector<QList<QStandardItem *>> &, int);

public:
    ShinyPIDSearcher(u32 pid, bool useTID, u16 tid, u32 year, u32 minDelay, u32 maxDelay, bool infinite);
    void startSearch();

public slots:
    void cancelSearch();

private:
    u32 pid;
    bool useTID, infinite;
    u16 tid;
    u32 year, minDelay, maxDelay;
    QMutex mutex;
    QVector<QList<QStandardItem *>> results;
    bool searching, cancel;
    int progress;

    void search();
    void update();
    QVector<QList<QStandardItem *>> getResults();

};

class TIDSIDSearcher : public QObject
{
    Q_OBJECT

signals:
    void finished();
    void updateProgress(const QVector<QList<QStandardItem *>> &, int);

public:
    TIDSIDSearcher(u16 tid, bool useSID, u16 searchSID, u32 year, u32 minDelay, u32 maxDelay, bool infinite);
    void startSearch();

public slots:
    void cancelSearch();

private:
    u16 tid;
    bool useSID, infinite;
    u16 searchSID;
    u32 year, minDelay, maxDelay;
    QMutex mutex;
    QVector<QList<QStandardItem *>> results;
    bool searching, cancel;
    int progress;

    void search();
    void update();
    QVector<QList<QStandardItem *>> getResults();

};
#endif // IDSEARCHER4_HPP
