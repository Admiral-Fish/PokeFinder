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

#ifndef DREAMRADARGENERATOR_HPP
#define DREAMRADARGENERATOR_HPP

#include <Core/Gen5/DreamRadarTemplate.hpp>
#include <Core/Gen5/Filters/StateFilter5.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Parents/Generators/Generator.hpp>

class DreamRadarState;

/**
 * @brief Dream radar generator for Gen 5
 */
class DreamRadarGenerator : public Generator<Profile5, StateFilter5>
{
public:
    /**
     * @brief Construct a new EventGenerator5 object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param badgeCount Numbers of badges
     * @param radarTemplate Dream radar templates
     * @param profile Profile Information
     * @param filter State filter
     */
    DreamRadarGenerator(u32 initialAdvances, u32 maxAdvances, u8 badgeCount, const std::vector<DreamRadarTemplate> &radarTemplates,
                        const Profile5 &profile, const StateFilter5 &filter);

    /**
     * @brief Generates states
     *
     * @param seed Starting PRNG state
     * @param memory Whether memory link is active
     *
     * @return Vector of computed states
     */
    std::vector<DreamRadarState> generate(u64 seed) const;

private:
    DreamRadarTemplate radarTemplate;
    u8 ivAdvances;
    u8 level;
    u8 pidAdvances;
};

#endif // DREAMRADARGENERATOR_HPP
