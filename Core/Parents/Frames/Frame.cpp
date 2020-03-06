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

#include "Frame.hpp"

Frame::Frame(u32 frame) : frame(frame)
{
}

u32 Frame::getSeed() const
{
    return seed;
}

void Frame::setSeed(u32 seed)
{
    this->seed = seed;
}

u32 Frame::getFrame() const
{
    return frame;
}

void Frame::setFrame(u32 frame)
{
    this->frame = frame;
}

u32 Frame::getPID() const
{
    return pid;
}

void Frame::setPID(u32 pid)
{
    this->pid = pid;
}

void Frame::setPID(u16 high, u16 low)
{
    pid = (high << 16) | low;
}

u8 Frame::getIV(u8 index) const
{
    return ivs[index];
}

void Frame::setIVs(u8 index, u8 iv)
{
    ivs[index] = iv;
}

void Frame::setIVs(u16 iv1, u16 iv2)
{
    ivs[0] = iv1 & 0x1f;
    ivs[1] = (iv1 >> 5) & 0x1f;
    ivs[2] = (iv1 >> 10) & 0x1f;
    ivs[3] = (iv2 >> 5) & 0x1f;
    ivs[4] = (iv2 >> 10) & 0x1f;
    ivs[5] = iv2 & 0x1f;
}

void Frame::setIVs(u32 iv)
{
    u16 iv1 = iv >> 16;
    ivs[0] = iv1 >> 10;
    ivs[1] = (iv1 >> 5) & 0x1f;
    ivs[2] = iv1 & 0x1f;

    u16 iv2 = iv & 0xffff;
    ivs[3] = iv2 >> 10;
    ivs[4] = (iv2 >> 5) & 0x1f;
    ivs[5] = iv2 & 0x1f;
}

void Frame::setIVs(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe)
{
    ivs[0] = hp;
    ivs[1] = atk;
    ivs[2] = def;
    ivs[3] = spa;
    ivs[4] = spd;
    ivs[5] = spe;
}

u8 Frame::getAbility() const
{
    return ability;
}

void Frame::setAbility(u8 ability)
{
    this->ability = ability;
}

u8 Frame::getGender() const
{
    return gender;
}

void Frame::setGender(u8 gender, u8 genderRatio)
{
    if (genderRatio == 255) // Genderless
    {
        this->gender = 2;
    }
    else if (genderRatio == 254) // Female
    {
        this->gender = 1;
    }
    else if (genderRatio == 0) // Male
    {
        this->gender = 0;
    }
    else // Random gender
    {
        this->gender = gender < genderRatio;
    }
}

void Frame::setGender(u8 gender)
{
    this->gender = gender;
}

u8 Frame::getHidden() const
{
    return hidden;
}

u8 Frame::getPower() const
{
    return power;
}

u8 Frame::getNature() const
{
    return nature;
}

void Frame::setNature(u8 nature)
{
    this->nature = nature;
}

u8 Frame::getLevel() const
{
    return level;
}

void Frame::setLevel(u8 level)
{
    this->level = level;
}

u8 Frame::getShiny() const
{
    return shiny;
}

void Frame::setShiny(u8 shiny)
{
    this->shiny = shiny;
}

void Frame::setShiny(u16 tsv, u16 psv, u8 val)
{
    if (tsv == psv)
    {
        shiny = 2; // Square
    }
    else if ((tsv ^ psv) < val)
    {
        shiny = 1; // Star
    }
    else
    {
        shiny = 0;
    }
}

void Frame::calculateHiddenPower()
{
    hidden = ((ivs[0] & 1) + ((ivs[1] & 1) << 1) + ((ivs[2] & 1) << 2) + ((ivs[5] & 1) << 3) + ((ivs[3] & 1) << 4) + ((ivs[4] & 1) << 5))
        * 15 / 63;

    power = 30
        + ((((ivs[0] >> 1) & 1) + (((ivs[1] >> 1) & 1) << 1) + (((ivs[2] >> 1) & 1) << 2) + (((ivs[5] >> 1) & 1) << 3)
            + (((ivs[3] >> 1) & 1) << 4) + (((ivs[4] >> 1) & 1) << 5))
           * 40 / 63);
}
