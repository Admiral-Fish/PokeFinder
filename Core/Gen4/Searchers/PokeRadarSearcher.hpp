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

#ifndef POKERADARSEARCHER_HPP
#define POKERADARSEARCHER_HPP

#include <Core/Gen4/EncounterArea4.hpp>
#include <Core/Gen4/Generators/PokeRadarGenerator.hpp>
#include <Core/Gen4/Profile4.hpp>
#include <Core/Gen4/States/PokeRadarState.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/Searchers/Searcher.hpp>
#include <array>
#include <set>
#include <tuple>
#include <vector>

enum class Lead : u8;

class PokeRadarSearcher : public Searcher<Profile4, PokeRadarState>
{
public:
    PokeRadarSearcher(u32 minAdvance, u32 maxAdvance, u32 minDelay, u32 maxDelay, u32 minPatchDistance, u32 maxPatchDistance,
                      u16 maxChain, u8 chainSlot, Lead lead, PokeRadarChainType chainType, const std::array<bool, 81> &grass,
                      const std::array<bool, 12> &encounterSlots, const EncounterArea4 &area, const Profile4 &profile,
                      const WildStateFilter &filter);

    void startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max);
    void cancelSearch();
    int getProgress() const;

private:
    void addPatchMatches(const WildGeneratorState4 &pokemon, u32 seed, u16 chainMin, u16 chainMax);
    std::vector<WildGeneratorState4> generatePokemon(u32 seed, bool chain) const;
    bool isShinyPatchType() const;
    bool patchMatchesType(const PokeRadarState &state) const;

    u32 minAdvance;
    u32 maxAdvance;
    u32 minDelay;
    u32 maxDelay;
    u32 minPatchDistance;
    u32 maxPatchDistance;
    u16 maxChain;
    u8 chainSlot;
    Lead lead;
    PokeRadarChainType chainType;
    std::array<bool, 81> grass;
    std::array<bool, 12> encounterSlots;
    EncounterArea4 area;
    WildStateFilter filter;
    std::set<std::tuple<u32, u32, u32, u16, u8, u32>> resultKeys;
};

#endif // POKERADARSEARCHER_HPP
