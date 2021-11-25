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

#ifndef DEN_HPP
#define DEN_HPP

#include <Core/Enum/Game.hpp>
#include <Core/Gen8/Raid.hpp>
#include <vector>

class Den
{
public:
    constexpr Den(u64 hash, const std::array<Raid, 12> &sword, const std::array<Raid, 12> &shield) :
        hash(hash), sword(sword), shield(shield)
    {
    }

    u64 getHash() const
    {
        return hash;
    }

    Raid getRaid(u8 index, Game version) const
    {
        return (version == Game::Sword) ? sword[index] : shield[index];
    }

    std::array<Raid, 12> getRaids(Game version) const
    {
        return (version == Game::Sword) ? sword : shield;
    }

private:
    u64 hash;
    std::array<Raid, 12> sword;
    std::array<Raid, 12> shield;
};

class DenEvent
{
public:
    DenEvent() = default;

    DenEvent(const std::vector<Raid> &sword, const std::vector<Raid> &shield) : sword(sword), shield(shield)
    {
    }

    Raid getRaid(u8 index, Game version) const
    {
        return (version == Game::Sword) ? sword[index] : shield[index];
    }

    std::vector<Raid> getRaids(Game version) const
    {
        return (version == Game::Sword) ? sword : shield;
    }

private:
    std::vector<Raid> sword;
    std::vector<Raid> shield;
};

#endif // DEN_HPP
