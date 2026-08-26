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

#include "PokeRadarSearcher.hpp"

#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen4/States/WildState4.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/RNG/LCRNGReverse.hpp>
#include <Core/Util/EncounterSlot.hpp>
#include <Core/Util/Utilities.hpp>
#include <algorithm>
#include <limits>
#include <mutex>
#include <unordered_map>
#include <vector>

static u32 getBattleAdvances(const EncounterArea4 &area, Game version)
{
    u32 advances = 0;

    if (area.getEncounter() == Encounter::OldRod || area.getEncounter() == Encounter::GoodRod || area.getEncounter() == Encounter::SuperRod)
    {
        advances += 1;
    }

    if ((version & Game::DP) != Game::None)
    {
        advances += 4;
    }

    if (!area.greatMarsh(version) && !area.safariZone(version))
    {
        advances += 1;
    }

    return advances;
}

static u32 getPostBattlePatchAdvances(u32 battleAdvances, Game version)
{
    if ((version & Game::DP) != Game::None && battleAdvances >= 4)
    {
        battleAdvances -= 4;
    }

    return battleAdvances;
}

static u32 getPostBattlePatchTypeAdvances(Game version)
{
    return (version & Game::DP) != Game::None ? 4 : 0;
}

static bool isPokeRadarChainLead(Lead lead)
{
    return lead <= Lead::SynchronizeEnd || lead == Lead::CuteCharmF || lead == Lead::CuteCharmM;
}

static Lead getPokeRadarSearcherLead(Lead lead, bool chain)
{
    return !chain || isPokeRadarChainLead(lead) ? lead : Lead::None;
}

static PokeRadarChainType getPokeRadarSearcherChainType(PokeRadarChainType chainType, bool chain)
{
    if (chain)
    {
        return chainType;
    }

    if (chainType == PokeRadarChainType::WeakShiny)
    {
        return PokeRadarChainType::Weak;
    }

    if (chainType == PokeRadarChainType::StrongShiny)
    {
        return PokeRadarChainType::Strong;
    }

    return chainType;
}

static u16 getRadarItem(u8 rand, Lead lead, const PersonalInfo *info)
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

PokeRadarSearcher::PokeRadarSearcher(u32 minAdvance, u32 maxAdvance, u32 minDelay, u32 maxDelay, u32 minPatchDistance,
                                     u16 maxChain, u8 chainSlot, Lead lead, PokeRadarChainType chainType, PokeRadarResult result,
                                     const std::array<bool, 81> &grass,
                                     const std::array<bool, 12> &encounterSlots, const EncounterArea4 &area, const Profile4 &profile,
                                     const WildStateFilter &filter, bool specificSynchronize) :
    Searcher(Method::PokeRadar, profile),
    minAdvance(minAdvance),
    maxAdvance(maxAdvance),
    minDelay(minDelay),
    maxDelay(maxDelay),
    minPatchDistance(minPatchDistance),
    maxChain(maxChain),
    chainSlot(chainSlot),
    lead(lead),
    specificSynchronize(specificSynchronize),
    chainType(chainType),
    result(result),
    grass(grass),
    encounterSlots(encounterSlots),
    area(area),
    filter(filter),
    currentPhaseProgress(0)
{
    setMaxProgress(1);
}

void PokeRadarSearcher::startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max)
{
    searching = true;

    u64 slotCount = 0;
    for (bool slot : encounterSlots)
    {
        slotCount += slot;
    }
    if (slotCount == 0)
    {
        slotCount = encounterSlots.size();
    }

    setMaxProgress(((slotCount + (maxChain == 0 ? 0 : 1)) * 2 * 100) + 1);
    progress = 0;

    searchPokemon(min, max, false);

    if (searching && maxChain != 0)
    {
        searchPokemon(min, max, true);
    }

    if (searching)
    {
        progress = maxProgress;
    }
}

