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

#include "Profile4.hpp"

constexpr u8 unownForms[4][2] = { { 0, 9 }, { 17, 21 }, { 10, 16 }, { 22, 25 } };

std::vector<u8> Profile4::getUndiscoveredUnownForms(const std::vector<u8> &unlocked) const
{
    std::vector<u8> forms;
    forms.reserve(26);

    for (u8 letter : unlocked)
    {
        if (!unownDiscovered[letter])
        {
            forms.emplace_back(letter);
        }
    }

    return forms;
}

std::vector<u8> Profile4::getUnlockedUnownForms() const
{
    std::vector<u8> forms;
    forms.reserve(26);

    for (int i = 0; i < 4; i++)
    {
        if (unownPuzzle[i])
        {
            for (u8 form = unownForms[i][0]; form <= unownForms[i][1]; form++)
            {
                forms.emplace_back(form);
            }
        }
    }

    return forms;
}

bool Profile4::operator==(const Profile4 &other) const
{
    return Profile::operator==(other) && dex == other.dex && unownDiscovered == other.unownDiscovered && unownPuzzle == other.unownPuzzle;
}

bool Profile4::operator!=(const Profile4 &other) const
{
    return !(operator==(other));
}
