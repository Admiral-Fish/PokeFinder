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

#include "UndergroundFilter.hpp"
#include <Core/Gen8/States/UndergroundState.hpp>
#include <algorithm>

UndergroundStateFilter::UndergroundStateFilter(u8 gender, u8 ability, u8 shiny, u8 heightMin, u8 heightMax, u8 weightMin, u8 weightMax,
                                               bool skip, const std::array<u8, 6> &min, const std::array<u8, 6> &max,
                                               const std::array<bool, 25> &natures, const std::array<bool, 16> &powers,
                                               const std::vector<u16> &species) :
    StateFilter(gender, ability, shiny, heightMin, heightMax, weightMin, weightMax, skip, min, max, natures, powers), species(species)
{
    std::sort(this->species.begin(), this->species.end());
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

    if (state.getHeight() < heightMin || state.getHeight() > heightMax)
    {
    return false;
    }

    if (state.getWeight() < weightMin || state.getWeight() > weightMax)
    {
        return false;
    }

    if (!std::binary_search(species.begin(), species.end(), state.getSpecie()))
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
        if (iv < ivMin[i] || iv > ivMax[i])
        {
            return false;
        }
    }

    return true;
}
