/*
 * This file is part of PokéFinder
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

#ifndef POKERADARSTATE_HPP
#define POKERADARSTATE_HPP

#include <Core/Gen4/States/WildState4.hpp>
#include <Core/Global.hpp>
#include <algorithm>
#include <array>
#include <optional>
#include <vector>

struct PokeRadarPatch
{
    u8 x;
    u8 y;
    u8 ring;
    bool active;
    bool strong;
    bool shiny;
    bool continueChain;
};

enum class PokeRadarResult : u8
{
    Defeat,
    Capture,
    ManualActivation
};

/**
 * @brief State class for Gen4 Poke Radar patch generation
 */
class PokeRadarState
{
public:
    PokeRadarState(u16 prng, u32 advances, const std::array<PokeRadarPatch, 4> &patches) :
        advances(advances),
        patchAdvances(advances),
        distance(0),
        chain(0),
        call(prng % 3),
        chatot(((prng % 8192) * 100) >> 13),
        noGraceSkip(0),
        graceSkip(0),
        displayBattleAdvances(false),
        battleAdvances(0),
        displayedPatchAdvances(0),
        displayPatchType(false),
        displayPatchStrong(false),
        displayPatchShiny(false),
        patchesVisible(true),
        battlePatchesVisible(true),
        patches(patches),
        battlePatches(patches)
    {
    }

    PokeRadarState(const PokeRadarState &state, const WildGeneratorState4 &pokemon) :
        advances(state.advances),
        patchAdvances(state.patchAdvances),
        distance(state.distance),
        chain(state.chain),
        call(state.call),
        chatot(state.chatot),
        noGraceSkip(state.noGraceSkip),
        graceSkip(state.graceSkip),
        displayBattleAdvances(state.displayBattleAdvances),
        battleAdvances(state.battleAdvances),
        displayedPatchAdvances(state.displayedPatchAdvances),
        displayPatchType(state.displayPatchType),
        displayPatchStrong(state.displayPatchStrong),
        displayPatchShiny(state.displayPatchShiny),
        patchesVisible(state.patchesVisible),
        battlePatchesVisible(state.battlePatchesVisible),
        results(state.results),
        resultPatches(state.resultPatches),
        patches(state.patches),
        battlePatches(state.battlePatches),
        pokemon(pokemon)
    {
    }

    PokeRadarState(const PokeRadarState &state, const WildGeneratorState4 &pokemon, u16 chain) :
        advances(pokemon.getAdvances()),
        patchAdvances(state.advances),
        distance(pokemon.getAdvances() - state.advances),
        chain(chain),
        call(pokemon.getCall()),
        chatot(pokemon.getChatot()),
        noGraceSkip(state.noGraceSkip),
        graceSkip(state.graceSkip),
        displayBattleAdvances(state.displayBattleAdvances),
        battleAdvances(state.battleAdvances),
        displayedPatchAdvances(state.displayedPatchAdvances),
        displayPatchType(state.displayPatchType),
        displayPatchStrong(state.displayPatchStrong),
        displayPatchShiny(state.displayPatchShiny),
        patchesVisible(state.patchesVisible),
        battlePatchesVisible(state.battlePatchesVisible),
        results(state.results),
        resultPatches(state.resultPatches),
        patches(state.patches),
        battlePatches(state.battlePatches),
        pokemon(pokemon)
    {
    }

    PokeRadarState(const PokeRadarState &state, const WildSearcherState4 &pokemon, u16 prng, u16 chain) :
        advances(pokemon.getAdvances()),
        patchAdvances(state.advances),
        distance(pokemon.getAdvances() - state.advances),
        chain(chain),
        call(prng % 3),
        chatot(((prng % 8192) * 100) >> 13),
        noGraceSkip(state.noGraceSkip),
        graceSkip(state.graceSkip),
        displayBattleAdvances(state.displayBattleAdvances),
        battleAdvances(state.battleAdvances),
        displayedPatchAdvances(state.displayedPatchAdvances),
        displayPatchType(state.displayPatchType),
        displayPatchStrong(state.displayPatchStrong),
        displayPatchShiny(state.displayPatchShiny),
        patchesVisible(state.patchesVisible),
        battlePatchesVisible(state.battlePatchesVisible),
        results(state.results),
        resultPatches(state.resultPatches),
        patches(state.patches),
        battlePatches(state.battlePatches),
        searcherPokemon(pokemon)
    {
    }

    bool getPatchesVisible() const
    {
        return patchesVisible;
    }

    void setPatchesVisible(bool visible)
    {
        patchesVisible = visible;
    }

