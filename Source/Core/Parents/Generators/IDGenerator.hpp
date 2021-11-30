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

#ifndef IDGENERATOR_HPP
#define IDGENERATOR_HPP

#include <Core/Parents/Filters/IDFilter.hpp>
#include <Core/Util/Global.hpp>
#include <vector>

template <class Filter = IDFilter>
class IDGenerator
{
public:
    IDGenerator() = default;

    IDGenerator(u32 initialAdvances, u32 maxAdvances, const Filter &filter) :
        initialAdvances(initialAdvances), maxAdvances(maxAdvances), filter(filter)
    {
    }

protected:
    u32 initialAdvances;
    u32 maxAdvances;
    Filter filter;
};

#endif // IDGENERATOR_HPP
