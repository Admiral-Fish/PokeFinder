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

#ifndef GENERATOR4_HPP
#define GENERATOR4_HPP

#include <Core/Gen4/EncounterArea4.hpp>
#include <Core/Gen4/Frame4.hpp>
#include <Core/Parents/Generator.hpp>

class Generator4 : public Generator
{

public:
    Generator4();
    Generator4(u32 maxResults, u32 initialFrame, u32 initialSeed, u16 tid, u16 sid, u32 offset, Method type, u8 genderRatio);
    void setEncounter(const EncounterArea4 &value);
    QVector<Frame4> generate(const FrameCompare &compare) const;

private:
    EncounterArea4 encounter;

    QVector<Frame4> generateMethod1(const FrameCompare &compare) const;
    QVector<Frame4> generateMethodJStationary(const FrameCompare &compare) const;
    QVector<Frame4> generateMethodJWild(const FrameCompare &compare) const;
    QVector<Frame4> generateMethodKStationary(const FrameCompare &compare) const;
    QVector<Frame4> generateMethodKWild(const FrameCompare &compare) const;
    QVector<Frame4> generateChainedShiny(const FrameCompare &compare) const;
    QVector<Frame4> generateWondercardIVs(const FrameCompare &compare) const;
    u16 chainedPIDLow(u16 low, u16 call1, u16 call2, u16 call3, u16 call4, u16 call5,
                      u16 call6, u16 call7, u16 call8, u16 call9, u16 call10, u16 call11,
                      u16 call12, u16 call13) const;
    u16 chainedPIDHigh(u16 high, u16 low, u16 tid, u16 sid) const;

};

#endif // GENERATOR4_HPP
