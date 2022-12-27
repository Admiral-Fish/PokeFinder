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

#ifndef DENLOADER_HPP
#define DENLOADER_HPP

#include <Core/Global.hpp>
#include <array>
#include <string>

class Den;
class DenEvent;

/**
 * @brief Provides methods to get Raids from various Dens
 */
namespace DenLoader
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
     * @brief Returns the den event initialized by init()
     *
     * @return Event den
     */
    const DenEvent *getEvent();

    /**
     * @brief Returns the location of the den
     *
     * @param index Den index
     *
     * @return Den location
     */
    u8 getLocation(u16 index);

    /**
     * @brief Return the coordinates of the den
     *
     * @param index Den index
     *
     * @return Den coordinates
     */
    std::array<u16, 2> getCoordinates(u16 index);

    /**
     * @brief Initalize the DenLoader with what \p path to use for events.
     * This must be called before using getEvent().
     *
     * @param path Language
     */
    void init(const std::string &path);
};

#endif // DENLOADER_HPP
