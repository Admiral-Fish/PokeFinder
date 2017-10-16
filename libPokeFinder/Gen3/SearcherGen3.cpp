/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish and bumba
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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

#include "SearcherGen3.hpp"

// Default constructor
SearcherGen3::SearcherGen3()
{
    tid = 12345;
    sid = 54321;
    psv = tid ^ sid;
    frame.SetIDs(tid, sid, psv);
}

// Constructor given user defined parameters
SearcherGen3::SearcherGen3(uint32_t tid, uint32_t sid)
{
    this->tid = tid;
    this->sid = sid;
    psv = tid ^ sid;
    frame.SetIDs(tid, sid, psv);
}

// Returns vector of frames for Channel Method
std::vector<FrameGen3> SearcherGen3::searchMethodChannel(uint32_t hp, uint32_t atk, uint32_t def, uint32_t spa, uint32_t spd, uint32_t spe, FrameCompare compare)
{
    std::vector<FrameGen3> frames;

    std::vector<uint32_t> seeds = euclidean.RecoverLower27BitsChannel(hp, atk, def, spa, spd, spe);
    int size = seeds.size();

    for (int i = 0; i < size; i++)
    {
        frame.SetIVsManual(hp, atk, def, spa, spd, spe);
        rng.seed = seeds[i];

        // Calculate PID
        rng.ReverseFrames(3);
        uint32_t pid2 = rng.Prev16Bit();
        uint32_t pid1 = rng.Prev16Bit();

        // Determine if PID needs to be XORed
        if ((pid2 > 7 ? 0 : 1) != (pid1 ^ rng.Prev16Bit() ^ 40122))
            pid1 ^= 0x8000;
        frame.SetPID(pid1, pid2);

        if (compare.CompareFramePID(frame))
        {
            frame.seed = rng.Prev32Bit();
            frames.push_back(frame);
        }
    }
    return frames;
}

// Returns vector of frames for Method Colo Shadows
std::vector<FrameGen3> SearcherGen3::searchMethodColo(uint32_t hp, uint32_t atk, uint32_t def, uint32_t spa, uint32_t spd, uint32_t spe, FrameCompare compare)
{
    std::vector<FrameGen3> frames;

    uint32_t first = (hp | (atk << 5) | (def << 10)) << 16;
    uint32_t second = (spe | (spa << 5) | (spd << 10)) << 16;
    std::vector<uint32_t> seeds = euclidean.RecoverLower16BitsIV(first, second);
    int size = seeds.size();

    // Need to eventually add Naturelock checking
    for (int i = 0; i < size; i += 2)
    {
        // Setup normal frame
        frame.SetIVsManual(hp, atk, def, spa, spd, spe);
        rng.seed = seeds[i + 1];
        rng.AdvanceFrames(1);
        frame.SetPID(rng.Next16Bit(), rng.Next16Bit());
        frame.seed = seeds[i] * 0xB9B33155 + 0xA170F641;
        if (compare.CompareFramePID(frame))
            frames.push_back(frame);

        // Setup XORed frame
        frame.pid ^= 0x80008000;
        frame.nature = frame.pid % 25;
        if (compare.CompareFramePID(frame))
        {
            frame.seed ^= 0x80000000;
            frames.push_back(frame);
        }
    }
    return frames;
}

