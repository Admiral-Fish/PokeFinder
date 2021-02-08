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

#include "ProfileSearcherState5.hpp"

ProfileSearcherState5::ProfileSearcherState5(u64 seed, u16 timer0, u8 vcount, u8 vframe, u8 gxstat, u8 second) :
    seed(seed), timer0(timer0), vcount(vcount), vframe(vframe), gxstat(gxstat), second(second)
{

}

u64 ProfileSearcherState5::getSeed() const
{
    return seed;
}

u16 ProfileSearcherState5::getTimer0() const
{
    return timer0;
}

u8 ProfileSearcherState5::getVcount() const
{
    return vcount;
}

u8 ProfileSearcherState5::getVframe() const
{
    return vframe;
}

u8 ProfileSearcherState5::getGxstat() const
{
    return gxstat;
}

u8 ProfileSearcherState5::getSecond() const
{
    return second;
}
