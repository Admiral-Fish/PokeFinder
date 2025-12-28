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

#ifndef PROFILE4_HPP
#define PROFILE4_HPP

#include <Core/Parents/Profile.hpp>
#include <array>
#include <vector>

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
     * @param dex Whether national pokedex is obtained
     */
    Profile4(const std::string &name, Game version, u16 tid, u16 sid, bool dex) : Profile(name, version, tid, sid), dex(dex)
    {
        unownDiscovered.fill(false);
        unownPuzzle.fill(false);
    }

    /**
     * @brief Construct a new Profile4 object
     *
     * @param name Profile name
     * @param version Game version
     * @param tid Trainer ID
     * @param sid Secret ID
     * @param dex Whether national pokedex is obtained
     * @param unownDiscovered Array of which Unown forms have been encountered
     * @param unownPuzzle Array of which Alpha Ruin puzzles have been completed
     */
    Profile4(const std::string &name, Game version, u16 tid, u16 sid, bool dex, const std::array<bool, 26> &unownDiscovered,
             const std::array<bool, 4> &unownPuzzle) :
        Profile(name, version, tid, sid), dex(dex), unownDiscovered(unownDiscovered), unownPuzzle(unownPuzzle)
    {
    }

    /**
     * @brief Get the National Dex object
     *
     * @return true National pokedex is obtained
     * @return false National pokedex is not obtained
     */
    bool getNationalDex() const
    {
        return dex;
    }

    /**
     * @brief Returns the discovered unowns
     *
     * @return Vector of discovered unowns
     */
    std::array<bool, 26> getUnownDiscovered() const
    {
        return unownDiscovered;
    }

    /**
     * @brief Returns the unown puzzles
     *
     * @return Vector of unown puzzles
     */
    std::array<bool, 4> getUnownPuzzle() const
    {
        return unownPuzzle;
    }

    /**
     * @brief Gets the list of undiscovered unown forms
     *
     * @param unlocked List of unlocked unown forms
     *
     * @return Vector of undiscovered unown forms
     */
    std::vector<u8> getUndiscoveredUnownForms(const std::vector<u8> &unlocked) const;

    /**
     * @brief Gets the list of unlocked unown forms
     *
     * @return Vector of unlocked unown forms
     */
    std::vector<u8> getUnlockedUnownForms() const;

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
    std::array<bool, 26> unownDiscovered;
    std::array<bool, 4> unownPuzzle;
};

#endif // PROFILE4_HPP
