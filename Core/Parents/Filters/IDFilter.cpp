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

#include "IDFilter.hpp"
#include <Core/Gen8/States/IDState8.hpp>
#include <algorithm>

IDFilter::IDFilter(const std::vector<u16> &tidFilter, const std::vector<u16> &sidFilter,
                   const std::vector<std::pair<u16, u16>> &tidSIDFilter, const std::vector<u16> &tsvFilter,
                   const std::vector<std::pair<u16, u16>> &tidTSVFilter, const std::vector<u32> &displayFilter) :
    displayFilter(displayFilter),
    tidSIDFilter(tidSIDFilter),
    tidTSVFilter(tidTSVFilter),
    sidFilter(sidFilter),
    tidFilter(tidFilter),
    tsvFilter(tsvFilter)
{
}

bool IDFilter::compareState(const IDState &state) const
{
    if (!tidFilter.empty() && std::find(tidFilter.begin(), tidFilter.end(), state.getTID()) == tidFilter.end())
    {
        return false;
    }

    if (!sidFilter.empty() && std::find(sidFilter.begin(), sidFilter.end(), state.getSID()) == sidFilter.end())
    {
        return false;
    }

    if (!tidSIDFilter.empty()
        && std::find_if(
               tidSIDFilter.begin(), tidSIDFilter.end(),
               [&state](const std::pair<u16, u16> &entry) { return entry.first == state.getTID() && entry.second == state.getSID(); })
            == tidSIDFilter.end())
    {
        return false;
    }

    if (!tsvFilter.empty() && std::find(tsvFilter.begin(), tsvFilter.end(), state.getTSV()) == tsvFilter.end())
    {
        return false;
    }

    if (!tidTSVFilter.empty()
        && std::find_if(
               tidTSVFilter.begin(), tidTSVFilter.end(),
               [&state](const std::pair<u16, u16> &entry) { return entry.first == state.getTID() && entry.second == state.getTSV(); })
            == tidTSVFilter.end())
    {
        return false;
    }

    return true;
}

bool IDFilter::compareState(const IDState8 &state) const
{
    if (!compareState(static_cast<const IDState &>(state)))
    {
        return false;
    }

    if (!displayFilter.empty() && std::find(displayFilter.begin(), displayFilter.end(), state.getDisplayTID()) == displayFilter.end())
    {
        return false;
    }

    return true;
}
