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

static u8 nextIV(MT &rng)
{
    return rng.next() >> 27;
}

static std::array<u8, 6> generateMTIVs(u64 seed, Game version, u32 ivAdvance, bool roamer)
{
    bool bw = (version & Game::BW) != Game::None;
    MT rng(seed >> 32, ivAdvance + (bw ? 0 : 2) + (roamer ? 1 : 0));

    std::array<u8, 6> ivs;
    ivs[0] = nextIV(rng);
    ivs[1] = nextIV(rng);
    ivs[2] = nextIV(rng);

    if (roamer)
    {
        ivs[4] = nextIV(rng);
        ivs[5] = nextIV(rng);
        ivs[3] = nextIV(rng);
    }
    else
    {
        ivs[3] = nextIV(rng);
        ivs[4] = nextIV(rng);
        ivs[5] = nextIV(rng);
    }

    return ivs;
}

AdjacentSeedState::AdjacentSeedState(u64 seed, const DateTime &dateTime, Buttons buttons, u16 timer0, u32 ivAdvance,
                                     const std::array<u8, 6> &ivs, u32 pidAdvance, bool target) :
    seed(seed),
    dateTime(dateTime),
    buttons(buttons),
    timer0(timer0),
    ivAdvance(ivAdvance),
    ivs(ivs),
    pidAdvance(pidAdvance),
    target(target)
{
}

Buttons AdjacentSeedState::getButtons() const
{
    return buttons;
}

const DateTime &AdjacentSeedState::getDateTime() const
{
    return dateTime;
}

u8 AdjacentSeedState::getIV(u8 index) const
{
    return ivs[index];
}

u32 AdjacentSeedState::getIVAdvance() const
{
    return ivAdvance;
}

u32 AdjacentSeedState::getPIDAdvance() const
{
    return pidAdvance;
}

u64 AdjacentSeedState::getSeed() const
{
    return seed;
}

u16 AdjacentSeedState::getTimer0() const
{
    return timer0;
}

bool AdjacentSeedState::isTarget() const
{
    return target;
}

std::vector<AdjacentSeedState> AdjacentSeedCalculator::generate(const AdjacentSeedSettings &settings)
{
    std::vector<AdjacentSeedState> states;
    u32 timer0Min = settings.profile.getTimer0Min();
    u32 timer0Max = settings.profile.getTimer0Max();
    timer0Min = timer0Min == 0 ? 0 : timer0Min - 1;
    timer0Max = timer0Max == 0xffff ? 0xffff : timer0Max + 1;

    u32 rangeSeconds = settings.seconds * 2 + 1;
    u32 timer0Count = timer0Max - timer0Min + 1;
    u32 ivCount = settings.maxIVAdvance - settings.minIVAdvance + 1;
    states.reserve(rangeSeconds * timer0Count * ivCount);

    SHA1 sha(settings.profile);
    sha.setButton(Keypresses::getValue(settings.buttons));

    for (int secondOffset = -static_cast<int>(settings.seconds); secondOffset <= static_cast<int>(settings.seconds); secondOffset++)
    {
        DateTime dateTime = settings.dateTime.addSeconds(secondOffset);
        Date date = dateTime.getDate();
        Time time = dateTime.getTime();
        sha.setDate(date);
        sha.setTime(time.hour(), time.minute(), time.second(), settings.profile.getDSType());

        for (u32 timer0 = timer0Min; timer0 <= timer0Max; timer0++)
        {
            sha.setTimer0(timer0, settings.profile.getVCount());
            auto alpha = sha.precompute();
            u64 seed = sha.hashSeed(alpha);

            for (u32 ivAdvance = settings.minIVAdvance; ivAdvance <= settings.maxIVAdvance; ivAdvance++)
            {
                std::array<u8, 6> ivs
                    = generateMTIVs(seed, settings.profile.getVersion(), ivAdvance, settings.method == AdjacentSeedMethod::Roamer);

                states.emplace_back(seed, dateTime, settings.buttons, static_cast<u16>(timer0), ivAdvance, ivs,
                                    Utilities5::initialAdvances(seed, settings.profile) + ivAdvance,
                                    dateTime == settings.dateTime && timer0 == settings.profile.getTimer0Min()
                                        && ivAdvance == settings.minIVAdvance);
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
