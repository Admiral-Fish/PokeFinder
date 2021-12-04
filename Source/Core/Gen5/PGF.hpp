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

#ifndef PGF_HPP
#define PGF_HPP

#include <Core/Util/Global.hpp>
#include <array>

class PGF
{
public:
    explicit PGF(const std::array<u8, 204> &data) :
        tid(*(u16 *)(data.data() + 0x0)),
        sid(*(u16 *)(data.data() + 0x2)),
        species(*(u16 *)(data.data() + 0x1a)),
        nature(data[0x34]),
        gender(data[0x35]),
        abilityType(data[0x36]),
        pidType(data[0x37]),
        ivs { data[0x43], data[0x44], data[0x45], data[0x47], data[0x48], data[0x46] },
        egg(data[0x5C] == 1)
    {
    }

    PGF(u16 tid, u16 sid, u16 species, u8 nature, u8 gender, u8 abilityType, u8 pidType, u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe,
        bool egg) :
        tid(tid),
        sid(sid),
        species(species),
        nature(nature),
        gender(gender),
        abilityType(abilityType),
        pidType(pidType),
        ivs { hp, atk, def, spa, spd, spe },
        egg(egg)
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

    u16 getSpecies() const
    {
        return species;
    }

    u8 getNature() const
    {
        return nature;
    }

    u8 getGender() const
    {
        return gender;
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

    bool isEgg() const
    {
        return egg;
    }

    u8 getAdvances() const
    {
        u8 advances = 8;

        for (u8 i = 0; i < 6; i++)
        {
            if (ivs[i] == 255) // Random IV
            {
                advances += 2;
            }
        }

        if (gender == 0 || gender == 1) // Forced male/female
        {
            advances += 2;
        }

        if (nature == 255) // Random nature
        {
            advances += 2;
        }

        return advances;
    }

private:
    u16 tid;
    u16 sid;
    u16 species;
    u8 nature; // 0xff -> unset
    u8 gender; // 0: male, 1: female, 2: random
    u8 abilityType; // 0: 0, 1: 1, 2: H, 3: 1/2, 4: 1/2/H
    u8 pidType; // 0: no shiny, 1: allow shiny, 2: force shiny
    u8 ivs[6]; // 0xff -> unset
    bool egg;
};

#endif // PGF_HPP
