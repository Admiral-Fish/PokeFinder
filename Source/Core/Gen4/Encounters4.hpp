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

#ifndef ENCOUNTERS4_HPP
#define ENCOUNTERS4_HPP

#include <Core/Global.hpp>
#include <vector>

class EncounterArea;
class Profile4;
class StaticTemplate4;
enum class Encounter : u8;

namespace Encounters4
{
    /**
     * @brief Gets wild encounters for the \p encouner and \p profile
     *
     * @param encounter Encounter type
     * @param modifier
     * @param profile Profile information
     *
     * @return Vector of wild encounters
     */
    std::vector<EncounterArea> getEncounters(Encounter encounter, int modifier, const Profile4 &profile);

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
    const StaticTemplate4 *getStaticEncounters(int type, size_t *size = nullptr);

    /**
     * @brief getStaticEncounter
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
}

#endif // ENCOUNTERS4_HPP