void PokeRadarSearcher::searchPokemon(const std::array<u8, 6> &min, const std::array<u8, 6> &max, bool chain)
{
    Lead effectiveLead = getPokeRadarSearcherLead(lead, chain);
    PokeRadarChainType searchChainType = getPokeRadarSearcherChainType(chainType, chain);
    u64 ivCombinations = 1;
    for (size_t i = 0; i < min.size(); i++)
    {
        ivCombinations *= max[i] - min[i] + 1;
    }

    bool allSlots = true;
    for (bool slot : encounterSlots)
    {
        allSlots &= !slot;
    }

    u8 startSlot = chain ? chainSlot : 0;
    u8 endSlot = chain ? chainSlot + 1 : static_cast<u8>((effectiveLead == Lead::CuteCharmF || effectiveLead == Lead::CuteCharmM) ? encounterSlots.size() : 1);
    std::set<std::tuple<u32, u32, u8, u32>> chainZeroPokemonKeys;
    for (u8 slot = startSlot; slot < endSlot; slot++)
    {
        if (!searching)
        {
            return;
        }

        if (chain && !allSlots && !encounterSlots[slot])
        {
            continue;
        }

        currentPhaseProgress = 0;
        std::vector<WildSearcherState4> pokemon;
        u64 currentIV = 0;
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

                                auto states = searchPokemonIVs(hp, atk, def, spa, spd, spe, slot, effectiveLead,
                                                               isShinyPatchType(searchChainType), chain);
                                pokemon.insert(pokemon.end(), states.begin(), states.end());
                                currentPhaseProgress = (++currentIV * 100) / ivCombinations;
                            }
                        }
                    }
                }
            }
        }

        progress += 100;
        for (size_t i = 0; i < pokemon.size(); i++)
        {
            if (!searching)
            {
                return;
            }

            currentPhaseProgress = (i * 100) / pokemon.size();
            if (!chain)
            {
                auto state = validateChainZeroPokemon(pokemon[i], allSlots);
                if (state.has_value())
                {
                    auto key = std::make_tuple(state->getSeed(), state->getAdvances(), state->getEncounterSlot(), state->getPID());
                    if (!chainZeroPokemonKeys.insert(key).second)
                    {
                        continue;
                    }

                    addPatchMatches(*state, 0, 0, searchChainType);
                }
            }
            else
            {
                addPatchMatches(pokemon[i], 1, maxChain, searchChainType);
            }
        }

        currentPhaseProgress = 0;
        progress += 100;
        if (progress > maxProgress)
        {
            progress = maxProgress;
        }
    }

}

std::vector<WildSearcherState4> PokeRadarSearcher::searchInitialSeeds(const std::vector<WildSearcherState4> &states, u32 displayAdvanceOffset) const
{
    std::vector<WildSearcherState4> results;
    u32 startAdvance = minAdvance + displayAdvanceOffset;
    u32 endAdvance = maxAdvance + displayAdvanceOffset;

    for (WildSearcherState4 state : states)
    {
        PokeRNGR rng(state.getSeed(), startAdvance);
        u32 seed = rng.getSeed();
        for (u32 cnt = startAdvance; cnt <= endAdvance; cnt++)
        {
            u8 hour = (seed >> 16) & 0xff;
            u16 delay = seed & 0xffff;
            if (hour < 24 && delay >= minDelay && delay <= maxDelay)
            {
                state.setSeed(seed);
                state.setAdvances(cnt - displayAdvanceOffset);
                results.emplace_back(state);
            }

            seed = rng.next();
        }
    }

    return results;
}

std::vector<WildSearcherState4> PokeRadarSearcher::searchPokemonIVs(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 index,
                                                                    Lead effectiveLead, bool shiny, bool applyFilter) const
{
    return searchInitialSeeds(shiny ? searchPokemonShinyIVs(hp, atk, def, spa, spd, spe, index, effectiveLead, applyFilter)
                                    : searchPokemonNormalIVs(hp, atk, def, spa, spd, spe, index, effectiveLead, applyFilter),
                              shiny && applyFilter ? 2 : 0);
}

