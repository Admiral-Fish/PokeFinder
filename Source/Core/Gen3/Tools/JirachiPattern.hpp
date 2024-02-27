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

#ifndef JIRACHIPATTERN_HPP
#define JIRACHIPATTERN_HPP

#include <Core/Global.hpp>
#include <vector>

namespace JirachiPattern
{
    /**
     * @brief Calculates series of actions to take to hit the target seed
     *
     * @param seed Starting seed
     * @param advance Target advance
     * @param bruteForce What frame range to brute force actions over
     *
     * @return List of actions to take to get the target Jirachi
     */
    std::vector<u8> calculateActions(u32 seed, u32 targetAdvance, u32 bruteForce);

    /**
     * @brief Runs the menu and Jirachi pattern to get the PRNG state that would generate the Jirachi
     *
     * @param seed Pre menu PRNG state
     *
     * @return PRNG state that generates the Jirachi
     */
    u32 computeJirachiSeed(u32 seed);
};

#endif // JIRACHIPATTERN_HPP
