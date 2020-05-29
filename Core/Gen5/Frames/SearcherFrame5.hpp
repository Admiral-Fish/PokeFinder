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

#ifndef EVENTFRAME5_HPP
#define EVENTFRAME5_HPP

#include <Core/Gen5/Frames/Frame5.hpp>
#include <Core/Parents/Frames/Frame.hpp>

template <typename FrameType>
class SearcherFrame5 : public Frame5
{
public:
    SearcherFrame5() = default;
    SearcherFrame5(const QDateTime &dt, u64 initialSeed, u16 buttons, u16 timer0, const FrameType &frame) :
        Frame5(dt, initialSeed, buttons, timer0),
        frame(frame)
    {
    }

    FrameType getFrame() const
    {
        return frame;
    }

private:
    FrameType frame;
};

#endif // EVENTFRAME5_HPP