// Returns vector of frames for Method H1
std::vector<FrameGen3> SearcherGen3::searchMethodH1(uint32_t hp, uint32_t atk, uint32_t def, uint32_t spa, uint32_t spd, uint32_t spe, FrameCompare compare)
{
    std::vector<FrameGen3> frames;

    uint32_t first = (hp | (atk << 5) | (def << 10)) << 16;
    uint32_t second = (spe | (spa << 5) | (spd << 10)) << 16;
    std::vector<uint32_t> seeds = cache.RecoverLower16BitsIV(first, second);
    int size = seeds.size();
    uint32_t seed;

    for (int i = 0; i < size; i++)
    {
        // Setup normal frame
        frame.SetIVsManual(hp, atk, def, spa, spd, spe);
        rng.seed = seeds[i];
        frame.SetPID(rng.Prev16Bit(), rng.Prev16Bit());
        seed = rng.Prev32Bit();

        // Use for loop to check both normal and sister spread
        for (int i = 0; i < 2; i++)
        {
            if (i == 1)
            {
                frame.pid ^= 0x80008000;
                frame.nature = frame.pid % 25;
                seed ^= 0x80000000;
            }

            if (!compare.CompareFramePID(frame))
                continue;

            LCRNG testRNG = PokeRNG(seed);
            uint32_t testPID, slot, testSeed;
            uint32_t nextRNG = seed >> 16;
            uint32_t nextRNG2 = testRNG.Prev16Bit();

            do
            {
                frame.leadType = None;

                // Check normal
                if ((nextRNG % 25) == frame.nature)
                {
                    slot = testRNG.seed * 0xeeb9eb65 + 0xa3561a1;
                    testSeed = slot * 0xeeb9eb65 + 0xa3561a1;
                    frame.seed = seed;
                    frame.encounterSlot = EncounterSlot::HSlot(slot >> 16, encounterType);
                    frames.push_back(frame);

                    slot = slot * 0xeeb9eb65 + 0xa3561a1;
                    testSeed = testSeed * 0xeeb9eb65 + 0xa3561a1;
                    frame.seed = testSeed;
                    frame.encounterSlot = EncounterSlot::HSlot(slot >> 16, encounterType);

                    // Check failed synch
                    if ((nextRNG2 & 1) == 1)
                    {
                        frame.leadType = Synchronize;
                        frames.push_back(frame);
                    }

                    // Check Cute Charm
                    if ((nextRNG2 % 3) > 0)
                    {
                        frame.leadType = CuteCharm;
                        frames.push_back(frame);
                    }
                }
                // Check synch
                else if ((nextRNG2 & 1) == 0)
                {
                    frame.leadType = Synchronize;
                    slot = testRNG.seed * 0xeeb9eb65 + 0xa3561a1;
                    testSeed = slot * 0xeeb9eb65 + 0xa3561a1;
                    frame.seed = testSeed;
                    frame.encounterSlot = EncounterSlot::HSlot(slot >> 16, encounterType);
                    frames.push_back(frame);
                }

                testPID = (nextRNG << 16) | nextRNG2;
                nextRNG = testRNG.Prev16Bit();
                nextRNG2 = testRNG.Prev16Bit();
            }
            while ((testPID % 25) != frame.nature);
        }
    }
    return frames;
}

// Returns vector of frames for Method H2
std::vector<FrameGen3> SearcherGen3::searchMethodH2(uint32_t hp, uint32_t atk, uint32_t def, uint32_t spa, uint32_t spd, uint32_t spe, FrameCompare compare)
{
    std::vector<FrameGen3> frames;

    uint32_t first = (hp | (atk << 5) | (def << 10)) << 16;
    uint32_t second = (spe | (spa << 5) | (spd << 10)) << 16;
    std::vector<uint32_t> seeds = cache.RecoverLower16BitsIV(first, second);
    int size = seeds.size();
    uint32_t seed;

    for (int i = 0; i < size; i++)
    {
        // Setup normal frame
        frame.SetIVsManual(hp, atk, def, spa, spd, spe);
        rng.seed = seeds[i];
        rng.ReverseFrames(1);
        frame.SetPID(rng.Prev16Bit(), rng.Prev16Bit());
        seed = rng.Prev32Bit();

        // Use for loop to check both normal and sister spread
        for (int i = 0; i < 2; i++)
        {
            if (i == 1)
            {
                frame.pid ^= 0x80008000;
                frame.nature = frame.pid % 25;
                seed ^= 0x80000000;
            }

            if (!compare.CompareFramePID(frame))
                continue;

            LCRNG testRNG = PokeRNG(seed);
            uint32_t testPID, slot, testSeed;
            uint32_t nextRNG = seed >> 16;
            uint32_t nextRNG2 = testRNG.Prev16Bit();

            do
            {
                frame.leadType = None;

                // Check normal
                if ((nextRNG % 25) == frame.nature)
                {
                    slot = testRNG.seed * 0xeeb9eb65 + 0xa3561a1;
                    testSeed = slot * 0xeeb9eb65 + 0xa3561a1;
                    frame.seed = seed;
                    frame.encounterSlot = EncounterSlot::HSlot(slot >> 16, encounterType);
                    frames.push_back(frame);

                    slot = slot * 0xeeb9eb65 + 0xa3561a1;
                    testSeed = testSeed * 0xeeb9eb65 + 0xa3561a1;
                    frame.seed = testSeed;
                    frame.encounterSlot = EncounterSlot::HSlot(slot >> 16, encounterType);

                    // Check failed synch
                    if ((nextRNG2 & 1) == 1)
                    {
                        frame.leadType = Synchronize;
                        frames.push_back(frame);
                    }

                    // Check Cute Charm
                    if ((nextRNG2 % 3) > 0)
                    {
                        frame.leadType = CuteCharm;
                        frames.push_back(frame);
                    }
                }
                // Check synch
                else if ((nextRNG2 & 1) == 0)
                {
                    frame.leadType = Synchronize;
                    slot = testRNG.seed * 0xeeb9eb65 + 0xa3561a1;
                    testSeed = slot * 0xeeb9eb65 + 0xa3561a1;
                    frame.seed = testSeed;
                    frame.encounterSlot = EncounterSlot::HSlot(slot >> 16, encounterType);
                    frames.push_back(frame);
                }

                testPID = (nextRNG << 16) | nextRNG2;
                nextRNG = testRNG.Prev16Bit();
                nextRNG2 = testRNG.Prev16Bit();
            }
            while ((testPID % 25) != frame.nature);
        }
    }
    return frames;
}

