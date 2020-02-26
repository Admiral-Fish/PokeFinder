/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef SEEDTIME3_HPP
#define SEEDTIME3_HPP

#include <Core/Util/Global.hpp>
#include <QWidget>

class SeedTimeModel3;

namespace Ui
{
    class SeedTime3;
}

class SeedTime3 : public QWidget
{
    Q_OBJECT
public:
    explicit SeedTime3(QWidget *parent = nullptr);
    explicit SeedTime3(u32 seed, QWidget *parent = nullptr);
    ~SeedTime3() override;

private:
    Ui::SeedTime3 *ui;
    SeedTimeModel3 *model = nullptr;
    u32 frame = 1;

    void setupModels();
    u16 originSeed(u32 seed);

private slots:
    void find();
};

#endif // SEEDTIME3_HPP
