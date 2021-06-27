/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "UnownSearcher3.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <algorithm>
#include <array>
#include <iostream>

UnownSearcher3::UnownSearcher3(u16 tid, u16 sid, u8 genderRatio, Method method, const StateFilter &filter) :
    UnownSearcher(tid, sid, genderRatio, method, filter), cache(method), searching(false), progress(0)
{
    // std::cout << "created\n";
    // std::cout.flush();
}

void UnownSearcher3::startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max)
{
    // std::cout << "started\n";
    // std::cout.flush();
    searching = true;

    for (u8 hp = min[0]; hp <= max[0]; hp++)
    {
        for (u8 atk = min[1]; atk <= max[1]; atk++)
        {
            for (u8 def = min[2]; def <= max[2]; def++)
            {
                for (u8 spa = min[3]; spa <= max[3]; spa++)
                {
                    for (u8 spd = min[4]; spd <= max[4]; spd++)
                    {
                        for (u8 spe = min[5]; spe <= max[5]; spe++)
                        {
                            if (!searching)
                            {
                                return;
                            }
                            auto states = search(hp, atk, def, spa, spd, spe);

                            std::lock_guard<std::mutex> guard(mutex);
                            results.insert(results.end(), states.begin(), states.end());
                            progress++;
                        }
                    }
                }
            }
        }
    }
}

void UnownSearcher3::cancelSearch()
{
    std::cout << "cancel\n";
    std::cout.flush();
    searching = false;
}

std::vector<UnownState> UnownSearcher3::getResults()
{
    // std::cout << "get results\n";
    // std::cout.flush();
    std::lock_guard<std::mutex> guard(mutex);
    auto data = std::move(results);
    return data;
}

int UnownSearcher3::getProgress() const
{
    return progress;
}

std::string UnownSearcher3::getLetter(u32 pid) const
{
    u32 val1, val2, val3, val4, val;
    val1 = (pid >> 24) & 3;
    val2 = (pid >> 16) & 3;
    val3 = (pid >> 8) & 3;
    val4 = pid & 3;
    val = (val1 << 6) | (val2 << 4) | (val3 << 2) | val4;
    return letters[val % 28];
}

std::string UnownSearcher3::getTargetLetter(u8 location, u8 slot) const
{
    switch (location)
    {
    case 0:
    {
        return slot < 99 ? "A" : "?";
    }
    case 1:
    {
        return slot < 50 ? "C" : slot < 80 ? "D" : slot < 94 ? "H" : slot < 99 ? "U" : "O";
    }
    case 2:
    {
        return slot < 60 ? "N" : slot < 90 ? "S" : slot < 98 ? "H" : "E";
    }
    case 3:
    {
        return slot < 40 ? "P" : slot < 60 ? "L" : slot < 80 ? "J" : slot < 94 ? "R" : "Q";
    }
    case 4:
    {
        return slot < 40 ? "Y" : slot < 60 ? "T" : slot < 85 ? "G" : slot < 98 ? "F" : "K";
    }
    case 5:
    {
        return slot < 50 ? "V" : slot < 80 ? "W" : slot < 90 ? "X" : slot < 98 ? "M" : "B";
    }
    case 6:
    {
        return slot < 99 ? "Z" : "!";
    }
    default:
    {
        return "0";
    }
    }
}

u32 UnownSearcher3::getLocation(std::string letter) const
{
    std::array<std::string,2> location0 = { "A", "?" };
    std::array<std::string,5> location1 = { "C", "D", "H", "U", "O" };
    std::array<std::string,4> location2 = { "N", "S", "I", "E" };
    std::array<std::string,5> location3 = { "P", "J", "L", "R", "Q" };
    std::array<std::string,5> location4 = { "Y", "G", "T", "F", "K" };
    std::array<std::string,5> location5 = { "V", "W", "X", "M", "B" };
    std::array<std::string,2> location6 = { "Z", "!" };

    if (std::find(location0.begin(), location0.end(), letter) != location0.end())
    {
        return 0;
    }
    if (std::find(location1.begin(), location1.end(), letter) != location1.end())
    {
        return 1;
    }
    if (std::find(location2.begin(), location2.end(), letter) != location2.end())
    {
        return 2;
    }
    if (std::find(location3.begin(), location3.end(), letter) != location3.end())
    {
        return 3;
    }
    if (std::find(location4.begin(), location4.end(), letter) != location4.end())
    {
        return 4;
    }
    if (std::find(location5.begin(), location5.end(), letter) != location5.end())
    {
        return 5;
    }
    if (std::find(location6.begin(), location6.end(), letter) != location6.end())
    {
        return 6;
    }
    return 7;
}

