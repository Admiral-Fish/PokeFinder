/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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
        if ((version & Game::Emerald) != Game::None)
        {
            return personal_e.data();
        }
        else if ((version & Game::RS) != Game::None)
        {
            return personal_rs.data();
        }
        else if ((version & Game::FireRed) != Game::None)
        {
            return personal_fr.data();
        }
        else if ((version & Game::LeafGreen) != Game::None)
        {
            return personal_lg.data();
        }
        else if ((version & Game::DP) != Game::None)
        {
            return personal_dp.data();
        }
        else if ((version & Game::Platinum) != Game::None)
        {
            return personal_pt.data();
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

    PersonalInfo getPersonal(Game version, u16 species, u8 form)
    {
        auto *info = getPersonal(version);
        PersonalInfo base = info[species];

        u16 formIndex = base.getFormStatIndex();

        if (form == 0 || formIndex == 0)
        {
            return base;
        }

        return info[formIndex + form - 1];
    }
}