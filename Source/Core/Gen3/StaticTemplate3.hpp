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

#ifndef STATICTEMPLATE3_HPP
#define STATICTEMPLATE3_HPP

#include <Core/Parents/StaticTemplate.hpp>

/**
 * @brief Contains additional information for Gen3 static encounters
 */
class StaticTemplate3 : public StaticTemplate
{
public:
    /**
     * @brief Construct a new StaticTemplate3 object
     *
     * @param version Game the template appears in
     * @param specie Specie of the template
     * @param form Form of the template
     * @param shiny Shiny state of the template
     * @param level Level of the template
     * @param buggedRoamer Whether the roamer has bugged IV generation
     */
    constexpr StaticTemplate3(Game version, u16 specie, u8 form, Shiny shiny, u8 level, bool buggedRoamer) :
        StaticTemplate(version, specie, form, shiny, 255, 255, 0, level), buggedRoamer(buggedRoamer)
    {
    }

    /**
     * @brief Determines if the roamer has bugged IV generation
     *
     * @return true Roamer has bugged IV generation
     * @return false Roamer does not have bugged IV generation
     */
    bool getBuggedRoamer() const
    {
        return buggedRoamer;
    }

private:
    bool buggedRoamer;
};

#endif // STATICTEMPLATE3_HPP
