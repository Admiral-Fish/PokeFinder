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

#include "Frame4.hpp"

Frame4::Frame4()
{
    tid = 12345;
    sid = 54321;
    psv = 12345 ^ 54321;
}

Frame4::Frame4(u16 tid, u16 sid, u16 psv)
{
    this->tid = tid;
    this->sid = sid;
    this->psv = psv;
}

void Frame4::setInheritance(u16 iv1, u16 iv2, u16 par1, u16 par2, u16 par3, u16 inh1, u16 inh2, u16 inh3, const QVector<u8> &parent1, const QVector<u8> &parent2, bool broken)
{
    ivs[0] = iv1 & 0x1f;
    ivs[1] = (iv1 >> 5) & 0x1f;
    ivs[2] = (iv1 >> 10) & 0x1f;
    ivs[3] = (iv2 >> 5) & 0x1f;
    ivs[4] = (iv2 >> 10) & 0x1f;
    ivs[5] = iv2 & 0x1f;

    QVector<u8> available =  { 0, 1, 2, 3, 4, 5 };
    QVector<u16> val = { inh1, inh2, inh3, par1, par2, par3 };

    for (u8 cnt = 0; cnt < 3; cnt++)
    {
        // Decide which parent (1 or 2) from which we'll pick an IV
        u8 par = val.at(cnt + 3) & 1;

        // Decide which stat to pick for IV inheritance
        u8 ivslot = available.at(val.at(cnt) % (6 - cnt));

        switch (ivslot)
        {
            case 0:
                ivs[0] = par == 0 ? parent1.at(0) : parent2.at(0);
                inheritance[0] = par == 0 ? 'A' : 'B';
                break;
            case 1:
                ivs[1] = par == 0 ? parent1.at(1) : parent2.at(1);
                inheritance[1] = par == 0 ? 'A' : 'B';
                break;
            case 2:
                ivs[2] = par == 0 ? parent1.at(2) : parent2.at(2);
                inheritance[2] = par == 0 ? 'A' : 'B';
                break;
            case 3:
                ivs[5] = par == 0 ? parent1.at(5) : parent2.at(5);
                inheritance[5] = par == 0 ? 'A' : 'B';
                break;
            case 4:
                ivs[3] = par == 0 ? parent1.at(3) : parent2.at(3);
                inheritance[3] = par == 0 ? 'A' : 'B';
                break;
            case 5:
                ivs[4] = par == 0 ? parent1.at(4) : parent2.at(4);
                inheritance[4] = par == 0 ? 'A' : 'B';
                break;
        }

        // Avoids repeat IV inheritance
        // In DPPt this doesn't work properly
        for (u8 i = broken ? cnt : ivslot; i < 5 - cnt; i++)
        {
            available[i] = available.at(i + 1);
        }
    }

    calculateHidden();
    calculatePower();
}

void Frame4::setPID(u32 pid, u16 genderRatio)
{
    this->pid = pid;
    nature = pid % 25;
    gender = genderRatio > 2 ? ((pid & 255) >= genderRatio ? 1 : 2) : genderRatio;
    ability = pid & 1;
    shiny = ((pid >> 16) ^ (pid & 0xFFFF) ^ psv) < 8;
}

void Frame4::setPID(u16 pid1, u16 pid2, u16 genderRatio)
{
    pid = (pid2 << 16) | pid1;
    nature = pid % 25;
    gender = genderRatio > 2 ? ((pid & 255) >= genderRatio ? 1 : 2) : genderRatio;
    ability = pid & 1;
    shiny = ((pid >> 16) ^ (pid & 0xFFFF) ^ psv) < 8;
}

QString Frame4::getCall() const
{
    u8 val = seed % 3;
    return val == 0 ? "E" : val == 1 ? "K" : "P";
}

QString Frame4::chatotPitch() const
{
    u8 val = ((seed & 0x1FFF) * 100) >> 13;
    QString pitch;

    if (val < 20)
    {
        pitch = QObject::tr("Low");
    }
    else if (val < 40)
    {
        pitch = QObject::tr("Mid-Low");
    }
    else if (val < 60)
    {
        pitch = QObject::tr("Mid");
    }
    else if (val < 80)
    {
        pitch = QObject::tr("Mid-High");
    }
    else
    {
        pitch = QObject::tr("High");
    }

    pitch = pitch + " (" + QString::number(val) + ")";
    return pitch;
}

u32 Frame4::getInitialSeed() const
{
    return initialSeed;
}

void Frame4::setInitialSeed(const u32 &value)
{
    initialSeed = value;
}

u32 Frame4::getSeed() const
{
    return seed;
}

void Frame4::setSeed(const u32 &value)
{
    seed = value;
}

u32 Frame4::getOccidentary() const
{
    return occidentary;
}

void Frame4::setOccidentary(const u32 &value)
{
    occidentary = value;
}

void Frame4::xorFrame()
{
    pid ^= 0x80008000;
    nature = pid % 25;
}
