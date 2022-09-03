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

#ifndef RNGEUCLIDEAN_HPP
#define RNGEUCLIDEAN_HPP

#include <Core/Global.hpp>
#include <vector>

enum class Method : u8;

/**
 * @brief Provides a way to compute origin seed given IVs or PID for GameCube games.
 * See https://crypto.stackexchange.com/a/10629 for how the following math works. Uses Euclidean divison to reduce the search space (kmax)
 * even further then RNGCache. Only beneficial for smaller multipliers such as XDRNG.
 */
namespace RNGEuclidean
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
     * @param seeds Array to write results
     *
     * @return Number of origin seeds (Safe upper bound of 12)
     */
    int recoverChannelIV(u32 hp, u32 atk, u32 def, u32 spa, u32 spd, u32 spe, u32 *seeds);

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
     * @return Number of origin seeds (Won't be higher than 6)
     */
    int recoverXDRNGIV(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u32 *seeds);

    /**
     * @brief Recovers origin seeds for two 16 bit calls
     *
     * @param pid PID value
     * @param seeds Array to write results
     *
     * @return Number of origin seeds (Won't be higher than 2)
     */
    int recoverXDRNGPID(u32 pid, u32 *seeds);
}

#endif // RNGEUCLIDEAN_HPP
