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

#ifndef STATICTEMPLATE4_HPP
#define STATICTEMPLATE4_HPP

#include <Core/Parents/StaticTemplate.hpp>

/**
 * @brief Contains additional information for Gen4 static encounters
 */
class StaticTemplate4 : public StaticTemplate
{
public:
    /**
     * @brief Construct a new StaticTemplate object
     *
     * @param version Game the template appears in
     * @param specie Specie of the template
     * @param level Level of the template
     * @param form Form of the template
     */
    constexpr StaticTemplate4(Game version, u16 specie, u8 level, u8 form = 0) : StaticTemplate(version, specie, level), form(form)
    {
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

private:
    u8 form;
};

#endif // STATICTEMPLATE4_HPP
