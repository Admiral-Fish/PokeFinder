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

#ifndef GAMECUBERTC_HPP
#define GAMECUBERTC_HPP

#include <QMainWindow>
#include <QStandardItemModel>
#include <QStandardItem>
#include <thread>
#include <QList>
#include <libPokeFinder/RNG/LCRNG.hpp>
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QMenu>
#include <QAction>
#include <QModelIndex>
#include <QClipboard>

typedef uint32_t u32;

namespace Ui {
class GameCubeRTC;
}

class GameCubeRTC : public QMainWindow
{
    Q_OBJECT

protected:
    void changeEvent(QEvent*);

signals:
    void updateView(QList<QStandardItem *>);

private:
    Ui::GameCubeRTC *ui;
    bool isSearching = false;
    QStandardItemModel *model = new QStandardItemModel(this);
    QDateTime date = QDateTime(QDate(2000, 1, 1), QTime(0, 0));
    QMenu *contextMenu = new QMenu();
    QModelIndex lastIndex;
    QModelIndex targetFrame;

    void setupModels();
    void calcRTC();

private slots:
    void on_pushButtonSearch_clicked();
    void updateTableView(QList<QStandardItem *> row);
    void copySeed();

    void on_tableViewGenerator_customContextMenuRequested(const QPoint &pos);

public:
    explicit GameCubeRTC(QWidget *parent = 0);
    ~GameCubeRTC();

};

#endif // GAMECUBERTC_HPP
