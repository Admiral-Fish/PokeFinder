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

#include "EncounterSlot.hpp"
#include <Core/Enum/Encounter.hpp>
#include <array>

/**
 * @brief Calculates the encounter slot table from the \p ranges
 *
 * @tparam size Number of entries in \p ranges
 * @tparam greater Whether to compare >= or <
 * @param ranges Table PRNG range values
 *
 * @return Encounter slot table
 */
template <int size, bool greater = false>
static consteval std::array<u8, 100> computeTable(const std::array<int, size> &ranges)
{
    std::array<u8, 100> table;

    int r = greater ? 99 : 0;
    for (int i = 0; i < size; i++)
    {
        u8 range = ranges[i];
        if constexpr (greater)
        {
            for (; r >= range; r--)
            {
                table[r] = i;
            }
        }
        else
        {
            for (; r < range; r++)
            {
                table[r] = i;
            }
        }
    }

    return table;
}

// Ground
constexpr auto grass = computeTable<12>(std::array<int, 12> { 20, 40, 50, 60, 70, 80, 85, 90, 94, 98, 99, 100 });
constexpr auto rocksmash = computeTable<2>(std::array<int, 2> { 80, 100 });
constexpr auto bug = computeTable<10, true>(std::array<int, 10> { 80, 60, 50, 40, 30, 20, 15, 10, 5, 0 });
constexpr auto headbutt = computeTable<6>(std::array<int, 6> { 50, 65, 80, 90, 95, 100 });

// Water
constexpr auto water0 = computeTable<2>(std::array<int, 2> { 70, 100 });
constexpr auto water1 = computeTable<3>(std::array<int, 3> { 60, 80, 100 });
constexpr auto water2 = computeTable<5>(std::array<int, 5> { 40, 80, 95, 99, 100 });
constexpr auto water3 = computeTable<5>(std::array<int, 5> { 40, 70, 85, 95, 100 });
constexpr auto water4 = computeTable<5>(std::array<int, 5> { 60, 90, 95, 99, 100 });

// BW Lucky Power
// clang-format off
constexpr std::array<std::array<u8, 100>, 4> grassBW = {
    grass,
    computeTable<12>(std::array<int, 12> { 10, 20, 30, 40, 50, 60, 70, 80, 85, 90, 95, 100 }),
    computeTable<12>(std::array<int, 12> { 5, 10, 15, 20, 30, 40, 50, 60, 70, 80, 90, 100 }),
    computeTable<12>(std::array<int, 12> { 1, 2, 6, 10, 15, 20, 30, 40, 50, 60, 80, 100 })
};

constexpr std::array<std::array<u8, 100>, 4> surfBW = {
    water4,
    computeTable<5>(std::array<int, 5> { 50, 80, 90, 95, 100 }),
    computeTable<5>(std::array<int, 5> { 40, 70, 80, 90, 100 }),
    computeTable<5>(std::array<int, 5> { 30, 50, 60, 80, 100 })
};

constexpr std::array<std::array<u8, 100>, 4> fishBW = {
    water2,
    computeTable<5>(std::array<int, 5> { 40, 75, 90, 95, 100 }),
    computeTable<5>(std::array<int, 5> { 30, 60, 80, 90, 100 }),
    computeTable<5>(std::array<int, 5> { 20, 40, 60, 80, 100 })
};
// clang-format on

namespace EncounterSlot
{
    u8 hSlot(u8 rand, Encounter encounter)
    {
        switch (encounter)
        {
        case Encounter::OldRod:
            return water0[rand];
        case Encounter::GoodRod:
            return water1[rand];
        case Encounter::SuperRod:
            return water2[rand];
        case Encounter::Surfing:
        case Encounter::RockSmash:
            return water4[rand];
        default:
            return grass[rand];
        }
    }

    u8 jSlot(u8 rand, Encounter encounter)
    {
        switch (encounter)
        {
        case Encounter::GoodRod:
        case Encounter::SuperRod:
            return water2[rand];
        case Encounter::OldRod:
        case Encounter::Surfing:
            return water4[rand];
        default:
            return grass[rand];
        }
    }

    u8 kSlot(u8 rand, Encounter encounter)
    {
        switch (encounter)
        {
        case Encounter::OldRod:
        case Encounter::GoodRod:
        case Encounter::SuperRod:
            return water3[rand];
        case Encounter::Surfing:
            return water4[rand];
        case Encounter::BugCatchingContest:
            return bug[rand];
        case Encounter::Headbutt:
        case Encounter::HeadbuttAlt:
        case Encounter::HeadbuttSpecial:
            return headbutt[rand];
        case Encounter::RockSmash:
            return rocksmash[rand];
        default:
            return grass[rand];
        }
    }

    u8 bwSlot(u8 rand, Encounter encounter, u8 luckyPower)
    {
        switch (encounter)
        {
        case Encounter::SuperRod:
            return fishBW[luckyPower][rand];
        case Encounter::Surfing:
            return surfBW[luckyPower][rand];
        default:
            return grassBW[luckyPower][rand];
        }
    }

    u8 bdspSlot(u8 rand, Encounter encounter)
    {
        switch (encounter)
        {
        case Encounter::GoodRod:
        case Encounter::SuperRod:
            return water2[rand];
        case Encounter::OldRod:
        case Encounter::Surfing:
            return water4[rand];
        default:
            return grass[rand];
        }
    }
}
