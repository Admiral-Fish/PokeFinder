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

#ifndef PROFILE8_HPP
#define PROFILE8_HPP

#include <Core/Enum/Game.hpp>
#include <Core/Parents/Profile.hpp>

/**
 * @brief Provides additional storage specific to Gen8
 */
class Profile8 : public Profile
{
public:
    /**
     * @brief Creates a new Profile8
     * @param name Profile name
     * @param version Game version
     * @param tid Trainer ID
     * @param sid Secret ID
     * @param shinyCharm Whether shiny charm is obtained
     * @param ovalCharm Whether oval charm is obtained
     * @param radar Whether pokeradar is enabled
     * @param swarm Whether swarms are enabled
     */
    Profile8(const std::string &name, Game version, u16 tid, u16 sid, bool shinyCharm, bool ovalCharm, bool radar, bool swarm);

    /**
     * @brief Returns whether the profile has the oval charm
     * @return True if oval charm is obtained, false otherwise
     */
    bool getOvalCharm() const;

    /**
     * @brief Returns whether the profile is using the pokeradar
     * @return True if pokeradar is enabled, false otherwise
     */
    bool getRadar() const;

    /**
     * @brief Returns whether the profile has the shiny charm
     * @return True if shiny charm is obtained, false otherwise
     */
    bool getShinyCharm() const;

    /**
     * @brief Returns whether the profile is using a swarm
     * @return True if swarm is enabled, false otherwise
     */
    bool getSwarm() const;

    /**
     * @brief Checks if two profiles are equal
     * @param other Profile to compare
     * @return True if equal, false otherwise
     */
    bool operator==(const Profile8 &other) const;

    /**
     * @brief Checks if two profiles are not equal
     * @param other Profile to compare
     * @return True if not equal, false otherwise
     */
    bool operator!=(const Profile8 &other) const;

private:
    bool ovalCharm;
    bool radar;
    bool shinyCharm;
    bool swarm;
};

#endif // PROFILE8_HPP
