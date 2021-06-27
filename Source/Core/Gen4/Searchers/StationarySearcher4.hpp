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

#ifndef STATIONARYSEARCHER4_HPP
#define STATIONARYSEARCHER4_HPP

#include <Core/Parents/Searchers/StationarySearcher.hpp>
#include <Core/Parents/States/StationaryState.hpp>
#include <Core/RNG/RNGCache.hpp>
#include <mutex>

class StationarySearcher4 : public StationarySearcher
{
public:
    StationarySearcher4() = default;
    StationarySearcher4(u16 tid, u16 sid, u8 genderRatio, Method method, const StateFilter &filter);
    void setDelay(u32 minDelay, u32 maxDelay);
    void setState(u32 minAdvance, u32 maxAdvance);
    void startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max);
    void cancelSearch();
    std::vector<StationaryState> getResults();
    int getProgress() const;

private:
    RNGCache cache;
    u32 minDelay;
    u32 maxDelay;
    u32 minAdvance;
    u32 maxAdvance;

    bool searching;
    int progress;
    std::vector<StationaryState> results;
    std::mutex mutex;

    std::vector<StationaryState> search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    std::vector<StationaryState> searchMethod1(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    std::vector<StationaryState> searchMethodJ(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    std::vector<StationaryState> searchMethodK(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    std::vector<StationaryState> searchWondercardIVs(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    std::vector<StationaryState> normalMethodJ(StationaryState state, u32 seed) const;
    std::vector<StationaryState> synchMethodJ(StationaryState state, u32 seed) const;
    std::vector<StationaryState> cuteCharmMethodJ(StationaryState state, u32 seed) const;
    std::vector<StationaryState> normalMethodK(StationaryState state, u32 seed) const;
    std::vector<StationaryState> synchMethodK(StationaryState state, u32 seed) const;
    std::vector<StationaryState> cuteCharmMethodK(StationaryState state, u32 seed) const;
    std::vector<StationaryState> searchInitialSeeds(const std::vector<StationaryState> &results) const;
};

#endif // STATIONARYSEARCHER4_HPP
