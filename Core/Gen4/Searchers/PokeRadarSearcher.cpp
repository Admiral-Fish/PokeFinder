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

#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen4/Generators/WildGenerator4.hpp>
#include <Core/Gen4/Searchers/WildSearcher4.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/Util/EncounterSlot.hpp>
#include <Core/Util/Utilities.hpp>
#include <algorithm>
#include <limits>
#include <mutex>
#include <unordered_map>
#include <vector>

static bool isPokeRadarChainLead(Lead lead)
{
    return lead <= Lead::SynchronizeEnd || lead == Lead::CuteCharmF || lead == Lead::CuteCharmM;
}

static Lead getPokeRadarSearcherLead(Lead lead, bool chain)
{
    return !chain || isPokeRadarChainLead(lead) ? lead : Lead::None;
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
                                     u32 maxPatchDistance, u16 maxChain, u8 chainSlot, Lead lead, PokeRadarChainType chainType,
                                     PokeRadarResult result, const std::array<bool, 81> &grass,
                                     const std::array<bool, 12> &encounterSlots, const EncounterArea4 &area, const Profile4 &profile,
                                     const WildStateFilter &filter, bool specificSynchronize) :
    Searcher(Method::PokeRadar, profile),
    minAdvance(minAdvance),
    maxAdvance(maxAdvance),
    minDelay(minDelay),
    maxDelay(maxDelay),
    minPatchDistance(minPatchDistance),
    maxPatchDistance(maxPatchDistance),
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
    currentPhaseProgress(0),
    currentSearcher(nullptr)
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

    WildSearcher4 wildSearcher(minAdvance, maxAdvance, minDelay, maxDelay, Method::PokeRadar, effectiveLead, false, isShinyPatchType(),
                               false, 0, area, profile, filter, specificSynchronize && effectiveLead <= Lead::SynchronizeEnd);

    u8 startSlot = chain ? chainSlot : 0;
    u8 endSlot = chain ? chainSlot + 1 : static_cast<u8>(encounterSlots.size());
    for (u8 slot = startSlot; slot < endSlot; slot++)
    {
        if (!searching)
        {
            return;
        }

        if (!allSlots && !encounterSlots[slot])
        {
            continue;
        }

        currentPhaseProgress = 0;
        wildSearcher.setMaxProgress(ivCombinations);
        {
            std::lock_guard<std::mutex> guard(currentSearcherMutex);
            currentSearcher = &wildSearcher;
        }
        wildSearcher.startSearch(min, max, slot);
        {
            std::lock_guard<std::mutex> guard(currentSearcherMutex);
            currentSearcher = nullptr;
        }

        progress += 100;
        auto pokemon = wildSearcher.getResults();
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
                    addPatchMatches(*state, 0, 0);
                }
            }
            else
            {
                addPatchMatches(pokemon[i], 1, maxChain);
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

std::optional<WildSearcherState4> PokeRadarSearcher::validateChainZeroPokemon(const WildSearcherState4 &pokemon, bool allSlots) const
{
    if (pokemon.getAdvances() == 0)
    {
        return std::nullopt;
    }

    PokeRNG go(pokemon.getSeed(), pokemon.getAdvances());

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
    state.setAdvances(pokemon.getAdvances() - 1);

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

void PokeRadarSearcher::cancelSearch()
{
    SearcherBase<PokeRadarState>::cancelSearch();
    std::lock_guard<std::mutex> guard(currentSearcherMutex);
    if (currentSearcher != nullptr)
    {
        currentSearcher->cancelSearch();
    }
}

int PokeRadarSearcher::getProgress() const
{
    if (maxProgress == 0)
    {
        return 100;
    }

    u64 currentProgress = 0;
    {
        std::lock_guard<std::mutex> guard(currentSearcherMutex);
        if (currentSearcher != nullptr)
        {
            currentProgress = currentSearcher->getProgress();
        }
    }
    if (currentProgress == 0)
    {
        currentProgress = currentPhaseProgress;
    }

    u64 value = ((progress + currentProgress) * 100) / maxProgress;
    if (value > 100)
    {
        value = 100;
    }

    return static_cast<int>(value);
}

void PokeRadarSearcher::addPatchMatches(const WildSearcherState4 &pokemon, u16 chainMin, u16 chainMax)
{
    if (chainMin != 0 && result != PokeRadarResult::ManualActivation)
    {
        addPostBattlePatchMatches(pokemon, chainMin, chainMax);
        return;
    }

    addManualPatchMatches(pokemon, chainMin, chainMax);
}

void PokeRadarSearcher::addManualPatchMatches(const WildSearcherState4 &pokemon, u16 chainMin, u16 chainMax)
{
    if (pokemon.getAdvances() < minPatchDistance)
    {
        return;
    }

    u32 start = pokemon.getAdvances() > maxPatchDistance ? pokemon.getAdvances() - maxPatchDistance : 0;
    u32 end = pokemon.getAdvances() - minPatchDistance;

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

            PokeRadarGenerator radar(patchAdvances, 0, chain, chainType, PokeRadarResult::ManualActivation, grass);
            PokeRadarState patchState = radar.generate(pokemon.getSeed()).front();
            if (patchMatchesType(patchState))
            {
                auto [noGraceSkip, graceSkip] = PokeRadarGenerator::getSkips(pokemon.getSeed(), pokemon.getAdvances());
                if (noGraceSkip != 0 && graceSkip != 0)
                {
                    return;
                }

                PokeRNG rng(pokemon.getSeed(), pokemon.getAdvances());
                std::lock_guard<std::mutex> guard(mutex);
                auto key = std::make_tuple(pokemon.getSeed(), pokemon.getAdvances(), patchState.getAdvances(), chain, pokemon.getEncounterSlot(),
                                           pokemon.getPID());
                if (!resultKeys.insert(key).second)
                {
                    return;
                }

                PokeRadarState result(patchState, pokemon, rng.nextUShort(), chain);
                result.setDisplayPatchType(chainType == PokeRadarChainType::Strong || chainType == PokeRadarChainType::StrongShiny,
                                           isShinyPatchType());
                result.setSkip(noGraceSkip, graceSkip);
                results.emplace_back(result);
                return;
            }
        }

        if (patchAdvances == start)
        {
            break;
        }
    }
}

