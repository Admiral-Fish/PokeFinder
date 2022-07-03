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

#include "StateFilter4.hpp"
#include <Core/Gen4/States/EggState4.hpp>
#include <Core/Gen4/States/State4.hpp>
#include <Core/Gen4/States/WildState4.hpp>

StateFilter4::StateFilter4(u8 gender, u8 ability, u8 shiny, bool skip, const std::array<u8, 6> &min, const std::array<u8, 6> &max,
                           const std::array<bool, 25> &natures, const std::array<bool, 16> &powers) :
    StateFilter(gender, ability, shiny, skip, min, max, natures, powers)
{
}

bool StateFilter4::compareState(const EggGeneratorState4 &state) const
{
    if (skip)
    {
        return true;
    }

    if (!powers[state.getHiddenPower()])
    {
        return false;
    }

    for (int i = 0; i < 6; i++)
    {
        u8 iv = state.getIV(i);
        if (iv < min[i] || iv > max[i])
        {
            return false;
        }
    }

    return true;
}

bool StateFilter4::compareState(const GeneratorState4 &state) const
{
    if (skip)
    {
        return true;
    }

    if (ability != 255 && ability != state.getAbility())
    {
        return false;
    }

    if (gender != 255 && gender != state.getGender())
    {
        return false;
    }

    if (!powers[state.getHiddenPower()])
    {
        return false;
    }

    if (!natures[state.getNature()])
    {
        return false;
    }

    if (shiny != 255 && !(shiny & state.getShiny()))
    {
        return false;
    }

    for (int i = 0; i < 6; i++)
    {
        u8 iv = state.getIV(i);
        if (iv < min[i] || iv > max[i])
        {
            return false;
        }
    }

    return true;
}

bool StateFilter4::compareState(const SearcherState4 &state) const
{
    if (ability != 255 && ability != state.getAbility())
    {
        return false;
    }

    if (gender != 255 && gender != state.getGender())
    {
        return false;
    }

    if (!powers[state.getHiddenPower()])
    {
        return false;
    }

    if (shiny != 255 && !(shiny & state.getShiny()))
    {
        return false;
    }

    return true;
}

WildStateFilter4::WildStateFilter4(u8 gender, u8 ability, u8 shiny, bool skip, const std::array<u8, 6> &min, const std::array<u8, 6> &max,
                                   const std::array<bool, 25> &natures, const std::array<bool, 16> &powers,
                                   const std::array<bool, 12> &encounterSlots) :
    WildStateFilter(gender, ability, shiny, skip, min, max, natures, powers, encounterSlots)
{
}

bool WildStateFilter4::compareState(const WildGeneratorState4 &state) const
{
    if (skip)
    {
        return true;
    }

    if (ability != 255 && ability != state.getAbility())
    {
        return false;
    }

    if (gender != 255 && gender != state.getGender())
    {
        return false;
    }

    if (!powers[state.getHiddenPower()])
    {
        return false;
    }

    if (shiny != 255 && !(shiny & state.getShiny()))
    {
        return false;
    }

    for (int i = 0; i < 6; i++)
    {
        u8 iv = state.getIV(i);
        if (iv < min[i] || iv > max[i])
        {
            return false;
        }
    }

    return true;
}

bool WildStateFilter4::compareState(const WildSearcherState4 &state) const
{
    if (ability != 255 && ability != state.getAbility())
    {
        return false;
    }

    if (gender != 255 && gender != state.getGender())
    {
        return false;
    }

    if (!powers[state.getHiddenPower()])
    {
        return false;
    }

    if (shiny != 255 && !(shiny & state.getShiny()))
    {
        return false;
    }

    return true;
}
