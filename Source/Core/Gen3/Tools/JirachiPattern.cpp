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

#include "JirachiPattern.hpp"
#include <Core/RNG/LCRNG.hpp>

/**
 * @brief Does the advance from playing the cutscene
 *
 * @param rng PRNG state
 * @param count Advance counter
 */
static void advanceCutscene(XDRNG &rng, u32 &count)
{
    rng.advance(1, &count);
}

/**
 * @brief Does the advance from accepting the Jirachi. It will advance 6-8 times
 *
 * @param rng PRNG state
 * @param count Advance counter
 */
static void advanceJirachi(XDRNG &rng, u32 &count)
{
    rng.advance(4, &count);

    bool flag = false;
    if (rng.nextUShort(&count) <= 0x4000)
    {
        flag = true;
    }
    else
    {
        flag = rng.nextUShort(&count) <= 0x547a;
    }

    rng.advance(flag ? 1 : 2, &count);
}

/**
 * @brief Does the advance from loading the menu. This will keep advancing until it gets a 1, 2, and 3
 *
 * @param rng PRNG state
 * @param count Advance counter
 */
static void advanceMenu(XDRNG &rng, u32 &count)
{
    u8 mask = 0;
    do
    {
        u8 num = rng.nextUShort(&count) >> 14;
        mask |= 1 << num;
    } while (mask < 14);
}

/**
 * @brief Does the advance from loading the title screen
 *
 * @param rng PRNG state
 * @param count Advance counter
 */
static void advanceTitleScreen(XDRNG &rng, u32 &count)
{
    rng.advance(1, &count);
}

/**
 * @brief Advance the internal search actions for use in brute force searching
 *
 * @param actions Search actions
 */
static void incrementSearchActions(std::vector<u8> &actions)
{
    size_t size = actions.size();

    bool increment = true;
    for (size_t i = 0; i < size; i++)
    {
        u8 compare = i == 0 ? 2 : 3;
        if (actions[i] >= compare)
        {
            increment = false;

            actions[i] = 0;
            if (i != size - 1)
            {
                actions[i + 1]++;
            }
        }
        else if (increment)
        {
            actions[i]++;
            break;
        }
    }
}

/**
 * @brief Determines if target seed passes the menu advance pattern
 * Working backwards from a seed check if the menu sequence will end on said seed
 * Menu will advance the prng until it collects a 1, 2, and 3
 *
 * @param seed Target seed
 *
 * @return true Target seed does pass menu pattern
 * @return false Target seed does not pass menu pattern
 */
static bool validateMenu(u32 seed)
{
    // Impossible to stop on a 0
    u8 target = seed >> 30;
    if (target == 0)
    {
        return false;
    }

    u8 mask = 1 << target;
    XDRNGR rng(seed);
    do
    {
        u8 num = rng.nextUShort() >> 14;

        // Menu keeps rolling for 1, 2, and 3 until we get one of each
        // If we hit our target before this happens then this seed doesn't pass the pattern
        if (num == target)
        {
            return false;
        }

        mask |= 1 << num;
    } while (mask < 14);

    return true;
}

namespace JirachiPattern
{
    std::vector<u8> calculateActions(u32 seed, u32 targetAdvance, u32 bruteForce)
    {
        // Not possible
        if (targetAdvance < 6)
        {
            return {};
        }

        // Special handling for advance range 6-8 where we only need to accept
        if (targetAdvance >= 6 && targetAdvance <= 8)
        {
            XDRNG rng(seed);
            u32 count = 0;
            advanceJirachi(rng, count);

            if (count == targetAdvance)
            {
                return { 255 };
            }
        }

        XDRNG menu(seed);
        u32 menuAdvance = 0;
        u32 menuCount = 0;

        // Use menu advance to get to the brute force range
        while (targetAdvance > bruteForce + menuAdvance)
        {
            menuCount++;
            advanceMenu(menu, menuAdvance);
        }

        // Search the brute force range
        for (size_t i = 1;; i++)
        {
            // This variable handles checking if all of the possibilities of the current search size exceed the target seed
            // This is preferred to guessing what value of 'i' that is
            bool done = true;

            std::vector<u8> searchActions(i, 0);
            while (true)
            {
                u32 searchAdvance = menuAdvance;
                XDRNG rng(menu);

                bool flag = true;
                for (u8 action : searchActions)
                {
                    // Reload menu
                    if (action == 0)
                    {
                        advanceMenu(rng, searchAdvance);
                    }
                    // Reject jirachi
                    else if (action == 1)
                    {
                        advanceJirachi(rng, searchAdvance);
                        advanceTitleScreen(rng, searchAdvance);
                        advanceMenu(rng, searchAdvance);
                    }
                    // Special cutscene
                    else
                    {
                        advanceCutscene(rng, searchAdvance);
                        advanceTitleScreen(rng, searchAdvance);
                        advanceMenu(rng, searchAdvance);
                    }

                    // Make sure we didn't go past the target
                    // Add a buffer of 6 since that is the minimum accepting Jirachi can advance
                    if (searchAdvance + 6 > targetAdvance)
                    {
                        flag = false;
                        break;
                    }
                }

                // Check if accepting the Jirachi will land on the target
                if (flag)
                {
                    done = false;

                    // If we land on target seed then return the actions to get to it
                    advanceJirachi(rng, searchAdvance);
                    if (searchAdvance == targetAdvance)
                    {
                        // Vector is constructed in the way that the initial menu advances are already set
                        std::vector<u8> actions(menuCount + searchActions.size() + 1, 0);

                        // Copy over the search actions
                        std::copy(searchActions.cbegin(), searchActions.cend(), actions.begin() + menuCount);

                        actions[actions.size() - 1] = 3;

                        return actions;
                    }
                }

                // Exit loop once all possibilities have been attempted
                if (std::count(searchActions.cbegin(), searchActions.cend(), 2) == i)
                {
                    break;
                }

                incrementSearchActions(searchActions);
            }

            if (done)
            {
                break;
            }
        }

        // If we get to this point then it is extremely unlikely to get to the the target seed from the current seed
        return {};
    }

    u32 computeJirachiSeed(u32 seed)
    {
        u32 count = 0;
        XDRNG rng(seed);

        advanceMenu(rng, count);
        advanceJirachi(rng, count);

        return rng.getSeed();
    }
}
