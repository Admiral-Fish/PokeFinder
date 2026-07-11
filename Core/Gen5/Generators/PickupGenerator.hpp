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

#ifndef PICKUPGENERATOR_HPP
#define PICKUPGENERATOR_HPP

#include <Core/Gen5/EncounterArea5.hpp>
#include <Core/Global.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <array>
#include <optional>
#include <vector>

class PickupState;

class PickupGenerator
{
public:
    struct Slot
    {
        bool active;
        u8 level;
        std::vector<u16> filter;
    };

    struct Request
    {
        u16 item;
        u8 count;
    };

    PickupGenerator(u32 initialAdvances, u32 maxAdvances, const std::array<Slot, 6> &pickupSlots, const Profile5 &profile,
                    bool includeInvalid = false);
    PickupGenerator(u32 initialAdvances, u32 maxAdvances, const std::array<Slot, 6> &pickupSlots, const EncounterArea5 &area,
                    const Profile5 &profile, bool includeInvalid = false);
    PickupGenerator(u32 initialAdvances, u32 maxAdvances, const std::array<Slot, 6> &pickupSlots, const std::vector<Request> &requests,
                    const EncounterArea5 &area, const Profile5 &profile, bool includeInvalid = false);
    PickupGenerator(u32 initialAdvances, u32 maxAdvances, const std::array<Slot, 6> &pickupSlots, const EncounterArea5 &area,
                    const Profile5 &profile, const WildStateFilter &filter, bool includeInvalid = false);

    std::vector<PickupState> generate(u64 seed, u32 ivAdvances = 0) const;

    static std::vector<u16> getLevelItems(u8 level);

private:
    bool compare(const std::array<u16, 6> &items) const;
    u16 getPickupItem(u8 level, u8 itemRand) const;

    u32 initialAdvances;
    u32 maxAdvances;
    std::array<Slot, 6> pickupSlots;
    std::vector<Request> requests;
    std::array<bool, 6> active;
    std::optional<EncounterArea5> area;
    Profile5 profile;
    WildStateFilter filter;
    bool includeInvalid;
};

#endif // PICKUPGENERATOR_HPP
