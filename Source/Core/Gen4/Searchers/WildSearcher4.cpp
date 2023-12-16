/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Gen4/States/WildState4.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/RNG/LCRNGReverse.hpp>
#include <Core/Util/EncounterSlot.hpp>
#include <Core/Util/Utilities.hpp>

static u16 getItem(u8 rand, Lead lead, const PersonalInfo *info)
{
    constexpr u8 ItemTableRange[2][2] = { { 45, 95 }, { 20, 80 } };

    if (info->getItem(0) == info->getItem(1) && info->getItem(0) != 0)
    {
        return info->getItem(0);
    }
    else if (rand < ItemTableRange[lead == Lead::CompoundEyes ? 1 : 0][0])
    {
        return 0;
    }
    else if (rand < ItemTableRange[lead == Lead::CompoundEyes ? 1 : 0][1])
    {
        return info->getItem(0);
    }
    else
    {
        return info->getItem(1);
    }
}

WildSearcher4::WildSearcher4(u32 minAdvance, u32 maxAdvance, u32 minDelay, u32 maxDelay, Method method, Lead lead, bool shiny,
                             const EncounterArea4 &area, const Profile4 &profile, const WildStateFilter &filter) :
    WildSearcher(method, lead, area, profile, filter),
    modifiedSlots(area.getSlots(lead)),
    maxAdvance(maxAdvance),
    minAdvance(minAdvance),
    maxDelay(maxDelay),
    minDelay(minDelay),
    thresh(area.getRate()),
    safari(area.safariZone(profile.getVersion())),
    shiny(shiny)
{
    if ((lead == Lead::SuctionCups
         && (area.getEncounter() == Encounter::OldRod || area.getEncounter() == Encounter::GoodRod
             || area.getEncounter() == Encounter::SuperRod))
        || (lead == Lead::ArenaTrap && area.getEncounter() == Encounter::RockSmash))
    {
        thresh *= 2;
    }
}

void WildSearcher4::startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max, u8 index)
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

                            auto states = search(hp, atk, def, spa, spd, spe, index);

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

std::vector<WildSearcherState4> WildSearcher4::search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 index) const
{
    std::vector<WildSearcherState4> states;

    if (method == Method::MethodJ)
    {
        states = searchMethodJ(hp, atk, def, spa, spd, spe);
    }
    else if (method == Method::MethodK)
    {
        states = searchMethodK(hp, atk, def, spa, spd, spe);
    }
    else if (method == Method::PokeRadar)
    {
        if (shiny)
        {
            states = searchPokeRadarShiny(hp, atk, def, spa, spd, spe, index);
        }
        else
        {
            states = searchPokeRadar(hp, atk, def, spa, spd, spe, index);
        }
    }

    return searchInitialSeeds(states);
}

