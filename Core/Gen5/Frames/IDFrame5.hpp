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

#ifndef IDFRAME5_HPP
#define IDFRAME5_HPP

#include <Core/Parents/Frames/IDFrame.hpp>
#include <QDateTime>

class IDFrame5 : public IDFrame
{
public:
    IDFrame5() = default;
    IDFrame5(u32 frame, u16 tid, u16 sid);
    void setDateTime(const QDateTime &dt);
    QDateTime getDateTime() const;
    void setInitialFrame(u32 initialFrame);
    u32 getInitialFrame() const;
    void setKeypress(u16 keypress);
    u16 getKeypress() const;
    void setSeed(u64 seed);
    u64 getSeed() const;

private:
    QDateTime dt;
    u32 initialFrame;
    u16 keypress;
    u64 seed;
};

#endif // IDFRAME5_HPP
