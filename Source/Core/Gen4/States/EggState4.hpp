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

#ifndef EGGSTATE4_HPP
#define EGGSTATE4_HPP

#include <Core/Parents/States/EggState.hpp>

class EggState4 : public EggState
{
public:
    EggState4() = default;

    explicit EggState4(u32 advance) : EggState(advance)
    {
    }

    u32 getInitialSeed() const
    {
        return initialSeed;
    }

    void setInitialSeed(u32 initialSeed)
    {
        this->initialSeed = initialSeed;
    }

    u32 getSecondaryAdvance() const
    {
        return secondaryAdvance;
    }

    void setSecondaryAdvance(u32 secondaryAdvance)
    {
        this->secondaryAdvance = secondaryAdvance;
    }

private:
    u32 initialSeed;
    u32 secondaryAdvance;
};

#endif // EGGSTATE4_HPP
