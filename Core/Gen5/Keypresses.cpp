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

#include "Keypresses.hpp"

constexpr u32 buttonValues[12]
    = { 0x10000, 0x20000, 0x40000, 0x80000, 0x1000000, 0x2000000, 0x4000000, 0x8000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000 };

enum Buttons : u16
{
    None = 0,
    R = 1 << 0, // -0x10000
    L = 1 << 1, // -0x20000
    X = 1 << 2, // -0x40000
    Y = 1 << 3, // -0x80000
    A = 1 << 4, // -0x1000000
    B = 1 << 5, // -0x2000000
    Select = 1 << 6, // -0x4000000
    Start = 1 << 7, // -0x8000000
    SelectStart = Select | Start,
    Right = 1 << 8, // -0x10000000
    Left = 1 << 9, // -0x20000000
    LeftRight = Left | Right,
    Up = 1 << 10, // -0x40000000
    Down = 1 << 11, // -0x80000000
    UpDown = Up | Down
};

namespace Keypresses
{
    namespace
    {
        bool valid(u32 button, bool skipLR)
        {
            if (skipLR && (button & Buttons::L || button & Buttons::R))
            {
                return false;
            }

            if ((button & Buttons::LeftRight) == Buttons::LeftRight)
            {
                return false;
            }

            if ((button & Buttons::UpDown) == Buttons::UpDown)
            {
                return false;
            }

            if ((button & Buttons::SelectStart) == Buttons::SelectStart && (button & Buttons::A || button & Buttons::B))
            {
                return false;
            }

            return true;
        }

        QVector<u16> getKeyPresses(int keys, bool skipLR)
        {
            QVector<u16> buttons;

            for (int i = 0; i < 12; i++)
            {
                u16 button1 = 1 << i;

                if (!valid(button1, skipLR))
                {
                    continue;
                }

                if (keys == 1)
                {
                    buttons.append(button1);
                }

                for (int j = i + 1; j < (keys > 1 ? 12 : 0); j++)
                {
                    u16 button2 = button1 | (1 << j);

                    if (!valid(button2, skipLR))
                    {
                        continue;
                    }

                    if (keys == 2)
                    {
                        buttons.append(button2);
                    }

                    for (int k = j + 1; k < (keys > 2 ? 12 : 0); k++)
                    {
                        u16 button3 = button2 | (1 << k);

                        if (!valid(button3, skipLR))
                        {
                            continue;
                        }

                        if (keys == 3)
                        {
                            buttons.append(button3);
                        }
                    }
                }
            }

            return buttons;
        }
    }

    QVector<u16> getKeyPresses(const QVector<bool> &keys, bool skipLR)
    {
        QVector<u16> buttons;
        if (keys.at(0))
        {
            buttons.append({ Buttons::None });
        }

        for (int i = 1; i < keys.size(); i++)
        {
            if (keys.at(i))
            {
                buttons.append(getKeyPresses(i, skipLR));
            }
        }

        return buttons;
    }

    QVector<u32> getValues(const QVector<u16> &buttons)
    {
        QVector<u32> values;
        for (const auto button : buttons)
        {
            u32 value = 0xff2f0000;

            for (int i = 0; i < 12; i++)
            {
                if (button & (1 << i))
                {
                    value -= buttonValues[i];
                }
            }

            values.append(value);
        }

        return values;
    }
}
