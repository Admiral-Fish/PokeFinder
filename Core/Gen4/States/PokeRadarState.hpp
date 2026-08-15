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
#include <array>
#include <optional>

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
        patches(patches)
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
        patches(state.patches),
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
        patches(state.patches),
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
        patches(state.patches),
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

    u32 getAdvances() const
    {
        return advances;
    }

    u32 getPatchAdvances() const
    {
        return patchAdvances;
    }

    u32 getDistance() const
    {
        return distance;
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
    std::array<PokeRadarPatch, 4> patches;
    std::optional<WildGeneratorState4> pokemon;
    std::optional<WildSearcherState4> searcherPokemon;
};

#endif // POKERADARSTATE_HPP
