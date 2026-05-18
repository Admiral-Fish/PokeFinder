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

#ifndef EGGSEARCHER3_HPP
#define EGGSEARCHER3_HPP

#include <Core/Gen3/Profile3.hpp>
#include <Core/Parents/Daycare.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/Searchers/Searcher.hpp>

class EggState3;

/**
 * @brief RS/FRLG egg seed searcher
 * Searches for held seed + pickup seed combinations that satisfy conditions
 */
class EggSearcher3 : public Searcher<Profile3, EggState3>
{
public:
    /**
     * @brief Construct a new EggSearcher3 object
     *
     * @param method Encounter method
     * @param compatability Parent compatability
     * @param daycare Daycare parent information
     * @param profile Profile Information
     * @param filter State filter
     * @param initialAdvancesHeld Initial number of held advances
     * @param maxAdvancesHeld Maximum number of held advances
     * @param offsetHeld Number of held advances to offset
     * @param initialAdvancesPickup Initial number of pickup advances
     * @param maxAdvancesPickup Maximum number of pickup advances
     * @param offsetPickup Number of pickup advances to offset
     */
    EggSearcher3(Method method, u8 compatability, const Daycare &daycare, const Profile3 &profile, const StateFilter &filter,
                 u32 initialAdvancesHeld, u32 maxAdvancesHeld, u32 offsetHeld, u32 initialAdvancesPickup, u32 maxAdvancesPickup,
                 u32 offsetPickup);

    /**
     * @brief Starts the search
     *
     * @param minIVs Minimum IV thresholds
     * @param maxIVs Maximum IV thresholds
     */
    void startSearch(const std::array<u8, 6> &minIVs, const std::array<u8, 6> &maxIVs);

private:
    StateFilter filter;
    u8 compatability;
    Daycare daycare;
    u32 initialAdvancesHeld;
    u32 maxAdvancesHeld;
    u32 offsetHeld;
    u32 initialAdvancesPickup;
    u32 maxAdvancesPickup;
    u32 offsetPickup;
    u8 iv1;
    u8 iv2;
    u8 inh;

    struct HeldEntry
    {
        u16 seed;
        u32 advances;
    };
};

#endif // EGGSEARCHER3_HPP
