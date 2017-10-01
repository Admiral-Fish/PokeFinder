/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish and bumba
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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

#include "Nature.hpp"

std::vector<QString> Nature::GetNatures()
{
    std::vector<QString> natures = { QObject::tr("Adamant"), QObject::tr("Bold"), QObject::tr("Brave"), QObject::tr("Calm"),
                                     QObject::tr("Careful"), QObject::tr("Hasty"), QObject::tr("Impish"), QObject::tr("Jolly"),
                                     QObject::tr("Lonely"), QObject::tr("Mild"), QObject::tr("Modest"), QObject::tr("Naive"),
                                     QObject::tr("Naughty"), QObject::tr("Quiet"), QObject::tr("Rash"), QObject::tr("Relaxed"),
                                     QObject::tr("Sassy"), QObject::tr("Timid"), QObject::tr("Gentle"), QObject::tr("Lax"),
                                     QObject::tr("Bashful"), QObject::tr("Docile"), QObject::tr("Hardy"), QObject::tr("Quirky"),
                                     QObject::tr("Serious") };
    return natures;
}

uint32_t Nature::GetAdjustedNature(uint32_t nature)
{
    switch (nature)
    {
        case 0:
            return 3;
        case 1:
            return 5;
        case 2:
            return 2;
        case 3:
            return 20;
        case 4:
            return 23;
        case 5:
            return 11;
        case 6:
            return 8;
        case 7:
            return 13;
        case 8:
            return 1;
        case 9:
            return 16;
        case 10:
            return 15;
        case 11:
            return 14;
        case 12:
            return 4;
        case 13:
            return 17;
        case 14:
            return 19;
        case 15:
            return 7;
        case 16:
            return 22;
        case 17:
            return 10;
        case 18:
            return 21;
        case 19:
            return 9;
        case 20:
            return 18;
        case 21:
            return 6;
        case 22:
            return 0;
        case 23:
            return 24;
        case 24:
            return 18;
        default:
            return 0;
    }
}

uint32_t Nature::GetReversedNature(uint32_t nature)
{
    switch (nature)
    {
        case 0:
            return 22;
        case 1:
            return 8;
        case 2:
            return 2;
        case 3:
            return 0;
        case 4:
            return 12;
        case 5:
            return 1;
        case 6:
            return 21;
        case 7:
            return 15;
        case 8:
            return 6;
        case 9:
            return 19;
        case 10:
            return 17;
        case 11:
            return 5;
        case 12:
            return 24;
        case 13:
            return 7;
        case 14:
            return 11;
        case 15:
            return 10;
        case 16:
            return 9;
        case 17:
            return 13;
        case 18:
            return 20;
        case 19:
            return 14;
        case 20:
            return 3;
        case 21:
            return 18;
        case 22:
            return 16;
        case 23:
            return 4;
        case 24:
            return 23;
        default:
            return 0;
    }
}
