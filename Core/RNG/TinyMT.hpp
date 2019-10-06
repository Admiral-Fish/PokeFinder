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

#ifndef TINYMT_HPP
#define TINYMT_HPP

#include <QVector>
#include <Core/RNG/IRNG.hpp>

namespace PokeFinderCore
{
    class TinyMT : public IRNG<u32>
    {
    public:
        TinyMT(u32 seed = 0, u32 frames = 0);
        TinyMT(const QVector<u32> &state, u32 frames = 0);
        void advanceFrames(u32 frames) override;
        u32 nextUInt(u32 frames = 0);
        u16 nextUShort(u32 frames = 0);
        u32 next(u32 frames = 0) override;
        void setSeed(u32 seed, u32 frames) override;
        QVector<u32> getState() const;

    private:
        QVector<u32> state;

        void initialize(u32 seed);
        void periodCertification();
        void nextState();
        u32 temper();
    };
}

#endif // TINYMT_HPP
