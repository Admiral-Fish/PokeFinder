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

#ifndef FRAME3_HPP
#define FRAME3_HPP

#include <Core/Parents/Frame.hpp>

namespace PokeFinderCore
{
    class Frame3 : public Frame
    {
    public:
        Frame3();
        Frame3(u16 tid, u16 sid, u16 psv);
        QString getTime() const;
        QString getTimeEgg() const;
        void setInheritance(u16 iv1, u16 iv2, u16 par1, u16 par2, u16 par3, u16 inh1, u16 inh2, u16 inh3,
            const QVector<u8> &parent1, const QVector<u8> &parent2, bool broken = false);
        void setPID(u32 pid, u16 genderRatio);
        void setPID(u16 high, u16 low, u16 genderRatio);
        QString getLockReason() const;
        void setLockReason(const QString &value);
        u32 getEggFrame() const;
        void setEggFrame(u32 value);
        u32 getSeed() const;
        void setSeed(u32 value);
        void xorFrame(bool flag = false);

    private:
        QString lockReason;
        u32 eggFrame {};
        u32 seed {};
    };
}

#endif // FRAME3_HPP
