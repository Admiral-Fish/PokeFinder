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

#include "GameCubeSearcher.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Enum/ShadowType.hpp>
#include <Core/RNG/LCRNG.hpp>

GameCubeSearcher::GameCubeSearcher(u16 tid, u16 sid, u8 genderRatio, Method method, const FrameFilter &filter) :
    Searcher(tid, sid, genderRatio, method, filter), euclidean(method), searching(false), progress(0)
{
}

void GameCubeSearcher::startSearch(const QVector<u8> &min, const QVector<u8> &max)
{
    searching = true;

    if (method == Method::Channel)
    {
        searchChannel(min.at(4), max.at(4));
        return;
    }

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

                            QVector<GameCubeFrame> frames = search(hp, atk, def, spa, spd, spe);

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

void GameCubeSearcher::cancelSearch()
{
    searching = false;
}

QVector<GameCubeFrame> GameCubeSearcher::getResults()
{
    std::lock_guard<std::mutex> guard(mutex);
    auto data(results);
    results.clear();
    return data;
}

int GameCubeSearcher::getProgress() const
{
    return progress;
}

void GameCubeSearcher::setupNatureLock(u8 num)
{
    lock = ShadowLock(num, method);
    type = lock.getType();
}

QVector<GameCubeFrame> GameCubeSearcher::search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe)
{
    switch (method)
    {
    case Method::XDColo:
        return searchXDColo(hp, atk, def, spa, spd, spe);
    case Method::XD:
    case Method::Colo:
        return searchXDColoShadow(hp, atk, def, spa, spd, spe);
    default:
        return QVector<GameCubeFrame>();
    }
}

QVector<GameCubeFrame> GameCubeSearcher::searchXDColo(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe)
{
    QVector<GameCubeFrame> frames;

    GameCubeFrame frame;
    frame.setIVs(hp, atk, def, spa, spd, spe);
    frame.calculateHiddenPower();
    if (!filter.compareHiddenPower(frame))
    {
        return frames;
    }

    QVector<QPair<u32, u32>> seeds = euclidean.recoverLower16BitsIV(hp, atk, def, spa, spd, spe);
    for (const auto &pair : seeds)
    {
        // Setup normal frame
        XDRNG rng(pair.second);
        rng.advanceFrames(1);

        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();

        frame.setSeed(pair.first * 0xB9B33155 + 0xA170F641);
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
        if (filter.comparePID(frame))
        {
            frame.setSeed(frame.getSeed() ^ 0x80000000);
            frames.append(frame);
        }
    }
    return frames;
}

QVector<GameCubeFrame> GameCubeSearcher::searchXDColoShadow(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe)
{
    QVector<GameCubeFrame> frames;

    GameCubeFrame frame;
    frame.setIVs(hp, atk, def, spa, spd, spe);
    frame.calculateHiddenPower();
    if (!filter.compareHiddenPower(frame))
    {
        return frames;
    }

    QVector<QPair<u32, u32>> seeds = euclidean.recoverLower16BitsIV(hp, atk, def, spa, spd, spe);
    for (const auto &pair : seeds)
    {
        // Setup normal frame
        XDRNG rng(pair.second);
        rng.advanceFrames(1);

        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();

        frame.setSeed(pair.first * 0xB9B33155 + 0xA170F641);
        frame.setPID(high, low);
        frame.setAbility(low & 1);
        frame.setGender(low & 255, genderRatio);
        frame.setNature(frame.getPID() % 25);
        frame.setShiny(tsv, high ^ low, 8);

        if (filter.comparePID(frame))
        {
            switch (type)
            {
            case ShadowType::SingleLock:
                if (lock.singleNL(frame.getSeed()))
                {
                    frame.setInfo(0);
                    frames.append(frame); // If this seed passes it is impossible for the sister spread to generate
                    continue;
                }
                break;
            case ShadowType::FirstShadow:
                if (lock.firstShadowNormal(frame.getSeed()))
                {
                    frame.setInfo(0);
                    frames.append(frame); // If this seed passes it is impossible for the sister spread to generate
                    continue;
                }
                break;
            case ShadowType::SecondShadow:
                if (lock.firstShadowUnset(frame.getSeed()))
                {
                    frame.setInfo(1); // Also unlikely for the other methods of encounter to pass
                    frames.append(frame); // If this seed passes it is impossible for the sister spread to generate
                    continue;
                }
                if (lock.firstShadowSet(frame.getSeed()))
                {
                    frame.setInfo(2); // Also unlikely for the other methods of encounter to pass
                    frames.append(frame); // If this seed passes it is impossible for the sister spread to generate
                    continue;
                }
                if (lock.firstShadowShinySkip(frame.getSeed()))
                {
                    frame.setInfo(3); // Also unlikely for the other methods of encounter to pass
                    frames.append(frame); // If this seed passes it is impossible for the sister spread to generate
                    continue;
                }
                break;
            case ShadowType::Salamence:
                if (lock.salamenceUnset(frame.getSeed()))
                {
                    frame.setInfo(1); // Also unlikely for the other methods of encounter to pass
                    frames.append(frame); // If this seed passes it is impossible for the sister spread to generate
                    continue;
                }
                if (lock.salamenceSet(frame.getSeed()))
                {
                    frame.setInfo(2); // Also unlikely for the other methods of encounter to pass
                    frames.append(frame); // If this seed passes it is impossible for the sister spread to generate
                    continue;
                }
                if (lock.salamenceShinySkip(frame.getSeed()))
                {
                    frame.setInfo(3); // Also unlikely for the other methods of encounter to pass
                    frames.append(frame); // If this seed passes it is impossible for the sister spread to generate
                    continue;
                }
                break;
            case ShadowType::EReader:
                if (lock.eReader(frame.getSeed(), frame.getPID()))
                {
                    frame.setInfo(0);
                    frames.append(frame); // If this seed passes it is impossible for the sister spread to generate
                    continue;
                }
                break;
            }
        }

        // Setup XORed frame
        frame.setSeed(frame.getSeed() ^ 0x80000000);
        frame.setPID(frame.getPID() ^ 0x80008000);
        frame.setNature(frame.getPID() % 25);

        if (filter.comparePID(frame))
        {
            switch (type)
            {
            case ShadowType::SingleLock:
                if (lock.singleNL(frame.getSeed()))
                {
                    frame.setInfo(0);
                    frames.append(frame);
                }
                break;
            case ShadowType::FirstShadow:
                if (lock.firstShadowNormal(frame.getSeed()))
                {
                    frame.setInfo(0);
                    frames.append(frame);
                }
                break;
            case ShadowType::SecondShadow:
                if (lock.firstShadowUnset(frame.getSeed()))
                {
                    frame.setInfo(1);
                    frames.append(frame);
                }
                else if (lock.firstShadowSet(frame.getSeed()))
                {
                    frame.setInfo(2);
                    frames.append(frame);
                }
                else if (lock.firstShadowShinySkip(frame.getSeed()))
                {
                    frame.setInfo(3);
                    frames.append(frame);
                }
                break;
            case ShadowType::Salamence:
                if (lock.salamenceUnset(frame.getSeed()))
                {
                    frame.setInfo(1);
                    frames.append(frame);
                }
                else if (lock.salamenceSet(frame.getSeed()))
                {
                    frame.setInfo(2);
                    frames.append(frame);
                }
                else if (lock.salamenceShinySkip(frame.getSeed()))
                {
                    frame.setInfo(3);
                    frames.append(frame);
                }
                break;
            case ShadowType::EReader:
                if (lock.eReader(frame.getSeed(), frame.getPID()))
                {
                    frame.setInfo(0);
                    frames.append(frame); // If this seed passes it is impossible for the sister spread to generate
                    continue;
                }
                break;
            }
        }
    }
    return frames;
}

