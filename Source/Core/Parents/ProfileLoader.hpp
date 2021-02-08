/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef PROFILELOADER_HPP
#define PROFILELOADER_HPP

#include <Core/Gen3/Profile3.hpp>
#include <Core/Gen4/Profile4.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <vector>

namespace ProfileLoader
{
    void init(const std::string &location);
}

namespace ProfileLoader3
{
    std::vector<Profile3> getProfiles();
    void addProfile(const Profile3 &profile);
    void removeProfile(const Profile3 &remove);
    void updateProfile(const Profile3 &update, const Profile3 &original);
}

namespace ProfileLoader4
{
    std::vector<Profile4> getProfiles();
    void addProfile(const Profile4 &profile);
    void removeProfile(const Profile4 &remove);
    void updateProfile(const Profile4 &update, const Profile4 &original);
}

namespace ProfileLoader5
{
    std::vector<Profile5> getProfiles();
    void addProfile(const Profile5 &profile);
    void removeProfile(const Profile5 &remove);
    void updateProfile(const Profile5 &update, const Profile5 &original);
}

#endif // PROFILELOADER_HPP
