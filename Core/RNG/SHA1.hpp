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

#ifndef SHA1_HPP
#define SHA1_HPP

#include <Core/Gen5/Nazos.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <QDate>

class SHA1
{
public:
    explicit SHA1(const Profile5 &profile);
    u64 hashSeed();
    void precompute();
    void setTime(u8 hour, u8 minute, u8 second);
    void setTimer0(u32 timer0);
    void setDate(u8 year, u8 month, u8 day, u8 week);
    void setButton(u32 button);

private:
    Profile5 profile;
    u32 data[80];
    u32 alpha[5];
};

#endif // SHA1_HPP
