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

#include "PGF.hpp"

PGF::PGF(const std::array<u8, 204> &data) :
    tid(static_cast<u16>(data[0x01] << 8) | data[0x00]),
    sid(static_cast<u16>(data[0x03] << 8) | data[0x02]),
    species(static_cast<u16>(data[0x1B] << 8) | data[0x1A]),
    nature(data[0x34]),
    gender(data[0x35]),
    abilityType(data[0x36]),
    pidType(data[0x37]),
    hp(data[0x43]),
    atk(data[0x44]),
    def(data[0x45]),
    spa(data[0x47]),
    spd(data[0x48]),
    spe(data[0x46]),
    egg(data[0x5C] == 1)
{
}

PGF::PGF(u16 tid, u16 sid, u16 species, u8 nature, u8 gender, u8 abilityType, u8 pidType, u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe,
         bool egg) :
    tid(tid),
    sid(sid),
    species(species),
    nature(nature),
    gender(gender),
    abilityType(abilityType),
    pidType(pidType),
    hp(hp),
    atk(atk),
    def(def),
    spa(spa),
    spd(spd),
    spe(spe),
    egg(egg)
{
}

u16 PGF::getTID() const
{
    return tid;
}

u16 PGF::getSID() const
{
    return sid;
}

u16 PGF::getSpecies() const
{
    return species;
}

u8 PGF::getNature() const
{
    return nature;
}

u8 PGF::getGender() const
{
    return gender;
}

u8 PGF::getAbilityType() const
{
    return abilityType;
}

u8 PGF::getPIDType() const
{
    return pidType;
}

u8 PGF::getIV(u8 index) const
{
    if (index == 0)
    {
        return hp;
    }

    if (index == 1)
    {
        return atk;
    }

    if (index == 2)
    {
        return def;
    }

    if (index == 3)
    {
        return spa;
    }

    if (index == 4)
    {
        return spd;
    }

    return spe;
}

bool PGF::isEgg() const
{
    return egg;
}

u8 PGF::getAdvances() const
{
    u8 advances = 8;

    for (u8 i = 0; i < 6; i++)
    {
        if (getIV(i) == 255) // Random IV
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
