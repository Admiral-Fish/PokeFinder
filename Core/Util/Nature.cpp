/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "Nature.hpp"

constexpr double modifiers[25][6] = {
    { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 }, // Hardy
    { 1.0, 1.1, 0.9, 1.0, 1.0, 1.0 }, // Lonely
    { 1.0, 1.1, 1.0, 1.0, 1.0, 0.9 }, // Brave
    { 1.0, 1.1, 1.0, 0.9, 1.0, 1.0 }, // Adamant
    { 1.0, 1.1, 1.0, 1.0, 0.9, 1.0 }, // Naughty
    { 1.0, 0.9, 1.1, 1.0, 1.0, 1.0 }, // Bold
    { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 }, // Docile
    { 1.0, 1.0, 1.1, 1.0, 1.0, 0.9 }, // Relaxed
    { 1.0, 1.0, 1.1, 0.9, 1.0, 1.0 }, // Impish
    { 1.0, 1.0, 1.1, 1.0, 0.9, 1.0 }, // Lax
    { 1.0, 0.9, 1.0, 1.0, 1.0, 1.1 }, // Timid
    { 1.0, 1.0, 0.9, 1.0, 1.0, 1.1 }, // Hasty
    { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 }, // Serious
    { 1.0, 1.0, 1.0, 0.9, 1.0, 1.1 }, // Jolly
    { 1.0, 1.0, 1.0, 1.0, 0.9, 1.1 }, // Naive
    { 1.0, 0.9, 1.0, 1.1, 1.0, 1.0 }, // Modest
    { 1.0, 1.0, 0.9, 1.1, 1.0, 1.0 }, // Mild
    { 1.0, 1.0, 1.0, 1.1, 1.0, 0.9 }, // Quiet
    { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 }, // Bashful
    { 1.0, 1.0, 1.0, 1.1, 0.9, 1.0 }, // Rash
    { 1.0, 0.9, 1.0, 1.0, 1.1, 1.0 }, // Calm
    { 1.0, 1.0, 0.9, 1.0, 1.1, 1.0 }, // Gentle
    { 1.0, 1.0, 1.0, 1.0, 1.1, 0.9 }, // Sassy
    { 1.0, 1.0, 1.0, 0.9, 1.1, 1.0 }, // Careful
    { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 } // Quirky
};

namespace Nature
{
    double getNatureModifier(u8 nature, u8 stat)
    {
        return modifiers[nature][stat];
    }
}
