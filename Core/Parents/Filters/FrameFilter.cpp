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

#include "FrameFilter.hpp"
#include <Core/Parents/Frames/Frame.hpp>
#include <Core/Parents/Frames/WildFrame.hpp>

FrameFilter::FrameFilter(u8 gender, u8 ability, u8 shiny, bool skip, const QVector<u8> &min, const QVector<u8> &max,
                         const QVector<bool> &natures, const QVector<bool> &powers, const QVector<bool> &encounters) :
    min(min),
    max(max),
    gender(gender),
    ability(ability),
    natures(natures),
    powers(powers),
    encounters(encounters),
    shiny(shiny),
    skip(skip)
{
}

bool FrameFilter::compareFrame(const Frame &frame) const
{
    return comparePID(frame) && compareIVs(frame);
}

bool FrameFilter::comparePID(const Frame &frame) const
{
    return compareShiny(frame) && compareAbility(frame) && compareGender(frame) && compareNature(frame);
}

bool FrameFilter::compareIVs(const Frame &frame) const
{
    return compareHiddenPower(frame) && compareIV(frame);
}

bool FrameFilter::compareAbility(const Frame &frame) const
{
    return skip || ability == 255 || ability == frame.getAbility();
}

bool FrameFilter::compareGender(const Frame &frame) const
{
    return skip || gender == 255 || gender == frame.getGender();
}

bool FrameFilter::compareNature(const Frame &frame) const
{
    return skip || natures.at(frame.getNature());
}

bool FrameFilter::compareShiny(const Frame &frame) const
{
    return skip || shiny == 255 || (shiny & frame.getShiny());
}

bool FrameFilter::compareIV(const Frame &frame) const
{
    if (skip)
    {
        return true;
    }

    for (u8 i = 0; i < 6; i++)
    {
        u8 iv = frame.getIV(i);

        if (iv < min.at(i) || iv > max.at(i))
        {
            return false;
        }
    }

    return true;
}

bool FrameFilter::compareHiddenPower(const Frame &frame) const
{
    return skip || powers.at(frame.getHidden());
}

bool FrameFilter::compareEncounterSlot(const WildFrame &frame) const
{
    return skip || encounters.at(frame.getEncounterSlot());
}
