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

#include "EncounterArea8.hpp"
#include <Core/Enum/Game.hpp>

bool EncounterArea8::greatMarsh(Game version) const
{
    if ((version & Game::BDSP) != Game::None)
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

bool EncounterArea8::trophyGarden(Game version) const
{
    if ((version & Game::BDSP) != Game::None)
    {
        return location == 117;
    }
    return false;
}
