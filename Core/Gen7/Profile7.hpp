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

#ifndef PROFILE7_HPP
#define PROFILE7_HPP

#include <Core/Parents/Profile.hpp>
#include <array>

enum class Game : u32;

/**
 * @brief Provides additional storage specific to Gen7
 */
class Profile7 : public Profile
{
public:
    /**
     * @brief Construct a new Profile7 object
     *
     * @param name Profile name
     * @param version Game version
     * @param tid Trainer ID
     * @param sid Secret ID
     * @param eggSeed TinyMT state for eggs
     * @param shinyCharm Whether shiny charm is obtained
     */
    Profile7(const std::string &name, Game version, u16 tid, u16 sid, const std::array<u32, 4> &eggSeed, bool shinyCharm) :
        Profile(name, version, tid, sid), eggSeed(eggSeed), shinyCharm(shinyCharm)
    {
    }

    /**
     * @brief Returns the profile egg seed
     *
     * @return Profile egg seed
     */
    std::array<u32, 4> getEggSeed() const
    {
        return eggSeed;
    }

    /**
     * @brief Returns whether the profile has the shiny charm
     *
     * @return true Shiny charm is obtained
     * @return false Shiny charm is not obtained
     */
    bool getShinyCharm() const
    {
        return shinyCharm;
    }

    /**
     * @brief Checks if two profiles are equal
     *
     * @param other Profile to compare
     *
     * @return true Profiles are equal
     * @return false Profils are not equal
     */
    bool operator==(const Profile7 &other) const;

    /**
     * @brief Checks if two profiles are not equal
     *
     * @param other Profile to compare
     *
     * @return true Profiles are not equal
     * @return false Profiles are equal
     */
    bool operator!=(const Profile7 &other) const;

private:
    std::array<u32, 4> eggSeed;
    bool shinyCharm;
};

#endif // PROFILE7_HPP
