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

#ifndef ENCOUNTERAREA5_HPP
#define ENCOUNTERAREA5_HPP

#include <Core/Parents/EncounterArea.hpp>

/**
 * @brief Contains information about the encounters for an area. This includes location, rate, and the slots.
 */
class EncounterArea5 : public EncounterArea
{
public:
    /**
     * @brief Construct a new EncounterArea4 object
     *
     * @param location Location number
     * @param rate Encounter rate of the area
     * @param encounter Encounter type of the area
     * @param pokemon Available pokemon of the area
     */
    EncounterArea5(u8 location, u8 rate, Encounter type, const std::array<Slot, 12> &pokemon) : EncounterArea(location, rate, type, pokemon)
    {
    }
};

#endif // ENCOUNTERAREA5_HPP