void PokeRadarSearcher::addPostBattlePatchMatches(const WildSearcherState4 &pokemon, u16 chainMin, u16 chainMax)
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

        const auto &patches = getPostBattlePatches(pokemon.getSeed(), chain);
        const PostBattlePatch *bestPatch = nullptr;
        std::vector<u32> bestBattleStartAdvances;
        u32 bestDistance = std::numeric_limits<u32>::max();

        for (const auto &patch : patches)
        {
            std::vector<u32> battleStartAdvances;
            u32 distance = std::numeric_limits<u32>::max();
            for (const auto &battleStart : patch.battleStarts)
            {
                if (battleStart.postBattleAdvance > pokemon.getAdvances())
                {
                    continue;
                }

                u32 currentDistance = pokemon.getAdvances() - battleStart.postBattleAdvance;
                if (currentDistance < minPatchDistance || currentDistance > maxPatchDistance)
                {
                    continue;
                }

                distance = std::min(distance, currentDistance);
                battleStartAdvances.emplace_back(battleStart.startAdvance);
            }

            if (!battleStartAdvances.empty() && distance < bestDistance)
            {
                bestPatch = &patch;
                bestBattleStartAdvances = std::move(battleStartAdvances);
                bestDistance = distance;
            }
        }

        if (bestPatch == nullptr)
        {
            continue;
        }

        PokeRNG rng(pokemon.getSeed(), pokemon.getAdvances());
        std::lock_guard<std::mutex> guard(mutex);
        auto key = std::make_tuple(pokemon.getSeed(), pokemon.getAdvances(), bestPatch->state.getAdvances(), chain, pokemon.getEncounterSlot(),
                                   pokemon.getPID());
        if (!resultKeys.insert(key).second)
        {
            return;
        }

        PokeRadarState state(bestPatch->state, pokemon, rng.nextUShort(), chain);
        state.setDisplayPatchType(chainType == PokeRadarChainType::Strong || chainType == PokeRadarChainType::StrongShiny,
                                  isShinyPatchType());
        state.setSkip(noGraceSkip, graceSkip);
        state.setDistance(bestDistance);
        state.setBattleStartAdvances(bestBattleStartAdvances);
        results.emplace_back(state);
        return;
    }
}

const std::vector<PokeRadarSearcher::PostBattlePatch> &PokeRadarSearcher::getPostBattlePatches(u32 seed, u16 chain)
{
    u64 key = (static_cast<u64>(seed) << 16) | chain;
    auto cached = postBattlePatches.find(key);
    if (cached != postBattlePatches.end())
    {
        return cached->second;
    }

    std::array<u8, 6> minIVs = {};
    std::array<u8, 6> maxIVs = { 31, 31, 31, 31, 31, 31 };
    std::array<bool, 25> natures;
    natures.fill(true);
    std::array<bool, 16> powers;
    powers.fill(true);
    std::array<bool, 12> slots;
    slots.fill(true);
    WildStateFilter noFilter(255, 255, 255, 1, 100, 0, 255, 0, 255, true, minIVs, maxIVs, natures, powers, slots);

    Lead effectiveLead = getPokeRadarSearcherLead(lead, true);
    WildGenerator4 generator(0, maxAdvance, 1, Method::PokeRadar, effectiveLead, false, isShinyPatchType(), false, 0, area, profile, noFilter);
    auto battleStates = generator.generate(seed, chainSlot);

    PokeRadarGenerator radar(0, 0, chain, chainType, result, grass);
    std::unordered_map<u32, size_t> indexByPatchAdvance;
    std::vector<PostBattlePatch> patches;
    for (const auto &battleState : battleStates)
    {
        if (!searching)
        {
            break;
        }

        PokeRadarState patchState = radar.generatePrevious(seed, battleState.getBattleAdvances());
        if (!patchMatchesType(patchState))
        {
            continue;
        }

        u32 patchAdvances = patchState.getAdvances();
        u32 postBattleAdvance = battleState.getBattleAdvances() + 4 + radar.getPostBattleAdvanceConsumption(patchState.getPatches());
        auto found = indexByPatchAdvance.find(patchAdvances);
        if (found == indexByPatchAdvance.end())
        {
            indexByPatchAdvance.emplace(patchAdvances, patches.size());
            patches.emplace_back(PostBattlePatch { patchState, { { battleState.getAdvances(), postBattleAdvance } } });
        }
        else
        {
            patches[found->second].battleStarts.emplace_back(BattleStart { battleState.getAdvances(), postBattleAdvance });
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

bool PokeRadarSearcher::isShinyPatchType() const
{
    return chainType == PokeRadarChainType::RegularShiny || chainType == PokeRadarChainType::StrongShiny;
}

bool PokeRadarSearcher::patchMatchesType(const PokeRadarState &state) const
{
    bool wantedStrong = chainType == PokeRadarChainType::Strong || chainType == PokeRadarChainType::StrongShiny;
    bool wantedShiny = isShinyPatchType();

    for (const auto &patch : state.getPatches())
    {
        if (patch.active && patch.strong == wantedStrong && patch.shiny == wantedShiny)
        {
            return true;
        }
    }

    return false;
}
