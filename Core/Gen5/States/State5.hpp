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

#ifndef STATE5_HPP
#define STATE5_HPP

#include <Core/Util/DateTime.hpp>
#include <Core/Util/Global.hpp>

class State5
{
public:
    State5() = default;

    State5(const DateTime &dt, u64 initialSeed, u16 buttons, u16 timer0) :
        dt(dt), initialSeed(initialSeed), buttons(buttons), timer0(timer0)
    {
    }

    DateTime getDateTime() const
    {
        return dt;
    }

    u64 getInitialSeed() const
    {
        return initialSeed;
    }

    u16 getButtons() const
    {
        return buttons;
    }

    u16 getTimer0() const
    {
        return timer0;
    }

protected:
    DateTime dt;
    u64 initialSeed;
    u16 buttons;
    u16 timer0;
};

#endif // STATE5_HPP
