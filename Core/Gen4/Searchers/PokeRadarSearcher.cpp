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
#include <Core/RNG/LCRNG.hpp>
#include <mutex>
#include <vector>

PokeRadarSearcher::PokeRadarSearcher(u32 minAdvance, u32 maxAdvance, u32 minDelay, u32 maxDelay, u32 minPatchDistance,
                                     u32 maxPatchDistance, u16 maxChain, u8 chainSlot, Lead lead, PokeRadarChainType chainType,
                                     const std::array<bool, 81> &grass, const std::array<bool, 12> &encounterSlots,
                                     const EncounterArea4 &area, const Profile4 &profile, const WildStateFilter &filter) :
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
    chainType(chainType),
    grass(grass),
    encounterSlots(encounterSlots),
    area(area),
    filter(filter)
{
    setMaxProgress(static_cast<u64>(maxDelay - minDelay + 1) * 24);
}

void PokeRadarSearcher::startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max)
{
    searching = true;
    (void)min;
    (void)max;

    for (u8 hour = 0; hour < 24; hour++)
    {
        for (u32 delay = minDelay; delay <= maxDelay; delay++)
        {
            if (!searching)
            {
                return;
            }

            u32 seed = (hour << 16) | delay;

            for (const auto &pokemon : generatePokemon(seed, false))
            {
                if (!searching)
                {
                    return;
                }

                addPatchMatches(pokemon, seed, 0, 0);
            }

            if (maxChain != 0)
            {
                for (const auto &pokemon : generatePokemon(seed, true))
                {
                    if (!searching)
                    {
                        return;
                    }

                    addPatchMatches(pokemon, seed, 1, maxChain);
                }
            }

            progress++;
        }
    }
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

    return static_cast<int>((progress * 100) / maxProgress);
}

std::vector<WildGeneratorState4> PokeRadarSearcher::generatePokemon(u32 seed, bool chain) const
{
    std::vector<WildGeneratorState4> pokemon;
    for (u8 offset = 1; offset <= 3; offset++)
    {
        WildGenerator4 generator(minAdvance, maxAdvance, offset, Method::PokeRadar, lead, false, isShinyPatchType(), false, 0, area, profile, filter);
        auto states = chain ? generator.generate(seed, chainSlot)
                            : isShinyPatchType() ? generator.generatePokeRadarShiny(seed) : generator.generatePokeRadar(seed);
        pokemon.insert(pokemon.end(), states.begin(), states.end());
    }

    return pokemon;
}

void PokeRadarSearcher::addPatchMatches(const WildGeneratorState4 &pokemon, u32 seed, u16 chainMin, u16 chainMax)
{
    if (pokemon.getAdvances() < minPatchDistance)
    {
        return;
    }

    u32 start = pokemon.getAdvances() > maxPatchDistance ? pokemon.getAdvances() - maxPatchDistance : 0;
    u32 end = pokemon.getAdvances() - minPatchDistance;

    for (u32 patchAdvances = start; patchAdvances <= end; patchAdvances++)
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
            PokeRadarState patchState = radar.generate(seed).front();
            if (patchMatchesType(patchState))
            {
                const Slot &slot = area.getPokemon(pokemon.getEncounterSlot());
                WildSearcherState4 actual(seed, pokemon.getPID(), pokemon.getIVs(), pokemon.getAbility(), pokemon.getGender(),
                                          pokemon.getLevel(), pokemon.getNature(), pokemon.getShiny(), pokemon.getEncounterSlot(),
                                          pokemon.getItem(), pokemon.getSpecie(), pokemon.getForm(), slot.getInfo());
                actual.setAdvances(pokemon.getAdvances());

                PokeRNG rng(seed, pokemon.getAdvances());
                std::lock_guard<std::mutex> guard(mutex);
                auto key = std::make_tuple(seed, pokemon.getAdvances(), patchState.getAdvances(), chain, pokemon.getEncounterSlot(),
                                           pokemon.getPID());
                if (!resultKeys.insert(key).second)
                {
                    break;
                }

                PokeRadarState result(patchState, actual, rng.nextUShort(), chain);
                auto [noGraceSkip, graceSkip] = PokeRadarGenerator::getSkips(seed, result.getAdvances());
                result.setSkip(noGraceSkip, graceSkip);
                results.emplace_back(result);
                break;
            }
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
