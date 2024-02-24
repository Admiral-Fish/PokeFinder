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

#ifndef STATICTEMPLATE4_HPP
#define STATICTEMPLATE4_HPP

#include <Core/Enum/Method.hpp>
#include <Core/Parents/StaticTemplate.hpp>

/**
 * @brief Contains additional information for Gen4 static encounters
 */
class StaticTemplate4 : public StaticTemplate
{
public:
    /**
     * @brief Construct a new StaticTemplate4 object
     *
     * @param version Game the template appears in
     * @param specie Specie of the template
     * @param form Form of the template
     * @param shiny Shiny state of the template
     * @param level Level of the template
     * @param method Template generation method
     */
    constexpr StaticTemplate4(Game version, u16 specie, u8 form, Shiny shiny, u8 level, Method method) :
        StaticTemplate(version, specie, form, shiny, 255, 255, 0, level), method(method)
    {
    }

    /**
     * @brief Determines the generation method of the template
     *
     * @return Template generation method
     */
    Method getMethod() const
    {
        return method;
    }

private:
    Method method;
};

#endif // STATICTEMPLATE4_HPP
