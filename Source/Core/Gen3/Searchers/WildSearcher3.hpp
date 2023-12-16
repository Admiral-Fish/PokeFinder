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

#ifndef WILDSEARCHER3_HPP
#define WILDSEARCHER3_HPP

#include <Core/Gen3/EncounterArea3.hpp>
#include <Core/Gen3/Profile3.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/Searchers/WildSearcher.hpp>

class WildSearcherState;

/**
 * @brief Wild encounter searcher for Gen3
 */
class WildSearcher3 : public WildSearcher<EncounterArea3, Profile3, WildStateFilter, WildSearcherState>
{
public:
    /**
     * @brief Construct a new WildSearcher3 object
     *
     * @param method Encounter method
     * @param lead Encounter lead
     * @param area Wild pokemon info
     * @param profile Profile Information
     * @param filter State filter
     */
    WildSearcher3(Method method, Lead lead, const EncounterArea3 &area, const Profile3 &profile, const WildStateFilter &filter);

    /**
     * @brief Starts the search
     *
     * @param min Minimum IVs
     * @param max Maximum IVs
     */
    void startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max);

private:
    std::vector<u8> modifiedSlots;
    u16 rate;
    bool ivAdvance;

    /**
     * @brief Searches for matching states from provided IVs
     *
     * @param hp HP IV
     * @param atk Atk IV
     * @param def Def IV
     * @param spa SpA IV
     * @param spd SpD IV
     * @param spe Spe IV
     * @param safari Whether the encounter location is the Safari Zone in RSE
     *
     * @return Vector of computed states
     */
    std::vector<WildSearcherState> search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, bool safari, bool tanoby) const;
};

#endif // WILDSEARCHER3_HPP
