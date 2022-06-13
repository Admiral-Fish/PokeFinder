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

#ifndef PERSONALINFO_HPP
#define PERSONALINFO_HPP

#include <Core/Util/Global.hpp>
#include <array>

class PersonalInfo
{
public:
    constexpr PersonalInfo(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u8 gender, u16 ability1, u16 ability2, u16 abilityH, u8 formCount,
                           u16 formStatIndex, u16 hatchSpecies, bool present) :
        stats { hp, atk, def, spa, spd, spe },
        gender(gender),
        ability1(ability1),
        ability2(ability2),
        abilityH(abilityH),
        formCount(formCount),
        formStatIndex(formStatIndex),
        hatchSpecies(hatchSpecies),
        present(present)
    {
    }

    std::array<u8, 6> getStats() const
    {
        return stats;
    }

    u16 getGender() const
    {
        return gender;
    }

    u16 getAbility1() const
    {
        return ability1;
    }

    u16 getAbility2() const
    {
        return ability2;
    }

    u16 getAbilityH() const
    {
        return abilityH;
    }

    u8 getFormCount() const
    {
        return formCount;
    }

    u16 getFormStatIndex() const
    {
        return formStatIndex;
    }

    u16 getHatchSpecies() const
    {
        return hatchSpecies;
    }

    bool getPresent() const
    {
        return present;
    }

private:
    std::array<u8, 6> stats;
    u8 gender;
    u16 ability1;
    u16 ability2;
    u16 abilityH;
    u8 formCount;
    u16 formStatIndex;
    u16 hatchSpecies;
    bool present;
};

#endif // PERSONALINFO_HPP
