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
#include <Core/RNG/LCRNG64.hpp>
#include <string>

class DateTime;
class PersonalInfo;

namespace Utilities
{
    /**
     * @brief Decompress provided data
     * The caller of this function is responsible for cleaning up the memory.
     *
     * @param compressedData Compressed data
     * @param compressedSize Size of compressed data
     * @param size Size of uncompressed data
     *
     * @return Uncompressed data
     */
    char *decompress(const char *compressedData, u32 compressedSize, u32 &size);

    /**
     * @brief Decompress provided data
     * The caller of this function is responsible for cleaning up the memory.
     *
     * @param compressedData Compressed data
     * @param compressedSize Size of compressed data
     * @param size Size of uncompressed data
     *
     * @return Uncompressed data
     */
    u8 *decompress(const u8 *compressedData, u32 compressedSize, u32 &size);
}

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

namespace Utilities5
{
    /**
     * @brief Modifies PID to force a gender
     *
     * @param pid PID value to modify
     * @param rng PRNG state
     * @param gender Gender to force
     * @param info Pokemon information
     *
     * @return Modified PID
     */
    u32 forceGender(u32 pid, BWRNG &rng, u8 gender, const PersonalInfo *info);

    /**
     * @brief Returns the chatot pitch
     *
     * @param prng PRNG state
     *
     * @return Chatot pitch
     */
    std::string getChatot(u8 prng);

    /**
     * @brief Calculates the initial set of advances for BW
     *
     * @param seed PRNG state
     *
     * @return Initial advance count
     */
    u32 initialAdvancesBW(u64 seed);

    /**
     * @brief Calculates the initial set of advances for BW2
     *
     * @param seed PRNG state
     * @param memory Whether memory link is activated or not
     *
     * @return Initial advance count
     */
    u32 initialAdvancesBW2(u64 seed, bool memory);

    /**
     * @brief Calculates the initial set of advances for BW ID
     *
     * @param seed PRNG state
     *
     * @return Initial advance count
     */
    u32 initialAdvancesBWID(u64 seed);

    /**
     * @brief Calculates the initial set of advances for BW2
     *
     * @param seed PRNG state
     *
     * @return Initial advance count
     */
    u32 initialAdvancesBW2ID(u64 seed);
}

#endif // UTILITIES_HPP
