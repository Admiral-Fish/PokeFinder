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

#ifndef PROFILE4_HPP
#define PROFILE4_HPP

#include <Core/Parents/Profile.hpp>

enum class Game : u32;

/**
 * @brief Provides additional storage specific to Gen4
 */
class Profile4 : public Profile
{
public:
    /**
     * @brief Construct a new Profile4 object
     *
     * @param name Profile name
     * @param version Game version
     * @param tid Trainer ID
     * @param sid Secret ID
     * @param dual Dual slot game version
     * @param radio Radio station
     * @param radar Whether pokeradar is enabled
     * @param swarm Whether swarms are enabled
     * @param dex Whether national pokedex is obtained
     */
    Profile4(const std::string &profileName, Game version, u16 tid, u16 sid, Game dual, int radio, bool radar, bool swarm, bool dex);

    /**
     * @brief Returns the dual slot game version
     *
     * @return Game version
     */
    Game getDualSlot() const;

    /**
     * @brief Get the National Dex object
     *
     * @return true National pokedex is obtained
     * @return false National pokedex is not obtained
     */
    bool getNationalDex() const;

    /**
     * @brief Returns whether the profile is using the pokeradar
     *
     * @return true Pokeradar is enabled
     * @return false Pokeradar is not enabled
     */
    bool getRadar() const;

    /**
     * @brief Returns the active radio station
     *
     * @return Radio number
     * 0: No radio
     * 1: Hoenn sound
     * 2: Sinnoh sound
     */
    int getRadio() const;

    /**
     * @brief Returns the string representation for the current radio station
     *
     * @return Radio string
     */
    std::string getRadioString() const;

    /**
     * @brief Returns whether the profile is using a swarm
     *
     * @return true Swarm is enabled
     * @return false Swarm is not enabled
     */
    bool getSwarm() const;

    /**
     * @brief Checks if two profiles are equal
     *
     * @param other Profile to compare
     *
     * @return true Profiles are equal
     * @return false Profiles are not equal
     */
    bool operator==(const Profile4 &other) const;

    /**
     * @brief Checks if two profiles are not equal
     *
     * @param other Profile to compare
     *
     * @return true Profiles are not equal
     * @return false Profiles are equal
     */
    bool operator!=(const Profile4 &other) const;

private:
    bool dex;
    bool radar;
    bool swarm;
    Game dual;
    int radio;
};

#endif // PROFILE4_HPP
