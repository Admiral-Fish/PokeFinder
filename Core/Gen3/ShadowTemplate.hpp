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

#ifndef SHADOWTEAM_HPP
#define SHADOWTEAM_HPP

#include <Core/Enum/ShadowType.hpp>
#include <Core/Gen3/LockInfo.hpp>
#include <Core/Parents/StaticTemplate.hpp>
#include <array>

/**
 * @brief Contains additional information for Gamecube static encounters
 */
class ShadowTemplate : public StaticTemplate
{
public:
    /**
     * @brief Construct a new ShadowTemplate object
     *
     * @param version Game the template appears in
     * @param specie Specie of the template
     * @param shiny Shiny state of the template
     * @param level Level of the template
     * @param locks Shadow locks of the template
     * @param count Number of shadow locks the template has
     * @param type Shadow type of the template
     */
    constexpr ShadowTemplate(Game version, u16 specie, Shiny shiny, u8 level, const std::array<LockInfo, 5> &locks, s8 count,
                             ShadowType type) :
        StaticTemplate(version, specie, 0, shiny, 255, 255, 0, level), type(type), locks(locks), count(count)
    {
    }

    /**
     * @brief Returns the number of shadow locks
     *
     * @return Shadow lock count
     */
    s8 getCount() const
    {
        return count;
    }

    /**
     * @brief Returns the shadow lock by the given \p index
     *
     * @param index Shadow lock index
     *
     * @return Shadow lock information
     */
    LockInfo getLock(s8 index) const
    {
        return locks[index];
    }

    /**
     * @brief Returns the shadow type of the template
     *
     * @return Shadow type
     */
    ShadowType getType() const
    {
        return type;
    }

private:
    ShadowType type;
    std::array<LockInfo, 5> locks;
    s8 count;
};

#endif // SHADOWTEAM_HPP
