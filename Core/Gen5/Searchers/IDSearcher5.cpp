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

#include "IDSearcher5.hpp"

std::vector<SearcherState5<IDState>> IDSearcher5::search(const IDGenerator5 &generator, const Date &date, u8 hour, u8 minute, u8 minSecond,
                                                         u8 maxSecond)
{
    SHA1 sha(profile);

    for (u16 timer0 = profile.getTimer0Min(); timer0 <= profile.getTimer0Max(); timer0++)
    {
        sha.setTimer0(timer0, profile.getVCount());
        sha.setDate(date);
        auto alpha = sha.precompute();
        for (const auto &keypress : keypresses)
        {
            sha.setButton(keypress.value);

            for (u8 second = minSecond; second <= maxSecond; second++)
            {
                sha.setTime(hour, minute, second, profile.getDSType());
                u64 seed = sha.hashSeed(alpha);

                auto states = generator.generate(seed);
                if (!states.empty())
                {
                    DateTime dt(date, Time(hour, minute, second));

                    results.reserve(results.capacity() + states.size());
                    for (const auto &state : states)
                    {
                        results.emplace_back(dt, seed, keypress.button, timer0, state);
                    }
                }
            }
        }
    }

    return results;
}
