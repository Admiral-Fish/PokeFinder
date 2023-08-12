/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef GAMECUBEGENERATOR_HPP
#define GAMECUBEGENERATOR_HPP

#include <Core/Gen3/Profile3.hpp>
#include <Core/Parents/Filters/StateFilter.hpp>
#include <Core/Parents/Generators/Generator.hpp>

class GeneratorState;
class StaticTemplate;
class ShadowTemplate;

/**
 * @brief Static encounter generator for GameCube
 */
class GameCubeGenerator : public Generator<Profile3, StateFilter>
{
public:
    /**
     * @brief Construct a new GameCubeGenerator object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param delay Number of advances to offset
     * @param method Encounter method
     * @param unset Whether first shadow pokemon is unset or not
     * @param profile Profile Information
     * @param filter State filter
     */
    GameCubeGenerator(u32 initialAdvances, u32 maxAdvances, u32 delay, Method method, bool unset, const Profile3 &profile,
                      const StateFilter &filter);

    /**
     * @brief Generates states for the \p shadowTemplate
     *
     * @param seed Starting PRNG state
     * @param shadowTemplate Pokemon template
     *
     * @return Vector of computed states
     */
    std::vector<GeneratorState> generate(u32 seed, const ShadowTemplate *shadowTemplate) const;

    /**
     * @brief Generates states for the \p staticTemplate
     *
     * @param seed Starting PRNG state
     * @param staticTemplate Pokemon template
     *
     * @return Vector of computed states
     */
    std::vector<GeneratorState> generate(u32 seed, const StaticTemplate *staticTemplate) const;

private:
    bool unset;

    /**
     * @brief Generates states for Channel Jirachi
     *
     * @param seed Starting PRNG state
     * @param staticTemplate Pokemon template
     *
     * @return Vector of computed states
     */
    std::vector<GeneratorState> generateChannel(u32 seed, const StaticTemplate *staticTemplate) const;

    /**
     * @brief Generates states for Colo shadows
     *
     * @param seed Starting PRNG state
     * @param shadowTemplate Pokemon template
     *
     * @return Vector of computed states
     */
    std::vector<GeneratorState> generateColoShadow(u32 seed, const ShadowTemplate *shadowTemplate) const;

    /**
     * @brief Generates states for Gales shadows
     *
     * @param seed Starting PRNG state
     * @param shadowTemplate Pokemon template
     *
     * @return Vector of computed states
     */
    std::vector<GeneratorState> generateGalesShadow(u32 seed, const ShadowTemplate *shadowTemplate) const;

    /**
     * @brief Generates states for Gales/Colo non-locks
     *
     * @param seed Starting PRNG state
     * @param staticTemplate Pokemon template
     *
     * @return Vector of computed states
     */
    std::vector<GeneratorState> generateNonLock(u32 seed, const StaticTemplate *staticTemplate) const;
};

#endif // GAMECUBEGENERATOR_HPP
