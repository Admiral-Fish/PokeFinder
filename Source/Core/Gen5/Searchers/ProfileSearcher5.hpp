/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#include <Core/Gen5/States/ProfileSearcherState5.hpp>
#include <Core/Util/DateTime.hpp>
#include <Core/Util/Global.hpp>
#include <array>
#include <atomic>
#include <mutex>
#include <vector>

enum Buttons : u16;
enum Game : u16;
enum Language : u8;
enum DSType : u8;

class ProfileSearcher5
{
public:
    ProfileSearcher5() = default;
    explicit ProfileSearcher5(const Date &date, const Time &time, u8 minSeconds, u8 maxSeconds, u8 minVCount, u8 maxVCount, u16 minTimer0,
                              u16 maxTimer0, u8 minGxStat, u8 maxGxStat, bool softReset, Game version, Language language, DSType dsType,
                              u64 mac, Buttons keypress);
    virtual ~ProfileSearcher5() = default;
    void startSearch(int threads, u8 minVFrame, u8 maxVFrame);
    void cancelSearch();
    std::vector<ProfileSearcherState5> getResults();
    int getProgress() const;

private:
    Date date;
    Time time;
    u8 minSeconds;
    u8 maxSeconds;
    u8 minVCount;
    u8 maxVCount;
    u16 minTimer0;
    u16 maxTimer0;
    u8 minGxStat;
    u8 maxGxStat;
    bool softReset;
    Game version;
    Language language;
    DSType dsType;
    u64 mac;
    Buttons keypress;

    bool searching;
    std::atomic<int> progress;
    std::vector<ProfileSearcherState5> results;
    std::mutex mutex;

    void search(u8 vframeStart, u8 vframeEnd);

protected:
    virtual bool valid(u64 seed) = 0;
};

class ProfileIVSearcher5 : public ProfileSearcher5
{
public:
    ProfileIVSearcher5() = default;
    explicit ProfileIVSearcher5(const std::array<u8, 6> &minIVs, const std::array<u8, 6> &maxIVs, const Date &date, const Time &time,
                                int minSeconds, int maxSeconds, u8 minVCount, u8 maxVCount, u16 minTimer0, u16 maxTimer0, u8 minGxStat,
                                u8 maxGxStat, bool softReset, Game version, Language language, DSType dsType, u64 mac, Buttons keypress);

private:
    std::array<u8, 6> minIVs;
    std::array<u8, 6> maxIVs;
    u8 offset;

    bool valid(u64 seed) override;
};

class ProfileNeedleSearcher5 : public ProfileSearcher5
{
public:
    ProfileNeedleSearcher5() = default;
    explicit ProfileNeedleSearcher5(const std::vector<u8> &needles, bool unovaLink, bool memoryLink, const Date &date, const Time &time,
                                    int minSeconds, int maxSeconds, u8 minVCount, u8 maxVCount, u16 minTimer0, u16 maxTimer0, u8 minGxStat,
                                    u8 maxGxStat, bool softReset, Game version, Language language, DSType dsType, u64 mac,
                                    Buttons keypress);

private:
    std::vector<u8> needles;
    bool unovaLink;
    bool memoryLink;
    bool game;

    bool valid(u64 seed) override;
};

class ProfileSeedSearcher5 : public ProfileSearcher5
{
public:
    ProfileSeedSearcher5() = default;
    explicit ProfileSeedSearcher5(u64 seed, const Date &date, const Time &time, int minSeconds, int maxSeconds, u8 minVCount, u8 maxVCount,
                                  u16 minTimer0, u16 maxTimer0, u8 minGxStat, u8 maxGxStat, bool softReset, Game version, Language language,
                                  DSType dsType, u64 mac, Buttons keypress);

private:
    u64 seed;

    bool valid(u64 seed) override;
};

#endif // PROFILESEARCHER5_HPP