// Returns vector of frames for Method H4
std::vector<FrameGen3> SearcherGen3::searchMethodH4(uint32_t hp, uint32_t atk, uint32_t def, uint32_t spa, uint32_t spd, uint32_t spe, FrameCompare compare)
{
    std::vector<FrameGen3> frames;

    uint32_t first = (hp | (atk << 5) | (def << 10)) << 16;
    uint32_t second = (spe | (spa << 5) | (spd << 10)) << 16;
    std::vector<uint32_t> seeds = cache.RecoverLower16BitsIV(first, second);
    int size = seeds.size();
    uint32_t seed;

    for (int i = 0; i < size; i++)
    {
        // Setup normal frame
        frame.SetIVsManual(hp, atk, def, spa, spd, spe);
        rng.seed = seeds[i];
        frame.SetPID(rng.Prev16Bit(), rng.Prev16Bit());
        seed = rng.Prev32Bit();

        // Use for loop to check both normal and sister spread
        for (int i = 0; i < 2; i++)
        {
            if (i == 1)
            {
                frame.pid ^= 0x80008000;
                frame.nature = frame.pid % 25;
                seed ^= 0x80000000;
            }

            if (!compare.CompareFramePID(frame))
                continue;

            LCRNG testRNG = PokeRNG(seed);
            uint32_t testPID, slot, testSeed;
            uint32_t nextRNG = seed >> 16;
            uint32_t nextRNG2 = testRNG.Prev16Bit();

            do
            {
                frame.leadType = None;

                // Check normal
                if (nextRNG % 25 == frame.nature)
                {
                    slot = testRNG.seed * 0xeeb9eb65 + 0xa3561a1;
                    testSeed = slot * 0xeeb9eb65 + 0xa3561a1;
                    frame.seed = seed;
                    frame.encounterSlot = EncounterSlot::HSlot(slot >> 16, encounterType);
                    frames.push_back(frame);

                    slot = slot * 0xeeb9eb65 + 0xa3561a1;
                    testSeed = testSeed * 0xeeb9eb65 + 0xa3561a1;
                    frame.seed = testSeed;
                    frame.encounterSlot = EncounterSlot::HSlot(slot >> 16, encounterType);

                    // Check failed synch
                    if ((nextRNG2 & 1) == 1)
                    {
                        frame.leadType = Synchronize;
                        frames.push_back(frame);
                    }

                    // Check Cute Charm
                    if ((nextRNG2 % 3) > 0)
                    {
                        frame.leadType = CuteCharm;
                        frames.push_back(frame);
                    }
                }

                // Check synch
                if ((nextRNG2 & 1) == 0)
                {
                    frame.leadType = Synchronize;
                    slot = testRNG.seed * 0xeeb9eb65 + 0xa3561a1;
                    testSeed = slot * 0xeeb9eb65 + 0xa3561a1;
                    frame.seed = seed;
                    frame.encounterSlot = EncounterSlot::HSlot(slot >> 16, encounterType);
                    frames.push_back(frame);
                }

                testPID = (nextRNG << 16) | nextRNG2;
                nextRNG = testRNG.Prev16Bit();
                nextRNG2 = testRNG.Prev16Bit();
            }
            while ((testPID % 25) != frame.nature);
        }
    }
    return frames;
}

