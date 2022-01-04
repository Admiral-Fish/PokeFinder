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
#include <Core/Util/Global.hpp>

enum class Game : u32;

class StaticTemplate
{
public:
    constexpr StaticTemplate(Game version, u16 species, u8 level) :
        version(version), species(species), level(level), ability(255), shiny(Shiny::Random), ivCount(0)
    {
    }

    constexpr StaticTemplate(Game version, u16 species, u8 level, u8 ivCount) :
        version(version), species(species), level(level), ability(255), shiny(Shiny::Random), ivCount(ivCount)
    {
    }

    constexpr StaticTemplate(Game version, u16 species, u8 level, u8 ability, Shiny shiny, u8 ivCount) :
        version(version), species(species), level(level), ability(ability), shiny(shiny), ivCount(ivCount)
    {
    }

    constexpr StaticTemplate(Game version, u16 species, u8 level, u8 ability, u8 ivCount) :
        version(version), species(species), level(level), ability(ability), shiny(Shiny::Random), ivCount(ivCount)
    {
    }

    Game getVersion() const
    {
        return version;
    }

    u16 getSpecies() const
    {
        return species;
    }

    u8 getLevel() const
    {
        return level;
    }

    u8 getAbility() const
    {
        return ability;
    }

    Shiny getShiny() const
    {
        return shiny;
    }

    u8 getIVCount() const
    {
        return ivCount;
    }

private:
    Game version;
    u16 species;
    u8 level;
    u8 ability; // 0: 0, 1: 1, 2: H, 3: 0/1, 4: 0/1/H
    Shiny shiny;
    u8 ivCount;
};

#endif // STATICTEMPLATE_HPP
