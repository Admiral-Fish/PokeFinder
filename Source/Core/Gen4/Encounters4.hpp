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

#ifndef ENCOUNTERS4_HPP
#define ENCOUNTERS4_HPP

#include <Core/Global.hpp>
#include <array>
#include <vector>

class EncounterArea4;
class Profile4;
class StaticTemplate4;
enum class Encounter : u8;
enum class Game : u32;

namespace Encounters4
{
    /**
     * @brief Gets wild encounters for the \p encouner and \p profile
     *
     * @param encounter Encounter type
     * @param time Time modifier
     * @param dual Dual slot version
     * @param radar Whether pokeradar is active
     * @param radio Radio station
     * @param swarm Whether swarm is active
     * @param replacement Replacement slots used by Great Marsh and Trophy Garden
     * @param blocks Active pokeblocks in the safari
     * @param profile Profile information
     *
     * @return Vector of wild encounters
     */
    std::vector<EncounterArea4> getEncounters(Encounter encounter, int time, Game dual, bool radar, int radio, bool swarm,
                                              const std::array<u16, 2> &replacement, const std::array<u8, 5> &blocks,
                                              const Profile4 *profile);

    /**
     * @brief Returns the changing pokemon of the Great Marsh
     *
     * @param profile Profile information
     *
     * @return Array of Great Marsh Pokemon
     */
    std::array<u16, 15> getGreatMarshPokemon(const Profile4 *profile);

    /**
     * @brief Gets static encounters from the \p type
     *
     * @param type Static encounter type
     * 0: starters
     * 1: fossils
     * 2: gifts
     * 3: game corner
     * 4: stationary
     * 5: legends
     * 6: events
     * 7: roamers
     * @param size Pointer to set number of encounters if not nullptr
     *
     * @return Pointer to static encounters area
     */
    const StaticTemplate4 *getStaticEncounters(int type, int *size = nullptr);

    /**
     * @brief Gets static encounters from the \p type and \p index
     *
     * @param type Static encounter type
     * 0: starters
     * 1: fossils
     * 2: gifts
     * 3: game corner
     * 4: stationary
     * 5: legends
     * 6: events
     * 7: roamers
     * @param index Pokemon index
     *
     * @return Pointer to static encounter
     */
    const StaticTemplate4 *getStaticEncounter(int type, int index);

    /**
     * @brief Returns the changing pokemon of the Trophy Garden
     *
     * @param profile Profile information
     *
     * @return Array of Trophy Garden Pokemon
     */
    std::array<u16, 16> getTrophyGardenPokemon(const Profile4 *profile);
}

#endif // ENCOUNTERS4_HPP
