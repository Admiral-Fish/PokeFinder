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

#include "StateFilter.hpp"

StateFilter::StateFilter(u8 gender, u8 ability, u8 shiny, bool skip, const std::array<u8, 6> &min, const std::array<u8, 6> &max,
                         const std::array<bool, 25> &natures, const std::array<bool, 16> &powers) :
    min(min), max(max), gender(gender), ability(ability), natures(natures), powers(powers), shiny(shiny), skip(skip)
{
}

bool StateFilter::compareAbility(u8 ability) const
{
    return skip || this->ability == 255 || this->ability == ability;
}

bool StateFilter::compareGender(u8 gender) const
{
    return skip || this->gender == 255 || this->gender == gender;
}

bool StateFilter::compareNature(u8 nature) const
{
    return skip || natures[nature];
}

bool StateFilter::compareShiny(u8 shiny) const
{
    return skip || this->shiny == 255 || (this->shiny & shiny);
}

bool StateFilter::compareIV(std::array<u8, 6> ivs) const
{
    if (skip)
    {
        return true;
    }

    for (int i = 0; i < 6; i++)
    {
        u8 iv = ivs[i];
        if (iv < min[i] || iv > max[i])
        {
            return false;
        }
    }

    return true;
}
