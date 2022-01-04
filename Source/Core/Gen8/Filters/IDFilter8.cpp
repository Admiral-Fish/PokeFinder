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

#include "IDFilter8.hpp"
#include <Core/Gen8/States/IDState8.hpp>
#include <algorithm>

IDFilter8::IDFilter8(const std::vector<u16> &tidFilter, const std::vector<u16> &sidFilter, const std::vector<u16> &tsvFilter,
                     const std::vector<u32> &g8tidFilter) :
    IDFilter(tidFilter, sidFilter, tsvFilter), g8tidFilter(g8tidFilter)
{
}

bool IDFilter8::compare(const IDState8 &state) const
{
    if (!tidFilter.empty() && std::find(tidFilter.begin(), tidFilter.end(), state.getTID()) == tidFilter.end())
    {
        return false;
    }

    if (!sidFilter.empty() && std::find(sidFilter.begin(), sidFilter.end(), state.getSID()) == sidFilter.end())
    {
        return false;
    }

    if (!tsvFilter.empty() && std::find(tsvFilter.begin(), tsvFilter.end(), state.getTSV()) == tsvFilter.end())
    {
        return false;
    }

    if (!g8tidFilter.empty() && std::find(g8tidFilter.begin(), g8tidFilter.end(), state.getG8TID()) == g8tidFilter.end())
    {
        return false;
    }

    return true;
}
