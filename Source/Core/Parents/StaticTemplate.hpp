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

#ifndef STATICTEMPLATE_HPP
#define STATICTEMPLATE_HPP

#include <Core/Enum/Shiny.hpp>
#include <Core/Global.hpp>

enum class Game : u32;

/**
 * @brief Contains information that impact the generation of Static encounters
 */
class StaticTemplate
{
public:
    /**
     * @brief Creates a new StaticTemplate
     * @param version Game the template appears in
     * @param species Species of the template
     * @param level Level of the template
     */
    constexpr StaticTemplate(Game version, u16 species, u8 level) :
        version(version), species(species), level(level), ability(255), shiny(Shiny::Random), ivCount(0)
    {
    }

    /**
     * @brief Creates a new StaticTemplate
     * @param version Game the template appears in
     * @param species Species of the template
     * @param level Level of the template
     * @param ivCount 31 IV count of the template
     */
    constexpr StaticTemplate(Game version, u16 species, u8 level, u8 ivCount) :
        version(version), species(species), level(level), ability(255), shiny(Shiny::Random), ivCount(ivCount)
    {
    }

    /**
     * @brief Creates a new StaticTemplate
     * @param version Game the template appears in
     * @param species Species of the template
     * @param level Level of the template
     * @param ability Ability of the template
     * @param shiny Shiny of the template
     * @param ivCount 31 IV count of the template
     */
    constexpr StaticTemplate(Game version, u16 species, u8 level, u8 ability, Shiny shiny, u8 ivCount) :
        version(version), species(species), level(level), ability(ability), shiny(shiny), ivCount(ivCount)
    {
    }

    /**
     * @brief Creates a new StaticTemplate
     * @param version Game the template appears in
     * @param species Species of the template
     * @param level Level of the template
     * @param ability Ability of the template
     * @param ivCount 31 IV count of the template
     */
    constexpr StaticTemplate(Game version, u16 species, u8 level, u8 ability, u8 ivCount) :
        version(version), species(species), level(level), ability(ability), shiny(Shiny::Random), ivCount(ivCount)
    {
    }

    /**
     * @brief Determines what game the template should appear in
     * @return Template game version
     */
    Game getVersion() const
    {
        return version;
    }

    /**
     * @brief Determines what species the template should have
     * @return Template species
     */
    u16 getSpecies() const
    {
        return species;
    }

    /**
     * @brief Determines what level the template should have
     * @return Template level
     */
    u8 getLevel() const
    {
        return level;
    }

    /**
     * @brief Determines what ability locking the template should have
     * @return Template ability
     * - 0: 0
     * - 1: 1
     * - 2: H
     * - 3: 0/1
     * - 4: 0/1/H
     */
    u8 getAbility() const
    {
        return ability;
    }

    /**
     * @brief Determines what shiny locking the template should have
     * @return Template Shiny value
     */
    Shiny getShiny() const
    {
        return shiny;
    }

    /**
     * @brief Determines how many 31 IVs the template should have
     * @return Template 31 IV count
     */
    u8 getIVCount() const
    {
        return ivCount;
    }

private:
    Game version;
    u16 species;
    u8 level;
    u8 ability;
    Shiny shiny;
    u8 ivCount;
};

#endif // STATICTEMPLATE_HPP
