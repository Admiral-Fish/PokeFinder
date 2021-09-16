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

#include "PersonalLoader.hpp"
#include <Core/Resources/Resources.hpp>

namespace PersonalLoader3
{
    std::vector<PersonalInfo> getPersonal()
    {
        std::vector<PersonalInfo> personal;

        for (int i = 0; i < personal3.size(); i += 0x1c)
        {
            u8 hp = personal3[i];
            u8 atk = personal3[i + 0x1];
            u8 def = personal3[i + 0x2];
            u8 spe = personal3[i + 0x3];
            u8 spa = personal3[i + 0x4];
            u8 spd = personal3[i + 0x5];
            u8 gender = personal3[i + 0x10];
            u16 ability1 = personal3[i + 0x16];
            u16 ability2 = personal3[i + 0x17];
            u8 formCount = 1;
            u8 abilityH = 0;
            u16 formStatIndex = 0;

            personal.emplace_back(hp, atk, def, spa, spd, spe, gender, ability1, ability2, abilityH, formCount, formStatIndex);
        }

        return personal;
    }
}

namespace PersonalLoader4
{
    std::vector<PersonalInfo> getPersonal()
    {
        std::vector<PersonalInfo> personal;

        for (int i = 0; i < personal4.size(); i += 0x2c)
        {
            u8 hp = personal4[i];
            u8 atk = personal4[i + 0x1];
            u8 def = personal4[i + 0x2];
            u8 spe = personal4[i + 0x3];
            u8 spa = personal4[i + 0x4];
            u8 spd = personal4[i + 0x5];
            u8 gender = personal4[i + 0x10];
            u16 ability1 = personal4[i + 0x16];
            u16 ability2 = personal4[i + 0x17];
            u8 formCount = personal4[i + 0x29];
            u8 abilityH = 0;
            u16 formStatIndex = *(u16 *)&personal4[i + 0x2a];

            personal.emplace_back(hp, atk, def, spa, spd, spe, gender, ability1, ability2, abilityH, formCount, formStatIndex);
        }

        return personal;
    }
}

namespace PersonalLoader5
{
    std::vector<PersonalInfo> getPersonal()
    {
        std::vector<PersonalInfo> personal;

        for (int i = 0; i < personal5.size(); i += 0x4c)
        {
            u8 hp = personal5[i];
            u8 atk = personal5[i + 0x1];
            u8 def = personal5[i + 0x2];
            u8 spe = personal5[i + 0x3];
            u8 spa = personal5[i + 0x4];
            u8 spd = personal5[i + 0x5];
            u8 gender = personal5[i + 0x12];
            u16 ability1 = personal5[i + 0x18];
            u16 ability2 = personal5[i + 0x19];
            u8 formCount = personal5[i + 0x20];
            u8 abilityH = personal5[i + 0x1a];
            u16 formStatIndex = *(u16 *)&personal5[i + 0x1c];

            personal.emplace_back(hp, atk, def, spa, spd, spe, gender, ability1, ability2, abilityH, formCount, formStatIndex);
        }

        return personal;
    }
}
