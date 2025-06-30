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

#ifndef STATICTEMPLATE8_HPP
#define STATICTEMPLATE8_HPP

#include <Core/Parents/StaticTemplate.hpp>

/**
 * @brief Contains additional information for Gen8 static encounters
 */
class StaticTemplate8 : public StaticTemplate
{
public:
    /**
     * @brief Construct a new StaticTemplate8 object
     *
     * @param version Game the template appears in
     * @param specie Specie of the template
     * @param form Form of the template
     * @param shiny Shiny state of the template
     * @param ability Ability of the template
     * @param gender Gender of the template
     * @param ivCount Gender of the template
     * @param level Level of the template
     * @param roamer Whether the template is a roamer
     */
    constexpr StaticTemplate8(Game version, u16 specie, u8 form, Shiny shiny, u8 ability, u8 gender, u8 ivCount, u8 level, bool roamer) :
        StaticTemplate(version, specie, form, shiny, ability, gender, ivCount, level), roamer(roamer)
    {
    }

    /**
     * @brief Determines whether the template is a roamer or not
     *
     * @return true Template is a roamer
     * @return false Template is not a roamer
     */
    bool getRoamer() const
    {
        return roamer;
    }

private:
    bool roamer;
};

#endif // STATICTEMPLATE8_HPP
