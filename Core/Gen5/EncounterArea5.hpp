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
#include <Core/Util/Translator.hpp>
#include <algorithm>

/**
 * @brief Contains information about the encounters for an area. This includes location, rate, and the slots.
 */
class EncounterArea5 : public EncounterArea
{
public:
    using EncounterArea::getPokemon;
    using EncounterArea::getSlots;

    /**
     * @brief Construct a new EncounterArea5 object
     *
     * @param location Location number
     * @param rate Encounter rate of the area
     * @param season Whether encounter area has seasonal encounters
     * @param encounter Encounter type of the area
     * @param pokemon Available pokemon of the area
     * @param swarm Optional swarm pokemon of the area
     */
    EncounterArea5(u8 location, u8 rate, bool season, Encounter encounter, const std::array<Slot, 12> &pokemon, const Slot &swarm = Slot()) :
        EncounterArea(location, rate, encounter, pokemon), swarm(swarm), season(season)
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
        const Slot &slot = getPokemon(encounterSlot);

        u8 min = slot.getMinLevel();
        u8 max = slot.getMaxLevel();
        u8 range = max - min + 1;

        u8 level = min + (prng % range);
        if (force)
        {
            bool diff = false;
            u8 max = level;
            for (u8 i = 0; i < getCount(); i++)
            {
                const Slot &other = getPokemon(i);
                if (other.getMinLevel() != other.getMaxLevel())
                {
                    diff = true;
                }

                if (slot.getSpecie() == other.getSpecie())
                {
                    max = std::max(max, other.getMaxLevel());
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
     * @brief Return number of encounter slots
     *
     * @return Encounter slot count
     */
    u8 getCount() const
    {
        return EncounterArea::getCount() + hasSwarm();
    }

    /**
     * @brief Calculates the level range of a \p specie across all possible slots it can be encountered
     *
     * @param specie Species number
     *
     * @return Pair of level range
     */
    std::pair<u8, u8> getLevelRange(u16 specie) const
    {
        auto range = EncounterArea::getLevelRange(specie);
        if (swarm.getSpecie() == (specie & 0x7ff) && swarm.getForm() == (specie >> 11))
        {
            if (range.first == 0 && range.second == 0)
            {
                range = { swarm.getMinLevel(), swarm.getMaxLevel() };
            }
            else
            {
                range.first = std::min(range.first, swarm.getMinLevel());
                range.second = std::max(range.second, swarm.getMaxLevel());
            }
        }
        return range;
    }

    /**
     * @brief Return the pokemon at the specific \p index
     *
     * @param index Pokemon index
     *
     * @return Pokemon
     */
    const Slot &getPokemon(int index) const
    {
        return index == 12 ? swarm : EncounterArea::getPokemon(index);
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

    /**
     * @brief Return vector of true/false which indicate slots that match the \p specie
     *
     * @param specie Species number
     *
     * @return Vector of true/false for matching slots
     */
    std::vector<bool> getSlots(u16 specie) const
    {
        auto flags = EncounterArea::getSlots(specie);
        if (hasSwarm())
        {
            flags.emplace_back(swarm.getSpecie() == (specie & 0x7ff) && swarm.getForm() == (specie >> 11));
        }
        return flags;
    }

    /**
     * @brief Return vector of names of all pokemon slots
     *
     * @return Vector of pokemon name
     */
    std::vector<std::string> getSpecieNames() const
    {
        return Translator::getSpecies(getUniqueSpecies());
    }

    /**
     * @brief Return the species numbers of unique pokemon of the area
     *
     * @return Vector of pokemon species
     */
    std::vector<u16> getUniqueSpecies() const
    {
        auto species = EncounterArea::getUniqueSpecies();
        if (hasSwarm() && std::find(species.begin(), species.end(), swarm.getSpecie()) == species.end())
        {
            species.emplace_back(swarm.getSpecie());
        }
        return species;
    }

    /**
     * @brief Returns if the area has a swarm encounter
     *
     * @return true Area has a swarm encounter
     * @return false Area does not have a swarm encounter
     */
    bool hasSwarm() const
    {
        return swarm.getSpecie() != 0;
    }

private:
    Slot swarm;
    bool season;
};

#endif // ENCOUNTERAREA5_HPP
