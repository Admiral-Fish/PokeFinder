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

void WildSearcher4::startSearch(const QVector<u8> &min, const QVector<u8> &max)
{
    searching = true;

    thresh = encounter == Encounter::OldRod ? 25 : encounter == Encounter::GoodRod ? 50 : encounter == Encounter::SuperRod ? 75 : 0;
    suctionCupThresh
        = encounter == Encounter::OldRod ? 90 : encounter == Encounter::GoodRod ? 100 : encounter == Encounter::SuperRod ? 100 : 0;
    rock = encounterArea.getEncounterRate();

    for (u8 hp = min.at(0); hp <= max.at(0); hp++)
    {
        for (u8 atk = min.at(1); atk <= max.at(1); atk++)
        {
            for (u8 def = min.at(2); def <= max.at(2); def++)
            {
                for (u8 spa = min.at(3); spa <= max.at(3); spa++)
                {
                    for (u8 spd = min.at(4); spd <= max.at(4); spd++)
                    {
                        for (u8 spe = min.at(5); spe <= max.at(5); spe++)
                        {
                            if (!searching)
                            {
                                return;
                            }

                            auto states = search(hp, atk, def, spa, spd, spe);

                            std::lock_guard<std::mutex> guard(mutex);
                            results.append(states);
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

QVector<WildState> WildSearcher4::getResults()
{
    std::lock_guard<std::mutex> guard(mutex);
    auto data(results);
    results.clear();
    return data;
}

int WildSearcher4::getProgress() const
{
    return progress;
}

QVector<WildState> WildSearcher4::search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    QVector<WildState> states;

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

QVector<WildState> WildSearcher4::searchMethodJ(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    QVector<WildState> states;

    WildState currentState;
    currentState.setIVs(hp, atk, def, spa, spd, spe);
    currentState.calculateHiddenPower();

    if (!filter.compareHiddenPower(currentState))
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

        currentState.setPID(high, low);

        for (const bool flag : { false, true })
        {
            if (flag)
            {
                currentState.setPID(currentState.getPID() ^ 0x80008000);
                seed ^= 0x80000000;
            }

            if (lead == Lead::CuteCharm)
            {
                states.append(cuteCharmMethodJ(currentState, seed));
            }
            else
            {
                if (lead == Lead::Search)
                {
                    states.append(cuteCharmMethodJ(currentState, seed));
                }

                currentState.setAbility(low & 1);
                currentState.setGender(low & 255, genderRatio);
                currentState.setNature(currentState.getPID() % 25);
                currentState.setShiny(tsv, high ^ low, 8);

                if (!filter.comparePID(currentState))
                {
                    continue;
                }

                if (lead == Lead::None)
                {
                    states.append(normalMethodJ(currentState, seed));
                }
                else if (lead == Lead::Synchronize)
                {
                    states.append(synchMethodJ(currentState, seed));
                }
                else if (lead == Lead::Search)
                {
                    states.append(normalMethodJ(currentState, seed));
                    states.append(synchMethodJ(currentState, seed));
                }
            }
        }
    }

    return states;
}

QVector<WildState> WildSearcher4::searchMethodK(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    QVector<WildState> states;

    WildState currentState;
    currentState.setIVs(hp, atk, def, spa, spd, spe);
    currentState.calculateHiddenPower();

    if (!filter.compareHiddenPower(currentState))
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

        currentState.setPID(high, low);

        for (const bool flag : { false, true })
        {
            if (flag)
            {
                currentState.setPID(currentState.getPID() ^ 0x80008000);
                seed ^= 0x80000000;
            }

            if (lead == Lead::CuteCharm)
            {
                states.append(cuteCharmMethodK(currentState, seed));
            }
            else
            {
                if (lead == Lead::Search)
                {
                    states.append(cuteCharmMethodK(currentState, seed));
                }

                currentState.setAbility(low & 1);
                currentState.setGender(low & 255, genderRatio);
                currentState.setNature(currentState.getPID() % 25);
                currentState.setShiny(tsv, high ^ low, 8);

                if (!filter.comparePID(currentState))
                {
                    continue;
                }

                if (lead == Lead::None)
                {
                    states.append(normalMethodK(currentState, seed));
                }
                else if (lead == Lead::Synchronize)
                {
                    states.append(synchMethodK(currentState, seed));
                }
                else if (lead == Lead::Search)
                {
                    states.append(normalMethodK(currentState, seed));
                    states.append(synchMethodK(currentState, seed));
                }
            }
        }
    }

    return states;
}

QVector<WildState> WildSearcher4::searchChainedShiny(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    QVector<WildState> states;

    WildState currentState;
    currentState.setIVs(hp, atk, def, spa, spd, spe);
    currentState.calculateHiddenPower();

    if (!filter.compareHiddenPower(currentState))
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

        currentState.setPID(high, low);
        currentState.setAbility(low & 1);
        currentState.setGender(low & 255, genderRatio);
        currentState.setNature(currentState.getPID() % 25);
        currentState.setShiny(true);

        if (filter.comparePID(currentState))
        {
            rng.next();
            currentState.setSeed(rng.next());
            states.append(currentState);

            // Sister spread shares PID
            currentState.setSeed(currentState.getSeed() ^ 0x80000000);
            states.append(currentState);
        }
    }

    return states;
}

QVector<WildState> WildSearcher4::searchInitialSeeds(const QVector<WildState> &results) const
{
    QVector<WildState> states;

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
                result.setAdvance(cnt);
                states.append(result);
            }

            test = rng.next();
        }
    }

    return states;
}

QVector<WildState> WildSearcher4::normalMethodJ(WildState currentState, u32 seed) const
{
    QVector<WildState> states;
    currentState.setLead(Lead::None);

    PokeRNGR rng(seed);
    u32 pid;
    u16 nextRNG = seed >> 16;
    u16 nextRNG2 = rng.nextUShort();

    do
    {
        if ((nextRNG / 0xa3e) == currentState.getNature())
        {
            if (encounterMethodJ(currentState, rng.getSeed()))
            {
                states.append(currentState);
            }
        }

        pid = static_cast<u32>((nextRNG << 16) | nextRNG2);
        nextRNG = rng.nextUShort();
        nextRNG2 = rng.nextUShort();
    } while (pid % 25 != currentState.getNature());

    return states;
}

QVector<WildState> WildSearcher4::synchMethodJ(WildState currentState, u32 seed) const
{
    QVector<WildState> states;
    currentState.setLead(Lead::Synchronize);

    PokeRNGR rng(seed);
    u32 pid;
    u16 nextRNG = seed >> 16;
    u16 nextRNG2 = rng.nextUShort();

    do
    {
        if ((nextRNG >> 15) == 0)
        {
            if (encounterMethodJ(currentState, rng.getSeed()))
            {
                states.append(currentState);
            }
        }
        else if ((nextRNG2 >> 15) == 1 && (nextRNG / 0xa3e) == currentState.getNature())
        {
            if (encounterMethodJ(currentState, rng.getSeed() * 0xeeb9eb65 + 0xa3561a1))
            {
                states.append(currentState);
            }
        }

        pid = static_cast<u32>((nextRNG << 16) | nextRNG2);
        nextRNG = rng.nextUShort();
        nextRNG2 = rng.nextUShort();
    } while (pid % 25 != currentState.getNature());

    return states;
}

QVector<WildState> WildSearcher4::cuteCharmMethodJ(WildState currentState, u32 seed) const
{
    QVector<WildState> states;

    PokeRNGR rng(seed);
    u16 high = currentState.getPID() >> 16;
    u16 low = currentState.getPID() & 0xffff;

    if ((low / 0x5556) != 0)
    {
        if (encounterMethodJ(currentState, seed))
        {
            u8 choppedPID = high / 0xa3e;
            u8 nature = choppedPID % 25;
            for (const u8 buffer : genderThreshHolds)
            {
                if (buffer == 0)
                {
                    currentState.setLead(Lead::CuteCharmFemale);
                }
                else if (buffer == 0x96)
                {
                    currentState.setLead(Lead::CuteCharm50M);
                }
                else if (buffer == 0xc8)
                {
                    currentState.setLead(Lead::CuteCharm25M);
                }
                else if (buffer == 0x4b)
                {
                    currentState.setLead(Lead::CuteCharm75M);
                }
                else if (buffer == 0x32)
                {
                    currentState.setLead(Lead::CuteCharm875M);
                }

                u8 pid = choppedPID + buffer;
                currentState.setPID(pid);
                currentState.setAbility(pid & 1);
                currentState.setGender(pid, genderRatio);
                currentState.setNature(nature);
                currentState.setShiny(tsv, pid, 8);

                if (filter.comparePID(currentState))
                {
                    states.append(currentState);
                }
            }
        }
    }

    return states;
}

bool WildSearcher4::encounterMethodJ(WildState &currentState, u32 seed) const
{
    PokeRNGR rng(seed);

    switch (encounter)
    {
    case Encounter::Grass:
        currentState.setEncounterSlot(EncounterSlot::jSlot(seed >> 16, encounter));
        currentState.setLevel(encounterArea.calcLevel(currentState.getEncounterSlot()));
        currentState.setSeed(rng.next());
        break;
    case Encounter::Surfing:
        currentState.setEncounterSlot(EncounterSlot::jSlot(rng.nextUShort(), encounter));
        currentState.setLevel(encounterArea.calcLevel(currentState.getEncounterSlot(), seed >> 16));
        currentState.setSeed(rng.next());
        break;
    case Encounter::OldRod:
    case Encounter::GoodRod:
    case Encounter::SuperRod:
    {
        u16 slot = rng.nextUShort();
        if ((rng.nextUShort() / 656) < thresh)
        {
            currentState.setEncounterSlot(EncounterSlot::jSlot(slot, encounter));
            currentState.setLevel(encounterArea.calcLevel(currentState.getEncounterSlot(), seed >> 16));
            currentState.setSeed(rng.next());
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

    return filter.compareEncounterSlot(currentState);
}

QVector<WildState> WildSearcher4::normalMethodK(WildState currentState, u32 seed) const
{
    QVector<WildState> states;

    PokeRNGR rng(seed);
    u32 pid;
    u16 nextRNG = seed >> 16;
    u16 nextRNG2 = rng.nextUShort();

    do
    {
        if ((nextRNG % 25) == currentState.getNature())
        {
            currentState.setLead(Lead::None);
            if (encounterMethodK(currentState, rng.getSeed()))
            {
                states.append(currentState);
            }
        }

        pid = static_cast<u32>((nextRNG << 16) | nextRNG2);
        nextRNG = rng.nextUShort();
        nextRNG2 = rng.nextUShort();
    } while (pid % 25 != currentState.getNature());

    return states;
}

QVector<WildState> WildSearcher4::synchMethodK(WildState currentState, u32 seed) const
{
    QVector<WildState> states;
    currentState.setLead(Lead::Synchronize);

    PokeRNGR rng(seed);
    u32 pid;
    u16 nextRNG = seed >> 16;
    u16 nextRNG2 = rng.nextUShort();

    do
    {
        if ((nextRNG & 1) == 0)
        {
            if (encounterMethodK(currentState, rng.getSeed()))
            {
                states.append(currentState);
            }
        }
        else if ((nextRNG2 & 1) == 1 && (nextRNG % 25) == currentState.getNature())
        {
            if (encounterMethodK(currentState, rng.getSeed() * 0xeeb9eb65 + 0xa3561a1))
            {
                states.append(currentState);
            }
        }

        pid = static_cast<u32>((nextRNG << 16) | nextRNG2);
        nextRNG = rng.nextUShort();
        nextRNG2 = rng.nextUShort();
    } while (pid % 25 != currentState.getNature());

    return states;
}

QVector<WildState> WildSearcher4::cuteCharmMethodK(WildState currentState, u32 seed) const
{
    QVector<WildState> states;

    PokeRNGR rng(seed);
    u16 high = currentState.getPID() >> 16;
    u16 low = currentState.getPID() & 0xffff;

    if ((low % 3) != 0)
    {
        if (encounterMethodK(currentState, seed))
        {
            u8 choppedPID = high % 25;
            u8 nature = choppedPID % 25;
            for (const u8 buffer : genderThreshHolds)
            {
                if (buffer == 0)
                {
                    currentState.setLead(Lead::CuteCharmFemale);
                }
                else if (buffer == 0x96)
                {
                    currentState.setLead(Lead::CuteCharm50M);
                }
                else if (buffer == 0xc8)
                {
                    currentState.setLead(Lead::CuteCharm25M);
                }
                else if (buffer == 0x4b)
                {
                    currentState.setLead(Lead::CuteCharm75M);
                }
                else if (buffer == 0x32)
                {
                    currentState.setLead(Lead::CuteCharm875M);
                }

                u8 pid = choppedPID + buffer;
                currentState.setPID(pid);
                currentState.setAbility(pid & 1);
                currentState.setGender(pid, genderRatio);
                currentState.setNature(nature);
                currentState.setShiny(tsv, pid, 8);

                if (filter.comparePID(currentState))
                {
                    states.append(currentState);
                }
            }
        }
    }

    return states;
}

bool WildSearcher4::encounterMethodK(WildState &currentState, u32 seed) const
{
    PokeRNGR rng(seed);

    switch (encounter)
    {
    case Encounter::Grass:
        currentState.setEncounterSlot(EncounterSlot::kSlot(seed >> 16, encounter));
        currentState.setLevel(encounterArea.calcLevel(currentState.getEncounterSlot()));
        currentState.setSeed(rng.next());
        break;
    case Encounter::Surfing:
        currentState.setEncounterSlot(EncounterSlot::kSlot(rng.nextUShort(), encounter));
        currentState.setLevel(encounterArea.calcLevel(currentState.getEncounterSlot(), seed >> 16));
        currentState.setSeed(rng.next());
        break;
    case Encounter::OldRod:
    case Encounter::GoodRod:
    case Encounter::SuperRod:
    {
        u16 slot = rng.nextUShort();
        u16 nibble = rng.nextUShort() % 100;
        if (nibble < thresh)
        {
            currentState.setEncounterSlot(EncounterSlot::kSlot(slot, encounter));
            currentState.setLevel(encounterArea.calcLevel(currentState.getEncounterSlot()));
            currentState.setSeed(rng.next());
        }
        else if (nibble < suctionCupThresh && currentState.getLead() == Lead::None)
        {
            currentState.setLead(Lead::SuctionCups);
            currentState.setEncounterSlot(EncounterSlot::kSlot(slot, encounter));
            currentState.setLevel(encounterArea.calcLevel(currentState.getEncounterSlot()));
            currentState.setSeed(rng.next());
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
            currentState.setEncounterSlot(EncounterSlot::kSlot(slot, encounter));
            currentState.setLevel(encounterArea.calcLevel(currentState.getEncounterSlot(), seed >> 16));
            rng.advance(1); // Blank (maybe item)
            currentState.setSeed(rng.next());
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

    return filter.compareEncounterSlot(currentState);
}
