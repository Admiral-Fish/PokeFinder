/*
 * This file is part of PokéFinder
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "ShinyPatchCalculator.hpp"
#include <Core/RNG/LCRNG.hpp>
#include <algorithm>

namespace ShinyPatchCalculator
{
    State calculate(u32 seed, u16 chain, bool captured, const Grass &grass)
    {
        State state {};
        PokeRNG rng(seed);

        // D/P/Pt draw ALL four positions before checking any patch's chain/shiny status.
        // See docs/shiny-patch.md for the corresponding game routines.
        for (u8 i = 0; i < 4; i++)
        {
            auto &patch = state.patches[i];
            patch.ring = 4 - i;
            u8 width = 2 * patch.ring + 1;
            u8 position = rng.nextUShort<false>(8 * patch.ring, &state.calls);
            if (position < 2 * width)
            {
                patch.x = position % width - patch.ring;
                patch.y = position < width ? -patch.ring : patch.ring;
            }
            else
            {
                position -= 2 * width;
                patch.x = position % 2 == 0 ? -patch.ring : patch.ring;
                patch.y = position / 2 + 1 - patch.ring;
            }
            patch.active = grass[(patch.y + 4) * 9 + patch.x + 4];
        }

        const u16 rate = 8200 - 200 * std::min<u16>(chain, 40);
        const u16 shinyThreshold = 0xffff / rate + 1;
        for (auto &patch : state.patches)
        {
            if (!patch.active)
            {
                continue;
            }

            patch.chainRoll = rng.nextUShort<false>(100, &state.calls);
            patch.chainCall = state.calls;
            patch.continues = patch.chainRoll < 8 + 20 * patch.ring + (captured ? 10 : 0);
            if (!patch.continues)
            {
                rng.next(&state.calls); // Failed chain: random shake type, NOT a shiny check.
            }
            else if (chain != 0)
            {
                patch.shinyRoll = rng.nextUShort(&state.calls);
                patch.shinyCall = state.calls;
                patch.shiny = patch.shinyRoll < shinyThreshold;
            }
            // At chain 0 a successful continuation returns without a shiny RNG call.
        }

        state.endSeed = rng.getSeed();
        return state;
    }
}
