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
#include <Core/Gen4/Searchers/WildSearcher4.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <mutex>
#include <vector>

static bool isPokeRadarChainLead(Lead lead)
{
    return lead <= Lead::SynchronizeEnd || lead == Lead::CuteCharmF || lead == Lead::CuteCharmM;
}

static Lead getPokeRadarSearcherLead(Lead lead, bool chain)
{
    return !chain || isPokeRadarChainLead(lead) ? lead : Lead::None;
}

PokeRadarSearcher::PokeRadarSearcher(u32 minAdvance, u32 maxAdvance, u32 minDelay, u32 maxDelay, u32 minPatchDistance,
                                     u32 maxPatchDistance, u16 maxChain, u8 chainSlot, Lead lead, PokeRadarChainType chainType,
                                     const std::array<bool, 81> &grass, const std::array<bool, 12> &encounterSlots,
                                     const EncounterArea4 &area, const Profile4 &profile, const WildStateFilter &filter,
                                     bool specificSynchronize) :
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
            addPatchMatches(pokemon[i], chain ? 1 : 0, chain ? maxChain : 0);
        }

        currentPhaseProgress = 0;
        progress += 100;
        if (progress > maxProgress)
        {
            progress = maxProgress;
        }
    }

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
