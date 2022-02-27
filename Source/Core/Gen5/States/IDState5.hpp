/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef IDSTATE5_HPP
#define IDSTATE5_HPP

#include <Core/Parents/States/IDState.hpp>
#include <Core/Util/DateTime.hpp>

enum class Buttons : u16;

class IDState5 : public IDState
{
public:
    IDState5(u32 advances, u16 tid, u16 sid) : IDState(advances, tid, sid)
    {
        tsv = (tid ^ sid) >> 3;
    }

    void setDateTime(const DateTime &dt)
    {
        this->dt = dt;
    }

    DateTime getDateTime() const
    {
        return dt;
    }

    void setInitialAdvances(u32 initialAdvances)
    {
        this->initialAdvances = initialAdvances;
    }

    u32 getInitialAdvances() const
    {
        return initialAdvances;
    }

    void setKeypress(Buttons keypress)
    {
        this->keypress = keypress;
    }

    Buttons getKeypress() const
    {
        return keypress;
    }

    void setSeed(u64 seed)
    {
        this->seed = seed;
    }

    u64 getSeed() const
    {
        return seed;
    }

private:
    DateTime dt;
    u32 initialAdvances;
    Buttons keypress;
    u64 seed;
};

#endif // IDSTATE5_HPP
