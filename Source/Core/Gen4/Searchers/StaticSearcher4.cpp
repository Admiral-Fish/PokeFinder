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

#include "StaticSearcher4.hpp"
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen4/States/State4.hpp>
#include <Core/Gen4/StaticTemplate4.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/RNG/LCRNGReverse.hpp>
#include <Core/Util/Utilities.hpp>

static bool isShiny(u32 pid, u16 tsv)
{
    u16 psv = ((pid >> 16) & (pid & 0xffff));
    return (psv ^ tsv) < 8;
}

StaticSearcher4::StaticSearcher4(u32 minAdvance, u32 maxAdvance, u32 minDelay, u32 maxDelay, Method method, Lead lead,
                                 const Profile4 &profile, const StateFilter &filter) :
    StaticSearcher(method, lead, profile, filter),
    maxAdvance(maxAdvance),
    minAdvance(minAdvance),
    maxDelay(maxDelay),
    minDelay(minDelay),
    buffer(0)
{
}

void StaticSearcher4::startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max, const StaticTemplate4 *staticTemplate)
{
    searching = true;
    if (lead == Lead::CuteCharmF)
    {
        buffer = 25 * ((staticTemplate->getInfo()->getGender() / 25) + 1);
    }

    for (u8 hp = min[0]; hp <= max[0]; hp++)
    {
        for (u8 atk = min[1]; atk <= max[1]; atk++)
        {
            for (u8 def = min[2]; def <= max[2]; def++)
            {
                for (u8 spa = min[3]; spa <= max[3]; spa++)
                {
                    for (u8 spd = min[4]; spd <= max[4]; spd++)
                    {
                        for (u8 spe = min[5]; spe <= max[5]; spe++)
                        {
                            if (!searching)
                            {
                                return;
                            }

                            auto states = search(hp, atk, def, spa, spd, spe, staticTemplate);

                            std::lock_guard<std::mutex> guard(mutex);
                            results.insert(results.end(), states.begin(), states.end());
                            progress++;
                        }
                    }
                }
            }
        }
    }
}

std::vector<SearcherState4> StaticSearcher4::search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe,
                                                    const StaticTemplate4 *staticTemplate) const
{
    std::vector<SearcherState4> states;

    if (method == Method::Method1)
    {
        states = searchMethod1(hp, atk, def, spa, spd, spe, staticTemplate);
    }
    else if (method == Method::MethodJ)
    {
        states = searchMethodJ(hp, atk, def, spa, spd, spe, staticTemplate);
    }
    else if (method == Method::MethodK)
    {
        states = searchMethodK(hp, atk, def, spa, spd, spe, staticTemplate);
    }

    return searchInitialSeeds(states);
}

std::vector<SearcherState4> StaticSearcher4::searchInitialSeeds(const std::vector<SearcherState4> &results) const
{
    std::vector<SearcherState4> states;

    for (SearcherState4 result : results)
    {
        PokeRNGR rng(result.getSeed(), minAdvance);
        u32 seed = rng.getSeed();
        for (u32 cnt = minAdvance; cnt <= maxAdvance; cnt++)
        {
            u8 hour = (seed >> 16) & 0xFF;
            u16 delay = seed & 0xFFFF;

            // Check if seed matches a valid gen 4 format
            if (hour < 24 && delay >= minDelay && delay <= maxDelay)
            {
                result.setSeed(seed);
                result.setAdvances(cnt);
                states.emplace_back(result);
            }

            seed = rng.next();
        }
    }

    return states;
}

