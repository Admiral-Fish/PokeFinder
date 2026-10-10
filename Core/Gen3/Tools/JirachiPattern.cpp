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
#include <algorithm>
#include <array>
#include <queue>
#include <unordered_map>

struct Node
{
    u32 depth;
    u32 depthEstimated;
    u32 seed;
    u32 totalAdvances;

    bool operator>(const Node &other) const
    {
        if (depthEstimated != other.depthEstimated)
        {
            return depthEstimated > other.depthEstimated;
        }
        return depth > other.depth;
    }
};

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
 * @brief Does actions for accepting a Jirachi
 *
 * @param seed PRNG state
 *
 * @return Resulting PRNG state and advance count
 */
static std::pair<u32, u32> doAccept(u32 seed)
{
    u32 count = 0;
    XDRNG rng(seed);

    advanceJirachi(rng, count);

    return { rng.getSeed(), count };
}

/**
 * @brief Does actions for watching special cutscene
 *
 * @param seed PRNG state
 *
 * @return Resulting PRNG state and advance count
 */
static std::pair<u32, u32> doCutscene(u32 seed)
{
    u32 count = 0;
    XDRNG rng(seed);

    advanceCutscene(rng, count);
    advanceTitleScreen(rng, count);
    advanceMenu(rng, count);

    return { rng.getSeed(), count };
}

/**
 * @brief Does actions for reload menu
 *
 * @param seed PRNG state
 *
 * @return Resulting PRNG state and advance count
 */
static std::pair<u32, u32> doMenu(u32 seed)
{
    u32 count = 0;
    XDRNG rng(seed);

    advanceMenu(rng, count);

    return { rng.getSeed(), count };
}

/**
 * @brief Does actions for rejecting a Jirachi
 *
 * @param seed PRNG state
 *
 * @return Resulting PRNG state and advance count
 */
static std::pair<u32, u32> doReject(u32 seed)
{
    u32 count = 0;
    XDRNG rng(seed);

    advanceJirachi(rng, count);
    advanceTitleScreen(rng, count);
    advanceMenu(rng, count);

    return { rng.getSeed(), count };
}

/**
 * @brief Computes heuristic for A* search. It is based on worst case advances from an action
 *
 * @return Estimated remaining actions that can be taken
 */
static u32 heuristic(u32 remainingFrames)
{
    return remainingFrames / 79;
}

namespace JirachiPattern
{
    std::vector<u8> calculateActions(u32 seed, u32 targetSeed, u32 maxActions)
    {
        // Special handling for advance range 6-8 where we only need to accept
        // If not possible or under 6 then can't hit target
        u32 targetAdvance = XDRNG::distance(seed, targetSeed);
        if (targetAdvance <= 8)
        {
            XDRNG rng(seed);
            u32 count = 0;
            advanceJirachi(rng, count);

            if (count == targetAdvance)
            {
                return { 3 };
            }
            else
            {
                return { };
            }
        }

        std::priority_queue<Node, std::vector<Node>, std::greater<Node>> queue;
        std::unordered_map<u32, std::pair<u32, u8>> parentMap;
        std::unordered_map<u32, int> bestCost;

        queue.push({ 0, heuristic(targetAdvance), seed, 0 });
        bestCost[seed] = 0;

        bool success = false;
        while (!queue.empty() && !success)
        {
            Node curr = queue.top();
            queue.pop();

            if (curr.depth > maxActions)
            {
                break;
            }

            if (curr.depth > bestCost[curr.seed])
            {
                continue;
            }

            int remainingAdvances = targetAdvance - curr.totalAdvances;
            if (remainingAdvances < 6)
            {
                continue;
            }

            std::array<std::pair<u32, u32>, 3> transitions = { doMenu(curr.seed), doReject(curr.seed), doCutscene(curr.seed) };
            for (u8 i = 0; i < transitions.size(); i++)
            {
                u32 nextSeed = transitions[i].first;
                u32 nextAdvance = transitions[i].second;

                u32 newAdvances = curr.totalAdvances + nextAdvance;
                u32 newDepth = curr.depth + 1;

                if (newAdvances <= targetAdvance && (bestCost.find(nextSeed) == bestCost.end() || newDepth < bestCost[nextSeed]))
                {
                    bestCost[nextSeed] = newDepth;
                    parentMap[nextSeed] = { curr.seed, i };

                    // Check for exit condition
                    auto accept = doAccept(nextSeed);
                    if (accept.first == targetSeed)
                    {
                        parentMap[targetSeed] = { nextSeed, 3 };
                        success = true;
                        break;
                    }

                    u32 newDepthEstimated = newDepth + heuristic(targetAdvance - newAdvances);
                    queue.push({ newDepth, newDepthEstimated, nextSeed, newAdvances });
                }
            }
        }

        std::vector<u8> actions;
        if (success)
        {
            u32 curr = targetSeed;
            while (curr != seed)
            {
                auto edge = parentMap[curr];
                actions.emplace_back(edge.second);
                curr = edge.first;
            }
            std::reverse(actions.begin(), actions.end());
        }

        return actions;
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