// Returns vector of frames for Method Gales Shadows
std::vector<FrameGen3> SearcherGen3::searchMethodXD(uint32_t hp, uint32_t atk, uint32_t def, uint32_t spa, uint32_t spd, uint32_t spe, FrameCompare compare)
{
    std::vector<FrameGen3> frames;

    uint32_t first = (hp | (atk << 5) | (def << 10)) << 16;
    uint32_t second = (spe | (spa << 5) | (spd << 10)) << 16;
    std::vector<uint32_t> seeds = euclidean.RecoverLower16BitsIV(first, second);
    int size = seeds.size();

    // Need to setup Naturelock checking
    for (int i = 0; i < size; i += 2)
    {
        // Setup normal frame
        frame.SetIVsManual(hp, atk, def, spa, spd, spe);
        rng.seed = seeds[i + 1];
        frame.SetPID(rng.Next16Bit(), rng.Next16Bit());
        frame.seed = seeds[i] * 0xB9B33155 + 0xA170F641;
        if (compare.CompareFramePID(frame))
            frames.push_back(frame);

        // Setup XORed frame
        frame.pid ^= 0x80008000;
        frame.nature = frame.pid % 25;
        if (compare.CompareFramePID(frame))
        {
            frame.seed ^= 0x80000000;
            frames.push_back(frame);
        }
    }
    return frames;
}

// Return vector of frames for Method XDColo
std::vector<FrameGen3> SearcherGen3::searchMethodXDColo(uint32_t hp, uint32_t atk, uint32_t def, uint32_t spa, uint32_t spd, uint32_t spe, FrameCompare compare)
{
    std::vector<FrameGen3> frames;

    uint32_t first = (hp | (atk << 5) | (def << 10)) << 16;
    uint32_t second = (spe | (spa << 5) | (spd << 10)) << 16;
    std::vector<uint32_t> seeds = euclidean.RecoverLower16BitsIV(first, second);
    int size = seeds.size();

    for (int i = 0; i < size; i += 2)
    {
        // Setup normal frame
        frame.SetIVsManual(hp, atk, def, spa, spd, spe);
        rng.seed = seeds[i + 1];
        frame.SetPID(rng.Next16Bit(), rng.Next16Bit());
        frame.seed = seeds[i] * 0xB9B33155 + 0xA170F641;
        if (compare.CompareFramePID(frame))
            frames.push_back(frame);

        // Setup XORed frame
        frame.pid ^= 0x80008000;
        frame.nature = frame.pid % 25;
        if (compare.CompareFramePID(frame))
        {
            frame.seed ^= 0x80000000;
            frames.push_back(frame);
        }
    }
    return frames;
}

// Returns vector of frames for Method 1
std::vector<FrameGen3> SearcherGen3::searchMethod1(uint32_t hp, uint32_t atk, uint32_t def, uint32_t spa, uint32_t spd, uint32_t spe, FrameCompare compare)
{
    std::vector<FrameGen3> frames;

    uint32_t first = (hp | (atk << 5) | (def << 10)) << 16;
    uint32_t second = (spe | (spa << 5) | (spd << 10)) << 16;
    std::vector<uint32_t> seeds = cache.RecoverLower16BitsIV(first, second);
    int size = seeds.size();

    for (int i = 0; i < size; i++)
    {
        // Setup normal frame
        frame.SetIVsManual(hp, atk, def, spa, spd, spe);
        rng.seed = seeds[i];
        frame.SetPID(rng.Prev16Bit(), rng.Prev16Bit());
        frame.seed = rng.Prev32Bit();
        if (compare.CompareFramePID(frame))
            frames.push_back(frame);

        // Setup XORed frame
        frame.pid ^= 0x80008000;
        frame.nature = frame.pid % 25;
        if (compare.CompareFramePID(frame))
        {
            frame.seed ^= 0x80000000;
            frames.push_back(frame);
        }
    }
    return frames;
}

// Returns vector of frames for Method 2
std::vector<FrameGen3> SearcherGen3::searchMethod2(uint32_t hp, uint32_t atk, uint32_t def, uint32_t spa, uint32_t spd, uint32_t spe, FrameCompare compare)
{
    std::vector<FrameGen3> frames;

    uint32_t first = (hp | (atk << 5) | (def << 10)) << 16;
    uint32_t second = (spe | (spa << 5) | (spd << 10)) << 16;
    std::vector<uint32_t> seeds = cache.RecoverLower16BitsIV(first, second);
    int size = seeds.size();

    for (int i = 0; i < size; i++)
    {
        // Setup normal frame
        frame.SetIVsManual(hp, atk, def, spa, spd, spe);
        rng.seed = seeds[i];
        rng.ReverseFrames(1);
        frame.SetPID(rng.Prev16Bit(), rng.Prev16Bit());
        frame.seed = rng.Prev32Bit();
        if (compare.CompareFramePID(frame))
            frames.push_back(frame);

        // Setup XORed frame
        frame.pid ^= 0x80008000;
        frame.nature = frame.pid % 25;
        if (compare.CompareFramePID(frame))
        {
            frame.seed ^= 0x80000000;
            frames.push_back(frame);
        }
    }
    return frames;
}

