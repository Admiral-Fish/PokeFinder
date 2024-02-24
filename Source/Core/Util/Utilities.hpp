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

#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <Core/Global.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <string>

class DateTime;
class Profile5;
enum class Game : u32;

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

    /**
     * @brief Determines the gender of the \p pid based on the gender ratio of the \p info
     *
     * @param pid Pokemon PID
     * @param info Pokemon information
     *
     * @return Gender value
     */
    inline u8 getGender(u32 pid, const PersonalInfo *info)
    {
        switch (info->getGender())
        {
        case 255: // Genderless
            return 2;
            break;
        case 254: // Female
            return 1;
            break;
        case 0: // Male
            return 0;
            break;
        default: // Random gender
            return (pid & 255) < info->getGender();
            break;
        }
    }

    /**
     * @brief Determines the shiny of the \p pid based on the \p tsv
     *
     * This is utilized by Gen 3-5
     *
     * @param pid Pokemon PID
     * @param tsv Trainer shiny value
     *
     * @return Shiny value
     */
    inline u8 getShiny(u32 pid, u16 tsv)
    {
        u16 psv = (pid >> 16) ^ (pid & 0xffff);
        if (tsv == psv)
        {
            return 2; // Square
        }
        else if ((tsv ^ psv) < 8)
        {
            return 1; // Star
        }
        else
        {
            return 0;
        }
    }
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
     * @param genderRatio Pokemon gender ratio
     *
     * @return Modified PID
     */
    u32 forceGender(u32 pid, BWRNG &rng, u8 gender, u8 genderRatio);

    /**
     * @brief Returns the chatot pitch
     *
     * @param prng PRNG state
     *
     * @return Chatot pitch
     */
    std::string getChatot(u8 prng);

    /**
     * @brief Calculates the initial set of advances for the \p profile
     *
     * @param seed PRNG state
     * @param profile Profile information
     *
     * @return Initial advance count
     */
    u32 initialAdvances(u64 seed, const Profile5 &profile);

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

    /**
     * @brief Calculates the initial set of advances for the \p profile
     *
     * @param seed PRNG state
     * @param version Game version
     *
     * @return Initial advance count
     */
    u32 initialAdvancesID(u64 seed, Game version);
}

#endif // UTILITIES_HPP
