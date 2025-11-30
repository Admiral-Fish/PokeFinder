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
     * @brief Construct a new StaticTemplate5 object
     *
     * @param version Game the template appears in
     * @param specie Specie of the template
     * @param form Form of the template
     * @param shiny Shiny state of the template
     * @param ability Ability of the template
     * @param gender Gender of the template
     * @param level Level of the template
     * @param curtis Whether the template is a Curtis trade
     * @param wild Whether the template is a wild encounter
     * @param yancy Whether the template is a Yancy trade
     */
    constexpr StaticTemplate5(Game version, u16 specie, u8 form, Shiny shiny, u8 ability, u8 gender, u8 level, bool curtis, bool wild,
                              bool yancy) :
        StaticTemplate(version, specie, form, shiny, ability, gender, 0, level), curtis(curtis), wild(wild), yancy(yancy)
    {
    }

    /**
     * @brief Determines if the template is a Curtis trade
     *
     * @return Template Curtis status
     */
    bool getCurtis() const
    {
        return curtis;
    }

    /**
     * @brief Determines if the template is an egg
     *
     * @return Template egg status
     */
    bool getEgg() const
    {
        // Larvesta and Happiny
        return specie == 636 || specie == 440;
    }

    /**
     * @brief Determines if the template is a roamer
     *
     * @return Template roamer status
     */
    bool getRoamer() const
    {
        // Tornadus and Thundurus
        return specie == 641 || specie == 642;
    }

    /**
     * @brief Determines if the template is a wild encounter
     *
     * @return Template wild status
     */
    bool getWild() const
    {
        return wild;
    }

    /**
     * @brief Determines if the template is a Yancy trade
     *
     * @return Template Yancy status
     */
    bool getYancy() const
    {
        return curtis;
    }

private:
    bool curtis;
    bool wild;
    bool yancy;
};

#endif // STATICTEMPLATE5_HPP
