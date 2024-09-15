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

#ifndef POKESPOTSTATE_HPP
#define POKESPOTSTATE_HPP

#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/States/State.hpp>

/**
 * @brief State class for Poke Spot generator encounters
 */
class PokeSpotState : public GeneratorState
{
public:
    /**
     * @brief Construct a new WildSearcherState3 object
     *
     * @param seed Seed of the state
     * @param pid Pokemon PID
     * @param ivs Pokemon IVs
     * @param ability Pokemon ability
     * @param gender Pokemon gender
     * @param level Pokemon level
     * @param nature Pokemon nature
     * @param shiny Pokemon shininess
     * @param encounterSlot Pokemon encounter slot
     * @param specie Pokemon specie
     * @param info Pokemon information
     */
    PokeSpotState(u32 advances, u32 pid, u8 gender, u8 shiny, u8 encounterSlot, u16 specie, const PersonalInfo *info) :
        GeneratorState(advances, pid, { 0, 0, 0, 0, 0, 0 }, 0, gender, 0, pid % 25, shiny, info),
        specie(specie),
        encounterSlot(encounterSlot)
    {
    }

    /**
     * @brief Returns the encounter slot of the state
     *
     * @return State encounter slot
     */
    u8 getEncounterSlot() const
    {
        return encounterSlot;
    }

    /**
     * @brief Returns the advances of the state
     *
     * @return State advances
     */
    u32 getEncounterAdvances() const
    {
        return encounterAdvances;
    }

    /**
     * @brief Returns the specie
     *
     * @return State specie
     */
    u16 getSpecie() const
    {
        return specie;
    }

    /**
     * @brief Updates state with things that are calculated on encounter
     *
     * @param advances Advances of the state
     * @param ability Pokemon ability
     * @param level Pokemon level
     * @param ivs Pokemon IVs
     * @param info Pokemon information
     */
    void update(u32 advances, u8 ability, u8 level, const std::array<u8, 6> &ivs, const PersonalInfo *info)
    {
        encounterAdvances = advances;
        this->ability = ability;
        abilityIndex = info->getAbility(ability);
        this->level = level;
        this->ivs = ivs;
        updateStats(info);
    }

private:
    u32 encounterAdvances;
    u16 specie;
    u8 encounterSlot;
};

#endif // POKESPOTSTATE_HPP