std::vector<WildSearcherState4> PokeRadarSearcher::searchPokemonNormalIVs(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 index,
                                                                          Lead effectiveLead, bool applyFilter) const
{
    std::vector<WildSearcherState4> states;

    std::array<u8, 6> ivs = { hp, atk, def, spa, spd, spe };
    const Slot &slot = area.getPokemon(index);
    const PersonalInfo *info = slot.getInfo();

    u8 buffer = 0;
    bool cuteCharm = false;
    if ((effectiveLead == Lead::CuteCharmF || effectiveLead == Lead::CuteCharmM) && !info->getFixedGender())
    {
        cuteCharm = true;
        if (effectiveLead == Lead::CuteCharmF)
        {
            buffer = 25 * ((info->getGender() / 25) + 1);
        }
    }

    auto seeds = LCRNGReverse::recoverPokeRNGIV(hp, atk, def, spa, spd, spe, Method::Method1);
    for (int i = 0; i < seeds.count; i++)
    {
        PokeRNGR rng(seeds[i]);
        u16 item = getRadarItem((PokeRNG(seeds[i]).advance(2) >> 16) % 100, effectiveLead, info);

        if (cuteCharm)
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
                                         Utilities::getShiny<true>(pid, tsv), index, item, slot.getSpecie(), 0, info);
                if (!applyFilter || filter.compareState(static_cast<const WildSearcherState &>(state)))
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
                if (effectiveLead == Lead::None || effectiveLead == Lead::CompoundEyes || effectiveLead == Lead::MagnetPull
                    || effectiveLead == Lead::Static)
                {
                    if ((nextRNG / 0xa3e) == nature)
                    {
                        seed = test.getSeed();
                        valid = true;
                    }
                }
                else if (effectiveLead <= Lead::SynchronizeEnd)
                {
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
                }

                if (valid)
                {
                    WildSearcherState4 state(seed, pid, ivs, pid & 1, Utilities::getGender(pid, info), slot.getMaxLevel(), nature,
                                             Utilities::getShiny<true>(pid, tsv), index, item, slot.getSpecie(), 0, info);
                    if (!applyFilter || filter.compareState(static_cast<const WildSearcherState &>(state)))
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

std::vector<WildSearcherState4> PokeRadarSearcher::searchPokemonShinyIVs(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 index,
                                                                         Lead effectiveLead, bool applyFilter) const
{
    std::vector<WildSearcherState4> states;

    std::array<u8, 6> ivs = { hp, atk, def, spa, spd, spe };
    const Slot &slot = area.getPokemon(index);
    const PersonalInfo *info = slot.getInfo();

    bool cuteCharm = (effectiveLead == Lead::CuteCharmF || effectiveLead == Lead::CuteCharmM) && !info->getFixedGender();

    auto cuteCharmCheck = [effectiveLead](const PersonalInfo *info, u32 pid) {
        if (effectiveLead == Lead::CuteCharmF)
        {
            return (pid & 0xff) >= info->getGender();
        }
        return (pid & 0xff) < info->getGender();
    };

    auto seeds = LCRNGReverse::recoverPokeRNGIV(hp, atk, def, spa, spd, spe, Method::Method1);
    for (int i = 0; i < seeds.count; i++)
    {
        PokeRNGR rng(seeds[i]);
        u16 item = getRadarItem((PokeRNG(seeds[i]).advance(2) >> 16) % 100, effectiveLead, info);

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
        if (specificSynchronize && effectiveLead <= Lead::SynchronizeEnd && nature != toInt(effectiveLead))
        {
            continue;
        }

        if (!filter.compareNature(nature))
        {
            continue;
        }

        if (effectiveLead <= Lead::SynchronizeEnd || cuteCharm)
        {
            u8 huntNature;
            u8 gender = (pid & 0xff) < info->getGender();
            do
            {
                PokeRNGR test(rng);

                bool valid = false;
                if (effectiveLead <= Lead::SynchronizeEnd)
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
                                             Utilities::getShiny<true>(pid, tsv), index, item, slot.getSpecie(), 0, info);
                    if (!applyFilter || filter.compareState(static_cast<const WildSearcherState &>(state)))
                    {
                        states.emplace_back(state);
                    }
                }

                u32 huntPID = shinyPID(rng);
                huntNature = huntPID % 25;
                if (cuteCharm && gender == ((huntPID & 0xff) < info->getGender()))
                {
                    break;
                }
            } while (huntNature != nature);
        }
        else
        {
            WildSearcherState4 state(rng.next(), pid, ivs, pid & 1, Utilities::getGender(pid, info), slot.getMaxLevel(), nature,
                                     Utilities::getShiny<true>(pid, tsv), index, item, slot.getSpecie(), 0, info);
            if (!applyFilter || filter.compareState(static_cast<const WildSearcherState &>(state)))
            {
                states.emplace_back(state);
            }
        }
    }

    return states;
}

