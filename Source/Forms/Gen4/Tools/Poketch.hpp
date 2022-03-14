/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef POKETCH_HPP
#define POKETCH_HPP

#include <Core/Util/Global.hpp>
#include <QWidget>

namespace Ui
{
    class Poketch;
}

class Poketch : public QWidget
{
    Q_OBJECT

public:
    explicit Poketch(u32 advances, QWidget *parent = nullptr);
    ~Poketch();

private slots:
    void on_pushButtonClose_clicked();

private:
    Ui::Poketch *ui;
    void setupModels(u32 advances);
};

#endif // POKETCH_HPP
