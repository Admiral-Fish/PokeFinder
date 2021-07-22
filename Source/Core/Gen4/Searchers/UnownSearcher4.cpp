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

#include "UnownSearcher4.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/Util/EncounterSlot.hpp>

constexpr u8 genderThreshHolds[5] = { 0, 0x96, 0xC8, 0x4B, 0x32 };

UnownSearcher4::UnownSearcher4(u16 tid, u16 sid, u8 genderRatio, Method method, const StateFilter &filter) :
    WildSearcher(tid, sid, genderRatio, method, filter), cache(method), searching(false), progress(0)
{
}

void UnownSearcher4::setEncounterArea(const u8 encounterArea)
{
    this->encounterArea = encounterArea;
}

void UnownSearcher4::setDelay(u32 minDelay, u32 maxDelay)
{
    this->minDelay = minDelay;
    this->maxDelay = maxDelay;
}

void UnownSearcher4::setState(u32 minAdvance, u32 maxAdvance)
{
    this->minAdvance = minAdvance;
    this->maxAdvance = maxAdvance;
}

void UnownSearcher4::startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max)
{
    searching = true;
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

                            auto states = search(hp, atk, def, spa, spd, spe);

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

void UnownSearcher4::cancelSearch()
{
    searching = false;
}

std::vector<UnownState4> UnownSearcher4::getResults()
{
    std::lock_guard<std::mutex> guard(mutex);
    auto data = std::move(results);
    return data;
}

int UnownSearcher4::getProgress() const
{
    return progress;
}

std::vector<UnownState4> UnownSearcher4::search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    std::vector<UnownState4> states;

    states = searchMethodJ(hp, atk, def, spa, spd, spe);

    return searchInitialSeeds(states);
}

std::vector<UnownState4> UnownSearcher4::searchMethodJ(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    std::vector<UnownState4> states;

    UnownState4 state;
    state.setIVs(hp, atk, def, spa, spd, spe);
    state.calculateHiddenPower();

    if (!filter.compareHiddenPower(state))
    {
        return states;
    }

    auto seeds = cache.recoverLower16BitsIV(hp, atk, def, spa, spd, spe);
    for (const u32 val : seeds)
    {
        PokeRNGR rng(val);
        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();
        u32 seed = rng.next();
        
        state.setPID(high, low);

        for (const bool flag : { false, true })
        {
            if (flag)
            {
                state.setPID(state.getPID() ^ 0x80008000);
                seed ^= 0x80000000;
            }
            PokeRNG go(seed);
            go.advance(5);
            if (encounterArea == 0) {
                state.setLetterIndex4(go.nextUShort()%20);
            }
            else
            {
                state.setLetterIndex4b(go.nextUShort()%2);
            }
            if (!filter.compareLetter(state))
            {
                continue;
            }

            if (lead == Lead::CuteCharm)
            {
                auto results = cuteCharmMethodJ(state, seed);
                states.insert(states.begin(), results.begin(), results.end());
            }
            else
            {
                if (lead == Lead::Search)
                {
                    auto results = cuteCharmMethodJ(state, seed);
                    states.insert(states.begin(), results.begin(), results.end());
                }

                state.setAbility(low & 1);
                state.setGender(low & 255, genderRatio);
                state.setNature(state.getPID() % 25);
                state.setShiny<8>(tsv, high ^ low);

                if (!filter.comparePID(state))
                {
                    continue;
                }

                if (lead == Lead::None)
                {
                    auto results = normalMethodJ(state, seed);
                    states.insert(states.begin(), results.begin(), results.end());
                }
                else if (lead == Lead::Synchronize)
                {
                    auto results = synchMethodJ(state, seed);
                    states.insert(states.begin(), results.begin(), results.end());
                }
                else if (lead == Lead::Search)
                {
                    auto results = normalMethodJ(state, seed);
                    states.insert(states.begin(), results.begin(), results.end());
                    results = synchMethodJ(state, seed);
                    states.insert(states.begin(), results.begin(), results.end());
                }
            }
        }
    }

    return states;
}

