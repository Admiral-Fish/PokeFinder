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
#include <Objects/Nature.hpp>
#include <RNG/LCRNG.hpp>
#include <RNG/RNGEuclidean.hpp>

namespace Ui
{
    class IVtoPID;
}

class IVtoPID : public QMainWindow
{
    Q_OBJECT

public:
    explicit IVtoPID(QWidget *parent = nullptr);
    ~IVtoPID() override;

private:
    Ui::IVtoPID *ui;
    QStandardItemModel *model = new QStandardItemModel(this);

    void setupModels();
    void getSeeds(u32 ivs1, u32 ivs2, u16 nature, u16 tid);
    void getSeedsChannel(u16 hp, u16 atk, u16 def, u16 spa, u16 spd, u16 spe, u16 nature);

private slots:
    void on_pushButtonFind_clicked();

};

#endif // IVTOPID
