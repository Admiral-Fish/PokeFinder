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

#ifndef EVENTGENERATOR8_HPP
#define EVENTGENERATOR8_HPP

#include <Core/Gen8/Filters/StateFilter8.hpp>
#include <Core/Gen8/WB8.hpp>
#include <Core/Parents/Generators/Generator.hpp>

/**
 * @brief Event generator for Gen 8
 */
class EventGenerator8 : public Generator<StateFilter8>
{
public:
    /**
     * @brief Construct a new EventGenerator8 object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param offset Number of advances to offset
     * @param tid Trainer ID
     * @param sid Secret ID
     * @param version Game version
     * @param wb8 Pokemon template
     * @param filter State filter
     */
    EventGenerator8(u32 initialAdvances, u32 maxAdvances, u32 offset, u16 tid, u16 sid, Game version, const WB8 &wb8,
                    const StateFilter8 &filter);

    /**
     * @brief Generates states
     *
     * @param seed0 Upper half of PRNG state
     * @param seed1 Lower half of PRNG state
     *
     * @return Vector of computed states
     */
    std::vector<GeneratorState> generate(u64 seed0, u64 seed1) const;

private:
    WB8 wb8;
};

#endif // EVENTGENERATOR8_HPP
