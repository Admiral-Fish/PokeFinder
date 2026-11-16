/*
 * This file is part of PokéFinder
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef SHINYPATCHCALCULATOR_HPP
#define SHINYPATCHCALCULATOR_HPP

#include <Core/Global.hpp>
#include <array>

namespace ShinyPatchCalculator
{
    /** Row-major 9x9 grid, centered on the player. True means radar-eligible grass
     * on the player's elevation and in the same map. Positive x/y is right/down. */
    using Grass = std::array<bool, 81>;

    struct Patch
    {
        s8 x = 0;
        s8 y = 0;
        u8 ring = 0;
        bool active = false;
        bool continues = false;
        bool shiny = false;
        u8 chainRoll = 0;
        u16 shinyRoll = 0; // Raw high 16 bits, only present when shinyCall != 0
        u8 chainCall = 0;
        u8 shinyCall = 0; // 1-based RNG call relative to the activation state
    };

    struct State
    {
        std::array<Patch, 4> patches; // Outermost (ring 4) to innermost (ring 1)
        u32 endSeed;
        u32 calls;
    };

    /**
     * @brief Simulates one D/P/Pt radar activation, including all four rings.
     *
     * @param seed RNG state immediately before the first patch-position call
     * @param chain Current chain count (0 disables shinies; odds cap at 40)
     * @param captured True only for patches automatically spawned after a capture;
     * manual radar resets and post-KO patches use false
     * @param grass Eligible tiles around the player, supplied by the caller
     */
    State calculate(u32 seed, u16 chain, bool captured, const Grass &grass);
}

#endif // SHINYPATCHCALCULATOR_HPP
