/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef EGG3_HPP
#define EGG3_HPP

#include <Core/Gen3/Frame3.hpp>
#include <Core/Objects/Egg.hpp>
#include <Core/Objects/FrameCompare.hpp>
#include <Core/RNG/LCRNG.hpp>

class Egg3 : public Egg
{

public:
    Egg3();
    Egg3(u32 maxFrame, u32 initialFrame, u16 tid, u16 sid, Method method, u32 seed = 0);
    QVector<Frame3> generate(const FrameCompare &compare);
    void setParents(const QVector<u8> &parent1, const QVector<u8> &parent2);
    void setMinRedraw(const u8 &value);
    void setMaxRedraw(const u8 &value);
    void setCompatability(const int &value);
    void setCalibration(const u8 &value);
    void setEverstone(bool value);
    void setMinPickup(const u32 &value);
    void setMaxPickup(const u32 &value);
    u32 getSeed() const;
    void setSeed(const u32 &value);

private:
    QVector<u8> parent1;
    QVector<u8> parent2;
    const u8 HABCDS[6] = { 0, 1, 2, 5, 3, 4 };
    const u8 ABCDS[5] = { 1, 2, 5, 3, 4 };
    const u8 ACDS[4] = { 1, 5, 3, 4 };
    u8 calibration;
    u8 minRedraw;
    u8 maxRedraw;
    int compatability;
    u32 minPickup;
    u32 maxPickup;
    bool everstone;

    QVector<Frame3> generateEmeraldPID(FrameCompare compare);
    QVector<Frame3> generateEmerald(FrameCompare compare);
    QVector<Frame3> generateEmeraldSplit(FrameCompare compare);
    QVector<Frame3> generateEmeraldAlternate(FrameCompare compare);
    QVector<Frame3> generateLower(FrameCompare compare);
    QVector<Frame3> generateUpper(QVector<Frame3> lower, FrameCompare compare);

};

#endif // EGG3_HPP
