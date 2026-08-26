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
#include <atomic>
#include <mutex>
#include <optional>
#include <set>
#include <tuple>
#include <unordered_map>
#include <vector>

enum class Lead : u8;
class WildSearcherState4;

class PokeRadarSearcher : public Searcher<Profile4, PokeRadarState>
{
public:
    PokeRadarSearcher(u32 minAdvance, u32 maxAdvance, u32 minDelay, u32 maxDelay, u32 minPatchDistance, u16 maxChain,
                      u8 chainSlot, Lead lead, PokeRadarChainType chainType, PokeRadarResult result,
                      const std::array<bool, 81> &grass, const std::array<bool, 12> &encounterSlots, const EncounterArea4 &area,
                      const Profile4 &profile, const WildStateFilter &filter, bool specificSynchronize = false);

    void startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max);
    void cancelSearch();
    int getProgress() const;

private:
    struct BattleStart
    {
        u32 startAdvance;
        u32 postBattleAdvance;
    };

    struct PostBattlePatch
    {
        PokeRadarState state;
        std::vector<BattleStart> battleStarts;
    };

    void addPatchMatches(const WildSearcherState4 &pokemon, u16 chainMin, u16 chainMax, PokeRadarChainType searchChainType);
    void addManualPatchMatches(const WildSearcherState4 &pokemon, u16 chainMin, u16 chainMax, PokeRadarChainType searchChainType);
    void addPostBattlePatchMatches(const WildSearcherState4 &pokemon, u16 chainMin, u16 chainMax, PokeRadarChainType searchChainType);
    const std::vector<PostBattlePatch> &getPostBattlePatches(u32 seed, u16 chain, PokeRadarChainType searchChainType);
    u32 calculatePokemonBattleAdvances(u32 seed, u32 advances, Lead effectiveLead, bool shiny) const;
    std::vector<WildSearcherState4> searchInitialSeeds(const std::vector<WildSearcherState4> &states, u32 displayAdvanceOffset = 0) const;
    void searchPokemon(const std::array<u8, 6> &min, const std::array<u8, 6> &max, bool chain);
    std::vector<WildSearcherState4> searchPokemonIVs(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 index, Lead effectiveLead,
                                                     bool shiny, bool applyFilter) const;
    std::vector<WildSearcherState4> searchPokemonNormalIVs(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 index,
                                                           Lead effectiveLead, bool applyFilter) const;
    std::vector<WildSearcherState4> searchPokemonShinyIVs(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 index,
                                                          Lead effectiveLead, bool applyFilter) const;
    std::optional<WildSearcherState4> validateChainZeroPokemon(const WildSearcherState4 &pokemon, bool allSlots) const;
    bool isShinyPatchType(PokeRadarChainType searchChainType) const;
    bool patchMatchesType(const PokeRadarState &state, PokeRadarChainType searchChainType) const;

    u32 minAdvance;
    u32 maxAdvance;
    u32 minDelay;
    u32 maxDelay;
    u32 minPatchDistance;
    u16 maxChain;
    u8 chainSlot;
    Lead lead;
    bool specificSynchronize;
    PokeRadarChainType chainType;
    PokeRadarResult result;
    std::array<bool, 81> grass;
    std::array<bool, 12> encounterSlots;
    EncounterArea4 area;
    WildStateFilter filter;
    std::atomic<u64> currentPhaseProgress;
    std::unordered_map<u64, std::vector<PostBattlePatch>> postBattlePatches;
    std::set<std::tuple<u32, u32, u32, u16, u8, u32>> resultKeys;
};

#endif // POKERADARSEARCHER_HPP
