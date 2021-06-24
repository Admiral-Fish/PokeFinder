/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "WildSearcher4.hpp"
#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/Util/EncounterSlot.hpp>

constexpr u8 genderThreshHolds[5] = { 0, 0x96, 0xC8, 0x4B, 0x32 };

WildSearcher4::WildSearcher4(u16 tid, u16 sid, u8 genderRatio, Method method, const StateFilter &filter) :
    WildSearcher(tid, sid, genderRatio, method, filter), cache(method), searching(false), progress(0)
{
}

void WildSearcher4::setEncounterArea(const EncounterArea4 &encounterArea)
{
    this->encounterArea = encounterArea;
}

void WildSearcher4::setDelay(u32 minDelay, u32 maxDelay)
{
    this->minDelay = minDelay;
    this->maxDelay = maxDelay;
}

void WildSearcher4::setState(u32 minAdvance, u32 maxAdvance)
{
    this->minAdvance = minAdvance;
    this->maxAdvance = maxAdvance;
}

void WildSearcher4::startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max)
{
    searching = true;

    thresh = encounter == Encounter::OldRod ? 25 : encounter == Encounter::GoodRod ? 50 : encounter == Encounter::SuperRod ? 75 : 0;
    suctionCupThresh
        = encounter == Encounter::OldRod ? 90 : encounter == Encounter::GoodRod ? 100 : encounter == Encounter::SuperRod ? 100 : 0;
    rock = encounterArea.getEncounterRate();

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

void WildSearcher4::cancelSearch()
{
    searching = false;
}

std::vector<WildState> WildSearcher4::getResults()
{
    std::lock_guard<std::mutex> guard(mutex);
    auto data = std::move(results);
    return data;
}

int WildSearcher4::getProgress() const
{
    return progress;
}

std::vector<WildState> WildSearcher4::search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    std::vector<WildState> states;

    if (method == Method::MethodJ)
    {
        states = searchMethodJ(hp, atk, def, spa, spd, spe);
    }
    else if (method == Method::MethodK)
    {
        states = searchMethodK(hp, atk, def, spa, spd, spe);
    }
    else if (method == Method::ChainedShiny)
    {
        states = searchChainedShiny(hp, atk, def, spa, spd, spe);
    }

    return searchInitialSeeds(states);
}

std::vector<WildState> WildSearcher4::searchMethodJ(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    std::vector<WildState> states;

    WildState state;
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

std::vector<WildState> WildSearcher4::searchMethodK(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    std::vector<WildState> states;

    WildState state;
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

            if (lead == Lead::CuteCharm)
            {
                auto results = cuteCharmMethodK(state, seed);
                states.insert(states.begin(), results.begin(), results.end());
            }
            else
            {
                if (lead == Lead::Search)
                {
                    auto results = cuteCharmMethodK(state, seed);
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

                if (lead == Lead::None || lead == Lead::SuctionCups)
                {
                    auto results = normalMethodK(state, seed);
                    states.insert(states.begin(), results.begin(), results.end());
                }
                else if (lead == Lead::Synchronize)
                {
                    auto results = synchMethodK(state, seed);
                    states.insert(states.begin(), results.begin(), results.end());
                }
                else if (lead == Lead::Search)
                {
                    auto results = normalMethodK(state, seed);
                    states.insert(states.begin(), results.begin(), results.end());
                    results = synchMethodK(state, seed);
                    states.insert(states.begin(), results.begin(), results.end());
                }
            }
        }
    }

    return states;
}

std::vector<WildState> WildSearcher4::searchChainedShiny(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    std::vector<WildState> states;

    WildState state;
    state.setIVs(hp, atk, def, spa, spd, spe);
    state.calculateHiddenPower();

    if (!filter.compareHiddenPower(state))
    {
        return states;
    }

    auto seeds = cache.recoverLower16BitsIV(hp, atk, def, spa, spd, spe);
    for (const auto &seed : seeds)
    {
        PokeRNGR rng(seed);

        u16 low = 0;
        for (int i = 15; i > 2; i--)
        {
            low |= (rng.nextUShort() & 1) << i;
        }

        u16 high = rng.nextUShort() & 7;
        low |= rng.nextUShort() & 7;
        high |= (tid ^ sid ^ low) & 0xfff8;

        state.setPID(high, low);
        state.setAbility(low & 1);
        state.setGender(low & 255, genderRatio);
        state.setNature(state.getPID() % 25);
        state.setShiny<8>(tsv, high ^ low);

        if (filter.comparePID(state))
        {
            state.setSeed(rng.next());
            states.emplace_back(state);

            // Sister spread shares PID
            state.setSeed(state.getSeed() ^ 0x80000000);
            states.emplace_back(state);
        }
    }

    return states;
}

