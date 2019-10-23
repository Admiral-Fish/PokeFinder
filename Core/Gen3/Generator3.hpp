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

#ifndef GENERATOR3_HPP
#define GENERATOR3_HPP

#include <Core/Gen3/EncounterArea3.hpp>
#include <Core/Gen3/LockInfo.hpp>
#include <Core/Parents/Generator.hpp>
#include <Core/RNG/LCRNG.hpp>

namespace PokeFinderCore
{
    class FrameCompare;
    class Frame3;

    class Generator3: public Generator
    {
    public:
        Generator3();
        Generator3(u32 maxResults, u32 initialFrame, u32 initialSeed, u16 tid, u16 sid, u32 offset, u8 genderRatio);
        QVector<Frame3> generate(const FrameCompare &compare) const;
        void setup(Method method);
        void setEncounter(const EncounterArea3 &value);
        void setShadowTeam(u8 index, int type);

    private:
        u8 ivFirst{};
        u8 ivSecond{};
        EncounterArea3 encounter;
        ShadowTeam team;
        int type;

        QVector<Frame3> generateMethodChannel(const FrameCompare &compare) const;
        QVector<Frame3> generateMethodH124(const FrameCompare &compare) const;
        QVector<Frame3> generateMethodXDColo(const FrameCompare &compare) const;
        QVector<Frame3> generateMethodXDColoShadow(const FrameCompare &compare) const;
        QVector<Frame3> generateMethod124(const FrameCompare &compare) const;
        QVector<Frame3> generateMethod1Reverse(const FrameCompare &compare) const;
        void generateNonShadows(XDRNG &rng) const;
        static bool cuteCharm125F(u32 pid) { return (pid & 0xff) < 31; }
        static bool cuteCharm875M(u32 pid) { return (pid & 0xff) >= 31; }
        static bool cuteCharm25F(u32 pid) { return (pid & 0xff) < 63; }
        static bool cuteCharm75M(u32 pid) { return (pid & 0xff) >= 63; }
        static bool cuteCharm50F(u32 pid) { return (pid & 0xff) < 127; }
        static bool cuteCharm50M(u32 pid) { return (pid & 0xff) >= 127; }
        static bool cuteCharm75F(u32 pid) { return (pid & 0xff) < 191; }
        static bool cuteCharm25M(u32 pid) { return (pid & 0xff) >= 191; }
    };
}

#endif // GENERATOR3_HPP
