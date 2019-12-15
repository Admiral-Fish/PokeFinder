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

#ifndef SEEDTOTIME3_HPP
#define SEEDTOTIME3_HPP

#include <Core/Util/Global.hpp>
#include <QStandardItemModel>
#include <QWidget>

namespace Ui
{
    class SeedToTime3;
}

class SeedToTime3 : public QWidget
{
    Q_OBJECT
public:
    explicit SeedToTime3(QWidget *parent = nullptr);
    explicit SeedToTime3(u32 seed, QWidget *parent = nullptr);
    ~SeedToTime3() override;

private:
    Ui::SeedToTime3 *ui;
    QStandardItemModel *model {};
    u32 frame = 1;

    void setupModels();
    u16 originSeed(u32 seed);
    void seedToTime(u32 seed, u32 year);

private slots:
    void find();
};

#endif // SEEDTOTIME3_HPP
