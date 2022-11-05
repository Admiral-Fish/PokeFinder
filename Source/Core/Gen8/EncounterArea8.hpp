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

#ifndef ENCOUNTERAREA8_HPP
#define ENCOUNTERAREA8_HPP

#include <Core/Parents/EncounterArea.hpp>
#include <Core/Parents/Slot.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/RNG/Xorshift.hpp>

class EncounterArea8 : public EncounterArea
{
public:
    EncounterArea8(u8 location, u8 rate, Encounter type, const std::vector<Slot> &pokemon) : EncounterArea(location, rate, type, pokemon)
    {
    }

    template <bool diff>
    u8 calculateLevel(u8 encounterSlot, RNGList<u32, Xorshift, 128> &rngList, bool force) const
    {
        if constexpr (diff)
        {
            const Slot &slot = pokemon[encounterSlot];

            u8 min = slot.getMinLevel();
            u8 max = slot.getMaxLevel();
            u8 range = max - min + 1;

            u8 rand = rngList.next() % range;
            if (force && (rngList.next() % 2) != 0)
            {
                return max;
            }

            return min + rand;
        }
        else
        {
            const Slot &slot = pokemon[encounterSlot];
            u8 level = slot.getMaxLevel();
            if (force && (rngList.next() % 2) != 0)
            {
                for (const Slot &s : pokemon)
                {
                    if (s.getSpecie() == slot.getSpecie())
                    {
                        level = std::max(level, s.getMaxLevel());
                    }
                }
            }
            return level;
        }
    }
};

#endif // ENCOUNTERAREA8_HPP