std::optional<WildSearcherState4> PokeRadarSearcher::validateChainZeroPokemon(const WildSearcherState4 &pokemon, bool allSlots) const
{
    if (pokemon.getAdvances() == 0)
    {
        return std::nullopt;
    }

    u32 advances = pokemon.getAdvances() - 1;
    PokeRNG go(pokemon.getSeed(), advances);

    u8 encounterSlot = EncounterSlot::jSlot(go.nextUShort<false>(100), area.getEncounter());
    if (!allSlots && !encounterSlots[encounterSlot])
    {
        return std::nullopt;
    }

    const Slot &slot = area.getPokemon(encounterSlot);
    const PersonalInfo *info = slot.getInfo();

    u8 buffer = 0;
    bool cuteCharm = false;
    if ((lead == Lead::CuteCharmF || lead == Lead::CuteCharmM) && !info->getFixedGender())
    {
        cuteCharm = true;
        if (lead == Lead::CuteCharmF)
        {
            buffer = 25 * ((info->getGender() / 25) + 1);
        }
    }

    bool cuteCharmFlag = false;
    if (cuteCharm)
    {
        cuteCharmFlag = go.nextUShort<false>(3) != 0;
    }

    u8 nature;
    if (lead <= Lead::SynchronizeEnd)
    {
        nature = go.nextUShort<false>(2) == 0 ? toInt(lead) : go.nextUShort<false>(25);
    }
    else
    {
        nature = go.nextUShort<false>(25);
    }

    u32 pid;
    if (cuteCharmFlag)
    {
        pid = buffer + nature;
    }
    else
    {
        do
        {
            u16 low = go.nextUShort();
            u16 high = go.nextUShort();
            pid = (high << 16) | low;
        } while (pid % 25 != nature);
    }

    u16 iv1 = go.nextUShort();
    u16 iv2 = go.nextUShort();
    std::array<u8, 6> ivs;
    ivs[0] = iv1 & 31;
    ivs[1] = (iv1 >> 5) & 31;
    ivs[2] = (iv1 >> 10) & 31;
    ivs[3] = (iv2 >> 5) & 31;
    ivs[4] = (iv2 >> 10) & 31;
    ivs[5] = iv2 & 31;

    u16 item = getRadarItem(go.nextUShort(100), lead, info);
    WildSearcherState4 state(pokemon.getSeed(), pid, ivs, pid & 1, Utilities::getGender(pid, info), slot.getMaxLevel(), nature,
                             Utilities::getShiny<true>(pid, tsv), encounterSlot, item, slot.getSpecie(), 0, info);
    state.setAdvances(advances == 0 ? 0 : advances - 1);

    if (state.getPID() != pokemon.getPID() || state.getIVs() != pokemon.getIVs())
    {
        return std::nullopt;
    }

    if (!filter.compareState(static_cast<const WildSearcherState &>(state)))
    {
        return std::nullopt;
    }

    return state;
}

