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

#ifndef STATICSEARCHER4_HPP
#define STATICSEARCHER4_HPP

#include <Core/Parents/Searchers/StaticSearcher.hpp>
#include <Core/Parents/States/StaticState.hpp>
#include <Core/RNG/RNGCache.hpp>
#include <mutex>

class StaticSearcher4 : public StaticSearcher
{
public:
    StaticSearcher4() = default;
    StaticSearcher4(u16 tid, u16 sid, u8 genderRatio, Method method, const StateFilter &filter);
    void setDelay(u32 minDelay, u32 maxDelay);
    void setState(u32 minAdvance, u32 maxAdvance);
    void startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max);
    void cancelSearch();
    std::vector<StaticState> getResults();
    int getProgress() const;

private:
    RNGCache cache;
    u32 minDelay;
    u32 maxDelay;
    u32 minAdvance;
    u32 maxAdvance;

    bool searching;
    int progress;
    std::vector<StaticState> results;
    std::mutex mutex;

    std::vector<StaticState> search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    std::vector<StaticState> searchMethod1(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    std::vector<StaticState> searchMethodJ(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    std::vector<StaticState> searchMethodK(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    std::vector<StaticState> searchWondercardIVs(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    std::vector<StaticState> normalMethodJ(StaticState state, u32 seed) const;
    std::vector<StaticState> synchMethodJ(StaticState state, u32 seed) const;
    std::vector<StaticState> cuteCharmMethodJ(StaticState state, u32 seed) const;
    std::vector<StaticState> normalMethodK(StaticState state, u32 seed) const;
    std::vector<StaticState> synchMethodK(StaticState state, u32 seed) const;
    std::vector<StaticState> cuteCharmMethodK(StaticState state, u32 seed) const;
    std::vector<StaticState> searchInitialSeeds(const std::vector<StaticState> &results) const;
};

#endif // STATICSEARCHER4_HPP
