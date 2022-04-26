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

#include "EncounterArea4.hpp"
#include <Core/Enum/Game.hpp>

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
