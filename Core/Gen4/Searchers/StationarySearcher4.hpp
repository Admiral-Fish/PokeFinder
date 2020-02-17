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

#ifndef STATIONARYSEARCHER4_HPP
#define STATIONARYSEARCHER4_HPP

#include <Core/Parents/Frames/StationaryFrame.hpp>
#include <Core/Parents/Searchers/StationarySearcher.hpp>
#include <Core/RNG/RNGCache.hpp>
#include <mutex>

class StationarySearcher4 : public StationarySearcher
{
public:
    StationarySearcher4() = default;
    StationarySearcher4(u16 tid, u16 sid, u8 genderRatio, Method method, const FrameFilter &filter);
    void setDelay(u32 minDelay, u32 maxDelay);
    void setFrame(u32 minFrame, u32 maxFrame);
    void startSearch(const QVector<u8> &min, const QVector<u8> &max);
    void cancelSearch();
    QVector<StationaryFrame> getResults();
    int getProgress() const;

private:
    RNGCache cache;
    u32 minDelay;
    u32 maxDelay;
    u32 minFrame;
    u32 maxFrame;

    bool searching;
    int progress;
    QVector<StationaryFrame> results;
    std::mutex mutex;

    QVector<StationaryFrame> search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    QVector<StationaryFrame> searchMethod1(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    QVector<StationaryFrame> searchMethodJ(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    QVector<StationaryFrame> searchMethodK(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    QVector<StationaryFrame> searchWondercardIVs(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    QVector<StationaryFrame> searchInitialSeeds(const QVector<StationaryFrame> &results) const;

    QVector<StationaryFrame> normalMethodJ(StationaryFrame frame, u32 seed) const;
    QVector<StationaryFrame> synchMethodJ(StationaryFrame frame, u32 seed) const;
    QVector<StationaryFrame> cuteCharmMethodJ(StationaryFrame frame, u32 seed) const;
    QVector<StationaryFrame> normalMethodK(StationaryFrame frame, u32 seed) const;
    QVector<StationaryFrame> synchMethodK(StationaryFrame frame, u32 seed) const;
    QVector<StationaryFrame> cuteCharmMethodK(StationaryFrame frame, u32 seed) const;
};

#endif // STATIONARYSEARCHER4_HPP
