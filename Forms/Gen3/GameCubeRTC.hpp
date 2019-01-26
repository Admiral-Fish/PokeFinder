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

#ifndef GAMECUBERTC_HPP
#define GAMECUBERTC_HPP

#include <QDateTime>
#include <QMainWindow>
#include <QMenu>
#include <QSettings>
#include <QStandardItemModel>
#include <QThread>
#include <Core/RNG/LCRNG.hpp>

namespace Ui
{
    class GameCubeRTC;
}

class Search;

class GameCubeRTC : public QMainWindow
{
    Q_OBJECT

public:
    explicit GameCubeRTC(QWidget *parent = nullptr);
    ~GameCubeRTC() override;

private:
    Ui::GameCubeRTC *ui;
    QStandardItemModel *model;
    QMenu *contextMenu;
    QModelIndex lastIndex;
    QModelIndex targetFrame;

    void setupModels();

private slots:
    void on_pushButtonSearch_clicked();
    void updateTableView(const QList<QStandardItem *> &row);
    void copySeed();
    void on_tableViewGenerator_customContextMenuRequested(const QPoint &pos);

};

class Search : public QThread
{
    Q_OBJECT

signals:
    void result(QList<QStandardItem *>);

public:
    Search(u32 initialSeed, u32 targetSeed, u32 minFrame, u32 maxFrame);
    void run() override;

public slots:
    void cancelSearch();

private:
    const QDateTime date = QDateTime(QDate(2000, 1, 1), QTime(0, 0));
    u32 initialSeed, targetSeed;
    u32 minFrame, maxFrame;
    bool cancel;

};

#endif // GAMECUBERTC_HPP
