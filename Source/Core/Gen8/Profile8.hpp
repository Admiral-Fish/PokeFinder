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

#ifndef PROFILE8_HPP
#define PROFILE8_HPP

#include <Core/Enum/Game.hpp>
#include <Core/Parents/Profile.hpp>

class Profile8 : public Profile
{
public:
    Profile8(Game version = Game::Sword) : Profile("-", version, 12345, 54321)
    {
    }

    Profile8(const std::string &name, Game version, u16 tid, u16 sid, bool shinyCharm) : Profile(name, version, tid, sid)
    {
    }

    bool getShinyCharm() const
    {
        return shinyCharm;
    }

private:
    bool shinyCharm;
};

#endif // PROFILE8_HPP
