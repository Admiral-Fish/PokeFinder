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

#include "StationarySearcher3.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/RNG/LCRNG.hpp>

StationarySearcher3::StationarySearcher3(u16 tid, u16 sid, u8 genderRatio, Method method, const FrameFilter &filter) :
    StationarySearcher(tid, sid, genderRatio, method, filter),
    cache(method),
    ivAdvance(method == Method::Method2 ? 1 : 0),
    searching(false),
    progress(0)
{
}

void StationarySearcher3::startSearch(const QVector<u8> &min, const QVector<u8> &max)
{
    searching = true;

    for (u8 hp = min.at(0); hp <= max.at(0); hp++)
    {
        for (u8 atk = min.at(1); atk <= max.at(1); atk++)
        {
            for (u8 def = min.at(2); def <= max.at(2); def++)
            {
                for (u8 spa = min.at(3); spa <= max.at(3); spa++)
                {
                    for (u8 spd = min.at(4); spd <= max.at(4); spd++)
                    {
                        for (u8 spe = min.at(5); spe <= max.at(5); spe++)
                        {
                            if (!searching)
                            {
                                return;
                            }

                            auto frames = search(hp, atk, def, spa, spd, spe);

                            std::lock_guard<std::mutex> guard(mutex);
                            results.append(frames);
                            progress++;
                        }
                    }
                }
            }
        }
    }
}

void StationarySearcher3::cancelSearch()
{
    searching = false;
}

QVector<Frame> StationarySearcher3::getResults()
{
    std::lock_guard<std::mutex> guard(mutex);
    auto data(results);
    results.clear();
    return data;
}

int StationarySearcher3::getProgress() const
{
    return progress;
}

QVector<Frame> StationarySearcher3::search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    switch (method)
    {
    case Method::Method1:
    case Method::Method2:
    case Method::Method4:
        return searchMethod124(hp, atk, def, spa, spd, spe);
    case Method::Method1Reverse:
        return searchMethod1Reverse(hp, atk, def, spa, spd, spe);
    default:
        return QVector<Frame>();
    }
}

QVector<Frame> StationarySearcher3::searchMethod124(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    QVector<Frame> frames;
    Frame frame;

    frame.setIVs(hp, atk, def, spa, spd, spe);
    frame.calculateHiddenPower();

    if (!filter.compareHiddenPower(frame))
    {
        return frames;
    }

    auto seeds = cache.recoverLower16BitsIV(hp, atk, def, spa, spd, spe);
    for (const u32 seed : seeds)
    {
        // Setup normal frame
        PokeRNGR rng(seed);
        rng.advanceFrames(ivAdvance);

        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();

        frame.setSeed(rng.nextUInt());
        frame.setPID(high, low);
        frame.setAbility(low & 1);
        frame.setGender(low & 255, genderRatio);
        frame.setNature(frame.getPID() % 25);
        frame.setShiny(tsv, high ^ low, 8);

        if (filter.comparePID(frame))
        {
            frames.append(frame);
        }

        // Setup XORed frame
        frame.setSeed(frame.getSeed() ^ 0x80000000);
        frame.setPID(frame.getPID() ^ 0x80008000);
        frame.setNature(frame.getPID() % 25);
        if (filter.comparePID(frame))
        {
            frames.append(frame);
        }
    }
    return frames;
}

QVector<Frame> StationarySearcher3::searchMethod1Reverse(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const
{
    QVector<Frame> frames;
    Frame frame;

    frame.setIVs(hp, atk, def, spa, spd, spe);
    frame.calculateHiddenPower();

    if (!filter.compareHiddenPower(frame))
    {
        return frames;
    }

    auto seeds = cache.recoverLower16BitsIV(hp, atk, def, spa, spd, spe);
    for (const u32 seed : seeds)
    {
        // Setup normal frame
        PokeRNGR rng(seed);
        rng.advanceFrames(ivAdvance);

        u16 low = rng.nextUShort();
        u16 high = rng.nextUShort();

        frame.setSeed(rng.nextUInt());
        frame.setPID(high, low);
        frame.setAbility(low & 1);
        frame.setGender(low & 255, genderRatio);
        frame.setNature(frame.getPID() % 25);
        frame.setShiny(tsv, high ^ low, 8);

        if (filter.comparePID(frame))
        {
            frames.append(frame);
        }

        // Setup XORed frame
        frame.setPID(frame.getPID() ^ 0x80008000);
        frame.setNature(frame.getPID() % 25);
        frame.setSeed(frame.getSeed() ^ 0x80000000);
        if (filter.comparePID(frame))
        {
            frames.append(frame);
        }
    }
    return frames;
}
