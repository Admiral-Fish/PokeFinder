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

#ifndef EGG3_HPP
#define EGG3_HPP

#include <Core/Gen3/Frame3.hpp>
#include <Core/Parents/Egg.hpp>

namespace PokeFinderCore
{

    class Egg3 : public Egg
    {

    public:
        Egg3();
        Egg3(u32 maxFrame, u32 initialFrame, u16 tid, u16 sid, Method method, u16 genderRatio, u32 seed = 0);
        QVector<Frame3> generate(const FrameCompare &compare) const;
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
        void setPickupSeed(const u16 &value);

    private:
        QVector<u8> parent1;
        QVector<u8> parent2;
        u8 calibration{};
        u8 minRedraw{};
        u8 maxRedraw{};
        int compatability{};
        u32 minPickup{};
        u32 maxPickup{};
        bool everstone{};
        u16 pickupSeed{};
        u8 iv1{}, iv2{}, inh1{}, inh2{}, inh3{}, par1{}, par2{}, par3{};

        QVector<Frame3> generateEmeraldPID(const FrameCompare &compare) const;
        QVector<Frame3> generateEmeraldIVs(const FrameCompare &compare) const;
        QVector<QPair<u32, u16>> generateLower(const FrameCompare &compare) const;
        QVector<Frame3> generateUpper(const QVector<QPair<u32, u16>> &lower, const FrameCompare &compare) const;

    };

}

#endif // EGG3_HPP
