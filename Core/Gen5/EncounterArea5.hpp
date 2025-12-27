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

#ifndef ENCOUNTERAREA5_HPP
#define ENCOUNTERAREA5_HPP

#include <Core/Parents/EncounterArea.hpp>

/**
 * @brief Contains information about the encounters for an area. This includes location, rate, and the slots.
 */
class EncounterArea5 : public EncounterArea
{
public:
    /**
     * @brief Construct a new EncounterArea5 object
     *
     * @param location Location number
     * @param rate Encounter rate of the area
     * @param season Whether encounter area has seasonal encounters
     * @param encounter Encounter type of the area
     * @param pokemon Available pokemon of the area
     */
    EncounterArea5(u8 location, u8 rate, bool season, Encounter encounter, const std::array<Slot, 12> &pokemon) :
        EncounterArea(location, rate, encounter, pokemon), season(season)
    {
    }

    /**
     * @brief Calculates the level of a pokemon
     *
     * @param encounterSlot Pokemon index to use
     * @param prng Level RNG call
     * @param force Whether Pressure lead is being used
     *
     * @return Level of the encounter
     */
    u8 calculateLevel(u8 encounterSlot, u8 prng, bool force) const
    {
        const Slot &slot = pokemon[encounterSlot];

        u8 min = slot.getMinLevel();
        u8 max = slot.getMaxLevel();
        u8 range = max - min + 1;

        u8 level = min + (prng % range);
        if (force)
        {
            bool diff = false;
            u8 max = level;
            for (u8 i = 0; i < pokemon.size() && pokemon[i].getSpecie() != 0; i++)
            {
                if (pokemon[i].getMinLevel() != pokemon[i].getMaxLevel())
                {
                    diff = true;
                }

                if (slot.getSpecie() == pokemon[i].getSpecie())
                {
                    max = std::max(max, pokemon[i].getMaxLevel());
                }
            }

            if (diff)
            {
                return std::min((u8)(level + 5), max);
            }
            else
            {
                return max;
            }
        }

        return level;
    }

    /**
     * @brief Returns if the encounter area has multiple seasonal differences
     *
     * @return true Differences based on the season
     * @return false No differences based on the season
     */
    bool getSeason() const
    {
        return season;
    }

private:
    bool season;
};

#endif // ENCOUNTERAREA5_HPP
