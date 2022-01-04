/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef STATE_HPP
#define STATE_HPP

#include <Core/Util/Global.hpp>

class State
{
public:
    State() = default;

    explicit State(u32 advances) : advances(advances)
    {
    }

    u32 getSeed() const
    {
        return seed;
    }

    void setSeed(u32 seed)
    {
        this->seed = seed;
    }

    u32 getAdvances() const
    {
        return advances;
    }

    void setAdvances(u32 advances)
    {
        this->advances = advances;
    }

    u32 getPID() const
    {
        return pid;
    }

    void setPID(u32 pid)
    {
        this->pid = pid;
    }

    void setPID(u16 high, u16 low)
    {
        pid = (high << 16) | low;
    }

    u8 getIV(u8 index) const
    {
        return ivs[index];
    }

    void setIVs(u8 index, u8 iv)
    {
        this->ivs[index] = iv;
    }

    void setIVs(u16 iv1, u16 iv2)
    {
        ivs[0] = iv1 & 0x1f;
        ivs[1] = (iv1 >> 5) & 0x1f;
        ivs[2] = (iv1 >> 10) & 0x1f;
        ivs[3] = (iv2 >> 5) & 0x1f;
        ivs[4] = (iv2 >> 10) & 0x1f;
        ivs[5] = iv2 & 0x1f;
    }

    void setIVs(u32 iv)
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

    void setIVs(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe)
    {
        ivs[0] = hp;
        ivs[1] = atk;
        ivs[2] = def;
        ivs[3] = spa;
        ivs[4] = spd;
        ivs[5] = spe;
    }

    void setIV(u8 index, u8 iv)
    {
        ivs[index] = iv;
    }

    u8 getAbility() const
    {
        return ability;
    }

    void setAbility(u8 ability)
    {
        this->ability = ability;
    }

    u8 getGender() const
    {
        return gender;
    }

    void setGender(u8 gender, u8 genderRatio)
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

    void setGender(u8 gender)
    {
        this->gender = gender;
    }

    u8 getHidden() const
    {
        return hidden;
    }

    u8 getPower() const
    {
        return power;
    }

    u8 getNature() const
    {
        return nature;
    }

    void setNature(u8 nature)
    {
        this->nature = nature;
    }

    u8 getLevel() const
    {
        return level;
    }

    void setLevel(u8 level)
    {
        this->level = level;
    }

    u8 getShiny() const
    {
        return shiny;
    }

    void setShiny(u8 shiny)
    {
        this->shiny = shiny;
    }

    template <u8 compare>
    void setShiny(u16 tsv, u16 psv)
    {
        static_assert(compare == 8 || compare == 16, "Improper shiny compare value");

        if (tsv == psv)
        {
            shiny = 2; // Square
        }
        else if ((tsv ^ psv) < compare)
        {
            shiny = 1; // Star
        }
        else
        {
            shiny = 0;
        }
    }

    void calculateHiddenPower()
    {
        constexpr int order[6] = { 0, 1, 2, 5, 3, 4 };

        u8 h = 0;
        u8 p = 0;
        for (int i = 0; i < 6; i++)
        {
            h += (ivs[order[i]] & 1) << i;
            p += ((ivs[order[i]] >> 1) & 1) << i;
        }

        hidden = h * 15 / 63;
        power = 30 + (p * 40 / 63);
    }

protected:
    u32 seed;
    u32 advances;
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

#endif // STATE_HPP
