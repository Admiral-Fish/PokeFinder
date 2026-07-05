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

#include "AdjacentSeedCalculator.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Gen5/Keypresses.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <Core/RNG/MT.hpp>
#include <Core/RNG/SHA1.hpp>
#include <Core/Util/Utilities.hpp>

static std::array<u8, 6> generateMTIVs(u64 seed, Game version, u32 ivAdvance, bool roamer)
{
    bool bw = (version & Game::BW) != Game::None;
    MT rng(seed >> 32, ivAdvance + (bw ? 0 : 2) + (roamer ? 1 : 0));

    std::array<u8, 6> ivs;
    ivs[0] = rng.next() >> 27;
    ivs[1] = rng.next() >> 27;
    ivs[2] = rng.next() >> 27;

    if (roamer)
    {
        ivs[4] = rng.next() >> 27;
        ivs[5] = rng.next() >> 27;
        ivs[3] = rng.next() >> 27;
    }
    else
    {
        ivs[3] = rng.next() >> 27;
        ivs[4] = rng.next() >> 27;
        ivs[5] = rng.next() >> 27;
    }

    return ivs;
}

static bool normalizeDSDateTime(DateTime &dateTime)
{
    const u32 minJD = Date(2000, 1, 1).getJD();
    const u32 maxJD = Date(2100, 1, 1).getJD();
    const u32 days = maxJD - minJD;

    Date date = dateTime.getDate();
    u32 jd = date.getJD();
    if (jd < minJD)
    {
        return false;
    }

    if (jd >= maxJD)
    {
        dateTime = DateTime(Date(minJD + ((jd - minJD) % days)), dateTime.getTime());
    }

    return true;
}

std::vector<AdjacentSeedState> AdjacentSeedCalculator::generate(const Profile5 &profile, const DateTime &dateTime, Buttons buttons,
                                                                int seconds, u32 minIVAdvance, u32 maxIVAdvance, bool roamer)
{
    u32 timer0Min = profile.getTimer0Min();
    u32 timer0Max = profile.getTimer0Max();
    timer0Min = timer0Min == 0 ? 0 : timer0Min - 1;
    timer0Max = timer0Max == 0xffff ? 0xffff : timer0Max + 1;

    u32 rangeSeconds = seconds * 2 + 1;
    u32 timer0Count = timer0Max - timer0Min + 1;
    u32 ivCount = maxIVAdvance - minIVAdvance + 1;

    SHA1 sha(profile);
    sha.setButton(Keypresses::getValue(buttons));

    std::vector<AdjacentSeedState> states;
    states.reserve(rangeSeconds * timer0Count * ivCount);

    for (int secondOffset = -seconds; secondOffset <= seconds; secondOffset++)
    {
        DateTime offset = dateTime.addSeconds(secondOffset);
        if (!normalizeDSDateTime(offset))
        {
            continue;
        }

        Date date = offset.getDate();
        Time time = offset.getTime();

        sha.setDate(date);
        sha.setTime(time.hour(), time.minute(), time.second(), profile.getDSType());

        for (u32 timer0 = timer0Min; timer0 <= timer0Max; timer0++)
        {
            sha.setTimer0(timer0, profile.getVCount());
            auto alpha = sha.precompute();
            u64 seed = sha.hashSeed(alpha);

            for (u32 ivAdvance = minIVAdvance; ivAdvance <= maxIVAdvance; ivAdvance++)
            {
                std::array<u8, 6> ivs = generateMTIVs(seed, profile.getVersion(), ivAdvance, roamer);

                states.emplace_back(seed, offset, buttons, static_cast<u16>(timer0), ivAdvance, ivs,
                                    Utilities5::initialAdvances(seed, profile) + ivAdvance,
                                    offset == dateTime && timer0 == profile.getTimer0Min() && ivAdvance == minIVAdvance);
            }
        }
    }

    return states;
}

std::vector<u32> AdjacentSeedCalculator::previewPRNG(u64 seed, const Profile5 &, u32 pidAdvance, u32 count)
{
    std::vector<u32> states;
    states.reserve(count);

    BWRNG rng(seed, pidAdvance);
    for (u32 i = 0; i < count; i++)
    {
        states.emplace_back(rng.nextUInt());
    }

    return states;
}
