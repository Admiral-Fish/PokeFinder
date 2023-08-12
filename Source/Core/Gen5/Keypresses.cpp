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

#include "Keypresses.hpp"
#include <Core/Enum/Buttons.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <bit>

/**
 * @brief Determines if the selected key presses are valid
 *
 * @param button Held buttons
 * @param count Number of held key presses
 * @param skipLR Whether to include combos with L/R
 *
 * @return true Held buttons are valid
 * @return false Held buttons are invalid
 */
static bool valid(Buttons button, int count, bool skipLR)
{
    if (skipLR && (button & Buttons::LR) == Buttons::LR)
    {
        return false;
    }

    if ((button & Buttons::UpDown) == Buttons::UpDown)
    {
        return false;
    }

    if ((button & Buttons::LeftRight) == Buttons::LeftRight)
    {
        return false;
    }

    if ((button & Buttons::SoftReset) == Buttons::SoftReset)
    {
        return false;
    }

    if (count >= 7 && (button & Buttons::SelectStart) == Buttons::SelectStart)
    {
        return false;
    }

    return true;
}

namespace Keypresses
{
    std::vector<Keypress> getKeypresses(const Profile5 &profile)
    {
        std::vector<Keypress> keypress;

        auto keypresses = profile.getKeypresses();
        bool skipLR = profile.getSkipLR();

        for (u16 bits = 0; bits < 0x1000; bits++)
        {
            auto combo = static_cast<Buttons>(bits);
            int count = std::popcount(bits);
            if (count <= 8 && keypresses[count] && valid(combo, count, skipLR))
            {
                Keypress k = { getValue(combo), combo };
                keypress.emplace_back(k);
            }
        }

        return keypress;
    }

    u32 getValue(Buttons button)
    {
        constexpr u32 values[12] = { 0x10000,   0x20000,   0x40000,    0x80000,    0x1000000,  0x2000000,
                                     0x4000000, 0x8000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000 };

        u32 value = 0xff2f0000;

        u16 bits = toInt(button);
        for (int i = 0; bits; bits >>= 1, i++)
        {
            if (bits & 1)
            {
                value -= values[i];
            }
        }

        return value;
    }
}
