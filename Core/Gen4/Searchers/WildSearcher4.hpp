/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Parents/Searchers/WildSearcher.hpp>
#include <Core/Parents/States/WildState.hpp>
#include <Core/RNG/RNGCache.hpp>
#include <mutex>

class WildSearcher4 : public WildSearcher
{
public:
    WildSearcher4() = default;
    WildSearcher4(u16 tid, u16 sid, u8 genderRatio, Method method, const StateFilter &filter);
    void setEncounterArea(const EncounterArea4 &encounterArea);
    void setDelay(u32 minDelay, u32 maxDelay);
    void setState(u32 minAdvance, u32 maxAdvance);
    void startSearch(const QVector<u8> &min, const QVector<u8> &max);
    void cancelSearch();
    QVector<WildState> getResults();
    int getProgress() const;

private:
    RNGCache cache;
    EncounterArea4 encounterArea;
    u32 minDelay;
    u32 maxDelay;
    u32 minAdvance;
    u32 maxAdvance;
    u8 thresh, suctionCupThresh;
    u16 rock;

    bool searching;
    int progress;
    QVector<WildState> results;
    std::mutex mutex;

    QVector<WildState> search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    QVector<WildState> searchMethodJ(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    QVector<WildState> searchMethodK(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    QVector<WildState> searchChainedShiny(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    QVector<WildState> searchInitialSeeds(const QVector<WildState> &results) const;

    QVector<WildState> normalMethodJ(WildState currentState, u32 seed) const;
    QVector<WildState> synchMethodJ(WildState currentState, u32 seed) const;
    QVector<WildState> cuteCharmMethodJ(WildState currentState, u32 seed) const;
    bool encounterMethodJ(WildState &currentState, u32 seed) const;
    QVector<WildState> normalMethodK(WildState currentState, u32 seed) const;
    QVector<WildState> synchMethodK(WildState currentState, u32 seed) const;
    QVector<WildState> cuteCharmMethodK(WildState currentState, u32 seed) const;
    bool encounterMethodK(WildState &currentState, u32 seed) const;
};

#endif // WILDSEARCHER4_HPP
