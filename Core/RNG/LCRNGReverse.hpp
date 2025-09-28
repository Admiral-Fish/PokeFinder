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

#ifndef LCRNGREVERSE_HPP
#define LCRNGREVERSE_HPP

#include <Core/Global.hpp>

enum class Method : u8;

template <int size>
struct RecoverySeeds
{
    int count;
    u32 seeds[size];

    RecoverySeeds() : count(0)
    {
    }

    u32 &operator[](int i)
    {
        return seeds[i];
    }
};

/**
 * @brief Provides a way to compute origin seed given IVs or PID.
 *
 * The PokeRNG attacks are a derivate of meet-in-the-middle attack (based on https://crypto.stackexchange.com/a/10609) combined with
 * patterns in modular arithmetic.
 *
 * The Channel and XDRNG attacks are Euclidean divisor based (https://crypto.stackexchange.com/a/10629).
 */
namespace LCRNGReverse
{
    /**
     * @brief Recovers origin seeds for six 5 bit calls
     *
     * @param hp HP iv
     * @param atk Atk iv
     * @param def Def iv
     * @param spa SpA iv
     * @param spd SpD iv
     * @param spe Spe iv
     *
     * @return Array of origin seeds (Safe upper bound of 12)
     */
    RecoverySeeds<12> recoverChannelIV(u32 hp, u32 atk, u32 def, u32 spa, u32 spd, u32 spe);

    /**
     * @brief Recovers origin seeds for two 16 bit calls(15 bits known) with or without gap
     *
     * @param hp HP iv
     * @param atk Atk iv
     * @param def Def iv
     * @param spa SpA iv
     * @param spd SpD iv
     * @param spe Spe iv
     * @param seeds Array to write results
     *
     * @return Array of origin seeds (Won't be higher than 6)
     */
    RecoverySeeds<6> recoverPokeRNGIV(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, Method method);

    /**
     * @brief Recovers origin seeds for two 16 bit calls
     *
     * @param pid PID value
     * @param seeds Array to write results
     *
     * @return Array of origin seeds (Won't be higher than 3)
     */
    RecoverySeeds<3> recoverPokeRNGPID(u32 pid);

    /**
     * @brief Recovers origin seeds for two 16 bit calls(15 bits known)
     *
     * @param hp HP iv
     * @param atk Atk iv
     * @param def Def iv
     * @param spa SpA iv
     * @param spd SpD iv
     * @param spe Spe iv
     * @param seeds Array to write results
     *
     * @return Array of origin seeds (Won't be higher than 6)
     */
    RecoverySeeds<6> recoverXDRNGIV(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe);

    /**
     * @brief Recovers origin seeds for two 16 bit calls
     *
     * @param pid PID value
     * @param seeds Array to write results
     *
     * @return Array of origin seeds (Won't be higher than 2)
     */
    RecoverySeeds<2> recoverXDRNGPID(u32 pid);
};

#endif // LCRNGREVERSE_HPP
