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

#include "Keypresses.hpp"
#include <Core/Enum/Buttons.hpp>
#include <Core/Gen5/Profile5.hpp>

constexpr Buttons keys[8] = { Buttons::R, Buttons::L, Buttons::X, Buttons::Y, Buttons::A, Buttons::B, Buttons::Select, Buttons::Start };
constexpr Buttons directions[8] = { Buttons::Right,   Buttons::Left,   Buttons::Up,        Buttons::Down,
                                    Buttons::RightUp, Buttons::LeftUp, Buttons::RightDown, Buttons::LeftDown };
constexpr u32 buttonValues[12]
    = { 0x10000, 0x20000, 0x40000, 0x80000, 0x1000000, 0x2000000, 0x4000000, 0x8000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000 };

static bool valid(Buttons button, bool skipLR)
{
    return !(skipLR && ((button & Buttons::L) != Buttons::None || (button & Buttons::R) != Buttons::None));
}

namespace Keypresses
{
    std::vector<Buttons> getKeyPresses(const Profile5 &profile)
    {
        std::vector<Buttons> buttons;

        auto keypresses = profile.getKeypresses();
        bool skipLR = profile.getSkipLR();

        if (keypresses[0])
        {
            buttons.emplace_back(Buttons::None);
        }

        for (u8 i = 0; i < 8; i++)
        {
            if (keypresses[1])
            {
                Buttons combo = keys[i];
                if (valid(combo, skipLR))
                {
                    buttons.emplace_back(keys[i]);
                }

                buttons.emplace_back(directions[i]);
            }

            if (keypresses[2])
            {
                for (u8 j = i + 1; j < 8; j++)
                {
                    Buttons combo = keys[i] | keys[j];
                    if (valid(combo, skipLR))
                    {
                        buttons.emplace_back(combo);
                    }
                }

                for (Buttons direction : directions)
                {
                    Buttons combo = keys[i] | direction;
                    if (valid(combo, skipLR))
                    {
                        buttons.emplace_back(combo);
                    }
                }
            }

            if (keypresses[3])
            {
                for (u8 j = i + 1; j < 8; j++)
                {
                    for (u8 k = j + 1; k < 8; k++)
                    {
                        Buttons combo = keys[i] | keys[j] | keys[k];
                        if (valid(combo, skipLR))
                        {
                            buttons.emplace_back(combo);
                        }
                    }

                    for (Buttons direction : directions)
                    {
                        Buttons combo = keys[i] | keys[j] | direction;
                        if (valid(combo, skipLR))
                        {
                            buttons.emplace_back(combo);
                        }
                    }
                }
            }
        }

        return buttons;
    }

    std::vector<u32> getValues(const std::vector<Buttons> &buttons)
    {
        std::vector<u32> values;
        for (const auto button : buttons)
        {
            u32 value = 0xff2f0000;

            for (int i = 0; i < 12; i++)
            {
                if ((button & static_cast<Buttons>(1 << i)) != Buttons::None)
                {
                    value -= buttonValues[i];
                }
            }

            values.emplace_back(value);
        }

        return values;
    }
}
