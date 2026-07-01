/*
 * This file is part of PokeFinder
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

#ifndef STEPENCOUNTER_HPP
#define STEPENCOUNTER_HPP

#include <Core/Enum/Encounter.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Global.hpp>
#include <algorithm>
#include <array>

namespace StepEncounter5
{
    constexpr u8 impossible = 255;

    struct Pattern
    {
        u8 up;
        u8 interval;
        u8 max;
        u8 safety;
    };

    inline const Pattern *getPattern(Game version, Encounter encounter, u8 rate)
    {
        if ((version & Game::BW2) == Game::None)
        {
            return nullptr;
        }

        if (encounter != Encounter::Grass && encounter != Encounter::GrassDark && encounter != Encounter::Surfing)
        {
            return nullptr;
        }

        static constexpr std::array<Pattern, 12> patterns = { {
            { 3, 1, 13, 2 }, // Weak Grass Lv1
            { 3, 1, 19, 2 }, // Weak Grass Lv2
            { 2, 3, 13, 3 }, // Strong Grass Lv1
            { 2, 2, 19, 2 }, // Strong Grass Lv2
            { 5, 1, 31, 2 }, // Weak Tall Grass Lv1
            { 5, 1, 31, 2 }, // Strong Tall Grass Lv1
            { 2, 18, 7, 8 }, // Cave Lv1
            { 2, 9, 7, 4 }, // Cave Lv2
            { 2, 3, 13, 3 }, // Desert Lv1
            { 1, 12, 7, 3 }, // Water/Surf Lv1
            { 1, 9, 7, 3 }, // Water/Surf Lv2
            { 2, 3, 13, 3 }, // Marsh Lv1
        } };

        if (rate == 0 || rate > patterns.size())
        {
            return nullptr;
        }

        return &patterns[rate - 1];
    }

    inline u8 getSteps(u8 ratio, const Pattern &pattern, u8 multiplier = 1)
    {
        if (ratio > pattern.max * multiplier)
        {
            return impossible;
        }

        u8 needed = std::max<u8>(1, (ratio + multiplier - 1) / multiplier);
        u8 steps = pattern.safety + std::max<u8>(1, ((needed - 1 + pattern.up - 1) / pattern.up) * pattern.interval);
        return steps - 1;
    }

    inline u8 getSteps(Game version, Encounter encounter, u8 rate, u8 ratio, bool stepModifier = false)
    {
        if ((version & Game::BW) != Game::None
            && (encounter == Encounter::Grass || encounter == Encounter::GrassDark || encounter == Encounter::Surfing))
        {
            u16 threshold = stepModifier ? rate * 2 : rate;
            return ratio <= threshold ? (ratio <= 1 ? 0 : 1) : impossible;
        }

        const Pattern *pattern = getPattern(version, encounter, rate);
        if (pattern == nullptr)
        {
            return impossible;
        }

        return getSteps(ratio, *pattern, stepModifier ? 2 : 1);
    }
}

#endif // STEPENCOUNTER_HPP