std::vector<SearcherState4> StaticSearcher4::searchMethod1(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe,
                                                           const StaticTemplate4 *staticTemplate) const
{
    std::vector<SearcherState4> states;
    std::array<u8, 6> ivs = { hp, atk, def, spa, spd, spe };
    const PersonalInfo *info = staticTemplate->getInfo();

    u32 seeds[6];
    int size = LCRNGReverse::recoverPokeRNGIV(hp, atk, def, spa, spd, spe, seeds, Method::Method1);
    for (int i = 0; i < size; i++)
    {
        PokeRNGR rng(seeds[i]);

        u32 pid;
        if (staticTemplate->getShiny() == Shiny::Always)
        {
            u16 low = 0;
            for (int j = 15; j > 2; j--)
            {
                low |= rng.nextUShort(2) << j;
            }

            u16 high = rng.nextUShort(8);
            low |= rng.nextUShort(8);
            high |= (low ^ tsv) & 0xfff8;

            pid = (high << 16) | low;
        }
        else
        {
            pid = rng.nextUShort() << 16;
            pid |= rng.nextUShort();

            if (staticTemplate->getShiny() == Shiny::Never)
            {
                while (isShiny(pid, tsv))
                {
                    pid = ARNG(pid).next();
                }
            }
        }

        u8 nature = pid % 25;
        if (!filter.compareNature(nature))
        {
            continue;
        }

        SearcherState4 state(rng.next(), pid, ivs, pid & 1, Utilities::getGender(pid, info), staticTemplate->getLevel(), nature,
                             Utilities::getShiny(pid, tsv), info);
        if (filter.compareState(static_cast<const SearcherState &>(state)))
        {
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<SearcherState4> StaticSearcher4::searchMethodJ(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe,
                                                           const StaticTemplate4 *staticTemplate) const
{
    std::vector<SearcherState4> states;
    std::array<u8, 6> ivs = { hp, atk, def, spa, spd, spe };
    const PersonalInfo *info = staticTemplate->getInfo();

    u32 seeds[6];
    int size = LCRNGReverse::recoverPokeRNGIV(hp, atk, def, spa, spd, spe, seeds, Method::Method1);
    for (int i = 0; i < size; i++)
    {
        PokeRNGR rng(seeds[i]);
        if (lead == Lead::CuteCharmF || lead == Lead::CuteCharmM)
        {
            u8 nature = rng.nextUShort<false>(25);
            if (rng.nextUShort<false>(3) != 0)
            {
                if (!filter.compareNature(nature))
                {
                    continue;
                }

                u32 pid = nature + buffer;
                SearcherState4 state(rng.next(), pid, ivs, pid & 1, Utilities::getGender(pid, info), staticTemplate->getLevel(), nature,
                                     Utilities::getShiny(pid, tsv), staticTemplate->getInfo());
                if (filter.compareState(static_cast<const SearcherState &>(state)))
                {
                    states.emplace_back(state);
                }
            }
        }
        else
        {
            u32 pid = rng.nextUShort() << 16;
            pid |= rng.nextUShort();

            u8 nature = pid % 25;
            if (!filter.compareNature(nature))
            {
                continue;
            }

            u8 huntNature;
            u16 nextRNG = rng.nextUShort();
            u16 nextRNG2 = rng.nextUShort();

            do
            {
                u32 seed[2];
                bool valid[2] = { false, false };

                if (lead == Lead::None)
                {
                    if ((nextRNG / 0xa3e) == nature)
                    {
                        seed[0] = rng.getSeed();
                        valid[0] = true;
                    }
                }
                else if (lead == Lead::Synchronize)
                {
                    if ((nextRNG >> 15) == 0)
                    {
                        seed[0] = rng.getSeed();
                        valid[0] = true;
                    }

                    if ((nextRNG2 >> 15) == 1 && (nextRNG / 0xa3e) == nature)
                    {
                        seed[1] = PokeRNGR(rng).next();
                        valid[1] = true;
                    }
                }

                for (int i = 0; i < 2; i++)
                {
                    if (valid[i])
                    {
                        SearcherState4 state(seed[i], pid, ivs, pid & 1, Utilities::getGender(pid, info), staticTemplate->getLevel(),
                                             nature, Utilities::getShiny(pid, tsv), info);
                        if (filter.compareState(static_cast<const SearcherState &>(state)))
                        {
                            states.emplace_back(state);
                        }
                    }
                }

                huntNature = static_cast<u32>((nextRNG << 16) | nextRNG2) % 25;
                nextRNG = rng.nextUShort();
                nextRNG2 = rng.nextUShort();
            } while (huntNature != nature);
        }
    }

    return states;
}

std::vector<SearcherState4> StaticSearcher4::searchMethodK(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe,
                                                           const StaticTemplate4 *staticTemplate) const
{
    std::vector<SearcherState4> states;
    std::array<u8, 6> ivs = { hp, atk, def, spa, spd, spe };
    const PersonalInfo *info = staticTemplate->getInfo();

    u32 seeds[6];
    int size = LCRNGReverse::recoverPokeRNGIV(hp, atk, def, spa, spd, spe, seeds, Method::Method1);
    for (int i = 0; i < size; i++)
    {
        PokeRNGR rng(seeds[i]);
        if (lead == Lead::CuteCharmF || lead == Lead::CuteCharmM)
        {
            u8 nature = rng.nextUShort(25);
            if (rng.nextUShort(3) != 0)
            {
                if (!filter.compareNature(nature))
                {
                    continue;
                }

                u32 pid = nature + buffer;
                SearcherState4 state(rng.next(), pid, ivs, pid & 1, Utilities::getGender(pid, info), staticTemplate->getLevel(), nature,
                                     Utilities::getShiny(pid, tsv), info);
                if (filter.compareState(static_cast<const SearcherState &>(state)))
                {
                    states.emplace_back(state);
                }
            }
        }
        else
        {
            u32 pid = rng.nextUShort() << 16;
            pid |= rng.nextUShort();

            u8 nature = pid % 25;
            if (!filter.compareNature(nature))
            {
                continue;
            }

            u8 huntNature;
            u16 nextRNG = rng.nextUShort();
            u16 nextRNG2 = rng.nextUShort();

            do
            {
                u32 seed[2];
                bool valid[2] = { false, false };

                if (lead == Lead::None)
                {
                    if ((nextRNG % 25) == nature)
                    {
                        seed[0] = rng.getSeed();
                        valid[0] = true;
                    }
                }
                else if (lead == Lead::Synchronize)
                {
                    if ((nextRNG % 2) == 0)
                    {
                        seed[0] = rng.getSeed();
                        valid[0] = true;
                    }

                    if ((nextRNG2 % 2) == 1 && (nextRNG % 25) == nature)
                    {
                        seed[1] = PokeRNGR(rng).next();
                        valid[1] = true;
                    }
                }

                for (int i = 0; i < 2; i++)
                {
                    if (valid[i])
                    {
                        SearcherState4 state(seed[i], pid, ivs, pid & 1, Utilities::getGender(pid, info), staticTemplate->getLevel(),
                                             nature, Utilities::getShiny(pid, tsv), info);
                        if (filter.compareState(static_cast<const SearcherState &>(state)))
                        {
                            states.emplace_back(state);
                        }
                    }
                }

                huntNature = static_cast<u32>((nextRNG << 16) | nextRNG2) % 25;
                nextRNG = rng.nextUShort();
                nextRNG2 = rng.nextUShort();
            } while (huntNature != nature);
        }
    }

    return states;
}