// Returns vector of frames for Method 4
std::vector<FrameGen3> SearcherGen3::searchMethod4(uint32_t hp, uint32_t atk, uint32_t def, uint32_t spa, uint32_t spd, uint32_t spe, FrameCompare compare)
{
    std::vector<FrameGen3> frames;

    uint32_t first = (hp | (atk << 5) | (def << 10)) << 16;
    uint32_t second = (spe | (spa << 5) | (spd << 10)) << 16;
    std::vector<uint32_t> seeds = cache.RecoverLower16BitsIV(first, second);
    int size = seeds.size();

    for (int i = 0; i < size; i++)
    {
        // Setup normal frame
        frame.SetIVsManual(hp, atk, def, spa, spd, spe);
        rng.seed = seeds[i];
        frame.SetPID(rng.Prev16Bit(), rng.Prev16Bit());
        frame.seed = rng.Prev32Bit();
        if (compare.CompareFramePID(frame))
            frames.push_back(frame);

        // Setup XORed frame
        frame.pid ^= 0x80008000;
        frame.nature = frame.pid % 25;
        if (compare.CompareFramePID(frame))
        {
            frame.seed ^= 0x80000000;
            frames.push_back(frame);
        }
    }
    return frames;
}

// Determines which generational method to return
std::vector<FrameGen3> SearcherGen3::Search(uint32_t hp, uint32_t atk, uint32_t def, uint32_t spa, uint32_t spd, uint32_t spe, FrameCompare compare)
{
    if (frameType == XDColo || frameType == Channel || frameType == XD || frameType == Colo)
        rng = XDRNG(0);
    else
        rng = PokeRNG(0);

    switch (frameType)
    {
        case Method1:
            cache.SwitchCache(Method1);
            return searchMethod1(hp, atk, def, spa, spd, spe, compare);
        case Method2:
            cache.SwitchCache(Method2);
            return searchMethod2(hp, atk, def, spa, spd, spe, compare);
        case Method4:
            cache.SwitchCache(Method4);
            return searchMethod4(hp, atk, def, spa, spd, spe, compare);
        case MethodH1:
            cache.SwitchCache(Method1);
            return searchMethodH1(hp, atk, def, spa, spd, spe, compare);
        case MethodH2:
            cache.SwitchCache(Method2);
            return searchMethodH2(hp, atk, def, spa, spd, spe, compare);
        case MethodH4:
            cache.SwitchCache(Method4);
            return searchMethodH4(hp, atk, def, spa, spd, spe, compare);
        case Colo:
            euclidean.SwitchEuclidean(Colo);
            return searchMethodColo(hp, atk, def, spa, spd, spe, compare);
        case XD:
            euclidean.SwitchEuclidean(XD);
            return searchMethodXD(hp, atk, def, spa, spd, spe, compare);
        case XDColo:
            euclidean.SwitchEuclidean(XDColo);
            return searchMethodXDColo(hp, atk, def, spa, spd, spe, compare);
        // case Channel:
        // Set to default to avoid compiler warning message
        default:
            euclidean.SwitchEuclidean(Channel);
            return searchMethodChannel(hp, atk, def, spa, spd, spe, compare);
    }
}

// Switches cache or euclidean to user defined method
void SearcherGen3::SetMethod(Method frameType)
{
    switch (frameType)
    {
        case Method1:
        case MethodH1:
            cache.SwitchCache(Method1);
            break;
        case Method2:
        case MethodH2:
            cache.SwitchCache(Method2);
            break;
        case Method4:
        case MethodH4:
            cache.SwitchCache(Method4);
            break;
        case Colo:
        case XD:
        case XDColo:
            euclidean.SwitchEuclidean(XDColo);
            break;
        // case Channel:
        // Set to default to avoid compiler warning message
        default:
            euclidean.SwitchEuclidean(Channel);
            break;
    }
}
