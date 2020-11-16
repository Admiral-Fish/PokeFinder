/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Parents/States/State.hpp>
#include <Core/Parents/States/WildState.hpp>

StateFilter::StateFilter(u8 gender, u8 ability, u8 shiny, bool skip, const std::array<u8, 6> &min, const std::array<u8, 6> &max,
                         const std::vector<bool> &natures, const std::vector<bool> &powers, const std::vector<bool> &encounters) :
    min(min), max(max), gender(gender), ability(ability), natures(natures), powers(powers), encounters(encounters), shiny(shiny), skip(skip)
{
}

bool StateFilter::compareState(const State &state) const
{
    return comparePID(state) && compareIVs(state);
}

bool StateFilter::comparePID(const State &state) const
{
    return compareShiny(state) && compareAbility(state) && compareGender(state) && compareNature(state);
}

bool StateFilter::compareIVs(const State &state) const
{
    return compareHiddenPower(state) && compareIV(state);
}

bool StateFilter::compareAbility(const State &state) const
{
    return skip || ability == 255 || ability == state.getAbility();
}

bool StateFilter::compareGender(const State &state) const
{
    return skip || gender == 255 || gender == state.getGender();
}

bool StateFilter::compareNature(const State &state) const
{
    return skip || natures.at(state.getNature());
}

bool StateFilter::compareShiny(const State &state) const
{
    return skip || shiny == 255 || (shiny & state.getShiny());
}

bool StateFilter::compareIV(const State &state) const
{
    if (skip)
    {
        return true;
    }

    for (u8 i = 0; i < 6; i++)
    {
        u8 iv = state.getIV(i);

        if (iv < min.at(i) || iv > max.at(i))
        {
            return false;
        }
    }

    return true;
}

bool StateFilter::compareHiddenPower(const State &state) const
{
    return skip || powers.at(state.getHidden());
}

bool StateFilter::compareEncounterSlot(const WildState &state) const
{
    return skip || encounters.at(state.getEncounterSlot());
}
