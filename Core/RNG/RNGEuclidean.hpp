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

#ifndef RNGEUCLIDEAN_HPP
#define RNGEUCLIDEAN_HPP

#include <Core/Util/Global.hpp>
#include <QVector>

enum Method : u8;

class RNGEuclidean
{
public:
    RNGEuclidean() = default;
    explicit RNGEuclidean(Method method);
    QVector<QPair<u32, u32>> recoverLower16BitsIV(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    QVector<QPair<u32, u32>> recoverLower16BitsPID(u32 pid) const;
    QVector<u32> recoverLower27BitsChannel(u32 hp, u32 atk, u32 def, u32 spa, u32 spd, u32 spe) const;

private:
    u64 base;
    u32 sub1;
    u32 sub2;
};

#endif // RNGEUCLIDEAN_HPP
