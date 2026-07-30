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

#ifndef PHENOMENONAREA_HPP
#define PHENOMENONAREA_HPP

#include <Core/Global.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <array>
#include <string>
#include <vector>

enum class PhenomenonType : u8
{
    Grass,
    Cave,
    Bridge
};

/**
 * @brief Contains information about the encounters for an area. This includes location, rate, and the items.
 */
class PhenomenonArea
{
public:
    /**
     * @brief Construct a new HiddenGrottoArea object
     *
     * @param location Location number
     * @param type Location area type
     */
    PhenomenonArea(u8 location, PhenomenonType type);

    /**
     * @brief Returns the item from the \p rng state
     *
     * @param rng PRNG state
     *
     * @return Item
     */
    u16 getItem(BWRNG &rng) const;

    /**
     * @brief Return vector of names of all item slots
     *
     * @return Vector of item name
     */
    std::vector<std::string> getItemNames() const;

    /**
     * @brief Returns the location of the area
     *
     * @return Location number
     */
    u8 getLocation() const;

    /**
     * @brief Return the encounter rate of the area
     *
     * @return Encounter rate
     */
    u16 getRate() const;

    /**
     * @brief Return the item numbers of unique item of the area
     *
     * @return Vector of item numbers
     */
    std::vector<u16> getUniqueItems() const;

private:
    std::array<u16, 28> items;
    PhenomenonType type;
    u8 itemCount;
    u8 location;
};

#endif // PHENOMENONAREA_HPP