std::vector<WildState> WildSearcher4::searchInitialSeeds(const std::vector<WildState> &results) const
{
    std::vector<WildState> states;

    for (WildState result : results)
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

std::vector<WildState> WildSearcher4::normalMethodJ(WildState state, u32 seed) const
{
    std::vector<WildState> states;
    state.setLead(Lead::None);

    PokeRNGR rng(seed);
    u32 pid;
    u16 nextRNG = seed >> 16;
    u16 nextRNG2 = rng.nextUShort();

    do
    {
        if ((nextRNG / 0xa3e) == state.getNature())
        {
            if (encounterMethodJ(state, rng.getSeed()))
            {
                states.emplace_back(state);
            }
        }

        pid = static_cast<u32>((nextRNG << 16) | nextRNG2);
        nextRNG = rng.nextUShort();
        nextRNG2 = rng.nextUShort();
    } while (pid % 25 != state.getNature());

    return states;
}

std::vector<WildState> WildSearcher4::synchMethodJ(WildState state, u32 seed) const
{
    std::vector<WildState> states;
    state.setLead(Lead::Synchronize);

    PokeRNGR rng(seed);
    u32 pid;
    u16 nextRNG = seed >> 16;
    u16 nextRNG2 = rng.nextUShort();

    do
    {
        if ((nextRNG >> 15) == 0)
        {
            if (encounterMethodJ(state, rng.getSeed()))
            {
                states.emplace_back(state);
            }
        }
        else if ((nextRNG2 >> 15) == 1 && (nextRNG / 0xa3e) == state.getNature())
        {
            if (encounterMethodJ(state, rng.getSeed() * 0xeeb9eb65 + 0xa3561a1))
            {
                states.emplace_back(state);
            }
        }

        pid = static_cast<u32>((nextRNG << 16) | nextRNG2);
        nextRNG = rng.nextUShort();
        nextRNG2 = rng.nextUShort();
    } while (pid % 25 != state.getNature());

    return states;
}

std::vector<WildState> WildSearcher4::cuteCharmMethodJ(WildState state, u32 seed) const
{
    std::vector<WildState> states;

    PokeRNGR rng(seed);
    u16 high = state.getPID() >> 16;
    u16 low = state.getPID() & 0xffff;

    if ((low / 0x5556) != 0)
    {
        if (encounterMethodJ(state, seed))
        {
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
    }

    return states;
}

bool WildSearcher4::encounterMethodJ(WildState &state, u32 seed) const
{
    PokeRNGR rng(seed);

    switch (encounter)
    {
    case Encounter::Grass:
        state.setEncounterSlot(EncounterSlot::jSlot(seed >> 16, encounter));
        state.setLevel(encounterArea.calcLevel(state.getEncounterSlot()));
        state.setSeed(rng.next());
        break;
    case Encounter::Surfing:
        state.setEncounterSlot(EncounterSlot::jSlot(rng.nextUShort(), encounter));
        state.setLevel(encounterArea.calcLevel(state.getEncounterSlot(), seed >> 16));
        state.setSeed(rng.next());
        break;
    case Encounter::OldRod:
    case Encounter::GoodRod:
    case Encounter::SuperRod:
    {
        u16 slot = rng.nextUShort();
        if ((rng.nextUShort() / 656) < thresh)
        {
            state.setEncounterSlot(EncounterSlot::jSlot(slot, encounter));
            state.setLevel(encounterArea.calcLevel(state.getEncounterSlot(), seed >> 16));
            state.setSeed(rng.next());
        }
        else
        {
            return false;
        }
        break;
    }
    default:
        break;
    }

    return filter.compareEncounterSlot(state);
}

std::vector<WildState> WildSearcher4::normalMethodK(WildState state, u32 seed) const
{
    std::vector<WildState> states;

    PokeRNGR rng(seed);
    u32 pid;
    u16 nextRNG = seed >> 16;
    u16 nextRNG2 = rng.nextUShort();

    do
    {
        if ((nextRNG % 25) == state.getNature())
        {
            state.setLead(Lead::None);
            if (encounterMethodK(state, rng.getSeed()))
            {
                states.emplace_back(state);
            }
        }

        pid = static_cast<u32>((nextRNG << 16) | nextRNG2);
        nextRNG = rng.nextUShort();
        nextRNG2 = rng.nextUShort();
    } while (pid % 25 != state.getNature());

    return states;
}

std::vector<WildState> WildSearcher4::synchMethodK(WildState state, u32 seed) const
{
    std::vector<WildState> states;
    state.setLead(Lead::Synchronize);

    PokeRNGR rng(seed);
    u32 pid;
    u16 nextRNG = seed >> 16;
    u16 nextRNG2 = rng.nextUShort();

    do
    {
        if ((nextRNG & 1) == 0)
        {
            if (encounterMethodK(state, rng.getSeed()))
            {
                states.emplace_back(state);
            }
        }
        else if ((nextRNG2 & 1) == 1 && (nextRNG % 25) == state.getNature())
        {
            if (encounterMethodK(state, rng.getSeed() * 0xeeb9eb65 + 0xa3561a1))
            {
                states.emplace_back(state);
            }
        }

        pid = static_cast<u32>((nextRNG << 16) | nextRNG2);
        nextRNG = rng.nextUShort();
        nextRNG2 = rng.nextUShort();
    } while (pid % 25 != state.getNature());

    return states;
}

std::vector<WildState> WildSearcher4::cuteCharmMethodK(WildState state, u32 seed) const
{
    std::vector<WildState> states;

    PokeRNGR rng(seed);
    u16 high = state.getPID() >> 16;
    u16 low = state.getPID() & 0xffff;

    if ((low % 3) != 0)
    {
        if (encounterMethodK(state, seed))
        {
            u8 choppedPID = high % 25;
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
    }

    return states;
}

bool WildSearcher4::encounterMethodK(WildState &state, u32 seed) const
{
    PokeRNGR rng(seed);

    switch (encounter)
    {
    case Encounter::Grass:
        state.setEncounterSlot(EncounterSlot::kSlot(seed >> 16, encounter));
        state.setLevel(encounterArea.calcLevel(state.getEncounterSlot()));
        state.setSeed(rng.next());
        break;
    case Encounter::Surfing:
        state.setEncounterSlot(EncounterSlot::kSlot(rng.nextUShort(), encounter));
        state.setLevel(encounterArea.calcLevel(state.getEncounterSlot(), seed >> 16));
        state.setSeed(rng.next());
        break;
    case Encounter::OldRod:
    case Encounter::GoodRod:
    case Encounter::SuperRod:
    {
        u16 slot = rng.nextUShort();
        u16 nibble = rng.nextUShort() % 100;
        if (nibble < thresh)
        {
            state.setEncounterSlot(EncounterSlot::kSlot(slot, encounter));
            state.setLevel(encounterArea.calcLevel(state.getEncounterSlot()));
            state.setSeed(rng.next());
        }
        else if (nibble < suctionCupThresh && (lead == Lead::SuctionCups || lead == Lead::Search))
        {
            state.setLead(Lead::SuctionCups);
            state.setEncounterSlot(EncounterSlot::kSlot(slot, encounter));
            state.setLevel(encounterArea.calcLevel(state.getEncounterSlot()));
            state.setSeed(rng.next());
        }
        else
        {
            return false;
        }
    }
    break;
    case Encounter::RockSmash:
    {
        u16 slot = rng.nextUShort();
        if ((rng.nextUShort() % 100) < rock)
        {
            state.setEncounterSlot(EncounterSlot::kSlot(slot, encounter));
            state.setLevel(encounterArea.calcLevel(state.getEncounterSlot(), seed >> 16));
            rng.advance(1); // Blank (maybe item)
            state.setSeed(rng.next());
        }
        else
        {
            return false;
        }
    }
    break;
    default:
        break;
    }

    return filter.compareEncounterSlot(state);
}
