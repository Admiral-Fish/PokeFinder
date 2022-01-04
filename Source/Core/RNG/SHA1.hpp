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

#ifndef SHA1_HPP
#define SHA1_HPP

#include <Core/Util/Global.hpp>

enum class DSType : u8;
enum class Game : u32;
enum class Language : u8;
class Date;
class Profile5;

class SHA1
{
public:
    explicit SHA1(const Profile5 &profile);
    SHA1(Game version, Language language, DSType type, u64 mac, bool softReset, u8 vFrame, u8 gxStat);
    u64 hashSeed();
    void precompute();
    void setTimer0(u32 timer0, u8 vcount);
    void setDate(const Date &date);
    void setTime(u8 hour, u8 minute, u8 second, DSType dsType);
    void setButton(u32 button);

private:
    u32 data[80];
    u32 alpha[5];
};

#endif // SHA1_HPP
