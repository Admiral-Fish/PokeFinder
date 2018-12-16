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

#ifndef LEAD_HPP
#define LEAD_HPP

// Different modifiers that can lead a party
enum Lead
{
    None            = 1 << 0,
    Search          = 1 << 1,
    Synchronize     = 1 << 2,
    CuteCharm       = 1 << 3,
    CuteCharm50M    = 1 << 4,
    CuteCharm75M    = 1 << 5,
    CuteCharm25M    = 1 << 6,
    CuteCharm875M   = 1 << 7,
    CuteCharm50F    = 1 << 8,
    CuteCharm75F    = 1 << 9,
    CuteCharm25F    = 1 << 10,
    CuteCharm125F   = 1 << 11,
    CuteCharmFemale = 1 << 12,
    SuctionCups     = 1 << 13
};

#endif // LEAD_HPP
