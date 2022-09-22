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

#ifndef SEEDTIME_HPP
#define SEEDTIME_HPP

#include <Core/Util/DateTime.hpp>

class HGSSRoamer;
enum class Game : u32;

/**
 * @brief State class for seed to date/time Gen 4
 */
class SeedTime4
{
public:
    /**
     * @brief Construct a new SeedTime4 object
     *
     * @param dateTime Date/time
     * @param delay Delay
     */
    SeedTime4(const DateTime &dateTime, u32 delay);

    /**
     * @brief Returns the date/time
     *
     * @return Date/time
     */
    DateTime getDateTime() const;

    /**
     * @brief Returns the delay
     *
     * @return Delay
     */
    u32 getDelay() const;

protected:
    DateTime dateTime;
    u32 delay;
};

/**
 * @brief State class for seed to date/time calibration Gen 4
 */
class SeedTimeCalibrate4 : public SeedTime4
{
public:
    /**
     * @brief Construct a new SeedTimeCalibrate4 object
     *
     * @param dateTime Date/time
     * @param delay Delay
     * @param version Game version
     * @param roamers Active roamers
     * @param routes Roamer locations
     */
    SeedTimeCalibrate4(const DateTime &dateTime, u32 delay, Game version, const std::array<bool, 3> &roamers,
                       const std::array<u8, 3> &routes);

    /**
     * @brief SeedTimeCalibrate4
     * @param other SeedTimeCalibrate4 to copy
     */
    SeedTimeCalibrate4(const SeedTimeCalibrate4 &other);

    /**
     * @brief Destroy the SeedTimeCalibrate4 object
     */
    ~SeedTimeCalibrate4();

    /**
     * @brief Returns the roamer information
     *
     * @return Roamer information
     */
    HGSSRoamer *getRoamer() const;

    /**
     * @brief Returns the seed
     *
     * @return Seed
     */
    u32 getSeed() const;

    /**
     * @brief Returns either the coin flip for call string
     *
     * @return Sequence string
     */
    std::string getSequence() const;

private:
    HGSSRoamer *roamer;
    u32 seed;
};

#endif // SEEDTIME_HPP
