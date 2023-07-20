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

#include "StateFilter.hpp"
#include <Core/Parents/States/State.hpp>
#include <Core/Parents/States/WildState.hpp>

StateFilter::StateFilter(u8 gender, u8 ability, u8 shiny, bool skip, const std::array<u8, 6> &min, const std::array<u8, 6> &max,
                         const std::array<bool, 25> &natures, const std::array<bool, 16> &powers) :
    natures(natures), powers(powers), max(max), min(min), skip(skip), ability(ability), gender(gender), shiny(shiny)
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

bool StateFilter::compareHiddenPower(u8 hiddenPower) const
{
    return skip || powers[hiddenPower];
}

bool StateFilter::compareIV(const std::array<u8, 6> &ivs) const
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

bool StateFilter::compareNature(u8 nature) const
{
    return skip || natures[nature];
}

bool StateFilter::compareShiny(u8 shiny) const
{
    return skip || this->shiny == 255 || (this->shiny & shiny);
}

bool StateFilter::compareState(const SearcherState &state) const
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

bool StateFilter::compareState(const State &state) const
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

WildStateFilter::WildStateFilter(u8 gender, u8 ability, u8 shiny, bool skip, const std::array<u8, 6> &min, const std::array<u8, 6> &max,
                                 const std::array<bool, 25> &natures, const std::array<bool, 16> &powers,
                                 const std::array<bool, 12> &encounterSlots) :
    StateFilter(gender, ability, shiny, skip, min, max, natures, powers), encounterSlots(encounterSlots)
{
}

bool WildStateFilter::compareEncounterSlot(u8 encounterSlot) const
{
    return skip || encounterSlots[encounterSlot];
}

bool WildStateFilter::compareState(const WildGeneratorState &state) const
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

bool WildStateFilter::compareState(const WildSearcherState &state) const
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

bool WildStateFilter::compareState(const WildState &state) const
{
    return StateFilter::compareState(static_cast<const State &>(state)) && encounterSlots[state.getEncounterSlot()];
}
