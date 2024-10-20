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

#ifndef SHA1_HPP
#define SHA1_HPP

#include <Core/Global.hpp>
#include <Core/RNG/SIMD.hpp>
#include <array>

enum class DSType : u8;
enum class Game : u32;
enum class Language : u8;
class Date;
class Profile5;

/**
 * @brief Simplified SHA1 hashing implementation optimized for creating Gen5 initial seeds
 */
class SHA1
{
public:
    /**
     * @brief Construct a new SHA1 object
     *
     * @param profile Profile input parameters
     */
    SHA1(const Profile5 &profile);

    /**
     * @brief Construct a new SHA1 object
     *
     * @param version Game version parameter
     * @param language Language parameter
     * @param type DS type parameter
     * @param mac MAC address parameter
     * @param softReset Soft reset parameter
     * @param vFrame VFrame parameter
     * @param gxStat GxStat parameter
     */
    SHA1(Game version, Language language, DSType type, u64 mac, bool softReset, u8 vFrame, u8 gxStat);

    /**
     * @brief Hashes input parameters from the precomputed \p alpha
     *
     * @param alpha Precomputed first 8 rounds alpha
     *
     * @return Hashed seed
     */
    u64 hashSeed(const std::array<u32, 5> &alpha);

    /**
     * @brief Precomputes the first 8 rounds of SHA1. Must first call \ref setTimer0() and \ref setDate().
     * For hashes computed on the same date, the first 8 rounds will be the same.
     */
    std::array<u32, 5> precompute();

    /**
     * @brief Sets the SHA1 parameter based on \p button
     *
     * @param button Keypress parameter
     */
    void setButton(u32 button);

    /**
     * @brief Sets the SHA1 parameter based on \p date
     *
     * @param date Date parameter
     */
    void setDate(const Date &date);

    /**
     * @brief Sets the SHA1 parameter based on \p timer0 and \p vcount
     *
     * @param timer0 Timer0 parameter
     * @param vcount VCount parameter
     */
    void setTimer0(u32 timer0, u8 vcount);

    /**
     * @brief Sets the SHA1 parameter based on time and \p dsType
     *
     * @param hour Hour parameter
     * @param minute Minute parameter
     * @param second Second parameter
     * @param dsType DS type parameter
     */
    void setTime(u8 hour, u8 minute, u8 second, DSType dsType);

    /**
     * @brief Sets the SHA1 parameter based on time and \p dsType
     *
     * @param time Time parameter
     * @param dsType DS type parameter
     */
    void setTime(u32 time, DSType dsType);

private:
    u32 data[80];
};

class MultiSHA1
{
public:
    /**
     * @brief Construct a new SHA1 object
     *
     * @param profile Profile input parameters
     */
    MultiSHA1(const Profile5 &profile);

    /**
     * @brief Construct a new SHA1 object
     *
     * @param version Game version parameter
     * @param language Language parameter
     * @param type DS type parameter
     * @param mac MAC address parameter
     * @param softReset Soft reset parameter
     * @param vFrame VFrame parameter
     * @param gxStat GxStat parameter
     */
    MultiSHA1(Game version, Language language, DSType type, u64 mac, bool softReset, u8 vFrame, u8 gxStat);

    /**
     * @brief Hashes input parameters from the precomputed \p alpha
     *
     * @param alpha Precomputed first 8 rounds alpha
     *
     * @return Hashed seed
     */
    std::array<u64, 4> hashSeed(const std::array<vuint128, 5> &alpha);

    /**
     * @brief Precomputes the first 8 rounds of SHA1. Must first call \ref setTimer0() and \ref setDate().
     * For hashes computed on the same date, the first 8 rounds will be the same.
     */
    std::array<vuint128, 5> precompute();

    /**
     * @brief Sets the SHA1 parameter based on \p button
     *
     * @param button Keypress parameter
     */
    void setButton(u32 button);

    /**
     * @brief Sets the SHA1 parameter based on \p date
     *
     * @param date Date parameter
     */
    void setDate(const Date &date);

    /**
     * @brief Sets the SHA1 parameter based on \p timer0 and \p vcount
     *
     * @param timer0 Timer0 parameter
     * @param vcount VCount parameter
     */
    void setTimer0(u32 timer0, u8 vcount);

    /**
     * @brief Sets the SHA1 parameter based on time and \p dsType
     *
     * @param hour Hour parameter
     * @param minute Minute parameter
     * @param second Second parameter
     * @param dsType DS type parameter
     */
    void setTime(u8 hour, u8 minute, u8 second, DSType dsType);

    /**
     * @brief Sets the SHA1 parameter based on time and \p dsType
     *
     * @param time Time parameter
     * @param dsType DS type parameter
     */
    void setTime(u32 time, DSType dsType);

private:
    vuint128 data[80];
};

#endif // SHA1_HPP
