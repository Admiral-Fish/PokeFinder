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

#include <Core/Enum/Game.hpp>
#include <Core/Enum/Shiny.hpp>
#include <Core/Global.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <array>
#include <string>

/**
 * @brief Contains information about how to generate a Raid pokemon
 */
class Raid
{
public:
    /**
     * @brief Construct a new Raid object
     *
     * @param ability Template ability
     * @param form Template form
     * @param ivCount Template IV count
     * @param gender Template gender
     * @param gigantamax Template gigantamax
     * @param specie Template specie
     * @param star Template stars
     * @param shinyType Template shiny type
     */
    constexpr Raid(u8 ability, u8 form, u8 ivCount, u8 gender, bool gigantamax, u16 specie, const std::array<bool, 5> &star,
                   Shiny shinyType = Shiny::Random) :
        form(form),
        specie(specie),
        gigantamax(gigantamax),
        shinyType(shinyType),
        ability(ability),
        ivCount(ivCount),
        gender(gender),
        star(star)
    {
    }

    /**
     * @brief Determines what ability the template should have
     *
     * @return Template ability
     */
    u8 getAbility() const
    {
        return ability;
    }

    /**
     * @brief Determines what form the template should have
     *
     * @return Template form
     */
    u8 getForm() const
    {
        return form;
    }

    /**
     * @brief Determines what gender the template should have
     *
     * @return Template gender
     */
    u8 getGender() const
    {
        return gender;
    }

    /**
     * @brief Determines what gigantamax the template should have
     *
     * @return Template gigantamax
     */
    bool getGigantamax() const
    {
        return gigantamax;
    }

    /**
     * @brief Returns the pokemon information
     *
     * @return Pokemon information
     */
    const PersonalInfo *getInfo() const
    {
        return PersonalLoader::getPersonal(Game::SwSh, specie, form);
    }

    /**
     * @brief Determines what IV count the template should have
     *
     * @return Template IV count
     */
    u8 getIVCount() const
    {
        return ivCount;
    }

    /**
     * @brief Determines what shiny type the template should have
     *
     * @return Template shiny type
     */
    Shiny getShinyType() const
    {
        return shinyType;
    }

    /**
     * @brief Determines what specie the template should have
     *
     * @return Template specie
     */
    u16 getSpecie() const
    {
        return specie;
    }

    /**
     * @brief Determines what stars the template should have
     *
     * @return Template stars
     */
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
    u16 form : 5;
    u16 specie : 11;
    bool gigantamax;
    Shiny shinyType;
    u8 ability;
    u8 ivCount;
    u8 gender;
    std::array<bool, 5> star;
};

#endif // RAID_HPP
