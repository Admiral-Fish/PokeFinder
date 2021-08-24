/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "HiddenGrottoFilter.hpp"
#include <Core/Gen5/States/HiddenGrottoState.hpp>

HiddenGrottoFilter::HiddenGrottoFilter(const std::vector<bool> &groups, const std::vector<bool> &encounterSlots,
                                       const std::vector<bool> &genders) :
    groups(groups), encounterSlots(encounterSlots), genders(genders)
{
}

bool HiddenGrottoFilter::compareState(const HiddenGrottoState &state) const
{
    if (!groups[state.getGroup()])
    {
        return false;
    }

    if (!encounterSlots[state.getSlot()])
    {
        return false;
    }

    if (!genders[state.getGender()])
    {
        return false;
    }

    return true;
}
