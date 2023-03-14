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

#ifndef STATICTEMPLATE_HPP
#define STATICTEMPLATE_HPP

#include <Core/Enum/Game.hpp>
#include <Core/Enum/Shiny.hpp>
#include <Core/Global.hpp>
#include <Core/Parents/PersonalLoader.hpp>

/**
 * @brief Parent class that contains information that impact the generation of static encounters
 */
class StaticTemplate
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
     * @param ivCount Gender of the template
     * @param level Level of the template
     */
    constexpr StaticTemplate(Game version, u16 specie, u8 form, Shiny shiny, u8 ability, u8 gender, u8 ivCount, u8 level) :
        version(version), form(form), specie(specie), shiny(shiny), ability(ability), gender(gender), ivCount(ivCount), level(level)
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
     * @brief Returns the pokemon information
     *
     * @return Pokemon information
     */
    const PersonalInfo *getInfo() const
    {
        return PersonalLoader::getPersonal(version, specie, form);
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
     * @brief Determines what level the template should have
     *
     * @return Template level
     */
    u8 getLevel() const
    {
        return level;
    }

    /**
     * @brief Determines what shiny the template should have
     *
     * @return Template shiny
     */
    Shiny getShiny() const
    {
        return shiny;
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
     * @brief Determines what game the template should appear in
     *
     * @return Template game version
     */
    Game getVersion() const
    {
        return version;
    }

protected:
    Game version;
    u16 specie;
    Shiny shiny;
    u8 ability;
    u8 form;
    u8 gender;
    u8 ivCount;
    u8 level;
};

#endif // STATICTEMPLATE_HPP
