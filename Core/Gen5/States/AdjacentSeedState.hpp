/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef ADJACENTSEEDSTATE_HPP
#define ADJACENTSEEDSTATE_HPP

#include <Core/Global.hpp>
#include <Core/Util/DateTime.hpp>
#include <array>

enum class Buttons : u16;

class AdjacentSeedState
{
public:
    AdjacentSeedState(u64 seed, const DateTime &dateTime, Buttons buttons, u16 timer0, u32 ivAdvance, const std::array<u8, 6> &ivs,
                      u32 pidAdvance, bool target) :
        seed(seed),
        dateTime(dateTime),
        buttons(buttons),
        timer0(timer0),
        ivAdvance(ivAdvance),
        ivs(ivs),
        pidAdvance(pidAdvance),
        target(target)
    {
    }

    Buttons getButtons() const
    {
        return buttons;
    }

    DateTime getDateTime() const
    {
        return dateTime;
    }

    u8 getIV(u8 index) const
    {
        return ivs[index];
    }

    u32 getIVAdvance() const
    {
        return ivAdvance;
    }

    u32 getPIDAdvance() const
    {
        return pidAdvance;
    }

    u64 getSeed() const
    {
        return seed;
    }

    u16 getTimer0() const
    {
        return timer0;
    }

    bool isTarget() const
    {
        return target;
    }

private:
    u64 seed;
    DateTime dateTime;
    Buttons buttons;
    u16 timer0;
    u32 ivAdvance;
    std::array<u8, 6> ivs;
    u32 pidAdvance;
    bool target;
};

#endif // ADJACENTSEEDSTATE_HPP
