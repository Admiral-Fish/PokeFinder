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
enum class Shiny : u8;

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
    void *decompress(const u8 *compressedData, u32 compressedSize, u32 &size);

    /**
     * @brief Decompress provided data and cast to specified \p Type
     * The caller of this function is responsible for cleaning up the memory.
     *
     * @tparam Type Data type to cast too
     * @param compressedData Compressed data
     * @param compressedSize Size of compressed data
     * @param size Size of uncompressed data
     *
     * @return Uncompressed data
     */
    template <typename Type>
    const Type *decompress(const u8 *compressedData, u32 compressedSize, u32 &size)
    {
        auto *data = reinterpret_cast<const Type *>(decompress(compressedData, compressedSize, size));
        size /= sizeof(Type);
        return data;
    }

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
     * @tparam old Whether to use the old or new shiny compare value
     * @param pid Pokemon PID
     * @param tsv Trainer shiny value
     *
     * @return Shiny value
     */
    template <bool old>
    inline u8 getShiny(u32 pid, u16 tsv)
    {
        constexpr u8 compare = old ? 8 : 16;

        u16 psv = (pid >> 16) ^ (pid & 0xffff);
        if (tsv == psv)
        {
            return 2; // Square
        }
        else if ((tsv ^ psv) < compare)
        {
            return 1; // Star
        }
        else
        {
            return 0;
        }
    }

    /**
     * @brief Determines if the \p pid is shiny based on the \p tsv
     *
     * @tparam old Whether to use the old or new shiny compare value
     * @param pid Pokemon PID
     * @param tsv Trainer shiny value
     *
     * @return Shiny value
     */
    template <bool old>
    inline u8 isShiny(u32 pid, u16 tsv)
    {
        constexpr u8 compare = old ? 8 : 16;

        u16 psv = (pid >> 16) ^ (pid & 0xffff);
        return (tsv ^ psv) < compare;
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
     * @brief Creates PID from parameters
     *
     * @param tsv Trainer shiny value
     * @param ability Ability to force
     * @param gender Gender to force
     * @param shiny Shiny to force
     * @param wild Whether encounter is considered a wild battle
     * @param ratio Gender ratio
     * @param rng PRNG state
     * @param info Pokemon information
     *
     * @return PID
     */
    u32 createPID(u16 tsv, u8 ability, u8 gender, Shiny shiny, bool wild, u8 ratio, BWRNG &rng);

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
