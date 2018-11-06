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

#ifndef IVTOPIDHPP
#define IVTOPIDHPP

#include <QMainWindow>
#include <QStandardItemModel>
#include <PokeFinderCore/Objects/Nature.hpp>
#include <PokeFinderCore/RNG/LCRNG.hpp>
#include <PokeFinderCore/RNG/RNGEuclidean.hpp>

namespace Ui
{
    class IVtoPID;
}

class IVtoPID : public QMainWindow
{
    Q_OBJECT

private:
    Ui::IVtoPID *ui;
    QStandardItemModel *model = new QStandardItemModel(this);

    void setupModels();
    void getSeeds(u32 ivs1, u32 ivs2, u32 nature, u32 tid);
    void getSeedsChannel(u32 hp, u32 atk, u32 def, u32 spa, u32 spd, u32 spe, u32 nature);

private slots:
    void on_pushButtonFind_clicked();

public:
    explicit IVtoPID(QWidget *parent = nullptr);
    ~IVtoPID() override;

};

#endif // IVTOPID
