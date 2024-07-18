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

#ifndef PROFILESEARCHER5_HPP
#define PROFILESEARCHER5_HPP

#include <Core/Gen5/Keypresses.hpp>
#include <Core/Global.hpp>
#include <Core/Parents/Searchers/Searcher.hpp>
#include <Core/Util/DateTime.hpp>
#include <array>

class ProfileSearcherState5;
enum class Game : u32;
enum class Language : u8;
enum class DSType : u8;

/**
 * @brief Parent class to calibrate profiles
 */
class ProfileSearcher5 : public SearcherBase<ProfileSearcherState5>
{
public:
    /**
     * @brief Construct a new ProfileSearcher5 object
     *
     * @param date Date to search
     * @param time Time to search
     * @param minSeconds Minimum seconds
     * @param maxSeconds Maximum seconds
     * @param minVCount Minimum VCount
     * @param maxVCount Maximum VCount
     * @param minTimer0 Minimum Timer0
     * @param maxTimer0 Maximum Timer0
     * @param minGxStat Minimum GxStat
     * @param maxGxStat Maximum GxStat
     * @param softReset Whether game was soft reset
     * @param version Game version
     * @param language Game language
     * @param dsType DS type
     * @param mac DS MAC address
     * @param buttons Keypresses selected
     */
    ProfileSearcher5(const Date &date, const Time &time, u8 minSeconds, u8 maxSeconds, u8 minVCount, u8 maxVCount, u16 minTimer0,
                     u16 maxTimer0, u8 minGxStat, u8 maxGxStat, bool softReset, Game version, Language language, DSType dsType, u64 mac,
                     Buttons buttons);

    /**
     * @brief Starts the search
     *
     * @param threads Number of threads to search with
     * @param minVFrame Minimum VFrame
     * @param maxVFrame Maximum VFrame
     */
    void startSearch(int threads, u8 minVFrame, u8 maxVFrame);

private:
    u64 mac;
    Date date;
    Game version;
    Keypress keypress;
    Time time;
    u16 maxTimer0;
    u16 minTimer0;
    bool softReset;
    DSType dsType;
    Language language;
    u8 maxGxStat;
    u8 minGxStat;
    u8 maxSeconds;
    u8 minSeconds;
    u8 maxVCount;
    u8 minVCount;

    void search(u8 minVFrame, u8 maxVFrame);

    /**
     * @brief Checks if seed matches the provided calibration data
     *
     * @param seed PRNG state
     *
     * @return true Seed is valid
     * @return false Seed is not valid
     */
    virtual bool valid(u64 seed) = 0;
};

/**
 * @brief Calibrates profiles based on IVs
 */
class ProfileIVSearcher5 final : public ProfileSearcher5
{
public:
    /**
     * @brief Construct a new ProfileSearcher5 object
     *
     * @param date Date to search
     * @param time Time to search
     * @param minSeconds Minimum seconds
     * @param maxSeconds Maximum seconds
     * @param minVCount Minimum VCount
     * @param maxVCount Maximum VCount
     * @param minTimer0 Minimum Timer0
     * @param maxTimer0 Maximum Timer0
     * @param minGxStat Minimum GxStat
     * @param maxGxStat Maximum GxStat
     * @param softReset Whether game was soft reset
     * @param version Game version
     * @param language Game language
     * @param dsType DS type
     * @param mac DS MAC address
     * @param buttons Keypresses selected
     * @param minIVs Minimum IVs
     * @param maxIVs Maximum IVs
     */
    ProfileIVSearcher5(const Date &date, const Time &time, int minSeconds, int maxSeconds, u8 minVCount, u8 maxVCount, u16 minTimer0,
                       u16 maxTimer0, u8 minGxStat, u8 maxGxStat, bool softReset, Game version, Language language, DSType dsType, u64 mac,
                       Buttons buttons, const std::array<u8, 6> &minIVs, const std::array<u8, 6> &maxIVs);

private:
    std::array<u8, 6> maxIVs;
    std::array<u8, 6> minIVs;
    u8 offset;

    /**
     * @brief Checks if seed matches the provided calibration IVs
     *
     * @param seed PRNG state
     *
     * @return true Seed is valid
     * @return false Seed is not valid
     */
    bool valid(u64 seed) final;
};

/**
 * @brief Calibrates profiles based on needles
 */
class ProfileNeedleSearcher5 final : public ProfileSearcher5
{
public:
    /**
     * @brief Construct a new ProfileSearcher5 object
     *
     * @param date Date to search
     * @param time Time to search
     * @param minSeconds Minimum seconds
     * @param maxSeconds Maximum seconds
     * @param minVCount Minimum VCount
     * @param maxVCount Maximum VCount
     * @param minTimer0 Minimum Timer0
     * @param maxTimer0 Maximum Timer0
     * @param minGxStat Minimum GxStat
     * @param maxGxStat Maximum GxStat
     * @param softReset Whether game was soft reset
     * @param version Game version
     * @param language Game language
     * @param dsType DS type
     * @param mac DS MAC address
     * @param buttons Keypresses selected
     * @param needles Save/Unova link needles
     * @param Whether Unova link is used to obtain needles
     * @param Whether memory link is enabled
     */
    ProfileNeedleSearcher5(const Date &date, const Time &time, int minSeconds, int maxSeconds, u8 minVCount, u8 maxVCount, u16 minTimer0,
                           u16 maxTimer0, u8 minGxStat, u8 maxGxStat, bool softReset, Game version, Language language, DSType dsType,
                           u64 mac, Buttons buttons, const std::vector<u8> &needles, bool unovaLink, bool memoryLink);

private:
    std::vector<u8> needles;
    bool game;
    bool memoryLink;
    bool unovaLink;

    /**
     * @brief Checks if seed matches the provided calibration needles
     *
     * @param seed PRNG state
     *
     * @return true Seed is valid
     * @return false Seed is not valid
     */
    bool valid(u64 seed) final;
};

/**
 * @brief Calibrates profiles based on seed
 */
class ProfileSeedSearcher5 final : public ProfileSearcher5
{
public:
    /**
     * @brief Construct a new ProfileSearcher5 object
     *
     * @param date Date to search
     * @param time Time to search
     * @param minSeconds Minimum seconds
     * @param maxSeconds Maximum seconds
     * @param minVCount Minimum VCount
     * @param maxVCount Maximum VCount
     * @param minTimer0 Minimum Timer0
     * @param maxTimer0 Maximum Timer0
     * @param minGxStat Minimum GxStat
     * @param maxGxStat Maximum GxStat
     * @param softReset Whether game was soft reset
     * @param version Game version
     * @param language Game language
     * @param dsType DS type
     * @param mac DS MAC address
     * @param buttons Keypresses selected
     * @param seed PRNG state
     */
    ProfileSeedSearcher5(const Date &date, const Time &time, int minSeconds, int maxSeconds, u8 minVCount, u8 maxVCount, u16 minTimer0,
                         u16 maxTimer0, u8 minGxStat, u8 maxGxStat, bool softReset, Game version, Language language, DSType dsType, u64 mac,
                         Buttons buttons, u64 seed);

private:
    u64 seed;

    /**
     * @brief Checks if seed matches the provided calibration seed
     *
     * @param seed PRNG state
     *
     * @return true Seed is valid
     * @return false Seed is not valid
     */
    bool valid(u64 seed) final;
};

#endif // PROFILESEARCHER5_HPP
