/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef ENCOUNTERAREA3_HPP
#define ENCOUNTERAREA3_HPP

#include <Core/Parents/EncounterArea.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/RNG/LCRNG.hpp>

enum class Game : u32;

/**
 * @brief Contains information about the encounters for an area. This includes location, rate, and the slots.
 */
class EncounterArea3 : public EncounterArea
{
public:
    /**
     * @brief Construct a new EncounterArea3 object
     *
     * @param location Location number
     * @param rate Encounter rate of the area
     * @param encounter Encounter type of the area
     * @param pokemon Available pokemon of the area
     */
    EncounterArea3(u8 location, u8 rate, Encounter encounter, const std::vector<Slot> &pokemon) :
        EncounterArea(location, rate, encounter, pokemon)
    {
    }

    /**
     * @brief Calculates the level of a pokemon. Takes into account any modification from Pressure
     *
     * @param encounterSlot Pokemon slot
     * @param rng RNG object
     * @param force Whether Pressure lead is being used
     *
     * @return Level of the encounter
     */
    u8 calculateLevel(u8 encounterSlot, PokeRNG &rng, bool force) const
    {
        const Slot &slot = pokemon[encounterSlot];

        u8 min = slot.getMinLevel();
        u8 max = slot.getMaxLevel();
        u8 range = max - min + 1;

        u8 rand = rng.nextUShort(range);
        if (force)
        {
            if (rng.nextUShort(2) == 0)
            {
                return max;
            }
            rand = rand == 0 ? rand : rand - 1;
        }

        return min + rand;
    }

    /**
     * @brief Calculates the level of a pokemon. Used by WildSearcher3 and assume Pressure is being used
     *
     * @param encounterSlot Pokemon slot
     * @param levelRand PRNG call for the level
     * @param force PRNG call for Pressure
     *
     * @return Level of the encounter
     */
    u8 calculateLevel(u8 encounterSlot, u16 levelRand, bool force) const
    {
        const Slot &slot = pokemon[encounterSlot];

        u8 min = slot.getMinLevel();
        u8 max = slot.getMaxLevel();
        u8 range = max - min + 1;

        u8 rand = levelRand % range;
        if (force)
        {
            return max;
        }
        else
        {
            rand = rand == 0 ? rand : rand - 1;
        }

        return min + rand;
    }

    /**
     * @brief Checks if the location is in the Safari Zone
     *
     * @param version Game version
     *
     * @return true Location is Safari Zone
     * @return false Location is not Safari Zone
     */
    bool safariZone(Game version) const;
};

#endif // ENCOUNTERAREA3_HPP