std::array<std::string,7> UnownSearcher3::getLetterSlots(u32 slot) const
{
    std::array<std::string,7> letterslots;
    if (slot < 99)
    {
        letterslots[0] = "A";
        letterslots[6] = "Z";
    }
    else
    {
        letterslots[0] = "?";
        letterslots[6] = "!";
    }
    if (slot < 50)
    {
        letterslots[1] = "C";
    }
    else if (slot < 80)
    {
        letterslots[1] = "D";
    }
    else if (slot < 94)
    {
        letterslots[1] = "H";
    }
    else if (slot < 99)
    {
        letterslots[1] = "U";
    }
    else
    {
        letterslots[1] = "O";
    }
    if (slot < 60)
    {
        letterslots[2] = "N";
    }
    else if (slot < 90)
    {
        letterslots[2] = "S";
    }
    else if (slot < 98)
    {
        letterslots[2] = "I";
    }
    else
    {
        letterslots[2] = "E";
    }
    if (slot < 40)
    {
        letterslots[3] = "P";
    }
    else if (slot < 60)
    {
        letterslots[3] = "L";
    }
    else if (slot < 80)
    {
        letterslots[3] = "J";
    }
    else if (slot < 94)
    {
        letterslots[3] = "R";
    }
    else
    {
        letterslots[3] = "Q";
    }
    if (slot < 40)
    {
        letterslots[4] = "Y";
    }
    else if (slot < 60)
    {
        letterslots[4] = "T";
    }
    else if (slot < 85)
    {
        letterslots[4] = "G";
    }
    else if (slot < 98)
    {
        letterslots[4] = "F";
    }
    else
    {
        letterslots[4] = "K";
    }
    if (slot < 50)
    {
        letterslots[5] = "V";
    }
    else if (slot < 80)
    {
        letterslots[5] = "W";
    }
    else if (slot < 90)
    {
        letterslots[5] = "X";
    }
    else if (slot < 98)
    {
        letterslots[5] = "M";
    }
    else
    {
        letterslots[5] = "B";
    }
    return letterslots;
}

std::vector<UnownState> UnownSearcher3::search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    std::vector<UnownState> states;

    UnownState state;
    state.setIVs(hp, atk, def, spa, spd, spe);
    state.calculateHiddenPower();
    if (!filter.compareHiddenPower(state))
    {
        return states;
    }

    auto seeds = cache.recoverLower16BitsIV(hp, atk, def, spa, spd, spe);
    for (const u32 val : seeds)
    {
        // std::cout << val << "\n";
        // std::cout.flush();

        // Use for loop to check both normal and sister spread
        for (const bool flag : { false, true })
        {
            u32 valmod = val;
            if (flag)
            {
                state.setPID(state.getPID() ^ 0x80008000);
                state.setNature(state.getPID() % 25);
                valmod ^= 0x80000000;
            }
            // Setup normal state
            PokeRNGR rng(valmod);
            rng.advance(method == Method::MethodH2);

            u16 low = rng.nextUShort();
            u16 high = rng.nextUShort();

            state.setPID(high, low);
    //        state.setAbility(low & 1);
    //        state.setGender(low & 255, genderRatio);
    //        state.setNature(state.getPID() % 25);
    //        state.setShiny(tsv, high ^ low, 8);

            u32 seed;

            std::array<std::string,7> letterslots;
            // std::cout << "started big loop\n";
            // std::cout.flush();
            bool sflag = true;
            do
            {
                PokeRNGR go(rng.getSeed());
                u16 lowtest = go.nextUShort();
                u16 hightest = go.nextUShort();
                u32 pidtest = hightest << 16 | lowtest;
                go.next();
                u8 slot = go.nextUShort() % 100;
                letterslots = getLetterSlots(slot);
                if (getLetter(pidtest) == getLetter(state.getPID()))
                {
                    sflag = false;
                    break;
                }
                if (std::find(letterslots.begin(), letterslots.end(), getLetter(state.getPID())) != letterslots.end())
                {
                    sflag = false;
                    u32 stateseed = go.next();
                    seed = go.next();

                    PokeRNG go2(seed);
                    go2.next();
                    u8 slot2 = go2.nextUShort() % 100;
                    go2.next();
                    std::string letter;
                    u32 pidtest2;
                    std::string targetLetter = getTargetLetter(getLocation(getLetter(state.getPID())), slot2);
                    u16 hightest2,lowtest2;
                    // std::cout << "start of letter loop\n";
                    // std::cout.flush();
                    while (letter != targetLetter)
                    {
                        hightest2 = go2.nextUShort();
                        lowtest2 = go2.nextUShort();
                        pidtest2 = (hightest2 << 16) | lowtest2;
                        letter = getLetter(pidtest2);
                    }
                    // std::cout << "end of letter loop\n";
                    // std::cout.flush();
                    state.setLetter(letter);
                    state.setPID(hightest2, lowtest2);
                    state.setAbility(lowtest2 & 1);
                    state.setGender(lowtest2 & 255, genderRatio);
                    state.setNature(state.getPID() % 25);
                    state.setShiny<8>(tsv, hightest2 ^ lowtest2);
                    // if (!filter.comparePID(state))
                    // {
                    //     rng.advance(2);
                    //     continue;
                    // }
                    state.setSeed(stateseed);
                    states.emplace_back(state);
                }
                rng.advance(2);
            } while (sflag & searching);
            // std::cout << "ended big loop\n";
            // std::cout.flush();

        }
    }

    return states;
}
