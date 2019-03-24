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

#ifndef SEARCHER3_HPP
#define SEARCHER3_HPP

#include <Core/Gen3/EncounterArea3.hpp>
#include <Core/Gen3/Frame3.hpp>
#include <Core/Gen3/ShadowLock.hpp>
#include <Core/Objects/Searcher.hpp>
#include <Core/RNG/RNGCache.hpp>
#include <Core/RNG/RNGEuclidean.hpp>

class Searcher3: public Searcher
{

public:
    Searcher3();
    Searcher3(u16 tid, u16 sid, u32 ratio, const FrameCompare &compare);
    QVector<Frame3> search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe);
    void setup(Method method);
    void setupNatureLock(int num);
    void setEncounter(const EncounterArea3 &value);

private:
    RNGCache cache;
    RNGEuclidean euclidean;
    Frame3 frame = Frame3(0, 0, 0);
    ShadowLock shadowLock;
    ShadowType type;
    FrameCompare compare;
    EncounterArea3 encounter;

    QVector<Frame3> searchMethodChannel(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe);
    QVector<Frame3> searchMethodColo(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe);
    QVector<Frame3> searchMethodH124(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe);
    QVector<Frame3> searchMethodXD(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe);
    QVector<Frame3> searchMethodXDColo(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe);
    QVector<Frame3> searchMethod124(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe);
    QVector<Frame3> searchMethod1Reverse(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe);

};

#endif // SEARCHER3_HPP
