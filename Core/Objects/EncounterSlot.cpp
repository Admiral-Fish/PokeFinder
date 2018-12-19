/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
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

Range::Range()
{
    min = 0;
    max = 99;
}

Range::Range(u8 min, u8 max)
{
    this->min = min;
    this->max = max;
}

u8 Range::getMax() const
{
    return max;
}

u8 Range::getMin() const
{
    return min;
}


u8 EncounterSlot::calcSlot(u8 compare, const QVector<Range> &ranges)
{
    u16 size = ranges.size();
    for (u16 i = 0; i < size; i++)
    {
        if (compare >= ranges.at(i).getMin() && compare <= ranges.at(i).getMax())
        {
            return i;
        }
    }
    return 0;
}

// Calcs the encounter slot for Method H 1/2/4 (Emerald, FRLG, RS)
u8 EncounterSlot::hSlot(u16 result, Encounter encounterType)
{
    u8 compare = result % 100;
    QVector<Range> ranges;
    switch (encounterType)
    {
        case Encounter::OldRod:
            ranges = { Range(0, 69), Range(70, 99) } ;
            return calcSlot(compare, ranges);
        case Encounter::GoodRod:
            ranges = { Range(0, 59), Range(60, 79), Range(80, 99) };
            return calcSlot(compare, ranges);
        case Encounter::SuperRod:
            ranges = { Range(0, 39), Range(40, 79), Range(80, 94), Range(95, 98), Range(99, 99) };
            return calcSlot(compare, ranges);
        case Encounter::Surfing:
        case Encounter::RockSmash:
            ranges = { Range(0, 59), Range(60, 89), Range(90, 94), Range(95, 98), Range(99, 99) };
            return calcSlot(compare, ranges);
        default:
            ranges = { Range(0, 19), Range(20, 39), Range(40, 49), Range(50, 59), Range(60, 69),
                       Range(70, 79), Range(80, 84), Range(85, 89), Range(90, 93), Range(94, 97),
                       Range(98, 98), Range(99, 99)
                 };
            return calcSlot(compare, ranges);
    }
}

// Calcs the encounter slot for Method J (DPPt)
u8 EncounterSlot::jSlot(u16 result, Encounter encounterType)
{
    u8 compare = result / 656;
    QVector<Range> ranges;
    switch (encounterType)
    {
        case Encounter::GoodRod:
        case Encounter::SuperRod:
            ranges = { Range(0, 39), Range(40, 79), Range(80, 94), Range(95, 98), Range(99, 99) };
            return calcSlot(compare, ranges);
        case Encounter::OldRod:
        case Encounter::Surfing:
            ranges = { Range(0, 59), Range(60, 89), Range(90, 94), Range(95, 98), Range(99, 99) };
            return calcSlot(compare, ranges);
        default:
            ranges = { Range(0, 19), Range(20, 39), Range(40, 49), Range(50, 59), Range(60, 69),
                       Range(70, 79), Range(80, 84), Range(85, 89), Range(90, 93), Range(94, 97),
                       Range(98, 98), Range(99, 99)
                 };
            return calcSlot(compare, ranges);
    }
}

// Calcs the encounter slot for Method K (HGSS)
u8 EncounterSlot::kSlot(u16 result, Encounter encounterType)
{
    u8 compare = result % 100;
    QVector<Range> ranges;
    switch (encounterType)
    {
        case Encounter::OldRod:
        case Encounter::GoodRod:
        case Encounter::SuperRod:
            ranges = { Range(0, 39), Range(40, 69), Range(70, 84), Range(85, 94), Range(95, 99) };
            return calcSlot(compare, ranges);
        case Encounter::Surfing:
            ranges = { Range(0, 59), Range(60, 89), Range(90, 94), Range(95, 98), Range(99, 99) };
            return calcSlot(compare, ranges);
        case Encounter::BugCatchingContest:
            ranges = { Range(80, 99), Range(60, 79), Range(50, 59), Range(40, 49), Range(30, 39),
                       Range(20, 29), Range(15, 19), Range(10, 14), Range(5, 9), Range(0, 4)
                 };
            return calcSlot(compare, ranges);
        case Encounter::SafariZone:
            return compare % 10;
        case Encounter::HeadButt:
            ranges = { Range(0, 49), Range(50, 64), Range(65, 79), Range(80, 89), Range(90, 94),
                       Range(95, 99)
                 };
            return calcSlot(compare, ranges);
        case Encounter::RockSmash: // Might be 80/20
            ranges = { Range(0, 89), Range(90, 99) };
            return calcSlot(compare, ranges);
        default:
            ranges = { Range(0, 19), Range(20, 39), Range(40, 49), Range(50, 59), Range(60, 69),
                       Range(70, 79), Range(80, 84), Range(85, 89), Range(90, 93), Range(94, 97),
                       Range(98, 98), Range(99, 99)
                 };
            return calcSlot(compare, ranges);
    }
}
