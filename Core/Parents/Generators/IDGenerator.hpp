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

#ifndef IDGENERATOR_HPP
#define IDGENERATOR_HPP

#include <Core/Global.hpp>
#include <Core/Parents/Filters/IDFilter.hpp>
#include <vector>

/**
 * @brief Parent generator class for TID/SID generation
 */
class IDGenerator
{
public:
    /**
     * @brief Construct a new IDGenerator object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param filter State filter
     */
    IDGenerator(u32 initialAdvances, u32 maxAdvances, const IDFilter &filter) :
        filter(filter), initialAdvances(initialAdvances), maxAdvances(maxAdvances)
    {
    }

protected:
    IDFilter filter;
    u32 initialAdvances;
    u32 maxAdvances;
};

#endif // IDGENERATOR_HPP
