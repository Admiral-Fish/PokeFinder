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

#include "IDFilter.hpp"
#include <Core/Parents/States/IDState.hpp>
#include <algorithm>

IDFilter::IDFilter(const std::vector<u16> &tidFilter, const std::vector<u16> &sidFilter, const std::vector<u16> &tsvFilter) :
    tidFilter(tidFilter), sidFilter(sidFilter), tsvFilter(tsvFilter)
{
}

bool IDFilter::compare(const IDState &state) const
{
    if (std::find(tidFilter.begin(), tidFilter.end(), state.getTID()) == tidFilter.end())
    {
        return false;
    }

    if (std::find(sidFilter.begin(), sidFilter.end(), state.getSID()) == sidFilter.end())
    {
        return false;
    }

    if (std::find(tsvFilter.begin(), tsvFilter.end(), state.getTSV()) == tsvFilter.end())
    {
        return false;
    }

    return true;
}
