/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef WILDSEARCHER4_HPP
#define WILDSEARCHER4_HPP

#include <Core/Gen4/EncounterArea4.hpp>
#include <Core/Gen4/Filters/StateFilter4.hpp>
#include <Core/Parents/Searchers/WildSearcher.hpp>
#include <Core/RNG/RNGCache.hpp>
#include <mutex>
/**
 * @brief Wild encounter searcher for Gen4
 */
class WildSearcher4 : public WildSearcher<WildStateFilter4>
{
public:
    /**
     * @brief Construct a new WildSearcher4 object
     *
     * @param minAdvance Minimum advances
     * @param maxAdvance Maximum advances
     * @param minDelay Minimum delay
     * @param maxDelay Maximum delay
     * @param tid Trainer ID
     * @param sid Secret ID
     * @param version Game version
     * @param method Encounter method
     * @param encounter Encounter type
     * @param lead Encounter lead
     * @param shiny Whether Poke Radar is forced shiny
     * @param filter State filter
     */
    WildSearcher4(u32 minAdvance, u32 maxAdvance, u32 minDelay, u32 maxDelay, u16 tid, u16 sid, Game version, Method method,
                  Encounter encounter, Lead lead, bool shiny, const EncounterArea4 &encounterArea, const WildStateFilter4 &filter);

    /**
     * @brief Cancels the running search
     */
    void cancelSearch();

    /**
     * @brief Returns the progress of the running search
     *
     * @return Progress
     */
    int getProgress() const;

    /**
     * @brief Returns the states of the running search
     *
     * @return Vector of computed states
     */
    std::vector<WildSearcherState4> getResults();

    /**
     * @brief Starts the search
     *
     * @param min Minimum IVs
     * @param max Maximum IVs
     * @param index Pokeradar slot index
     */
    void startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max, u8 index);

private:
    EncounterArea4 encounterArea;
    std::mutex mutex;
    std::vector<u8> modifiedSlots;
    std::vector<WildSearcherState4> results;
    int progress;
    RNGCache cache;
    u32 maxAdvance;
    u32 minAdvance;
    u32 maxDelay;
    u32 minDelay;
    u16 thresh;
    bool searching;
    bool shiny;

    /**
     * @brief Searches for matching states from provided IVs
     *
     * @param hp HP IV
     * @param atk Atk IV
     * @param def Def IV
     * @param spa SpA IV
     * @param spd SpD IV
     * @param spe Spe IV
     * @param index Pokeradar slot index
     *
     * @return Vector of computed states
     */
    std::vector<WildSearcherState4> search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 index) const;

    /**
     * @brief Searches for initial seeds within min/max advances and min/max delay
     *
     * @param results Original results
     *
     * @return Computed results
     */
    std::vector<WildSearcherState4> searchInitialSeeds(const std::vector<WildSearcherState4> &results) const;

    /**
     * @brief Searches for matching states from provided IVs via Method J
     *
     * @param hp HP IV
     * @param atk Atk IV
     * @param def Def IV
     * @param spa SpA IV
     * @param spd SpD IV
     * @param spe Spe IV
     *
     * @return Vector of computed states
     */
    std::vector<WildSearcherState4> searchMethodJ(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;

    /**
     * @brief Searches for matching states from provided IVs via Method K
     *
     * @param hp HP IV
     * @param atk Atk IV
     * @param def Def IV
     * @param spa SpA IV
     * @param spd SpD IV
     * @param spe Spe IV
     *
     * @return Vector of computed states
     */
    std::vector<WildSearcherState4> searchMethodK(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;

    /**
     * @brief Searches for matching states from provided IVs via Poke Radar
     *
     * @param hp HP IV
     * @param atk Atk IV
     * @param def Def IV
     * @param spa SpA IV
     * @param spd SpD IV
     * @param spe Spe IV
     *
     * @return Vector of computed states
     */
    std::vector<WildSearcherState4> searchPokeRadar(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 index) const;
};

#endif // WILDSEARCHER4_HPP
