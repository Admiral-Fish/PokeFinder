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

#include <QClipboard>
#include <QMainWindow>
#include <QMenu>
#include <QStandardItemModel>
#include <Core/Objects/Method.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/RNG/RNGCache.hpp>
#include <Core/RNG/RNGEuclidean.hpp>

namespace Ui
{
    class PIDtoIVs;
}

class PIDtoIVs : public QMainWindow
{
    Q_OBJECT

signals:
    void moveResultsToStationary(QString, QString, u8, u8, u8, u8, u8, u8);

public:
    explicit PIDtoIVs(QWidget *parent = nullptr);
    ~PIDtoIVs() override;

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
    QString calcIVsXD(u16 iv1, u16 iv2);
    QString calcIVsChannel(u32 iv1);
    void addSeed(u32 seed, u32 iv1);
    void addSeedGC(u32 seed, u16 iv1, u16 iv2);
    void addSeedChannel(u32 seed, u32 iv1);

private slots:
    void on_pushButtonGenerate_clicked();
    void on_tableView_customContextMenuRequested(const QPoint &pos);
    void copySeed();

};

#endif // PIDTOIVS_HPP
