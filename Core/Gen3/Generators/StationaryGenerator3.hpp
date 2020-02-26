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

#ifndef STATIONARYGENERATOR3_HPP
#define STATIONARYGENERATOR3_HPP

#include <Core/Parents/Frames/Frame.hpp>
#include <Core/Parents/Generators/StationaryGenerator.hpp>
#include <QVector>

class StationaryGenerator3 : public StationaryGenerator
{
public:
    StationaryGenerator3() = default;
    StationaryGenerator3(u32 initialFrame, u32 maxResults, u16 tid, u16 sid, u8 genderRatio, Method method, const FrameFilter &filter);
    QVector<Frame> generate(u32 seed) const;

private:
    QVector<Frame> generateMethod124(u32 seed) const;
    QVector<Frame> generateMethod1Reverse(u32 seed) const;
};

#endif // STATIONARYGENERATOR3_HPP
