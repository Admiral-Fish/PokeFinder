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

#ifndef UNOWNSEARCHER4_HPP
#define UNOWNSEARCHER4_HPP

#include <Core/Gen4/EncounterArea4.hpp>
#include <Core/Parents/Searchers/UnownSearcher.hpp>
#include <Core/Parents/Searchers/WildSearcher.hpp>
#include <Core/Gen4/States/UnownState4.hpp>
#include <Core/RNG/RNGCache.hpp>
#include <mutex>

class UnownSearcher4 : public WildSearcher
{
public:
    UnownSearcher4() = default;
    UnownSearcher4(u16 tid, u16 sid, u8 genderRatio, Method method, const StateFilter &filter);
    void setEncounterArea(const u8 encounterArea);
    void setDelay(u32 minDelay, u32 maxDelay);
    void setState(u32 minAdvance, u32 maxAdvance);
    void startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max);
    void cancelSearch();
    std::vector<UnownState4> getResults();
    int getProgress() const;

private:
    RNGCache cache;
    u8 encounterArea;
    u32 minDelay;
    u32 maxDelay;
    u32 minAdvance;
    u32 maxAdvance;
    u8 thresh, suctionCupThresh;
    u16 rock;

    bool searching;
    int progress;
    std::vector<UnownState4> results;
    std::mutex mutex;

    std::vector<UnownState4> search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    std::vector<UnownState4> searchMethodJ(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    std::vector<UnownState4> searchMethodK(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    std::vector<UnownState4> searchChainedShiny(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    std::vector<UnownState4> searchInitialSeeds(const std::vector<UnownState4> &results) const;

    std::vector<UnownState4> normalMethodJ(UnownState4 state, u32 seed) const;
    std::vector<UnownState4> synchMethodJ(UnownState4 state, u32 seed) const;
    std::vector<UnownState4> cuteCharmMethodJ(UnownState4 state, u32 seed) const;
    bool encounterMethodJ(UnownState4 &state, u32 seed) const;
    std::vector<UnownState4> normalMethodK(UnownState4 state, u32 seed) const;
    std::vector<UnownState4> synchMethodK(UnownState4 state, u32 seed) const;
    std::vector<UnownState4> cuteCharmMethodK(UnownState4 state, u32 seed) const;
    bool encounterMethodK(UnownState4&state, u32 seed) const;
};

#endif // UNOWNSEARCHER4_HPP