u32 PokeRadarSearcher::calculatePokemonBattleAdvances(u32 seed, u32 advances, Lead effectiveLead, bool shiny) const
{
    u32 battleAdvances = getBattleAdvances(area, profile.getVersion()) + advances + 1;
    PokeRNG go(seed, advances + 1);

    const Slot &slot = area.getPokemon(chainSlot);
    const PersonalInfo *info = slot.getInfo();
    const bool cuteCharm = (effectiveLead == Lead::CuteCharmF || effectiveLead == Lead::CuteCharmM) && !info->getFixedGender();

    if (!shiny)
    {
        bool cuteCharmFlag = false;
        if (cuteCharm)
        {
            cuteCharmFlag = go.nextUShort<false>(3, &battleAdvances) != 0;
        }

        u8 nature;
        if (effectiveLead <= Lead::SynchronizeEnd)
        {
            nature = go.nextUShort<false>(2, &battleAdvances) == 0 ? toInt(effectiveLead) : go.nextUShort<false>(25, &battleAdvances);
        }
        else
        {
            nature = go.nextUShort<false>(25, &battleAdvances);
        }

        if (!cuteCharmFlag)
        {
            u32 pid;
            do
            {
                u16 low = go.nextUShort(&battleAdvances);
                u16 high = go.nextUShort(&battleAdvances);
                pid = (high << 16) | low;
            } while (pid % 25 != nature);
        }
    }
    else
    {
        auto cuteCharmCheck = [effectiveLead](const PersonalInfo *info, u32 pid) {
            if (effectiveLead == Lead::CuteCharmF)
            {
                return (pid & 0xff) >= info->getGender();
            }
            return (pid & 0xff) < info->getGender();
        };

        auto shinyPID = [this, &go, &battleAdvances]() {
            u16 low = go.nextUShort(&battleAdvances) & 7;
            u16 high = go.nextUShort(&battleAdvances) & 7;
            u16 shinyValue = tsv >> 3;
            for (int i = 0; i < 13; i++)
            {
                u16 pidBit = 1 << (i + 3);
                if (shinyValue & (1 << i))
                {
                    if (go.nextUShort(&battleAdvances) & 1)
                    {
                        low |= pidBit;
                    }
                    else
                    {
                        high |= pidBit;
                    }
                }
                else if (go.nextUShort(&battleAdvances) & 1)
                {
                    low |= pidBit;
                    high |= pidBit;
                }
            }
            return static_cast<u32>((high << 16) | low);
        };

        u32 pid;
        if (cuteCharm && go.nextUShort<false>(3, &battleAdvances) != 0)
        {
            do
            {
                pid = shinyPID();
            } while (!cuteCharmCheck(info, pid));
        }
        else if (effectiveLead <= Lead::SynchronizeEnd && go.nextUShort<false>(2, &battleAdvances) == 0)
        {
            do
            {
                pid = shinyPID();
            } while (pid % 25 != toInt(effectiveLead));
        }
        else
        {
            shinyPID();
        }
    }

    go.nextUShort(&battleAdvances);
    go.nextUShort(&battleAdvances);
    go.nextUShort(100, &battleAdvances);

    return battleAdvances;
}

void PokeRadarSearcher::cancelSearch()
{
    SearcherBase<PokeRadarState>::cancelSearch();
}

int PokeRadarSearcher::getProgress() const
{
    if (maxProgress == 0)
    {
        return 100;
    }

    u64 currentProgress = 0;
    currentProgress = currentPhaseProgress;

    u64 value = ((progress + currentProgress) * 100) / maxProgress;
    if (value > 100)
    {
        value = 100;
    }

    return static_cast<int>(value);
}

void PokeRadarSearcher::addPatchMatches(const WildSearcherState4 &pokemon, u16 chainMin, u16 chainMax, PokeRadarChainType searchChainType)
{
    if (chainMin != 0 && result != PokeRadarResult::ManualActivation)
    {
        addPostBattlePatchMatches(pokemon, chainMin, chainMax, searchChainType);
        return;
    }

    addManualPatchMatches(pokemon, chainMin, chainMax, searchChainType);
}

