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
     * @param level Level of the template
     */
    constexpr StaticTemplate8(Game version, u16 specie, u8 level) : StaticTemplate(version, specie, level), ability(255), ivCount(0)
    {
    }

    /**
     * @brief Construct a new StaticTemplate8 object
     *
     * @param version Game the template appears in
     * @param specie Specie of the template
     * @param level Level of the template
     * @param ivCount IV count of the template
     */
    constexpr StaticTemplate8(Game version, u16 specie, u8 level, u8 ivCount) :
        StaticTemplate(version, specie, level), ability(255), ivCount(ivCount)
    {
    }

    /**
     * @brief Construct a new StaticTemplate8 object
     *
     * @param version Game the template appears in
     * @param specie Specie of the template
     * @param level Level of the template
     * @param ability Ability of the template
     * @param ivCount IV count of the template
     */
    constexpr StaticTemplate8(Game version, u16 specie, u8 level, u8 ability, u8 ivCount) :
        StaticTemplate(version, specie, level), ability(ability), ivCount(ivCount)
    {
    }

    /**
     * @brief Construct a new StaticTemplate8 object
     *
     * @param version Game the template appears in
     * @param specie Specie of the template
     * @param level Level of the template
     * @param ability Ability of the template
     * @param shiny Shiny state of the template
     * @param ivCount IV count of the template
     */
    constexpr StaticTemplate8(Game version, u16 specie, u8 level, u8 ability, Shiny shiny, u8 ivCount) :
        StaticTemplate(version, specie, level, shiny), ability(ability), ivCount(ivCount)
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
     * @brief Determines what IV count the template should have
     *
     * @return Template IV count
     */
    u8 getIVCount() const
    {
        return ivCount;
    }

private:
    u8 ability;
    u8 ivCount;
};

#endif // STATICTEMPLATE8_HPP
