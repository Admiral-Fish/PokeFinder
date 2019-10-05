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

#ifndef MTRNG_HPP
#define MTRNG_HPP

#include <QVector>
#include <Core/RNG/IRNG.hpp>

namespace PokeFinderCore
{

    class MT : public IRNG
    {

    public:
        void advanceFrames(u32 frames) override;

    protected:
        const QVector<u32> mag01 = { 0x0, 0x9908B0DF };
        QVector<u32> mt;
        u32 seed;
        u32 index;

        virtual void shuffle();
        virtual void initialize(u32 seed);
        void setSeed(u32 seed) override;
        void setSeed(u32 seed, u32 frames) override;
        u16 nextUShort() override;
        u32 getSeed() override;

    };

    class MersenneTwister : public MT
    {

    public:
        MersenneTwister();
        MersenneTwister(u32 seed, u32 frames = 0);
        u32 nextUInt() override;

    };

    class MersenneTwisterUntempered : public MT
    {

    public:
        MersenneTwisterUntempered();
        MersenneTwisterUntempered(u32 seed, u32 frames = 0);
        u32 nextUInt() override;

    };

    class MersenneTwisterFast : public MT
    {

    public:
        MersenneTwisterFast();
        MersenneTwisterFast(u32 seed, u32 calls, u32 frames = 0);
        u32 nextUInt() override;

    private:
        u16 max;
        u16 maxCalls;

        void initialize(u32 seed) override;
        void shuffle() override;

    };

}

#endif //MTRNG_HPP
