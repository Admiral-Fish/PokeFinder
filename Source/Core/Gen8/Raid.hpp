/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#include <Core/Global.hpp>
#include <Core/Parents/StaticTemplate.hpp>
#include <array>
#include <string>

/**
 * @brief Contains information about how to generate a Raid pokemon
 */
class Raid : public StaticTemplate
{
public:
    /**
     * @brief Construct a new Raid object
     *
     * @param specie Specie of the template
     * @param form Form of the template
     * @param shiny Shiny state of the template
     * @param ability Ability of the template
     * @param gender Gender of the template
     * @param ivCount Gender of the template
     * @param gigantamax Template gigantamax
     * @param star Template stars
     */
    constexpr Raid(u16 specie, u8 form, Shiny shiny, u8 ability, u8 gender, u8 ivCount, bool gigantamax, const std::array<bool, 5> &star) :
        StaticTemplate(Game::SwSh, specie, form, shiny, ability, gender, ivCount, 0), gigantamax(gigantamax), star(star)
    {
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
    bool gigantamax;
    std::array<bool, 5> star;
};

#endif // RAID_HPP
