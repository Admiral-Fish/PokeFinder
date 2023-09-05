/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef STATICTEMPLATE5_HPP
#define STATICTEMPLATE5_HPP

#include <Core/Parents/StaticTemplate.hpp>

/**
 * @brief Contains additional information for Gen5 static encounters
 */
class StaticTemplate5 : public StaticTemplate
{
public:
    /**
     * @brief Construct a new StaticTemplate object
     *
     * @param version Game the template appears in
     * @param specie Specie of the template
     * @param form Form of the template
     * @param shiny Shiny state of the template
     * @param ability Ability of the template
     * @param gender Gender of the template
     * @param level Level of the template
     */
    constexpr StaticTemplate5(Game version, u16 specie, u8 form, Shiny shiny, u8 ability, u8 gender, u8 level) :
        StaticTemplate(version, specie, form, shiny, ability, gender, 0, level),
        gift(false),
        larvesta(specie == 636),
        roamer(specie == 641 || specie == 642)
    {
    }

    /**
     * @brief Determines if the template is a gift
     *
     * @return Template gift status
     */
    bool getGift() const
    {
        return gift;
    }

    /**
     * @brief Determines if the template is the Larvesta egg
     *
     * @return Template Larvesta egg status
     */
    bool getLarvesta() const
    {
        return gift;
    }

    /**
     * @brief Determines if the template is a roamer
     *
     * @return Template roamer status
     */
    bool getRoamer() const
    {
        return roamer;
    }

private:
    bool gift;
    bool larvesta;
    bool roamer;
};

#endif // STATICTEMPLATE5_HPP