void PokeRadarSearcher::addManualPatchMatches(const WildSearcherState4 &pokemon, u16 chainMin, u16 chainMax, PokeRadarChainType searchChainType)
{
    if (pokemon.getAdvances() < minPatchDistance)
    {
        return;
    }

    u32 end = pokemon.getAdvances() - minPatchDistance;
    u32 maxPatchMatches = chainMin != 0 && isShinyPatchType(searchChainType) ? 5 : 1;

    for (u32 patchAdvances = end;; patchAdvances--)
    {
        if (!searching)
        {
            return;
        }

        for (u16 chain = chainMin; chain <= chainMax; chain++)
        {
            if (!searching)
            {
                return;
            }

            PokeRadarGenerator radar(patchAdvances, 0, chain, searchChainType, PokeRadarResult::ManualActivation, grass);
            PokeRadarState patchState = radar.generate(pokemon.getSeed()).front();
            if (patchMatchesType(patchState, searchChainType))
            {
                auto [noGraceSkip, graceSkip] = PokeRadarGenerator::getSkips(pokemon.getSeed(), pokemon.getAdvances());
                if (noGraceSkip != 0 && graceSkip != 0)
                {
                    return;
                }

                PokeRNG rng(pokemon.getSeed(), pokemon.getAdvances());
                std::vector<u32> targetPatchAdvances = { patchState.getAdvances() };
                for (u32 nextPatchAdvances = patchAdvances == 0 ? 0 : patchAdvances - 1;
                     targetPatchAdvances.size() < maxPatchMatches && nextPatchAdvances < patchAdvances;)
                {
                    if (!searching)
                    {
                        return;
                    }

                    PokeRadarGenerator nextRadar(nextPatchAdvances, 0, chain, searchChainType, PokeRadarResult::ManualActivation, grass);
                    PokeRadarState nextPatchState = nextRadar.generate(pokemon.getSeed()).front();
                    if (patchMatchesType(nextPatchState, searchChainType)
                        && std::ranges::find(targetPatchAdvances, nextPatchState.getAdvances()) == targetPatchAdvances.end())
                    {
                        targetPatchAdvances.emplace_back(nextPatchState.getAdvances());
                    }

                    if (nextPatchAdvances == 0)
                    {
                        break;
                    }
                    nextPatchAdvances--;
                }

                std::lock_guard<std::mutex> guard(mutex);
                auto key = std::make_tuple(pokemon.getSeed(), pokemon.getAdvances(), patchState.getAdvances(), chain, pokemon.getEncounterSlot(),
                                           pokemon.getPID());
                if (!resultKeys.insert(key).second)
                {
                    return;
                }

                PokeRadarState result(patchState, pokemon, rng.nextUShort(), chain);
                result.setDisplayPatchType(searchChainType == PokeRadarChainType::Strong || searchChainType == PokeRadarChainType::StrongShiny,
                                           isShinyPatchType(searchChainType));
                result.setSkip(noGraceSkip, graceSkip);
                if (targetPatchAdvances.size() > 1)
                {
                    std::ranges::sort(targetPatchAdvances);
                    result.setBattleStartAdvances(targetPatchAdvances);
                }
                results.emplace_back(result);
                return;
            }
        }

        if (patchAdvances == 0)
        {
            break;
        }
    }
}

void PokeRadarSearcher::addPostBattlePatchMatches(const WildSearcherState4 &pokemon, u16 chainMin, u16 chainMax, PokeRadarChainType searchChainType)
{
    auto [noGraceSkip, graceSkip] = PokeRadarGenerator::getSkips(pokemon.getSeed(), pokemon.getAdvances());
    if (noGraceSkip != 0 && graceSkip != 0)
    {
        return;
    }

    for (u16 chain = chainMin; chain <= chainMax; chain++)
    {
        if (!searching)
        {
            return;
        }

        const auto &patches = getPostBattlePatches(pokemon.getSeed(), chain, searchChainType);
        const PostBattlePatch *bestPatch = nullptr;
        std::vector<u32> bestBattleStartAdvances;
        u32 bestDistance = std::numeric_limits<u32>::max();
        struct Candidate
        {
            const PostBattlePatch *patch;
            u32 distance;
            u32 battleStartAdvance;
        };
        std::vector<Candidate> candidates;

        for (const auto &patch : patches)
        {
            for (const auto &battleStart : patch.battleStarts)
            {
                if (battleStart.postBattleAdvance > pokemon.getAdvances())
                {
                    continue;
                }

                u32 currentDistance = pokemon.getAdvances() - battleStart.postBattleAdvance;
                if (currentDistance < minPatchDistance)
                {
                    continue;
                }

                candidates.emplace_back(Candidate { &patch, currentDistance, battleStart.startAdvance });
            }
        }

        if (candidates.empty())
        {
            continue;
        }

        std::ranges::sort(candidates, [](const Candidate &left, const Candidate &right) {
            if (left.distance != right.distance)
            {
                return left.distance < right.distance;
            }
            return left.patch->state.getAdvances() > right.patch->state.getAdvances();
        });

        bestPatch = candidates.front().patch;
        bestDistance = candidates.front().distance;
        for (const auto &candidate : candidates)
        {
            if (std::ranges::find(bestBattleStartAdvances, candidate.battleStartAdvance) == bestBattleStartAdvances.end())
            {
                bestBattleStartAdvances.emplace_back(candidate.battleStartAdvance);
                if (bestBattleStartAdvances.size() == 5)
                {
                    break;
                }
            }
        }
        std::ranges::sort(bestBattleStartAdvances);

        PokeRNG rng(pokemon.getSeed(), pokemon.getAdvances());
        std::lock_guard<std::mutex> guard(mutex);
        auto key = std::make_tuple(pokemon.getSeed(), pokemon.getAdvances(), bestPatch->state.getAdvances(), chain, pokemon.getEncounterSlot(),
                                   pokemon.getPID());
        if (!resultKeys.insert(key).second)
        {
            return;
        }

        PokeRadarState state(bestPatch->state, pokemon, rng.nextUShort(), chain);
        state.setDisplayPatchType(searchChainType == PokeRadarChainType::Strong || searchChainType == PokeRadarChainType::StrongShiny,
                                  isShinyPatchType(searchChainType));
        state.setSkip(noGraceSkip, graceSkip);
        state.setDistance(bestDistance);
        state.setBattleStartAdvances(bestBattleStartAdvances);
        results.emplace_back(state);
        return;
    }
}

