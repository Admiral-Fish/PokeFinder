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

#include "ProfileSearcher5.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Gen5/States/ProfileSearcherState5.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <Core/RNG/MTFast.hpp>
#include <Core/RNG/SHA1.hpp>
#include <Core/Util/Utilities.hpp>

ProfileSearcher5::ProfileSearcher5(const Date &date, const Time &time, u8 minSeconds, u8 maxSeconds, u8 minVCount, u8 maxVCount,
                                   u16 minTimer0, u16 maxTimer0, u8 minGxStat, u8 maxGxStat, bool softReset, Game version,
                                   Language language, DSType dsType, u64 mac, Buttons buttons) :
    mac(mac),
    date(date),
    version(version),
    keypress { Keypresses::getValue(buttons), buttons },
    time(time),
    maxTimer0(maxTimer0),
    minTimer0(minTimer0),
    softReset(softReset),
    dsType(dsType),
    language(language),
    maxGxStat(maxGxStat),
    minGxStat(minGxStat),
    maxSeconds(maxSeconds),
    minSeconds(minSeconds),
    maxVCount(maxVCount),
    minVCount(minVCount)
{
}

void ProfileSearcher5::startSearch(int threads, u8 minVFrame, u8 maxVFrame)
{
    searching = true;

    u8 diff = maxVFrame - minVFrame + 1;
    if (diff < threads)
    {
        threads = diff;
    }

    auto *threadContainer = new std::thread[threads];

    auto split = (diff / threads);
    for (int i = 0; i < threads; i++, minVFrame += split)
    {
        if (i == threads - 1)
        {
            threadContainer[i] = std::thread([=] { search(minVFrame, maxVFrame); });
        }
        else
        {
            threadContainer[i] = std::thread([=] { search(minVFrame, minVFrame + split - 1); });
        }
    }

    for (int i = 0; i < threads; i++)
    {
        threadContainer[i].join();
    }

    delete[] threadContainer;
}

void ProfileSearcher5::search(u8 minVFrame, u8 maxVFrame)
{
    u8 hour = time.hour();
    u8 minute = time.minute();

    for (u16 vframe = minVFrame; vframe <= maxVFrame; vframe++)
    {
        for (u16 gxStat = minGxStat; gxStat <= maxGxStat; gxStat++)
        {
            SHA1 sha(version, language, dsType, mac, softReset, vframe, gxStat);
            sha.setDate(date);
            sha.setButton(keypress.value);
            for (u32 timer0 = minTimer0; timer0 <= maxTimer0; timer0++)
            {
                for (u16 vcount = minVCount; vcount <= maxVCount; vcount++)
                {
                    sha.setTimer0(timer0, vcount);
                    auto alpha = sha.precompute();
                    for (u8 second = minSeconds; second <= maxSeconds; second++)
                    {
                        if (!searching)
                        {
                            return;
                        }

                        sha.setTime(hour, minute, second, dsType);

                        u64 seed = sha.hashSeed(alpha);
                        if (valid(seed))
                        {
                            std::lock_guard<std::mutex> lock(mutex);
                            results.emplace_back(seed, static_cast<u16>(timer0), static_cast<u8>(vcount), static_cast<u8>(vframe),
                                                 static_cast<u8>(gxStat), second);
                        }
                    }
                }
                progress++;
            }
        }
    }
}

ProfileIVSearcher5::ProfileIVSearcher5(const Date &date, const Time &time, int minSeconds, int maxSeconds, u8 minVCount, u8 maxVCount,
                                       u16 minTimer0, u16 maxTimer0, u8 minGxStat, u8 maxGxStat, bool softReset, Game version,
                                       Language language, DSType dsType, u64 mac, Buttons buttons, const std::array<u8, 6> &minIVs,
                                       const std::array<u8, 6> &maxIVs) :
    ProfileSearcher5(date, time, minSeconds, maxSeconds, minVCount, maxVCount, minTimer0, maxTimer0, minGxStat, maxGxStat, softReset,
                     version, language, dsType, mac, buttons),
    maxIVs(maxIVs),
    minIVs(minIVs),
    offset((version & Game::BW2) != Game::None ? 2 : 0)
{
}

bool ProfileIVSearcher5::valid(u64 seed)
{
    MTFast<8, true> rng(seed >> 32, offset);

    for (u8 i = 0; i < 6; i++)
    {
        u8 iv = rng.next();
        if (iv < minIVs[i] || iv > maxIVs[i])
        {
            return false;
        }
    }

    return true;
}

ProfileNeedleSearcher5::ProfileNeedleSearcher5(const Date &date, const Time &time, int minSeconds, int maxSeconds, u8 minVCount,
                                               u8 maxVCount, u16 minTimer0, u16 maxTimer0, u8 minGxStat, u8 maxGxStat, bool softReset,
                                               Game version, Language language, DSType dsType, u64 mac, Buttons buttons,
                                               const std::vector<u8> &needles, bool unovaLink, bool memoryLink) :
    ProfileSearcher5(date, time, minSeconds, maxSeconds, minVCount, maxVCount, minTimer0, maxTimer0, minGxStat, maxGxStat, softReset,
                     version, language, dsType, mac, buttons),
    needles(needles),
    game((version & Game::BW) != Game::None),
    memoryLink(memoryLink),
    unovaLink(unovaLink)
{
}

bool ProfileNeedleSearcher5::valid(u64 seed)
{
    u8 advances = game ? Utilities5::initialAdvancesBW(seed) : Utilities5::initialAdvancesBW2(seed, memoryLink);

    // Opening the menu advances 1
    if (unovaLink && !memoryLink)
    {
        advances++;
    }

    BWRNG rng(seed, advances);
    for (u8 needle : needles)
    {
        u8 rand = rng.nextUInt(8);
        if (rand != needle)
        {
            return false;
        }

        if (unovaLink)
        {
            rng.next();
        }
    }

    return true;
}

ProfileSeedSearcher5::ProfileSeedSearcher5(const Date &date, const Time &time, int minSeconds, int maxSeconds, u8 minVCount, u8 maxVCount,
                                           u16 minTimer0, u16 maxTimer0, u8 minGxStat, u8 maxGxStat, bool softReset, Game version,
                                           Language language, DSType dsType, u64 mac, Buttons buttons, u64 seed) :
    ProfileSearcher5(date, time, minSeconds, maxSeconds, minVCount, maxVCount, minTimer0, maxTimer0, minGxStat, maxGxStat, softReset,
                     version, language, dsType, mac, buttons),
    seed(seed)
{
}

bool ProfileSeedSearcher5::valid(u64 seed)
{
    return this->seed == seed;
}
