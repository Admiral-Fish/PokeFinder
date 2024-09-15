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

#ifndef ENCOUNTERAREA8_HPP
#define ENCOUNTERAREA8_HPP

#include <Core/Parents/EncounterArea.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/RNG/Xorshift.hpp>

/**
 * @brief Contains information about the encounters for an area. This includes location, rate, and the slots.
 */
class EncounterArea8 : public EncounterArea
{
public:
    /**
     * @brief Construct a new EncounterArea8 object
     *
     * @param location Location number
     * @param rate Encounter rate of the area
     * @param encounter Encounter type of the area
     * @param pokemon Available pokemon of the area
     */
    EncounterArea8(u8 location, u8 rate, Encounter type, const std::array<Slot, 12> &pokemon) : EncounterArea(location, rate, type, pokemon)
    {
    }

    /**
     * @brief Calculates the level of a pokemon. Takes into account any modification from Pressure
     *
     * @tparam diff Whether min and max levels are different
     * @param encounterSlot Pokemon slot
     * @param rngList RNG object
     * @param force Whether Pressure lead is being used
     *
     * @return Level of the encounter
     */
    template <bool diff>
    u8 calculateLevel(u8 encounterSlot, RNGList<u32, Xorshift, 128> &rngList, bool force) const
    {
        if constexpr (diff)
        {
            const Slot &slot = pokemon[encounterSlot];

            u8 min = slot.getMinLevel();
            u8 max = slot.getMaxLevel();
            u8 range = max - min + 1;

            u8 rand = rngList.next() % range;
            if (force && (rngList.next() % 2) != 0)
            {
                return max;
            }

            return min + rand;
        }
        else
        {
            const Slot &slot = pokemon[encounterSlot];
            u8 level = slot.getMaxLevel();
            if (force && (rngList.next() % 2) != 0)
            {
                for (const Slot &s : pokemon)
                {
                    if (s.getSpecie() == slot.getSpecie())
                    {
                        level = std::max(level, s.getMaxLevel());
                    }
                }
            }
            return level;
        }
    }

    /**
     * @brief Checks if the location is in the Great Marsh
     *
     * @param version Game version
     *
     * @return true Location is Great Marsh
     * @return false Location is not Great Marsh
     */
    bool greatMarsh(Game version) const;

    /**
     * @brief Checks if the location is in the Trophy Garden
     *
     * @param version Game version
     *
     * @return true Location is Trophy Garden
     * @return false Location is not Trophy Garden
     */
    bool trophyGarden(Game version) const;

    /**
     * @brief Returns the Unown form to use for Solaceon Ruins
     *
     * @return Unown form
     */
    u8 unownForm(u32 prng) const;
};

#endif // ENCOUNTERAREA8_HPP
