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

#include "StateFilter8.hpp"
#include "Core/Gen8/States/UndergroundState.hpp"
#include <Core/Gen8/States/EggState8.hpp>
#include <Core/Gen8/States/State8.hpp>
#include <Core/Gen8/States/WildState8.hpp>

StateFilter8::StateFilter8(u8 gender, u8 ability, u8 shiny, bool skip, const std::array<u8, 6> &min, const std::array<u8, 6> &max,
                           const std::array<bool, 25> &natures, const std::array<bool, 16> &powers) :
    StateFilter(gender, ability, shiny, skip, min, max, natures, powers)
{
}

bool StateFilter8::compareState(const EggState8 &state) const
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

bool StateFilter8::compareState(const State8 &state) const
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

WildStateFilter8::WildStateFilter8(u8 gender, u8 ability, u8 shiny, bool skip, const std::array<u8, 6> &min, const std::array<u8, 6> &max,
                                   const std::array<bool, 25> &natures, const std::array<bool, 16> &powers,
                                   const std::array<bool, 12> &encounterSlots) :
    WildStateFilter(gender, ability, shiny, skip, min, max, natures, powers, encounterSlots)
{
}

bool WildStateFilter8::compareState(const WildState8 &state) const
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

UndergroundStateFilter::UndergroundStateFilter(u8 gender, u8 ability, u8 shiny, bool skip, const std::array<u8, 6> &min,
                                               const std::array<u8, 6> &max, const std::array<bool, 25> &natures,
                                               const std::array<bool, 16> &powers, const std::vector<u16> &species) :
    StateFilter(gender, ability, shiny, skip, min, max, natures, powers), species(species)
{
}

bool UndergroundStateFilter::compareState(const UndergroundState &state) const
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

    if (!natures[state.getNature()])
    {
        return false;
    }

    if (std::find(species.begin(), species.end(), state.getSpecie()) == species.end())
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