std::vector<WildSearcherState4> WildSearcher4::searchInitialSeeds(const std::vector<WildSearcherState4> &results) const
{
    std::vector<WildSearcherState4> states;

    for (WildSearcherState4 result : results)
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

std::vector<WildSearcherState4> WildSearcher4::searchMethodJ(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    std::vector<WildSearcherState4> states;

    std::array<u8, 6> ivs = { hp, atk, def, spa, spd, spe };
    bool grass = area.getEncounter() == Encounter::Grass;
    bool nibble = area.getEncounter() == Encounter::OldRod || area.getEncounter() == Encounter::GoodRod
        || area.getEncounter() == Encounter::SuperRod;

    u32 seeds[6];
    int size = LCRNGReverse::recoverPokeRNGIV(hp, atk, def, spa, spd, spe, seeds, Method::Method1);
    for (int i = 0; i < size; i++)
    {
        PokeRNGR rng(seeds[i]);

        PokeRNG forward(seeds[i]);
        forward.advance(1);
        u8 itemRand = forward.nextUShort(100);
        u8 unownForm = area.unownForm(forward.nextUShort());

        if (lead == Lead::CuteCharmF || lead == Lead::CuteCharmM)
        {
            u8 nature = rng.nextUShort<false>(25);
            if (!filter.compareNature(nature))
            {
                continue;
            }

            if (rng.nextUShort<false>(3) != 0)
            {
                u16 levelRand = grass ? 0 : rng.nextUShort();
                u8 encounterSlot = EncounterSlot::jSlot(rng.nextUShort<false>(100), area.getEncounter());
                if (!filter.compareEncounterSlot(encounterSlot))
                {
                    continue;
                }

                if (!nibble || rng.nextUShort(100) < thresh)
                {
                    const Slot &slot = area.getPokemon(encounterSlot);
                    const PersonalInfo *info = slot.getInfo();

                    u8 buffer = 0;
                    switch (info->getGender())
                    {
                    case 0:
                    case 254:
                    case 255:
                        continue;
                    default:
                        if (lead == Lead::CuteCharmF)
                        {
                            buffer = 25 * ((info->getGender() / 25) + 1);
                        }
                        break;
                    }

                    u8 level;
                    if (grass)
                    {
                        level = area.EncounterArea::calculateLevel(encounterSlot);
                    }
                    else
                    {
                        level = area.EncounterArea::calculateLevel(encounterSlot, levelRand);
                    }

                    u16 item = getItem(itemRand, lead, info);

                    u8 form = 0;
                    if (slot.getSpecie() == 201)
                    {
                        form = unownForm;
                    }

                    u32 pid = nature + buffer;
                    WildSearcherState4 state(rng.next(), pid, ivs, pid & 1, Utilities::getGender(pid, info), level, nature,
                                             Utilities::getShiny(pid, tsv), encounterSlot, item, slot.getSpecie(), form, info);
                    if (filter.compareState(static_cast<const WildSearcherState &>(state)))
                    {
                        states.emplace_back(state);
                    }
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
                PokeRNGR test(rng);

                u8 encounterSlot;
                u16 levelRand = 0;
                bool force = false;
                bool valid = false;
                switch (lead)
                {
                case Lead::None:
                case Lead::CompoundEyes:
                    if ((nextRNG / 0xa3e) == nature)
                    {
                        if (grass)
                        {
                            encounterSlot = EncounterSlot::jSlot(nextRNG2 / 0x290, area.getEncounter());
                        }
                        else
                        {
                            levelRand = nextRNG2;
                            encounterSlot = EncounterSlot::jSlot(test.nextUShort<false>(100), area.getEncounter());
                        }
                        valid = filter.compareEncounterSlot(encounterSlot);
                    }
                    break;
                case Lead::Synchronize:
                    if ((nextRNG / 0x8000) == 0)
                    {
                        if (grass)
                        {
                            encounterSlot = EncounterSlot::jSlot(nextRNG2 / 0x290, area.getEncounter());
                        }
                        else
                        {
                            levelRand = nextRNG2;
                            encounterSlot = EncounterSlot::jSlot(test.nextUShort<false>(100), area.getEncounter());
                        }
                        valid = filter.compareEncounterSlot(encounterSlot);
                    }
                    else if ((nextRNG2 / 0x8000) == 1 && (nextRNG / 0xa3e) == nature)
                    {
                        if (grass)
                        {
                            encounterSlot = EncounterSlot::jSlot(test.nextUShort<false>(100), area.getEncounter());
                        }
                        else
                        {
                            levelRand = test.nextUShort();
                            encounterSlot = EncounterSlot::jSlot(test.nextUShort<false>(100), area.getEncounter());
                        }
                        valid = filter.compareEncounterSlot(encounterSlot);
                    }
                    break;
                case Lead::MagnetPull:
                case Lead::Static:
                    if ((nextRNG / 0xa3e) == nature)
                    {
                        u16 encounterRand;
                        if (grass)
                        {
                            encounterRand = nextRNG2;
                        }
                        else
                        {
                            levelRand = nextRNG2;
                            encounterRand = test.nextUShort();
                        }

                        if (test.nextUShort<false>(2) == 0 && !modifiedSlots.empty())
                        {
                            encounterSlot = modifiedSlots[encounterRand % modifiedSlots.size()];
                        }
                        else
                        {
                            encounterSlot = EncounterSlot::jSlot(encounterRand / 0x290, area.getEncounter());
                        }
                        valid = filter.compareEncounterSlot(encounterSlot);
                    }
                    break;
                case Lead::Pressure:
                    if ((nextRNG / 0xa3e) == nature)
                    {
                        force = (nextRNG2 / 0x8000) != 0;
                        if (grass)
                        {
                            encounterSlot = EncounterSlot::jSlot(test.nextUShort<false>(100), area.getEncounter());
                        }
                        else
                        {
                            levelRand = test.nextUShort();
                            encounterSlot = EncounterSlot::jSlot(test.nextUShort<false>(100), area.getEncounter());
                        }
                        valid = filter.compareEncounterSlot(encounterSlot);
                    }
                    break;
                default:
                    break;
                }

                if (valid && (!nibble || test.nextUShort<false>(100) < thresh))
                {
                    u8 level;
                    if (area.getEncounter() == Encounter::Grass)
                    {
                        level = area.calculateLevel<false>(encounterSlot, levelRand, force);
                    }
                    else
                    {
                        level = area.calculateLevel<true>(encounterSlot, levelRand, force);
                    }

                    const Slot &slot = area.getPokemon(encounterSlot);
                    const PersonalInfo *info = slot.getInfo();
                    u16 item = getItem(itemRand, lead, info);

                    u8 form = 0;
                    if (slot.getSpecie() == 201)
                    {
                        form = unownForm;
                    }

                    WildSearcherState4 state(test.next(), pid, ivs, pid & 1, Utilities::getGender(pid, info), level, nature,
                                             Utilities::getShiny(pid, tsv), encounterSlot, item, slot.getSpecie(), form, info);
                    if (filter.compareState(static_cast<const WildSearcherState &>(state)))
                    {
                        states.emplace_back(state);
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

std::vector<WildSearcherState4> WildSearcher4::searchMethodK(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    std::vector<WildSearcherState4> states;

    std::array<u8, 6> ivs = { hp, atk, def, spa, spd, spe };
    bool grass = area.getEncounter() == Encounter::Grass;
    bool nibble = area.getEncounter() == Encounter::RockSmash || area.getEncounter() == Encounter::OldRod
        || area.getEncounter() == Encounter::GoodRod || area.getEncounter() == Encounter::SuperRod;

    u32 seeds[6];
    int size = LCRNGReverse::recoverPokeRNGIV(hp, atk, def, spa, spd, spe, seeds, Method::Method1);
    for (int i = 0; i < size; i++)
    {
        PokeRNGR rng(seeds[i]);
        u8 itemRand = (PokeRNG(seeds[i]).advance(2) >> 16) % 100;
        if (lead == Lead::CuteCharmF || lead == Lead::CuteCharmM)
        {
            u8 nature = rng.nextUShort(25);
            if (!filter.compareNature(nature))
            {
                continue;
            }

            if (rng.nextUShort(3) != 0)
            {
                u8 encounterSlot;
                u16 levelRand;
                if (safari)
                {
                    encounterSlot = rng.nextUShort(10);
                }
                else if (grass)
                {
                    encounterSlot = EncounterSlot::kSlot(rng.nextUShort(100), area.getEncounter());
                }
                else
                {
                    levelRand = rng.nextUShort();
                    encounterSlot = EncounterSlot::kSlot(rng.nextUShort(100), area.getEncounter());
                }

                if (!filter.compareEncounterSlot(encounterSlot))
                {
                    continue;
                }

                if (!nibble || rng.nextUShort(100) < thresh)
                {
                    const Slot &slot = area.getPokemon(encounterSlot);
                    const PersonalInfo *info = slot.getInfo();

                    u8 buffer = 0;
                    switch (info->getGender())
                    {
                    case 0:
                    case 254:
                    case 255:
                        continue;
                    default:
                        if (lead == Lead::CuteCharmF)
                        {
                            buffer = 25 * ((info->getGender() / 25) + 1);
                        }
                        break;
                    }

                    u8 level;
                    if (grass || safari)
                    {
                        level = area.EncounterArea::calculateLevel(encounterSlot);
                    }
                    else
                    {
                        level = area.EncounterArea::calculateLevel(encounterSlot, levelRand);
                    }

                    u16 item = getItem(itemRand, lead, info);

                    u32 pid = nature + buffer;
                    WildSearcherState4 state(rng.next(), pid, ivs, pid & 1, Utilities::getGender(pid, info), level, nature,
                                             Utilities::getShiny(pid, tsv), encounterSlot, item, slot.getSpecie(), 0, info);
                    if (filter.compareState(static_cast<const WildSearcherState &>(state)))
                    {
                        states.emplace_back(state);
                    }
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
                PokeRNGR test(rng);

                u8 encounterSlot;
                u16 levelRand = 0;
                bool force = false;
                bool valid = false;
                switch (lead)
                {
                case Lead::None:
                case Lead::ArenaTrap:
                case Lead::CompoundEyes:
                case Lead::SuctionCups:
                    if ((nextRNG % 25) == nature)
                    {
                        if (safari)
                        {
                            encounterSlot = nextRNG2 % 10;
                        }
                        else if (grass)
                        {
                            encounterSlot = EncounterSlot::kSlot(nextRNG2 % 100, area.getEncounter());
                        }
                        else
                        {
                            levelRand = nextRNG2;
                            encounterSlot = EncounterSlot::kSlot(test.nextUShort(100), area.getEncounter());
                        }
                        valid = filter.compareEncounterSlot(encounterSlot);
                    }
                    break;
                case Lead::Synchronize:
                    if ((nextRNG % 2) == 0)
                    {
                        if (safari)
                        {
                            encounterSlot = nextRNG2 % 10;
                        }
                        else if (grass)
                        {
                            encounterSlot = EncounterSlot::kSlot(nextRNG2 % 100, area.getEncounter());
                        }
                        else
                        {
                            levelRand = nextRNG2;
                            encounterSlot = EncounterSlot::kSlot(test.nextUShort(100), area.getEncounter());
                        }
                        valid = filter.compareEncounterSlot(encounterSlot);
                    }
                    else if ((nextRNG2 % 2) == 1 && (nextRNG % 25) == nature)
                    {
                        if (safari)
                        {
                            encounterSlot = test.nextUShort(10);
                        }
                        else if (grass)
                        {
                            encounterSlot = EncounterSlot::kSlot(test.nextUShort(100), area.getEncounter());
                        }
                        else
                        {
                            levelRand = test.nextUShort();
                            encounterSlot = EncounterSlot::kSlot(test.nextUShort(100), area.getEncounter());
                        }
                        valid = filter.compareEncounterSlot(encounterSlot);
                    }
                    break;
                case Lead::MagnetPull:
                case Lead::Static:
                    if ((nextRNG % 25) == nature)
                    {
                        u16 encounterRand;
                        if (grass || safari)
                        {
                            encounterRand = nextRNG2;
                        }
                        else
                        {
                            levelRand = nextRNG2;
                            encounterRand = test.nextUShort();
                        }

                        if (test.nextUShort(2) == 0 && !modifiedSlots.empty())
                        {
                            encounterSlot = modifiedSlots[encounterRand % modifiedSlots.size()];
                        }
                        else if (safari)
                        {
                            encounterSlot = encounterRand % 10;
                        }
                        else
                        {
                            encounterSlot = EncounterSlot::kSlot(encounterRand % 100, area.getEncounter());
                        }
                        valid = filter.compareEncounterSlot(encounterSlot);
                    }
                    break;
                case Lead::Pressure:
                    if ((nextRNG % 25) == nature)
                    {
                        force = (nextRNG2 % 2) != 0;
                        if (safari)
                        {
                            encounterSlot = test.nextUShort(10);
                        }
                        else if (grass)
                        {
                            encounterSlot = EncounterSlot::kSlot(test.nextUShort(100), area.getEncounter());
                        }
                        else
                        {
                            levelRand = test.nextUShort();
                            encounterSlot = EncounterSlot::kSlot(test.nextUShort(100), area.getEncounter());
                        }
                        valid = filter.compareEncounterSlot(encounterSlot);
                    }
                    break;
                default:
                    break;
                }

                if (valid && (!nibble || test.nextUShort(100) < thresh))
                {
                    u8 level;
                    if (area.getEncounter() == Encounter::Grass || safari)
                    {
                        level = area.calculateLevel<false>(encounterSlot, levelRand, force);
                    }
                    else
                    {
                        level = area.calculateLevel<true>(encounterSlot, levelRand, force);
                    }

                    const Slot &slot = area.getPokemon(encounterSlot);
                    const PersonalInfo *info = slot.getInfo();
                    u16 item = getItem(itemRand, lead, info);

                    WildSearcherState4 state(test.next(), pid, ivs, pid & 1, Utilities::getGender(pid, info), level, nature,
                                             Utilities::getShiny(pid, tsv), encounterSlot, item, slot.getSpecie(), 0, info);
                    if (filter.compareState(static_cast<const WildSearcherState &>(state)))
                    {
                        states.emplace_back(state);
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

std::vector<WildSearcherState4> WildSearcher4::searchPokeRadar(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 index) const
{
    std::vector<WildSearcherState4> states;

    std::array<u8, 6> ivs = { hp, atk, def, spa, spd, spe };
    const Slot &slot = area.getPokemon(index);
    const PersonalInfo *info = slot.getInfo();

    bool cuteCharm = false;
    u8 buffer = 0;
    switch (info->getGender())
    {
    case 0:
    case 254:
    case 255:
        break;
    default:
        cuteCharm = true;
        if (lead == Lead::CuteCharmF)
        {
            buffer = 25 * ((info->getGender() / 25) + 1);
        }
        break;
    }

    u32 seeds[6];
    int size = LCRNGReverse::recoverPokeRNGIV(hp, atk, def, spa, spd, spe, seeds, Method::Method1);
    for (int i = 0; i < size; i++)
    {
        PokeRNGR rng(seeds[i]);
        u16 item = getItem((PokeRNG(seeds[i]).advance(2) >> 16) % 100, lead, info);

        if ((lead == Lead::CuteCharmF || lead == Lead::CuteCharmM) && cuteCharm)
        {
            u8 nature = rng.nextUShort<false>(25);
            if (!filter.compareNature(nature))
            {
                continue;
            }

            if (rng.nextUShort<false>(3) != 0)
            {
                u32 pid = nature + buffer;
                WildSearcherState4 state(rng.next(), pid, ivs, pid & 1, Utilities::getGender(pid, info), slot.getMaxLevel(), nature,
                                         Utilities::getShiny(pid, tsv), index, item, slot.getSpecie(), 0, info);
                if (filter.compareState(static_cast<const WildSearcherState &>(state)))
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
                PokeRNGR test(rng);

                bool valid = false;
                u32 seed;
                switch (lead)
                {
                case Lead::None:
                case Lead::CompoundEyes:
                    if ((nextRNG / 0xa3e) == nature)
                    {
                        seed = test.getSeed();
                        valid = true;
                    }
                    break;
                case Lead::Synchronize:
                    if ((nextRNG / 0x8000) == 0)
                    {
                        seed = test.getSeed();
                        valid = true;
                    }
                    else if ((nextRNG2 / 0x8000) == 1 && (nextRNG / 0xa3e) == nature)
                    {
                        seed = test.next();
                        valid = true;
                    }
                    break;
                default:
                    break;
                }

                if (valid)
                {
                    WildSearcherState4 state(seed, pid, ivs, pid & 1, Utilities::getGender(pid, info), slot.getMaxLevel(), nature,
                                             Utilities::getShiny(pid, tsv), index, item, slot.getSpecie(), 0, info);
                    if (filter.compareState(static_cast<const WildSearcherState &>(state)))
                    {
                        states.emplace_back(state);
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

std::vector<WildSearcherState4> WildSearcher4::searchPokeRadarShiny(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 index) const
{
    std::vector<WildSearcherState4> states;

    std::array<u8, 6> ivs = { hp, atk, def, spa, spd, spe };
    const Slot &slot = area.getPokemon(index);
    const PersonalInfo *info = slot.getInfo();

    bool cuteCharm = false;
    switch (info->getGender())
    {
    case 0:
    case 254:
    case 255:
        break;
    default:
        cuteCharm = true;
        break;
    }

    auto cuteCharmCheck = [this](const PersonalInfo *info, u32 pid) {
        if (lead == Lead::CuteCharmF)
        {
            return (pid & 0xff) >= info->getGender();
        }
        return (pid & 0xff) < info->getGender();
    };

    u32 seeds[6];
    int size = LCRNGReverse::recoverPokeRNGIV(hp, atk, def, spa, spd, spe, seeds, Method::Method1);
    for (int i = 0; i < size; i++)
    {
        PokeRNGR rng(seeds[i]);
        u16 item = getItem((PokeRNG(seeds[i]).advance(2) >> 16) % 100, lead, info);

        auto shinyPID = [this](PokeRNGR &rng) {
            u16 low = 0;
            for (int j = 15; j > 2; j--)
            {
                low |= rng.nextUShort(2) << j;
            }
            u16 high = rng.nextUShort(8);
            low |= rng.nextUShort(8);
            high |= (low ^ tsv) & 0xfff8;
            return static_cast<u32>(high << 16) | low;
        };

        u32 pid = shinyPID(rng);
        u8 nature = pid % 25;
        if (!filter.compareNature(nature))
        {
            continue;
        }

        if (lead == Lead::Synchronize || (cuteCharm && (lead == Lead::CuteCharmF || lead == Lead::CuteCharmM)))
        {
            u8 huntNature;
            u8 gender = (pid & 0xff) < info->getGender();
            do
            {
                PokeRNGR test(rng);

                bool valid = false;
                if (lead == Lead::Synchronize)
                {
                    valid = test.nextUShort<false>(2) == 0;
                }
                else
                {
                    valid = test.nextUShort<false>(3) != 0 && cuteCharmCheck(info, pid);
                }

                if (valid)
                {
                    WildSearcherState4 state(test.next(), pid, ivs, pid & 1, Utilities::getGender(pid, info), slot.getMaxLevel(), nature,
                                             Utilities::getShiny(pid, tsv), index, item, slot.getSpecie(), 0, info);
                    if (filter.compareState(static_cast<const WildSearcherState &>(state)))
                    {
                        states.emplace_back(state);
                    }
                }

                u32 huntPID = shinyPID(rng);
                huntNature = huntPID % 25;
                if (lead == Lead::CuteCharmF || lead == Lead::CuteCharmM)
                {
                    if (gender == ((huntPID & 0xff) < info->getGender()))
                    {
                        break;
                    }
                }
            } while (huntNature != nature);
        }
        else
        {
            WildSearcherState4 state(rng.next(), pid, ivs, pid & 1, Utilities::getGender(pid, info), slot.getMaxLevel(), nature,
                                     Utilities::getShiny(pid, tsv), index, item, slot.getSpecie(), 0, info);
            if (filter.compareState(static_cast<const WildSearcherState &>(state)))
            {
                states.emplace_back(state);
            }
        }
    }

    return states;
}
