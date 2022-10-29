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

#ifndef IDGENERATOR8_HPP
#define IDGENERATOR8_HPP

#include <Core/Parents/Generators/IDGenerator.hpp>

class IDState8;

/**
 * @brief TID/SID generator for Gen8
 */
class IDGenerator8 : public IDGenerator
{
public:
    /**
     * @brief Construct a new IDGenerator3 object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param filter State filter
     */
    IDGenerator8(u32 initialAdvances, u32 maxAdvances, const IDFilter &filter);

    /**
     * @brief Generates states
     *
     * @param seed0 Upper half of PRNG state
     * @param seed1 Lower half of PRNG state
     *
     * @return Vector of computed states
     */
    std::vector<IDState8> generate(u64 seed0, u64 seed1);
};

#endif // IDGENERATOR8_HPP
