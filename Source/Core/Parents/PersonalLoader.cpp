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

#include "PersonalLoader.hpp"
#include <Core/Resources/Personal.hpp>
#include <Core/Enum/Game.hpp>

namespace PersonalLoader
{
    const PersonalInfo *getPersonal(Game version)
    {
        if ((version & Game::Gen3) != Game::None)
        {
            return personal_rsefrlg.data();
        }
        else if ((version & Game::DPPt) != Game::None)
        {
            return personal_dppt.data();
        }
        else if ((version & Game::HGSS) != Game::None)
        {
            return personal_hgss.data();
        }
        else if ((version & Game::BW) != Game::None)
        {
            return personal_bw.data();
        }
        else if ((version & Game::BW2) != Game::None)
        {
            return personal_b2w2.data();
        }
        else if ((version & Game::SwSh) != Game::None)
        {
            return personal_swsh.data();
        }
        else
        {
            return personal_bdsp.data();
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
