/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef METHOD_HPP
#define METHOD_HPP

#include <Core/Global.hpp>

/**
 * @brief Enum to encompass different encounter methods
 */
enum class Method : u8
{
    None,

    Method1,
    Method1Reverse,
    Method2,
    Method4,

    XDColo,
    Channel,

    EBred,
    EBredSplit,
    EBredAlternate,
    EBredPID,
    RSFRLGBred,
    RSFRLGBredSplit,
    RSFRLGBredAlternate,
    RSFRLGBredMixed,

    CuteCharmDPPt,
    CuteCharmHGSS,
    MethodJ,
    MethodK,
    PokeRadar,
    WondercardIVs,

    Gen4Normal,
    Gen4Masuda,
    DPPtIVs,
    HGSSIVs,
    Gen4Combined,

    Method5IVs,
    Method5CGear,
    Method5
};

/**
 * @brief Converts enum to number
 *
 * @param method Input method
 *
 * @return Converted number
 */
constexpr u8 toInt(Method method)
{
    return static_cast<u8>(method);
}

#endif // METHOD_HPP
