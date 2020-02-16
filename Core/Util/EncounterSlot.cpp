/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <vector>

namespace
{
    u8 calcSlot(u8 compare, const std::vector<u8> &ranges)
    {
        for (u8 i = 0; i < ranges.size(); i++)
        {
            if (compare < ranges.at(i))
            {
                return i;
            }
        }
        return 255;
    }

    u8 calcSlot(u8 compare, const std::vector<std::pair<u8, u8>> &ranges)
    {
        for (u8 i = 0; i < ranges.size(); i++)
        {
            if (compare >= ranges.at(i).first && compare <= ranges.at(i).second)
            {
                return i;
            }
        }
        return 255;
    }
}

namespace EncounterSlot
{
    // Calcs the encounter slot for Method H 1/2/4 (Emerald, FRLG, RS)
    u8 hSlot(u16 result, Encounter encounter)
    {
        u8 compare = result % 100;
        switch (encounter)
        {
        case Encounter::OldRod:
            return calcSlot(compare, { 70, 100 });
        case Encounter::GoodRod:
            return calcSlot(compare, { 60, 80, 100 });
        case Encounter::SuperRod:
            return calcSlot(compare, { 40, 80, 95, 99, 100 });
        case Encounter::Surfing:
        case Encounter::RockSmash:
            return calcSlot(compare, { 60, 90, 95, 99, 100 });
        default:
            return calcSlot(compare, { 20, 40, 50, 60, 70, 80, 85, 90, 94, 98, 99, 100 });
        }
    }

    // Calcs the encounter slot for Method J (DPPt)
    u8 jSlot(u16 result, Encounter encounter)
    {
        u8 compare = result / 656;
        switch (encounter)
        {
        case Encounter::GoodRod:
        case Encounter::SuperRod:
            return calcSlot(compare, { 40, 80, 95, 99, 100 });
        case Encounter::OldRod:
        case Encounter::Surfing:
            return calcSlot(compare, { 60, 90, 95, 99, 100 });
        default:
            return calcSlot(compare, { 20, 40, 50, 60, 70, 80, 85, 90, 94, 98, 99, 100 });
        }
    }

    // Calcs the encounter slot for Method K (HGSS)
    u8 kSlot(u16 result, Encounter encounter)
    {
        u8 compare = result % 100;
        switch (encounter)
        {
        case Encounter::OldRod:
        case Encounter::GoodRod:
        case Encounter::SuperRod:
            return calcSlot(compare, { 40, 70, 85, 95, 100 });
        case Encounter::Surfing:
            return calcSlot(compare, { 60, 90, 95, 99, 100 });
        case Encounter::BugCatchingContest:
            return calcSlot(
                compare,
                { { 80, 99 }, { 60, 79 }, { 50, 59 }, { 40, 49 }, { 30, 39 }, { 20, 29 }, { 15, 19 }, { 10, 14 }, { 5, 9 }, { 0, 4 } });
        case Encounter::SafariZone:
            return compare % 10;
        case Encounter::HeadButt:
            return calcSlot(compare, { 50, 65, 80, 90, 95, 100 });
        case Encounter::RockSmash:
            return calcSlot(compare, { 80, 100 });
        default:
            return calcSlot(compare, { 20, 40, 50, 60, 70, 80, 85, 90, 94, 98, 99, 100 });
        }
    }
}
