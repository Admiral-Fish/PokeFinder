/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef WB8_HPP
#define WB8_HPP

#include <Core/Util/Global.hpp>
#include <array>

class WB8
{
public:
    WB8(const std::array<u8, 732> &data) :
        tid(*(u16 *)(data.data() + 0x20)),
        sid(*(u16 *)(data.data() + 0x22)),
        ec(*(u32 *)(data.data() + 0x28)),
        pid(*(u32 *)(data.data() + 0x2c)),
        species(*(u16 *)(data.data() + 0x288)),
        form(data[0x28a]),
        gender(data[0x28b]),
        egg(data[0x28d] == 1),
        nature(data[0x28e]),
        abilityType(data[0x28f]),
        pidType(data[0x290]),
        ivs { data[0x2b2], data[0x2b3], data[0x2b4], data[0x2b6], data[0x2b7], data[0x2b5] }
    {
    }

    WB8(u16 tid, u16 sid, u32 ec, u32 pid, u16 species, u8 form, u8 gender, bool egg, u8 nature, u8 abilityType, u8 pidType, u8 hp, u8 atk,
        u8 def, u8 spa, u8 spd, u8 spe) :
        tid(tid),
        sid(sid),
        ec(ec),
        pid(pid),
        species(species),
        form(form),
        gender(gender),
        egg(egg),
        nature(nature),
        abilityType(abilityType),
        pidType(pidType),
        ivs { hp, atk, def, spa, spd, spe }
    {
    }

    u16 getTID() const
    {
        return tid;
    }

    u16 getSID() const
    {
        return sid;
    }

    u32 getEC() const
    {
        return ec;
    }

    u32 getPID() const
    {
        return pid;
    }

    u16 getSpecies() const
    {
        return species;
    }

    u8 getForm() const
    {
        return form;
    }

    u8 getGender() const
    {
        return gender;
    }

    bool isEgg() const
    {
        return egg;
    }

    u8 getNature() const
    {
        return nature;
    }

    u8 getAbilityType() const
    {
        return abilityType;
    }

    u8 getPIDType() const
    {
        return pidType;
    }

    u8 getIV(u8 index) const
    {
        return ivs[index];
    }

private:
    u16 tid;
    u16 sid;
    u32 ec;
    u32 pid;
    u16 species;
    u8 form;
    u8 gender;
    bool egg;
    u8 nature; // 0xff -> unset
    u8 abilityType; // 0: 0, 1: 1, 2: H, 3: 1/2, 4: 1/2/H
    u8 pidType; // 0: never, 1: random, 2: star, 3: square, 4: static
    u8 ivs[6];
};

#endif // WB8_HPP