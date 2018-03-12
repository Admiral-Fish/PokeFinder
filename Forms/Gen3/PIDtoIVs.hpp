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

#ifndef PIDTOIVS_HPP
#define PIDTOIVS_HPP

#include <QMainWindow>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QList>
#include <libPokeFinder/RNG/RNGCache.hpp>
#include <libPokeFinder/Objects/Method.hpp>
#include <libPokeFinder/RNG/RNGEuclidean.hpp>
#include <libPokeFinder/RNG/LCRNG.hpp>

typedef uint32_t u32;

using std::vector;
namespace Ui {
class PIDtoIVs;
}

class PIDtoIVs : public QMainWindow
{
    Q_OBJECT

public:
    explicit PIDtoIVs(QWidget *parent = 0);
    ~PIDtoIVs();

private slots:
    void on_pushButtonGenerate_clicked();

private:
    Ui::PIDtoIVs *ui;
    QStandardItemModel *m = new QStandardItemModel(this);

    void setupModels();
    void calcFromPID(u32 pid);
    void calcMethod124(u32 pid);
    void calcMethodXD(u32 pid);
    QString calcIVs1(u32 iv1);
    QString calcIVs2(u32 iv1);
    QString calcIVs4(u32 iv1);
    QString calcIVsXD(u32 iv1, u32 iv2);
    void addSeed(u32 seed, u32 iv1);
    void addSeedGC(u32 seed, u32 iv1, u32 iv2);

};

#endif // PIDTOIVS_HPP
