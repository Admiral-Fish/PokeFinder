/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2019 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef SEARCHER4_HPP
#define SEARCHER4_HPP

#include <Core/Gen4/EncounterArea4.hpp>
#include <Core/Gen4/Frame4.hpp>
#include <Core/Objects/Searcher.hpp>
#include <Core/RNG/RNGCache.hpp>

class Searcher4 : public Searcher
{

public:
    Searcher4();
    Searcher4(u16 tid, u16 sid, u8 genderRatio, u32 minDelay, u32 maxDelay, u32 minFrame, u32 maxFrame, const FrameCompare &compare, Method method);
    void setEncounter(const EncounterArea4 &value);
    QVector<Frame4> search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;

private:
    RNGCache cache;
    EncounterArea4 encounter;
    FrameCompare compare;
    u8 unbiasedBuffer[5] = { 0, 0x96, 0xC8, 0x4B, 0x32 };
    u8 genderRatio;
    u32 minDelay;
    u32 maxDelay;
    u32 minFrame;
    u32 maxFrame;

    QVector<Frame4> searchMethod1(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    QVector<Frame4> searchMethodJ(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    QVector<Frame4> searchMethodJSynch(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    QVector<Frame4> searchMethodJCuteCharm(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    QVector<Frame4> searchMethodJSearch(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    QVector<Frame4> searchMethodK(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    QVector<Frame4> searchMethodKSynch(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    QVector<Frame4> searchMethodKCuteCharm(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    QVector<Frame4> searchMethodKSuctionCups(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    QVector<Frame4> searchMethodKSearch(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    QVector<Frame4> searchChainedShiny(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    QVector<Frame4> searchWondercardIVs(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
    QVector<Frame4> searchInitialSeeds(const QVector<Frame4> &results) const;
    u16 chainedPIDLow(const u16 *calls) const;
    u16 chainedPIDHigh(u16 high, u16 low, u16 tid, u16 sid) const;

};

#endif // SEARCHER4_HPP
