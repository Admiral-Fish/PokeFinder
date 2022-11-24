/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef ENCOUNTERS8_HPP
#define ENCOUNTERS8_HPP

#include <Core/Global.hpp>
#include <vector>

class EncounterArea8;
class Profile8;
class StaticTemplate8;
class UndergroundArea;
enum class Encounter : u8;

namespace Encounters8
{
    /**
     * @brief Gets underground encounters filtered by the \p flag
     *
     * @param storyFlag Story flag
     * @param profile Profile information
     *
     * @return Vector of underground encounters
     */
    std::vector<UndergroundArea> getEncounters(int storyFlag, bool diglett, const Profile8 *profile);

    /**
     * @brief Gets wild encounters for the \p encounter and \p profile
     *
     * @param encounter Encounter type
     * @param time Time modifier
     * @param radar Whether pokeradar is active
     * @param swarm Whether swarm is active
     * @param profile Profile information
     *
     * @return Vector of wild encounters
     */
    std::vector<EncounterArea8> getEncounters(Encounter encounter, int time, bool radar, bool swarm, const Profile8 *profile);

    /**
     * @brief Gets static encounters from the \p type
     *
     * @param type Static encounter type
     * 0: starters
     * 1: gifts
     * 2: fossils
     * 3: stationary
     * 4: roamers
     * 5: legends
     * 6: ramanasParkPureSpace
     * 7: ramanasParkStrangeSpace
     * 8: mythics
     * @param size Pointer to set number of encounters if not nullptr
     *
     * @return Pointer to static encounters area
     */
    const StaticTemplate8 *getStaticEncounters(int index, int *size = nullptr);

    /**
     * @brief Gets static encounters from the \p type and \p index
     *
     * @param type Static encounter type
     * 0: starters
     * 1: gifts
     * 2: fossils
     * 3: stationary
     * 4: roamers
     * 5: legends
     * 6: ramanasParkPureSpace
     * 7: ramanasParkStrangeSpace
     * 8: mythics
     * @param index Pokemon index
     *
     * @return Pointer to static encounter
     */
    const StaticTemplate8 *getStaticEncounter(int type, int index);
}

#endif // ENCOUNTERS8_HPP
