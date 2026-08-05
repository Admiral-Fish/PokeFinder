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

#include "PokeRadarGenerator.hpp"

#include <Core/RNG/LCRNG.hpp>
#include <algorithm>

PokeRadarGenerator::PokeRadarGenerator(u32 initialAdvances, u32 maxAdvances, u16 chainCount, PokeRadarChainType chainType,
                                       PokeRadarResult result, const std::array<bool, 81> &grass) :
    initialAdvances(initialAdvances),
    maxAdvances(maxAdvances),
    chainCount(chainCount),
    chainType(chainType),
    result(result),
    grass(grass)
{
}

std::pair<u8, u8> PokeRadarGenerator::getSkips(u32 seed, u32 advances)
{
    PokeRNG noGrace(seed, advances);
    u8 noGraceSkip = noGrace.nextUShort<false>(100) < 40 ? 1 : 0;

    PokeRNG grace(seed, advances);
    u8 graceSkip = 0;
    if (grace.nextUShort<false>(100) < 5)
    {
        graceSkip++;
        if (grace.nextUShort<false>(100) < 40)
        {
            graceSkip++;
        }
    }

    return { noGraceSkip, graceSkip };
}

std::vector<PokeRadarState> PokeRadarGenerator::generate(u32 seed) const
{
    std::vector<PokeRadarState> states;

    PokeRNG rng(seed, initialAdvances);
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++)
    {
        PokeRNG go(rng);
        states.emplace_back(rng.nextUShort(), initialAdvances + cnt, generatePatches(go));
    }

    return states;
}

PokeRadarState PokeRadarGenerator::generate(u32 seed, u32 advances, u32 patchAdvances) const
{
    PokeRNG display(seed, advances);
    PokeRNG go(seed, patchAdvances);
    return PokeRadarState(display.nextUShort(), advances, generatePatches(go));
}

PokeRadarState PokeRadarGenerator::generatePrevious(u32 seed, u32 advances) const
{
    PokeRNG display(seed);
    PokeRNG go(seed);
    if (advances == 0)
    {
        PokeRNGR reverse(seed);
        go.setSeed(reverse.next());
    }
    else
    {
        go.jump(advances - 1);
    }
    return PokeRadarState(display.nextUShort(), advances == 0 ? 0 : advances - 1, generatePatches(go));
}

PokeRadarPatch PokeRadarGenerator::buildPatch(u8 ring, u8 rand) const
{
    u8 size = 9 - ring * 2;
    u8 offset = ring;
    u8 x;
    u8 y;

    if (rand < size)
    {
        x = offset + rand;
        y = offset;
    }
    else if (rand < size * 2)
    {
        x = offset + rand - size;
        y = offset + size - 1;
    }
    else
    {
        u8 side = rand - size * 2;
        x = offset + (side % 2 == 0 ? 0 : size - 1);
        y = offset + (side / 2) + 1;
    }

    PokeRadarPatch patch = { x, y, ring, isGrass(x, y), false, false, false };
    return patch;
}

std::array<PokeRadarPatch, 4> PokeRadarGenerator::generatePatches(PokeRNG &go) const
{
    constexpr std::array<u8, 4> ringTileCount = { 32, 24, 16, 8 };
    std::array<PokeRadarPatch, 4> patches;
    PokeRadarResult effectiveResult = chainCount == 0 ? PokeRadarResult::ManualActivation : result;

    for (u8 ring = 0; ring < 4; ring++)
    {
        patches[ring] = buildPatch(ring, go.nextUShort<false>(ringTileCount[ring]));
    }

    if (effectiveResult != PokeRadarResult::ManualActivation)
    {
        go.nextUShort();
    }

    constexpr std::array<u8, 4> ratesDefeat = { 88, 68, 48, 28 };
    constexpr std::array<u8, 4> ratesCapture = { 98, 78, 58, 38 };
    const auto &continueRates = effectiveResult == PokeRadarResult::Capture ? ratesCapture : ratesDefeat;

    const bool strongChain = chainType == PokeRadarChainType::Strong || chainType == PokeRadarChainType::StrongShiny;
    for (auto &patch : patches)
    {
        if (!patch.active)
        {
            continue;
        }

        bool rolledContinue = go.nextUShort<false>(100) < continueRates[patch.ring];
        patch.continueChain = chainCount == 0 || rolledContinue;
        if (rolledContinue)
        {
            patch.strong = chainCount != 0 && strongChain;

            if (chainCount != 0)
            {
                u16 shinyRate = std::max<u16>(8200 - std::min<u16>(chainCount, 40) * 200, 200);
                patch.shiny = go.nextUShort<false>(shinyRate) == 0;
            }
        }
        else
        {
            patch.strong = go.nextUShort<false>(100) >= 50;
        }
    }

    return patches;
}

bool PokeRadarGenerator::isGrass(u8 x, u8 y) const
{
    return (x != 4 || y != 4) && grass[y * 9 + x];
}