    bool getBattlePatchesVisible() const
    {
        return battlePatchesVisible;
    }

    void setBattlePatchesVisible(bool visible)
    {
        battlePatchesVisible = visible;
    }

    u32 getAdvances() const
    {
        return advances;
    }

    u32 getPatchAdvances() const
    {
        return patchAdvances;
    }

    const std::vector<u32> &getBattleStartAdvances() const
    {
        return battleStartAdvances;
    }

    bool hasBattleStartAdvances() const
    {
        return !battleStartAdvances.empty();
    }

    const std::vector<PokeRadarResult> &getResults() const
    {
        return results;
    }

    void setResults(const std::vector<PokeRadarResult> &results)
    {
        this->results = results;
    }

    void addResult(PokeRadarResult result)
    {
        if (std::ranges::find(results, result) == results.end())
        {
            results.emplace_back(result);
        }
    }

    const std::optional<std::array<PokeRadarPatch, 4>> &getResultPatches(PokeRadarResult result) const
    {
        return resultPatches[static_cast<size_t>(result)];
    }

    void setResultPatches(PokeRadarResult result, const std::array<PokeRadarPatch, 4> &patches)
    {
        resultPatches[static_cast<size_t>(result)] = patches;
    }

    void setBattleStartAdvances(const std::vector<u32> &advances)
    {
        battleStartAdvances = advances;
    }

    u32 getDistance() const
    {
        return distance;
    }

    void setDistance(u32 value)
    {
        distance = value;
    }

    u16 getChain() const
    {
        return chain;
    }

    u8 getCall() const
    {
        return call;
    }

    u8 getChatot() const
    {
        return chatot;
    }

    u8 getNoGraceSkip() const
    {
        return noGraceSkip;
    }

    u8 getGraceSkip() const
    {
        return graceSkip;
    }

    void setSkip(u8 noGrace, u8 grace)
    {
        noGraceSkip = noGrace;
        graceSkip = grace;
    }

    u32 getDisplayedBattleAdvances() const
    {
        return displayBattleAdvances ? battleAdvances : pokemon.value().getBattleAdvances();
    }

    void setDisplayedBattleAdvances(u32 advances)
    {
        displayBattleAdvances = true;
        battleAdvances = advances;
    }

    u32 getDisplayedPatchAdvances() const
    {
        return displayedPatchAdvances;
    }

    void setDisplayedPatchAdvances(u32 advances)
    {
        displayedPatchAdvances = advances;
    }

    bool hasDisplayPatchType() const
    {
        return displayPatchType;
    }

    bool getDisplayPatchStrong() const
    {
        return displayPatchStrong;
    }

    bool getDisplayPatchShiny() const
    {
        return displayPatchShiny;
    }

    void setDisplayPatchType(bool strong, bool shiny)
    {
        displayPatchType = true;
        displayPatchStrong = strong;
        displayPatchShiny = shiny;
    }

    const std::array<PokeRadarPatch, 4> &getPatches() const
    {
        return patches;
    }

    const std::array<PokeRadarPatch, 4> &getBattlePatches() const
    {
        return battlePatches;
    }

    void setBattlePatches(const std::array<PokeRadarPatch, 4> &patches)
    {
        battlePatches = patches;
    }

    bool hasPokemon() const
    {
        return pokemon.has_value() || searcherPokemon.has_value();
    }

    const WildGeneratorState4 &getPokemon() const
    {
        return pokemon.value();
    }

    bool hasSearcherPokemon() const
    {
        return searcherPokemon.has_value();
    }

    const WildSearcherState4 &getSearcherPokemon() const
    {
        return searcherPokemon.value();
    }

private:
    u32 advances;
    u32 patchAdvances;
    u32 distance;
    u16 chain;
    u8 call;
    u8 chatot;
    u8 noGraceSkip;
    u8 graceSkip;
    bool displayBattleAdvances;
    u32 battleAdvances;
    u32 displayedPatchAdvances;
    bool displayPatchType;
    bool displayPatchStrong;
    bool displayPatchShiny;
    bool patchesVisible;
    bool battlePatchesVisible;
    std::vector<u32> battleStartAdvances;
    std::vector<PokeRadarResult> results;
    std::array<std::optional<std::array<PokeRadarPatch, 4>>, 3> resultPatches;
    std::array<PokeRadarPatch, 4> patches;
    std::array<PokeRadarPatch, 4> battlePatches;
    std::optional<WildGeneratorState4> pokemon;
    std::optional<WildSearcherState4> searcherPokemon;
};

#endif // POKERADARSTATE_HPP