const std::vector<PokeRadarSearcher::PostBattlePatch> &PokeRadarSearcher::getPostBattlePatches(u32 seed, u16 chain,
                                                                                               PokeRadarChainType searchChainType)
{
    u64 key = (static_cast<u64>(seed) << 24) | (static_cast<u64>(chain) << 8) | static_cast<u8>(searchChainType);
    auto cached = postBattlePatches.find(key);
    if (cached != postBattlePatches.end())
    {
        return cached->second;
    }

    Lead effectiveLead = getPokeRadarSearcherLead(lead, true);
    PokeRadarGenerator radar(0, 0, chain, searchChainType, result, grass);
    std::unordered_map<u32, size_t> indexByPatchAdvance;
    std::vector<PostBattlePatch> patches;
    for (u32 advances = 0; advances <= maxAdvance; advances++)
    {
        if (!searching)
        {
            break;
        }

        u32 battleAdvances = calculatePokemonBattleAdvances(seed, advances, effectiveLead, false);
        u32 battlePatchAdvances = getPostBattlePatchAdvances(battleAdvances, profile.getVersion());
        u32 battlePatchTypeAdvances = getPostBattlePatchTypeAdvances(profile.getVersion());
        PokeRadarState patchState = radar.generatePrevious(seed, battlePatchAdvances, battlePatchTypeAdvances);
        if (!patchMatchesType(patchState, searchChainType))
        {
            continue;
        }

        u32 postBattleAdvance = battleAdvances + 4 + radar.getPostBattleAdvanceConsumption(patchState.getPatches());
        u32 displayAdvance = advances == 0 ? 0 : advances - 1;
        u32 patchAdvances = patchState.getAdvances();
        auto found = indexByPatchAdvance.find(patchAdvances);
        if (found == indexByPatchAdvance.end())
        {
            indexByPatchAdvance.emplace(patchAdvances, patches.size());
            patches.emplace_back(PostBattlePatch { patchState, { { displayAdvance, postBattleAdvance } } });
        }
        else
        {
            patches[found->second].battleStarts.emplace_back(BattleStart { displayAdvance, postBattleAdvance });
        }
    }

    std::ranges::sort(patches, [](const PostBattlePatch &left, const PostBattlePatch &right) {
        return left.state.getAdvances() > right.state.getAdvances();
    });
    for (auto &patch : patches)
    {
        std::ranges::sort(patch.battleStarts, [](const BattleStart &left, const BattleStart &right) {
            return left.startAdvance < right.startAdvance;
        });
    }

    auto inserted = postBattlePatches.emplace(key, std::move(patches)).first;
    return inserted->second;
}

bool PokeRadarSearcher::isShinyPatchType(PokeRadarChainType searchChainType) const
{
    return searchChainType == PokeRadarChainType::WeakShiny || searchChainType == PokeRadarChainType::StrongShiny;
}

bool PokeRadarSearcher::patchMatchesType(const PokeRadarState &state, PokeRadarChainType searchChainType) const
{
    bool wantedStrong = searchChainType == PokeRadarChainType::Strong || searchChainType == PokeRadarChainType::StrongShiny;
    bool wantedShiny = isShinyPatchType(searchChainType);

    for (const auto &patch : state.getPatches())
    {
        if (patch.active && patch.strong == wantedStrong && patch.shiny == wantedShiny)
        {
            return true;
        }
    }

    return false;
}
