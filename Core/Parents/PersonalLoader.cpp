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

#include "PersonalLoader.hpp"
#include <Core/Enum/Game.hpp>
#include <Core/Resources/Personal.hpp>

namespace PersonalLoader
{
    const PersonalInfo *getPersonal(Game version)
    {
        if ((version & Game::Gen3) != Game::None)
        {
            return PERSONAL_RSEFRLG.data();
        }
        else if ((version & Game::Diamond) != Game::None)
        {
            return PERSONAL_D.data();
        }
        else if ((version & Game::Pearl) != Game::None)
        {
            return PERSONAL_P.data();
        }
        else if ((version & Game::Platinum) != Game::None)
        {
            return PERSONAL_PT.data();
        }
        else if ((version & Game::HGSS) != Game::None)
        {
            return PERSONAL_HGSS.data();
        }
        else if ((version & Game::BW) != Game::None)
        {
            return PERSONAL_BW.data();
        }
        else if ((version & Game::BW2) != Game::None)
        {
            return PERSONAL_B2W2.data();
        }
        else if ((version & Game::SwSh) != Game::None)
        {
            return PERSONAL_SWSH.data();
        }
        else
        {
            return PERSONAL_BDSP.data();
        }
    }

    const PersonalInfo *getPersonal(Game version, u16 specie, u8 form)
    {
        const PersonalInfo *info = getPersonal(version);
        const PersonalInfo *base = &info[specie];

        // Determine if we need to offset the index based off form
        u16 formIndex = base->getFormStatIndex();
        if (form == 0 || formIndex == 0)
        {
            return base;
        }
        return &info[formIndex + form - 1];
    }
}
