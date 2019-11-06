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

#ifndef EGG_HPP
#define EGG_HPP

#include <Core/Util/Global.hpp>
#include <Core/Util/Method.hpp>
#include <QVector>

namespace PokeFinderCore
{
    class Egg
    {
    public:
        void setEverstoneNature(u32 value);
        u32 getSeed() const;
        void setSeed(u32 value);

    protected:
        u16 psv {};
        u16 sid {};
        u16 tid {};
        u8 genderRatio {};
        Method frameType = Method::EBred;
        u32 seed {};
        u32 initialFrame {};
        u32 maxResults {};
        u32 everstoneNature {};
    };
}

#endif // EGG_HPP
