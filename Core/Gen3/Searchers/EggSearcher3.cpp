/*
 * This file is part of PokeFinder
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

#include "EggSearcher3.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Gen3/States/EggState3.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/Util/Utilities.hpp>

static void setInheritance(const Daycare &daycare, std::array<u8, 6> &ivs, std::array<u8, 6> &inheritance, const u8 *inh, const u8 *par)
{
    constexpr u8 order[6] = { 0, 1, 2, 5, 3, 4 };

    u8 available[6] = { 0, 1, 2, 3, 4, 5 };
    auto avoid = [&available](u8 index, u8 size) {
        for (u8 i = index; i < size; i++)
        {
            available[i] = available[i + 1];
        }
    };

    u8 stat = available[inh[0]];
    ivs[order[stat]] = daycare.getParentIV(par[0], order[stat]);
    inheritance[order[stat]] = par[0] + 1;

    avoid(stat, 5);

    stat = available[inh[1]];
    ivs[order[stat]] = daycare.getParentIV(par[1], order[stat]);
    inheritance[order[stat]] = par[1] + 1;

    avoid(stat, 4);

    stat = available[inh[2]];
    ivs[order[stat]] = daycare.getParentIV(par[2], order[stat]);
    inheritance[order[stat]] = par[2] + 1;
}

EggSearcher3::EggSearcher3(Method method, u8 compatability, const Daycare &daycare, const Profile3 &profile, const StateFilter &filter,
                           u32 initialAdvancesHeld, u32 maxAdvancesHeld, u32 offsetHeld, u32 initialAdvancesPickup, u32 maxAdvancesPickup,
                           u32 offsetPickup) :
    Searcher(method, profile),
    filter(filter),
    compatability(compatability),
    daycare(daycare),
    initialAdvancesHeld(initialAdvancesHeld),
    maxAdvancesHeld(maxAdvancesHeld),
    offsetHeld(offsetHeld),
    initialAdvancesPickup(initialAdvancesPickup),
    maxAdvancesPickup(maxAdvancesPickup),
    offsetPickup(offsetPickup)
{
    switch (method)
    {
    case Method::RSFRLGBred:
        iv1 = 1;
        iv2 = 0;
        inh = 1;
        break;
    case Method::RSFRLGBredSplit:
        iv1 = 0;
        iv2 = 1;
        inh = 1;
        break;
    case Method::RSFRLGBredAlternate:
        iv1 = 1;
        iv2 = 0;
        inh = 2;
        break;
    case Method::RSFRLGBredMixed:
        iv1 = 0;
        iv2 = 0;
        inh = 2;
        break;
    default:
        iv1 = 1;
        iv2 = 0;
        inh = 1;
        break;
    }
}

void EggSearcher3::startSearch(const std::array<u8, 6> &minIVs, const std::array<u8, 6> &maxIVs)
{
    searching = true;
    setMaxProgress(0x10000);

    const PersonalInfo *base = PersonalLoader::getPersonal(profile.getVersion(), daycare.getEggSpecie());
    const PersonalInfo *male = nullptr;
    if (daycare.getEggSpecie() == 29) // Nidoran
    {
        male = PersonalLoader::getPersonal(profile.getVersion(), 32);
    }
    else if (daycare.getEggSpecie() == 314) // Illumise
    {
        male = PersonalLoader::getPersonal(profile.getVersion(), 313);
    }

    std::array<bool, 25> natureFilter;
    for (u8 n = 0; n < 25; n++)
    {
        natureFilter[n] = filter.compareNature(n);
    }

    // Phase 1 Pass 1: Count held entries per pid_low
    auto *counts = new u32[0x10000]();
    auto *index = new u32[0x10000 + 1]();

    for (u32 seed = 0; seed <= 0xFFFF; seed++)
    {
        PokeRNG rng(seed, initialAdvancesHeld + offsetHeld);
        for (u32 cnt = 0; cnt <= maxAdvancesHeld; cnt++, rng.next())
        {
            PokeRNG go(rng);
            if (((go.nextUShort() * 100) / 0xFFFF) < compatability)
            {
                u16 pidLow = go.nextUShort(0xFFFE) + 1;

                u8 ability = pidLow & 1;
                if (!filter.compareAbility(ability))
                {
                    continue;
                }
                const PersonalInfo *info = base;
                if (male && (pidLow & 0x8000))
                {
                    info = male;
                }
                if (!filter.compareGender(Utilities::getGender(pidLow, info)))
                {
                    continue;
                }

                counts[pidLow]++;
            }
        }
    }

    // Build offset index
    u32 totalEntries = 0;
    for (u32 i = 0; i < 0x10000; i++)
    {
        index[i] = totalEntries;
        totalEntries += counts[i];
    }
    index[0x10000] = totalEntries;

    // Allocate flat storage
    std::vector<HeldEntry> heldData(totalEntries);
    auto *writePos = new u32[0x10000]();

    // Phase 1 Pass 2: Fill entries
    for (u32 seed = 0; seed <= 0xFFFF; seed++)
    {
        PokeRNG rng(seed, initialAdvancesHeld + offsetHeld);
        for (u32 cnt = 0; cnt <= maxAdvancesHeld; cnt++, rng.next())
        {
            PokeRNG go(rng);
            if (((go.nextUShort() * 100) / 0xFFFF) < compatability)
            {
                u16 pidLow = go.nextUShort(0xFFFE) + 1;

                u8 ability = pidLow & 1;
                if (!filter.compareAbility(ability))
                {
                    continue;
                }
                const PersonalInfo *info = base;
                if (male && (pidLow & 0x8000))
                {
                    info = male;
                }
                if (!filter.compareGender(Utilities::getGender(pidLow, info)))
                {
                    continue;
                }

                u32 pos = index[pidLow] + writePos[pidLow]++;
                heldData[pos] = { static_cast<u16>(seed), initialAdvancesHeld + cnt };
            }
        }
    }

    delete[] counts;
    delete[] writePos;

    // Phase 2: Precompute pid_low values grouped by nature
    std::vector<u16> pidLowByNature[25];
    for (u32 p = 1; p < 0x10000; p += 2)
    {
        pidLowByNature[p % 25].push_back(p);
    }

    // Phase 3: Iterate all pickup seeds
    constexpr size_t maxResults = 100000;
    size_t resultCount = 0;

    for (u32 seed = 0; seed <= 0xFFFF; seed++)
    {
        if (!searching)
        {
            delete[] index;
            return;
        }

        PokeRNG rng(static_cast<u16>(seed), initialAdvancesPickup + offsetPickup);
        for (u32 cnt = 0; cnt <= maxAdvancesPickup; cnt++, rng.next())
        {
            PokeRNG go(rng);

            u16 pidHigh = go.nextUShort();

            go.advance(iv1);
            u16 iv1Val = go.nextUShort();
            go.advance(iv2);
            u16 iv2Val = go.nextUShort();

            std::array<u8, 6> ivs;
            ivs[0] = iv1Val & 31;
            ivs[1] = (iv1Val >> 5) & 31;
            ivs[2] = (iv1Val >> 10) & 31;
            ivs[3] = (iv2Val >> 5) & 31;
            ivs[4] = (iv2Val >> 10) & 31;
            ivs[5] = iv2Val & 31;

            go.advance(inh);
            u8 inhVals[3];
            inhVals[0] = go.nextUShort(6);
            inhVals[1] = go.nextUShort(5);
            inhVals[2] = go.nextUShort(4);

            u8 par[3];
            par[0] = go.nextUShort(2);
            par[1] = go.nextUShort(2);
            par[2] = go.nextUShort(2);

            std::array<u8, 6> inheritance = { 0, 0, 0, 0, 0, 0 };
            setInheritance(daycare, ivs, inheritance, inhVals, par);

            // IV filter
            bool ivMatch = true;
            for (int i = 0; i < 6; i++)
            {
                if (ivs[i] < minIVs[i] || ivs[i] > maxIVs[i])
                {
                    ivMatch = false;
                    break;
                }
            }
            if (!ivMatch)
            {
                continue;
            }

            // Hidden Power filter
            u8 hiddenPower = ((((ivs[0] & 1) + 2 * (ivs[1] & 1) + 4 * (ivs[2] & 1) + 8 * (ivs[3] & 1) + 16 * (ivs[4] & 1)
                                + 32 * (ivs[5] & 1))
                               * 15)
                              / 63);
            if (!filter.compareHiddenPower(hiddenPower))
            {
                continue;
            }

            // Match pid_low values against held states
            u8 natureContrib = static_cast<u8>((static_cast<u32>(pidHigh) << 16) % 25);

            for (u8 natureTarget = 0; natureTarget < 25; natureTarget++)
            {
                if (!natureFilter[natureTarget])
                {
                    continue;
                }

                u8 targetMod = (natureTarget - natureContrib + 25) % 25;

                for (u16 pidLow : pidLowByNature[targetMod])
                {
                    if (!searching || resultCount >= maxResults)
                    {
                        delete[] index;
                        return;
                    }

                    u8 shiny = Utilities::getShiny<true>((static_cast<u32>(pidHigh) << 16) | pidLow, tsv);
                    if (!filter.compareShiny(shiny))
                    {
                        continue;
                    }

                    u32 start = index[pidLow];
                    u32 end = index[pidLow + 1];
                    if (start == end)
                    {
                        continue;
                    }

                    const PersonalInfo *info = base;
                    if (male && (pidLow & 0x8000))
                    {
                        info = male;
                    }

                    for (u32 i = start; i < end; i++)
                    {
                        const auto &held = heldData[i];
                        EggState3 state(held.advances, pidLow, Utilities::getGender(pidLow, info), info);
                        state.setSeedHeld(held.seed);
                        state.setSeedPickup(static_cast<u16>(seed));
                        state.update(initialAdvancesPickup + cnt, (static_cast<u32>(pidHigh) << 16) | pidLow,
                                     shiny, ivs, inheritance, info);

                        if (filter.compareIV(state.getIVs()) && filter.compareHiddenPower(state.getHiddenPower())
                            && filter.compareNature(state.getNature()) && filter.compareShiny(state.getShiny()))
                        {
                            std::lock_guard<std::mutex> guard(mutex);
                            results.emplace_back(state);
                            resultCount++;
                        }
                    }
                }
            }
        }

        progress++;
    }

    delete[] index;
}
