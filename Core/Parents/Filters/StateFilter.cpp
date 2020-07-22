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

StateFilter::StateFilter(u8 gender, u8 ability, u8 shiny, bool skip, const QVector<u8> &min, const QVector<u8> &max,
                         const QVector<bool> &natures, const QVector<bool> &powers, const QVector<bool> &encounters) :
    min(min), max(max), gender(gender), ability(ability), natures(natures), powers(powers), encounters(encounters), shiny(shiny), skip(skip)
{
}

bool StateFilter::compareState(const State &currentState) const
{
    return comparePID(currentState) && compareIVs(currentState);
}

bool StateFilter::comparePID(const State &currentState) const
{
    return compareShiny(currentState) && compareAbility(currentState) && compareGender(currentState) && compareNature(currentState);
}

bool StateFilter::compareIVs(const State &currentState) const
{
    return compareHiddenPower(currentState) && compareIV(currentState);
}

bool StateFilter::compareAbility(const State &currentState) const
{
    return skip || ability == 255 || ability == currentState.getAbility();
}

bool StateFilter::compareGender(const State &currentState) const
{
    return skip || gender == 255 || gender == currentState.getGender();
}

bool StateFilter::compareNature(const State &currentState) const
{
    return skip || natures.at(currentState.getNature());
}

bool StateFilter::compareShiny(const State &currentState) const
{
    return skip || shiny == 255 || (shiny & currentState.getShiny());
}

bool StateFilter::compareIV(const State &currentState) const
{
    if (skip)
    {
        return true;
    }

    for (u8 i = 0; i < 6; i++)
    {
        u8 iv = currentState.getIV(i);

        if (iv < min.at(i) || iv > max.at(i))
        {
            return false;
        }
    }

    return true;
}

bool StateFilter::compareHiddenPower(const State &currentState) const
{
    return skip || powers.at(currentState.getHidden());
}

bool StateFilter::compareEncounterSlot(const WildState &currentState) const
{
    return skip || encounters.at(currentState.getEncounterSlot());
}
