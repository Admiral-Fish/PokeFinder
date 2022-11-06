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

#ifndef EGGGENERATOR8_HPP
#define EGGGENERATOR8_HPP

#include <Core/Gen8/Filters/StateFilter8.hpp>
#include <Core/Parents/Generators/EggGenerator.hpp>

class EggState8;

/**
 * @brief Egg generator for Gen8
 */
class EggGenerator8 : public EggGenerator<StateFilter8>
{
public:
    /**
     * @brief Construct a new EggGenerator object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param offset Number of advances to offset
     * @param version Game version
     * @param tid Trainer ID
     * @param sid Secret ID
     * @param compatability Parent compatability
     * @param daycare Daycare parent information
     * @param shinyCharm Whether the shiny charm is obtained
     * @param filter State filter
     */
    EggGenerator8(u32 initialAdvances, u32 maxAdvances, u32 offset, u16 tid, u16 sid, Game version, u8 compatability,
                  const Daycare &daycare, bool shinyCharm, const StateFilter8 &filter);

    /**
     * @brief Generates states
     *
     * @param seed0 Upper half of PRNG state
     * @param seed1 Lower half of PRNG state
     *
     * @return Vector of computed states
     */
    std::vector<EggState8> generate(u64 seed0, u64 seed1) const;

private:
    bool shinyCharm;
};

#endif // EGGGENERATOR8_HPP
