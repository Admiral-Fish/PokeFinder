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

#ifndef EGGSEARCHER4_HPP
#define EGGSEARCHER4_HPP

#include <Core/Gen4/Profile4.hpp>
#include <Core/Parents/Searchers/Searcher.hpp>

class EggGenerator4;
class EggSearcherState4;

/**
 * @brief Egg encounter searcher for Gen4
 */
class EggSearcher4 : public Searcher<Profile4, EggSearcherState4>
{
public:
    /**
     * @brief Construct a new EggSearcher4 object
     *
     * @param minDelay Minimum delay
     * @param maxDelay Maximum delay
     * @param profile Profile Information
     */
    EggSearcher4(u32 minDelay, u32 maxDelay, const Profile4 &profile);

    /**
     * @brief Starts the search
     *
     * @param generator Egg generator
     */
    void startSearch(const EggGenerator4 &generator);

private:
    u32 maxDelay;
    u32 minDelay;
};

#endif // EGGSEARCHER4_HPP
