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

#ifndef PIDTOIVS_HPP
#define PIDTOIVS_HPP

#include <QMainWindow>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QList>
#include <QVector>
#include <PokeFinderCore/RNG/RNGCache.hpp>
#include <PokeFinderCore/Objects/Method.hpp>
#include <PokeFinderCore/RNG/RNGEuclidean.hpp>
#include <PokeFinderCore/RNG/LCRNG.hpp>
#include <QMenu>
#include <QAction>
#include <QModelIndex>
#include <QClipboard>
#include <QStringList>

typedef uint32_t u32;

namespace Ui
{
    class PIDtoIVs;
}

class PIDtoIVs : public QMainWindow
{
    Q_OBJECT

protected:
    void changeEvent(QEvent *);

signals:
    void moveResultsToStationary(QString, QString, u32, u32, u32, u32, u32, u32);

private:
    Ui::PIDtoIVs *ui;
    QStandardItemModel *model = new QStandardItemModel(this);
    QMenu *contextMenu = new QMenu();
    QModelIndex lastIndex;
    QModelIndex targetFrame;

    void setupModels();
    void calcFromPID(u32 pid);
    void calcMethod124(u32 pid);
    void calcMethodXD(u32 pid);
    void calcMethodChannel(u32 pid);
    QString calcIVs(u32 iv1, int num);
    QString calcIVsXD(u32 iv1, u32 iv2);
    QString calcIVsChannel(u32 iv1);
    void addSeed(u32 seed, u32 iv1);
    void addSeedGC(u32 seed, u32 iv1, u32 iv2);
    void addSeedChannel(u32 seed, u32 iv1);

private slots:
    void on_pushButtonGenerate_clicked();
    void on_tabePIDToIV_customContextMenuRequested(const QPoint &pos);
    void copySeed();

public:
    explicit PIDtoIVs(QWidget *parent = nullptr);
    ~PIDtoIVs();

};

#endif // PIDTOIVS_HPP
