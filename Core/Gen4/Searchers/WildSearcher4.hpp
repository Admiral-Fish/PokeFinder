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
#include <Core/Parents/Frames/WildFrame.hpp>
#include <Core/Parents/Searchers/WildSearcher.hpp>
#include <Core/RNG/RNGCache.hpp>
#include <mutex>

class WildSearcher4 : public WildSearcher
{
public:
    WildSearcher4() = default;
    WildSearcher4(u16 tid, u16 sid, u8 genderRatio, Method method, const FrameFilter &filter);
    void setEncounterArea(const EncounterArea4 &encounterArea);
    void setDelay(u32 minDelay, u32 maxDelay);
    void setFrame(u32 minFrame, u32 maxFrame);
    void startSearch(const QVector<u8> &min, const QVector<u8> &max);
    void cancelSearch();
    QVector<WildFrame> getResults();
    int getProgress() const;

private:
    RNGCache cache;
    EncounterArea4 encounterArea;
    u32 minDelay;
    u32 maxDelay;
    u32 minFrame;
    u32 maxFrame;
    u8 thresh, suctionCupThresh;
    u16 rock;

    bool searching;
    int progress;
    QVector<WildFrame> results;
    std::mutex mutex;

    QVector<WildFrame> search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    QVector<WildFrame> searchMethodJ(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    QVector<WildFrame> searchMethodK(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    QVector<WildFrame> searchChainedShiny(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    QVector<WildFrame> searchInitialSeeds(const QVector<WildFrame> &results) const;

    QVector<WildFrame> normalMethodJ(WildFrame frame, u32 seed) const;
    QVector<WildFrame> synchMethodJ(WildFrame frame, u32 seed) const;
    QVector<WildFrame> cuteCharmMethodJ(WildFrame frame, u32 seed) const;
    bool encounterMethodJ(WildFrame &frame, u32 seed) const;
    QVector<WildFrame> normalMethodK(WildFrame frame, u32 seed) const;
    QVector<WildFrame> synchMethodK(WildFrame frame, u32 seed) const;
    QVector<WildFrame> cuteCharmMethodK(WildFrame frame, u32 seed) const;
    bool encounterMethodK(WildFrame &frame, u32 seed) const;
};

#endif // WILDSEARCHER4_HPP
