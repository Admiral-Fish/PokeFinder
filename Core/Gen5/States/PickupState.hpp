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

#ifndef PICKUPSTATE_HPP
#define PICKUPSTATE_HPP

#include <Core/Global.hpp>
#include <Core/Gen5/States/WildState5.hpp>
#include <array>
#include <optional>

class PickupState
{
public:
    PickupState(u32 advances, u16 prng, bool valid, const std::array<bool, 6> &active, const std::array<u16, 6> &items) :
        advances(advances), chatot(prng / 82), valid(valid), active(active), items(items)
    {
    }

    PickupState(u32 advances, u16 prng, bool valid, const std::array<bool, 6> &active, const std::array<u16, 6> &items,
                const std::optional<WildState5> &wild) :
        advances(advances), chatot(prng / 82), valid(valid), active(active), items(items), wild(wild)
    {
    }

    u32 getAdvances() const
    {
        return advances;
    }

    u8 getChatot() const
    {
        return chatot;
    }

    bool getActive(u8 slot) const
    {
        return active[slot];
    }

    u16 getItem(u8 slot) const
    {
        return items[slot];
    }

    bool isValid() const
    {
        return valid;
    }

    const std::optional<WildState5> &getWild() const
    {
        return wild;
    }

private:
    u32 advances;
    u8 chatot;
    bool valid;
    std::array<bool, 6> active;
    std::array<u16, 6> items;
    std::optional<WildState5> wild;
};

#endif // PICKUPSTATE_HPP
