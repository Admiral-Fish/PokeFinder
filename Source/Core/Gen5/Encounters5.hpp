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

#ifndef ENCOUNTERS5_HPP
#define ENCOUNTERS5_HPP

#include <Core/Global.hpp>
#include <vector>

class DreamRadarTemplate;
class EncounterArea5;
class HiddenGrottoArea;
class Profile5;
class StaticTemplate5;
enum class Encounter : u8;

namespace Encounters5
{
    /**
     * @brief Gets dream radar encounters
     *
     * @param size Pointer to set number of encounters if not nullptr
     *
     * @return Pointer to dream radar encounters
     */
    const DreamRadarTemplate *getDreamRadarEncounters(int *size = nullptr);

    /**
     * @brief Gets dream radar encounter from the \p index
     *
     * @param index Pokemon index
     *
     * @return Pointer to dream radar encounter
     */
    const DreamRadarTemplate *getDreamRadarEncounters(int index);

    /**
     * @brief Gets wild encounters for the \p encounter and \p profile
     *
     * @param encounter Encounter type
     * @param season Season modifier
     * @param profile Profile information
     *
     * @return Vector of wild encounters
     */
    std::vector<EncounterArea5> getEncounters(Encounter encounter, u8 season, const Profile5 *profile);

    /**
     * @brief Gets hidden grotto encounters
     *
     * @return Vector of grotto encounters
     */
    std::vector<HiddenGrottoArea> getHiddenGrottoEncounters();

    /**
     * @brief Gets static encounters from the \p type
     *
     * @param type Static encounter type
     * 0: starters
     * 1: fossils
     * 2: gifts
     * 3: stationary
     * 4: legends
     * 5: mythics
     * 6: roamers
     * @param size Pointer to set number of encounters if not nullptr
     *
     * @return Pointer to static encounters area
     */
    const StaticTemplate5 *getStaticEncounters(int index, int *size = nullptr);

    /**
     * @brief Gets static encounters from the \p type and \p index
     *
     * @param type Static encounter type
     * 0: starters
     * 1: fossils
     * 2: gifts
     * 3: stationary
     * 4: legends
     * 5: mythics
     * 6: roamers
     * @param index Pokemon index
     *
     * @return Pointer to static encounter
     */
    const StaticTemplate5 *getStaticEncounter(int type, int index);
}

#endif // ENCOUNTERS5_HPP
