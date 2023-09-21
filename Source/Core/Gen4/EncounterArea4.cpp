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

#include "EncounterArea4.hpp"
#include <Core/Enum/Game.hpp>

constexpr std::array<u8, 20> unown0 = { 0, 1, 2, 6, 7, 9, 10, 11, 12, 14, 15, 16, 18, 19, 20, 21, 22, 23, 24, 25 };
constexpr std::array<u8, 1> unown1 = { 5 };
constexpr std::array<u8, 1> unown2 = { 17 };
constexpr std::array<u8, 1> unown3 = { 8 };
constexpr std::array<u8, 1> unown4 = { 13 };
constexpr std::array<u8, 1> unown5 = { 4 };
constexpr std::array<u8, 1> unown6 = { 3 };
constexpr std::array<u8, 2> unown7 = { 26, 27 };

bool EncounterArea4::greatMarsh(Game version) const
{
    if ((version & Game::DPPt) != Game::None)
    {
        switch (location)
        {
        case 23: // Great Marsh Area 1
        case 24: // Great Marsh Area 2
        case 25: // Great Marsh Area 3
        case 26: // Great Marsh Area 4
        case 27: // Great Marsh Area 5
        case 28: // Great Marsh Area 6
            return true;
        }
    }
    return false;
}

bool EncounterArea4::safariZone(Game version) const
{
    if ((version & Game::HGSS) != Game::None)
    {
        switch (location)
        {
        case 148: // Safari Zone Gate
        case 149: // Safari Zone (Plains)
        case 150: // Safari Zone (Meadow)
        case 151: // Safari Zone (Savannah)
        case 152: // Safari Zone (Peak)
        case 153: // Safari Zone (Rocky Beach)
        case 154: // Safari Zone (Wetland)
        case 155: // Safari Zone (Forest)
        case 156: // Safari Zone (Swamp)
        case 157: // Safari Zone (Marshland)
        case 158: // Safari Zone (Wasteland)
        case 159: // Safari Zone (Mountain)
        case 160: // Safari Zone (Desert)
            return true;
        }
    }
    return false;
}

bool EncounterArea4::trophyGarden(Game version) const
{
    if ((version & Game::DPPt) != Game::None)
    {
        return location == 117;
    }
    return false;
}

u8 EncounterArea4::unownForm(u16 prng) const
{
    switch (location)
    {
    case 29:
        return unown7[prng % unown7.size()];
    case 30:
        return unown0[prng % unown0.size()];
    case 32:
        return unown1[prng % unown1.size()];
    case 34:
        return unown2[prng % unown2.size()];
    case 40:
        return unown3[prng % unown3.size()];
    case 41:
        return unown4[prng % unown4.size()];
    case 42:
        return unown5[prng % unown5.size()];
    case 43:
        return unown6[prng % unown6.size()];
    }
    return 0;
}