std::vector<UnownState4> UnownSearcher4::searchInitialSeeds(const std::vector<UnownState4> &results) const
{
    std::vector<UnownState4> states;

    for (UnownState4 result : results)
    {
        PokeRNGR rng(result.getSeed());
        rng.advance(minAdvance);

        u32 test = rng.getSeed();

        for (u32 cnt = minAdvance; cnt <= maxAdvance; cnt++)
        {
            u8 hour = (test >> 16) & 0xFF;
            u16 delay = test & 0xFFFF;

            // Check if seed matches a valid gen 4 format
            if (hour < 24 && delay >= minDelay && delay <= maxDelay)
            {
                result.setSeed(test);
                result.setAdvances(cnt);
                states.emplace_back(result);
            }

            test = rng.next();
        }
    }

    return states;
}

std::vector<UnownState4> UnownSearcher4::normalMethodJ(UnownState4 state, u32 seed) const
{
    std::vector<UnownState4> states;
    state.setLead(Lead::None);

    PokeRNGR rng(seed);
    u32 pid;
    u16 nextRNG = seed >> 16;
    u16 nextRNG2 = rng.nextUShort();

    do
    {
        if ((nextRNG / 0xa3e) == state.getNature())
        {
            encounterMethodJ(state, rng.getSeed());
            states.emplace_back(state);
        }

        pid = static_cast<u32>((nextRNG << 16) | nextRNG2);
        nextRNG = rng.nextUShort();
        nextRNG2 = rng.nextUShort();
    } while (pid % 25 != state.getNature());

    return states;
}

std::vector<UnownState4> UnownSearcher4::synchMethodJ(UnownState4 state, u32 seed) const
{
    std::vector<UnownState4> states;
    state.setLead(Lead::Synchronize);

    PokeRNGR rng(seed);
    u32 pid;
    u16 nextRNG = seed >> 16;
    u16 nextRNG2 = rng.nextUShort();

    do
    {
        if ((nextRNG >> 15) == 0)
        {
            encounterMethodJ(state, rng.getSeed());
            states.emplace_back(state);
        }
        else if ((nextRNG2 >> 15) == 1 && (nextRNG / 0xa3e) == state.getNature())
        {
            encounterMethodJ(state, rng.getSeed());
            states.emplace_back(state);
        }

        pid = static_cast<u32>((nextRNG << 16) | nextRNG2);
        nextRNG = rng.nextUShort();
        nextRNG2 = rng.nextUShort();
    } while (pid % 25 != state.getNature());

    return states;
}

std::vector<UnownState4> UnownSearcher4::cuteCharmMethodJ(UnownState4 state, u32 seed) const
{
    std::vector<UnownState4> states;

    PokeRNGR rng(seed);
    u16 high = state.getPID() >> 16;
    u16 low = state.getPID() & 0xffff;

    if ((low / 0x5556) != 0)
    {
        encounterMethodJ(state, rng.getSeed());
        u8 choppedPID = high / 0xa3e;
        u8 nature = choppedPID % 25;
        for (const u8 buffer : genderThreshHolds)
        {
            if (buffer == 0)
            {
                state.setLead(Lead::CuteCharmFemale);
            }
            else if (buffer == 0x96)
            {
                state.setLead(Lead::CuteCharm50M);
            }
            else if (buffer == 0xc8)
            {
                state.setLead(Lead::CuteCharm25M);
            }
            else if (buffer == 0x4b)
            {
                state.setLead(Lead::CuteCharm75M);
            }
            else if (buffer == 0x32)
            {
                state.setLead(Lead::CuteCharm875M);
            }

            u8 pid = choppedPID + buffer;
            state.setPID(pid);
            state.setAbility(pid & 1);
            state.setGender(pid, genderRatio);
            state.setNature(nature);
            state.setShiny<8>(tsv, pid);

            if (filter.comparePID(state))
            {
                states.emplace_back(state);
            }
        }
    }

    return states;
}

bool UnownSearcher4::encounterMethodJ(UnownState4 &state, u32 seed) const
{
    PokeRNGR rng(seed);
    state.setSeed(rng.next());
    return true;
}
