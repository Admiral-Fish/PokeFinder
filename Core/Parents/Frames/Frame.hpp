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

#ifndef FRAME_HPP
#define FRAME_HPP

#include <Core/Util/Global.hpp>

class Frame
{
public:
    Frame() = default;
    explicit Frame(u32 frame);
    u32 getSeed() const;
    void setSeed(u32 seed);
    u32 getFrame() const;
    void setFrame(u32 frame);
    u32 getPID() const;
    void setPID(u32 pid);
    void setPID(u16 high, u16 low);
    u8 getIV(u8 index) const;
    void setIVs(u8 index, u8 iv);
    void setIVs(u16 iv1, u16 iv2);
    void setIVs(u32 iv);
    void setIVs(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe);
    u8 getAbility() const;
    void setAbility(u8 ability);
    u8 getGender() const;
    void setGender(u8 gender, u8 genderRatio);
    void setGender(u8 gender);
    u8 getHidden() const;
    u8 getPower() const;
    u8 getNature() const;
    void setNature(u8 nature);
    u8 getLevel() const;
    void setLevel(u8 level);
    u8 getShiny() const;
    void setShiny(u8 shiny);
    void setShiny(u16 tsv, u16 psv, u8 val);
    void calculateHiddenPower();

protected:
    u32 seed;
    u32 frame;
    u32 pid;
    u8 ivs[6];
    u8 ability;
    u8 gender;
    u8 hidden;
    u8 power;
    u8 nature;
    u8 level;
    u8 shiny;
};

#endif // FRAME_HPP