void GameCubeSearcher::searchChannel(u8 minSpD, u8 maxSpD)
{
    for (u8 spd = minSpD; spd <= maxSpD; spd++)
    {
        u32 lower = static_cast<u32>(spd << 27);
        u32 upper = lower | 0x7ffffff;

        for (u64 seed = lower; seed <= upper; seed++, progress++)
        {
            if (!searching)
            {
                return;
            }

            XDRNGR rng(static_cast<u32>(seed));

            u8 spa = rng.nextUShort() >> 11;
            u8 spe = rng.nextUShort() >> 11;
            u8 def = rng.nextUShort() >> 11;
            u8 atk = rng.nextUShort() >> 11;
            u8 hp = rng.nextUShort() >> 11;

            GameCubeFrame frame;
            frame.setIVs(hp, atk, def, spa, spd, spe);
            frame.calculateHiddenPower();
            if (!filter.compareIVs(frame))
            {
                continue;
            }

            rng.advanceFrames(3);

            // Calculate PID
            u16 low = rng.nextUShort();
            u16 high = rng.nextUShort();
            u16 sid = rng.nextUShort();

            // Determine if PID needs to be XORed
            if ((low > 7 ? 0 : 1) != (high ^ sid ^ 40122))
            {
                high ^= 0x8000;
            }

            frame.setPID(high, low);
            frame.setAbility(low & 1);
            frame.setGender(low & 255, genderRatio);
            frame.setNature(frame.getPID() % 25);
            frame.setShiny(40122 ^ sid, high ^ low, 8);

            u32 originSeed = rng.nextUInt();
            if (filter.comparePID(frame) && validateJirachi(originSeed))
            {
                frame.setSeed(originSeed);

                std::lock_guard<std::mutex> lock(mutex);
                results.append(frame);
            }
        }
    }
}

bool GameCubeSearcher::validateJirachi(u32 seed)
{
    XDRNGR rng(seed);

    u16 num1 = rng.nextUShort();
    u16 num2 = rng.nextUShort();
    u16 num3 = rng.nextUShort();

    rng.advanceFrames(3);
    if (num1 <= 0x4000) // 6 advances
    {
        if (validateMenu(rng.getSeed()))
        {
            return true;
        }
    }

    rng.advanceFrames(1);
    if (num2 > 0x4000 && num1 <= 0x547a) // 7 advances
    {
        if (validateMenu(rng.getSeed()))
        {
            return true;
        }
    }

    rng.advanceFrames(1);
    if (num3 > 0x4000 && num2 > 0x547a) // 8 advances
    {
        if (validateMenu(rng.getSeed()))
        {
            return true;
        }
    }

    return false;
}

bool GameCubeSearcher::validateMenu(u32 seed)
{
    u8 target = seed >> 30;

    if (target == 0)
    {
        return false;
    }

    XDRNGR rng(seed);
    u8 mask = static_cast<u8>(1 << target);
    while ((mask & 14) != 14)
    {
        u8 num = rng.nextUInt() >> 30;
        if (num == target)
        {
            return false;
        }

        mask |= 1 << num;
    }

    return true;
}
