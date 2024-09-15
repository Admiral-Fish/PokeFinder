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

#include "EncounterArea3.hpp"
#include <Core/Enum/Game.hpp>

bool EncounterArea3::feebasLocation(Game version) const
{
    if ((version & Game::Emerald) != Game::None)
    {
        return location == 33;
    }
    else if ((version & Game::RS) != Game::None)
    {
        return location == 73;
    }
    return false;
}

bool EncounterArea3::safariZone(Game version) const
{
    if ((version & Game::RS) != Game::None)
    {
        switch (location)
        {
        case 90: // Safari Zone Northeast
        case 187: // Safari Zone Northeast
        case 89: // Safari Zone Northwest
        case 186: // Safari Zone Northwest
        case 92: // Safari Zone Southeast
        case 189: // Safari Zone Southeast
        case 91: // Safari Zone Southwest
        case 188: // Safari Zone Southwest
            return true;
        }
    }
    else if ((version & Game::Emerald) != Game::None)
    {
        switch (location)
        {
        case 73: // Safari Zone North
        case 98: // Safari Zone Northeast
        case 74: // Safari Zone Northwest
        case 20: // Safari Zone South
        case 97: // Safari Zone Southeast
        case 72: // Safari Zone Southwest
            return true;
        }
    }
    return false;
}

bool EncounterArea3::tanobyChamber(Game version) const
{
    if ((version & Game::FRLG) != Game::None)
    {
        switch (location)
        {
        case 0: // Monean Chamber
        case 1: // Liptoo Chamber
        case 2: // Weepth Chamber
        case 3: // Dilford Chamber
        case 4: // Scufib Chamber
        case 5: // Rixy Chamber
        case 6: // Viapois Chamber
            return true;
        }
    }
    return false;
}
