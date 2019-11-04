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

#include "Characteristic.hpp"

namespace PokeFinderCore
{
    Characteristic::Characteristic(u8 index, u8 stat, u8 result)
    {
        this->index = index;
        this->stat = stat;
        this->result = result;
        active = true;
    }

    QVector<Characteristic> Characteristic::getCharacteristics()
    {
        QVector<Characteristic> characteristics = { Characteristic(), Characteristic(0, 0, 0), Characteristic(1, 0, 1),
            Characteristic(2, 0, 2), Characteristic(3, 0, 3), Characteristic(4, 0, 4), Characteristic(5, 1, 0),
            Characteristic(6, 1, 1), Characteristic(7, 1, 2), Characteristic(8, 1, 3), Characteristic(9, 1, 4),
            Characteristic(10, 2, 0), Characteristic(11, 2, 1), Characteristic(12, 2, 2), Characteristic(13, 2, 3),
            Characteristic(14, 2, 4), Characteristic(15, 5, 0), Characteristic(16, 5, 1), Characteristic(17, 5, 2),
            Characteristic(18, 5, 3), Characteristic(19, 5, 4), Characteristic(20, 3, 0), Characteristic(21, 3, 1),
            Characteristic(22, 3, 2), Characteristic(23, 3, 3), Characteristic(24, 3, 4), Characteristic(25, 4, 0),
            Characteristic(26, 4, 1), Characteristic(27, 4, 2), Characteristic(28, 4, 3), Characteristic(29, 4, 4) };
        return characteristics;
    }

    u8 Characteristic::getIndex() const { return index; }

    u8 Characteristic::getStat() const { return stat; }

    u8 Characteristic::getResult() const { return result; }

    bool Characteristic::getActive() const { return active; }

}
