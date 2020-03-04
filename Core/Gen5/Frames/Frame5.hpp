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

#ifndef FRAME5_HPP
#define FRAME5_HPP

#include <Core/Util/Global.hpp>
#include <QDateTime>

class Frame5
{
public:
    QDateTime getDateTime() const
    {
        return dt;
    }

    void setDateTime(const QDateTime &dt)
    {
        this->dt = dt;
    }

    u64 getInitialSeed() const
    {
        return initialSeed;
    }

    void setInitialSeed(u64 initialSeed)
    {
        this->initialSeed = initialSeed;
    }

    u16 getButtons() const
    {
        return buttons;
    }

    void setButtons(u16 buttons)
    {
        this->buttons = buttons;
    }

    u16 getTimer0() const
    {
        return timer0;
    }

    void setTimer0(u16 timer0)
    {
        this->timer0 = timer0;
    }

protected:
    QDateTime dt;
    u64 initialSeed;
    u16 buttons;
    u16 timer0;
};

#endif // FRAME5_HPP
