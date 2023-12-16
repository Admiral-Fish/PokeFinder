/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef EVENTSEARCHER4_HPP
#define EVENTSEARCHER4_HPP

#include <Core/Gen4/Profile4.hpp>
#include <Core/Gen4/States/State4.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/Searchers/Searcher.hpp>

/**
 * @brief Event encounter searcher for Gen4
 */
class EventSearcher4 : public Searcher<Profile4, SearcherState4>
{
public:
    /**
     * @brief Construct a new EventSearcher4 object
     *
     * @param minAdvance Minimum advances
     * @param maxAdvance Maximum advances
     * @param minDelay Minimum delay
     * @param maxDelay Maximum delay
     * @param profile Profile Information
     * @param filter State filter
     */
    EventSearcher4(u32 minAdvance, u32 maxAdvance, u32 minDelay, u32 maxDelay, const Profile4 &profile, const StateFilter &filter);

    /**
     * @brief Starts the search
     *
     * @param min Minimum IVs
     * @param max Maximum IVs
     * @param species Pokemon specie
     * @param nature Pokemon nature
     * @param level Pokemon level
     */
    void startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max, u16 species, u8 nature, u8 level);

private:
    u32 maxAdvance;
    u32 minAdvance;
    u32 maxDelay;
    u32 minDelay;
    StateFilter filter;

    /**
     * @brief Searches for matching states from provided IVs
     *
     * @param hp HP IV
     * @param atk Atk IV
     * @param def Def IV
     * @param spa SpA IV
     * @param spd SpD IV
     * @param spe Spe IV
     * @param species Pokemon specie
     * @param nature Pokemon nature
     * @param level Pokemon level
     *
     * @return Vector of computed states
     */
    std::vector<SearcherState4> search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u16 species, u8 nature, u8 level) const;

    /**
     * @brief Searches for initial seeds within min/max advances and min/max delay
     *
     * @param results Original results
     *
     * @return Computed results
     */
    std::vector<SearcherState4> searchInitialSeeds(const std::vector<SearcherState4> &results) const;
};

#endif // EVENTSEARCHER4_HPP
