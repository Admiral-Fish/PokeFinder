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

#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <Core/Global.hpp>
#include <string>

class DateTime;

namespace Utilities3
{
    /**
     * @brief Calculates initial seed from the \p dateTime
     *
     * @param dateTime Starting date/time
     *
     * @return Initial seed
     */
    u16 calcSeed(const DateTime &dateTime);
}

namespace Utilities4
{
    /**
     * @brief Calculates initial seed from the \p dateTime and \p delay
     *
     * @param dateTime Starting date/time
     * @param delay Starting delay
     *
     * @return Initial seed
     */
    u32 calcSeed(const DateTime &dateTime, u32 delay);

    /**
     * @brief Returns the coin flip sequence for the \p seed
     *
     * @param seed PRNG state
     *
     * @return Coin flips
     */
    std::string coinFlips(u32 seed);

    /**
     * @brief Calculates the call sequence from the \p seed and \p info
     *
     * @param seed PRNG state
     * @param info Roamer information
     *
     * @return Call sequence string
     */
    std::string getCalls(u32 seed, u8 skips);

    /**
     * @brief Returns the chatot pitch
     *
     * @param prng PRNG state
     *
     * @return Chatot pitch
     */
    std::string getChatot(u8 prng);
}

/*namespace Utilities5
{
    std::string getChatot(u32 seed);
    u32 initialAdvancesBW(u64 seed);
    u32 initialAdvancesBW2(u64 seed, bool memory);
    u32 initialAdvancesBWID(u64 seed);
    u32 initialAdvancesBW2ID(u64 seed);
    u32 forceGender(u32 pid, u64 rand, u8 gender, u8 genderRatio);
}*/

#endif // UTILITIES_HPP
