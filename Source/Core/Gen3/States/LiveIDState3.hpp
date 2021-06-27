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

#ifndef LIVEIDSTATE3_HPP
#define LIVEIDSTATE3_HPP

#include <Core/Parents/States/IDState.hpp>
#include <string>

class LiveIDState3 : public IDState
{
public:
    LiveIDState3() = default;
    LiveIDState3(u32 advance, u16 tid, u16 sid, u16 initial, std::string shiny, std::string date);
    u16 getInitial() const;
    std::string getShiny() const;
    std::string getDate() const;

protected:
    u16 initial;
    std::string shiny;
    std::string date;
};

#endif // LIVEIDSTATE3_HPP
