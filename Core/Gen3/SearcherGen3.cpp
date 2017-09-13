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

SearcherGen3::SearcherGen3()
{
    tid = 12345;
    sid = 54321;
    psv = tid ^ sid;
    frame.SetIDs(tid, sid, psv);
}

SearcherGen3::SearcherGen3(uint32_t tid, uint32_t sid)
{
    this->tid = tid;
    this->sid = sid;
    psv = tid ^ sid;
    frame.SetIDs(tid, sid, psv);
}

std::vector<FrameGen3> SearcherGen3::searchMethodChannel(uint32_t hp, uint32_t atk, uint32_t def, uint32_t spa, uint32_t spd, uint32_t spe)
{
    std::vector<FrameGen3> frames;

    std::vector<uint32_t> seeds = euclidean.RecoverLower27BitsChannel(hp, atk, def, spa, spd, spe);
    int size = seeds.size();

    for (int i = 0; i < size; i++)
    {
        frame.SetIVsManual(hp, atk, def, spa, spd, spe);
        rng.SetSeed(seeds[i]);
        rng.ReverseFrames(3);
        uint32_t pid2 = rng.Prev16Bit();
        uint32_t pid1 = rng.Prev16Bit();
        if ((pid2 > 7 ? 0 : 1) != (pid1 ^ rng.Prev16Bit() ^ 40122))
            pid1 ^= 0x8000;
        frame.SetPID(pid1, pid2);
        frame.seed = rng.Prev16Bit();
        frames.push_back(frame);
    }
    return frames;
}

std::vector<FrameGen3> SearcherGen3::searchMethodColo(uint32_t hp, uint32_t atk, uint32_t def, uint32_t spa, uint32_t spd, uint32_t spe)
{
    std::vector<FrameGen3> frames;

    return frames;
}

std::vector<FrameGen3> SearcherGen3::searchMethodH1(uint32_t hp, uint32_t atk, uint32_t def, uint32_t spa, uint32_t spd, uint32_t spe)
{
    std::vector<FrameGen3> frames;

    return frames;
}

std::vector<FrameGen3> SearcherGen3::searchMethodH2(uint32_t hp, uint32_t atk, uint32_t def, uint32_t spa, uint32_t spd, uint32_t spe)
{
    std::vector<FrameGen3> frames;

    return frames;
}

std::vector<FrameGen3> SearcherGen3::searchMethodH4(uint32_t hp, uint32_t atk, uint32_t def, uint32_t spa, uint32_t spd, uint32_t spe)
{
    std::vector<FrameGen3> frames;

    return frames;
}

std::vector<FrameGen3> SearcherGen3::searchMethodXD(uint32_t hp, uint32_t atk, uint32_t def, uint32_t spa, uint32_t spd, uint32_t spe)
{
    std::vector<FrameGen3> frames;

    return frames;
}

std::vector<FrameGen3> SearcherGen3::searchMethodXDColo(uint32_t hp, uint32_t atk, uint32_t def, uint32_t spa, uint32_t spd, uint32_t spe)
{
    std::vector<FrameGen3> frames;

    uint32_t first = (hp | (atk << 5) | (def << 10)) << 16;
    uint32_t second = (spe | (spa << 5) | (spd << 10)) << 16;
    std::vector<uint32_t> seeds = euclidean.RecoverLower16BitsIV(first, second);
    int size = seeds.size();

    for (int i = 0; i < size; i += 2)
    {
        frame.SetIVsManual(hp, atk, def, spa, spd, spe);
        rng.SetSeed(seeds[i + 1]);
        frame.SetPID(rng.Next16Bit(), rng.Next16Bit());
        frame.seed = seeds[i] * 0xB9B33155 + 0xA170F641;
        frames.push_back(frame);
    }
    return frames;
}

std::vector<FrameGen3> SearcherGen3::searchMethod1(uint32_t hp, uint32_t atk, uint32_t def, uint32_t spa, uint32_t spd, uint32_t spe)
{
    std::vector<FrameGen3> frames;

    return frames;
}

std::vector<FrameGen3> SearcherGen3::searchMethod2(uint32_t hp, uint32_t atk, uint32_t def, uint32_t spa, uint32_t spd, uint32_t spe)
{
    std::vector<FrameGen3> frames;

    return frames;
}

std::vector<FrameGen3> SearcherGen3::searchMethod4(uint32_t hp, uint32_t atk, uint32_t def, uint32_t spa, uint32_t spd, uint32_t spe)
{
    std::vector<FrameGen3> frames;

    return frames;
}

std::vector<FrameGen3> SearcherGen3::Search(uint32_t hp, uint32_t atk, uint32_t def, uint32_t spa, uint32_t spd, uint32_t spe)
{
    if (frameType == XDColo || frameType == Channel)
        rng.SetXDRNG();
    else
        rng.SetPokeRNG();

    switch (frameType)
    {
        case Method1:
            cache.SwitchCache(Method1);
            return searchMethod1(hp, atk, def, spa, spd, spe);
        case Method2:
            cache.SwitchCache(Method2);
            return searchMethod2(hp, atk, def, spa, spd, spe);
        case Method4:
            cache.SwitchCache(Method4);
            return searchMethod4(hp, atk, def, spa, spd, spe);
        case MethodH1:
            cache.SwitchCache(Method1);
            return searchMethodH1(hp, atk, def, spa, spd, spe);
        case MethodH2:
            cache.SwitchCache(Method2);
            return searchMethodH2(hp, atk, def, spa, spd, spe);
        case MethodH4:
            cache.SwitchCache(Method4);
            return searchMethodH4(hp, atk, def, spa, spd, spe);
        case Colo:
            euclidean.SwitchEuclidean(Colo);
            return searchMethodColo(hp, atk, def, spa, spd, spe);
        case XD:
            euclidean.SwitchEuclidean(XD);
            return searchMethodXD(hp, atk, def, spa, spd, spe);
        case XDColo:
            euclidean.SwitchEuclidean(XDColo);
            return searchMethodXDColo(hp, atk, def, spa, spd, spe);
        // case Channel:
        // Set to default to avoid compiler warning message
        default:
            euclidean.SwitchEuclidean(Channel);
            return searchMethodChannel(hp, atk, def, spa, spd, spe);
    }
}

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
