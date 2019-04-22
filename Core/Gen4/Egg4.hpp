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

#ifndef EGG4_HPP
#define EGG4_HPP

#include <Core/Gen4/Frame4.hpp>
#include <Core/Parents/Egg.hpp>

class Egg4 : public Egg
{

public:
    Egg4();
    Egg4(u32 maxFrame, u32 initialFrame, u16 tid, u16 sid, Method method, u32 seed, u8 genderRatio);
    void setParents(const QVector<u8> &parent1, const QVector<u8> &parent2);
    QVector<Frame4> generate(const FrameCompare &compare) const;

private:
    QVector<u8> parent1;
    QVector<u8> parent2;

    QVector<Frame4> generatePID(const FrameCompare &compare) const;
    QVector<Frame4> generatePIDMasuada(const FrameCompare &compare) const;
    QVector<Frame4> generateIVsDPPt(const FrameCompare &compare) const;
    QVector<Frame4> generateIVsHGSS(const FrameCompare &compare) const;

};

#endif // EGG4_HPP
