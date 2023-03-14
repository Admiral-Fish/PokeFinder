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

#ifndef ENCOUNTERS3_HPP
#define ENCOUNTERS3_HPP

#include <Core/Global.hpp>
#include <vector>

class EncounterArea;
class EncounterArea3;
class ShadowTemplate;
class StaticTemplate;
enum class Encounter : u8;
enum class Game : u32;

namespace Encounters3
{
    /**
     * @brief Gets wild encounters for the \p encouner and \p version
     *
     * @param encounter Encounter type
     * @param version Game version
     *
     * @return Vector of wild encounters
     */
    std::vector<EncounterArea3> getEncounters(Encounter encounter, Game version);

    /**
     * @brief Gets wild encounters for Poke Spots
     *
     * @return Vector of Poke Spot encounters
     */
    std::vector<EncounterArea> getPokeSpotEncounters();

    /**
     * @brief Gets shadow teams
     *
     * @param size Pointer to set number of encounters if not nullptr
     *
     * @return Pointer to shadow teams
     */
    const ShadowTemplate *getShadowTeams(size_t *size = nullptr);

    /**
     * @brief Gets shadow team from the \p index
     *
     * @param index Pokemon index
     *
     * @return Pointer to shadow team
     */
    const ShadowTemplate *getShadowTeam(int index);

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
     * 7: gales/colo
     * 8: channel
     * @param size Pointer to set number of encounters if not nullptr
     *
     * @return Pointer to static encounters area
     */
    const StaticTemplate *getStaticEncounters(int type, size_t *size = nullptr);

    /**
     * @brief Gets the static encounter from the \p type and \p index
     *
     * @param type Static encounter type
     * 0: starters
     * 1: fossils
     * 2: gifts
     * 3: game corner
     * 4: stationary
     * 5: legends
     * 6: events
     * 7: gales/colo
     * 8: channel
     * @param index Pokemon index
     *
     * @return Pointer to static encounter
     */
    const StaticTemplate *getStaticEncounter(int type, int index);
};

#endif // ENCOUNTERS3_HPP
