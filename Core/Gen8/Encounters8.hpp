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

#ifndef ENCOUNTERS8_HPP
#define ENCOUNTERS8_HPP

#include <Core/Global.hpp>
#include <array>
#include <vector>

class Den;
class DenEvent;
class EncounterArea8;
class Profile8;
class StaticTemplate8;
class UndergroundArea;
enum class Encounter : u8;

struct EncounterSettings8
{
    int time;
    std::array<u16, 2> replacement;
    bool radar;
    bool swarm;
};

namespace Encounters8
{
    /**
     * @brief Returns the den for the \p index and \p rarity
     *
     * @param index Den index
     * @param rarity Den rarity
     *
     * @return Den
     */
    const Den *getDen(u16 index, u8 rarity);

    /**
     * @brief Return the coordinates of the den
     *
     * @param index Den index
     *
     * @return Den coordinates
     */
    std::array<u16, 2> getDenCoordinates(u16 index);

    /**
     * @brief Returns the den event
     *
     * @param index Den index
     *
     * @return Event den
     */
    const DenEvent *getDenEvent(u8 index);

    /**
     * @brief Returns the location of the den
     *
     * @param index Den index
     *
     * @return Den location
     */
    u8 getDenLocation(u16 index);

    /**
     * @brief Gets wild encounters for the \p encounter and \p profile
     *
     * @param encounter Encounter type
     * @param settings Settings that impact wild encounter slots
     * @param profile Profile information
     *
     * @return Vector of wild encounters
     */
    std::vector<EncounterArea8> getEncounters(Encounter encounter, const EncounterSettings8 &settings, const Profile8 *profile);

    /**
     * @brief Returns the changing pokemon of the Great
     *
     * @param profile Profile information
     *
     * @return Array of Great Marsh Pokemon
     */
    std::array<u16, 14> getGreatMarshPokemon(const Profile8 *profile);

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

    /**
     * @brief Returns the changing pokemon of the Trophy Garden
     *
     * @return Array of Trophy Garden Pokemon
     */
    std::array<u16, 16> getTrophyGardenPokemon();

    /**
     * @brief Gets underground encounters filtered by the \p flag
     *
     * @param storyFlag Story flag
     * @param profile Profile information
     *
     * @return Vector of underground encounters
     */
    std::vector<UndergroundArea> getUndergroundEncounters(int storyFlag, bool diglett, const Profile8 *profile);
}

#endif // ENCOUNTERS8_HPP
