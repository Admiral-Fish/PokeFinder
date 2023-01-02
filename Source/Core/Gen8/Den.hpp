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

#ifndef DEN_HPP
#define DEN_HPP

#include <Core/Enum/Game.hpp>
#include <Core/Gen8/Raid.hpp>
#include <vector>

/**
 * @brief Contains all the Raid pokemon within a Den and the identifying hash value
 */
class Den
{
public:
    /**
     * @brief Construct a new Den object
     *
     * @param hash Den hash
     * @param sword Sword raids
     * @param shield Sword shields
     */
    constexpr Den(u64 hash, const std::array<Raid, 12> &sword, const std::array<Raid, 12> &shield) :
        hash(hash), shield(shield), sword(sword)
    {
    }

    /**
     * @brief Returns the hash value of the Den
     *
     * @return Den hash
     */
    u64 getHash() const
    {
        return hash;
    }

    /**
     * @brief Returns the raid for the \p index and \p version
     *
     * @param index Raid index
     * @param version Game version
     *
     * @return Den raid
     */
    Raid getRaid(u8 index, Game version) const
    {
        return (version == Game::Sword) ? sword[index] : shield[index];
    }

    /**
     * @brief Returns the raids for the \p version
     *
     * @param version Game version
     *
     * @return Den raids
     */
    std::array<Raid, 12> getRaids(Game version) const
    {
        return (version == Game::Sword) ? sword : shield;
    }

private:
    u64 hash;
    std::array<Raid, 12> shield;
    std::array<Raid, 12> sword;
};

/**
 * @brief Contains all the Raid pokemon within a Den event
 */
class DenEvent
{
public:
    DenEvent() = default;

    /**
     * @brief Construct a new Den object
     *
     * @param sword Sword raids
     * @param shield Sword shields
     */
    DenEvent(const std::vector<Raid> &sword, const std::vector<Raid> &shield) : shield(shield), sword(sword)
    {
    }

    /**
     * @brief Returns the raid for the \p index and \p version
     *
     * @param index Raid index
     * @param version Game version
     *
     * @return Den raid
     */
    Raid getRaid(u8 index, Game version) const
    {
        return (version == Game::Sword) ? sword[index] : shield[index];
    }

    /**
     * @brief Returns the raids for the \p version
     *
     * @param version Game version
     *
     * @return Den raids
     */
    std::vector<Raid> getRaids(Game version) const
    {
        return (version == Game::Sword) ? sword : shield;
    }

private:
    std::vector<Raid> shield;
    std::vector<Raid> sword;
};

#endif // DEN_HPP
