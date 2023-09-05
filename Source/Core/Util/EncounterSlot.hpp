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

#ifndef ENCOUNTERSLOT_HPP
#define ENCOUNTERSLOT_HPP

#include <Core/Global.hpp>

enum class Encounter : u8;

namespace EncounterSlot
{
    /**
     * @brief Calculates the encounter slot for Gen 3
     *
     * @param rand PRNG call
     * @param encounter Encounter type
     *
     * @return Encounter slot
     */
    u8 hSlot(u8 rand, Encounter encounter);

    /**
     * @brief Calculates the encounter slot for DPPt
     *
     * @param rand PRNG call
     * @param encounter Encounter type
     *
     * @return Encounter slot
     */
    u8 jSlot(u8 rand, Encounter encounter);

    /**
     * @brief Calculates the encounter slot for HGSS
     *
     * @param rand PRNG call
     * @param encounter Encounter type
     *
     * @return Encounter slot
     */
    u8 kSlot(u8 rand, Encounter encounter);

    /**
     * @brief Calculates the encounter slot for BW/BW2
     *
     * @param rand PRNG call
     * @param encounter Encounter type
     * @param luckyPower Lucky power level
     *
     * @return Encounter slot
     */
    u8 bwSlot(u8 rand, Encounter encounter, u8 luckyPower);

    /**
     * @brief Calculates the encounter slot for BDSP
     *
     * @param rand PRNG call
     * @param encounter Encounter type
     *
     * @return Encounter slot
     */
    u8 bdspSlot(u8 rand, Encounter encounter);
};

#endif // ENCOUNTERSLOT_HPP
