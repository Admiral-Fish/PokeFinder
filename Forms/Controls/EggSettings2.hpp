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

#ifndef EGGSETTINGS2_HPP
#define EGGSETTINGS2_HPP

#include <Core/Parents/Daycare.hpp>
#include <QWidget>

namespace Ui
{
    class EggSettings2;
}

class EggSettings2 : public QWidget
{
    Q_OBJECT

public:
    explicit EggSettings2(QWidget *parent = nullptr);
    ~EggSettings2();
    void setup(u8 generation);
    Daycare getDaycareSettings() const;
    bool compatibleParents() const;
    bool reorderParents();

private:
    Ui::EggSettings2 *ui;

    void setupModels();
};

#endif // EGGSETTINGS2_HPP
