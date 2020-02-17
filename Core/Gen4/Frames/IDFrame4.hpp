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

#ifndef IDFRAME4_HPP
#define IDFRAME4_HPP

#include <Core/Parents/Frames/IDFrame.hpp>

class IDFrame4 : public IDFrame
{
public:
    IDFrame4() = default;
    IDFrame4(u32 seed, u16 tid, u16 sid);
    u32 getSeed() const;
    void setSeed(u32 seed);
    u32 getDelay() const;
    void setDelay(u32 delay);
    u8 getSeconds() const;
    void setSeconds(u8 seconds);

private:
    u32 seed;
    u32 delay;
    u8 seconds;
};

#endif // IDFRAME4_HPP
