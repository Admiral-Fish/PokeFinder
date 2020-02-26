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

#ifndef RNGCACHE_HPP
#define RNGCACHE_HPP

#include <Core/Util/Global.hpp>
#include <QVector>

enum Method : u8;

class RNGCache
{
public:
    RNGCache() = default;
    explicit RNGCache(Method method);
    QVector<u32> recoverLower16BitsIV(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    QVector<u32> recoverLower16BitsPID(u32 pid) const;

private:
    u32 add;
    u32 k;
    u32 mult;
    u8 low[0x10000];
    bool flags[0x10000];
};

#endif // RNGCACHE_HPP
