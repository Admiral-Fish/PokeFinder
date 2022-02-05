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

#ifndef RAID_HPP
#define RAID_HPP

#include <Core/Enum/Shiny.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Util/Global.hpp>
#include <array>
#include <string>

class Raid
{
public:
    constexpr Raid(u8 ability, u8 altform, u8 ivCount, u8 gender, bool gigantamax, u16 species, const PersonalInfo &info,
                   const std::array<bool, 5> &star, Shiny shinyType = Shiny::Random) :
        ability(ability),
        altform(altform),
        ivCount(ivCount),
        gender(gender),
        gigantamax(gigantamax),
        species(species),
        info(info),
        star(star),
        shinyType(shinyType)
    {
    }

    u8 getAbility() const
    {
        return ability;
    }

    u8 getAltForm() const
    {
        return altform;
    }

    u8 getIVCount() const
    {
        return ivCount;
    }

    u8 getGender() const
    {
        return gender;
    }

    PersonalInfo getInfo() const
    {
        return info;
    }

    bool getGigantamax() const
    {
        return gigantamax;
    }

    u16 getSpecies() const
    {
        return species;
    }

    Shiny getShinyType() const
    {
        return shinyType;
    }

    std::string getStarDisplay() const
    {
        u8 low = 4;
        u8 high = 0;
        for (u8 i = 0; i < 5; i++)
        {
            if (star[i])
            {
                if (i < low)
                {
                    low = i;
                }
                if (i > high)
                {
                    high = i;
                }
            }
        }

        if (low == high)
        {
            return std::to_string(low + 1) + "★";
        }

        return std::to_string(low + 1) + "-" + std::to_string(high + 1) + "★";
    }

private:
    u8 ability;
    u8 altform;
    u8 ivCount;
    u8 gender;
    bool gigantamax;
    u16 species;
    PersonalInfo info;
    std::array<bool, 5> star;
    Shiny shinyType;
};

#endif // RAID_HPP
