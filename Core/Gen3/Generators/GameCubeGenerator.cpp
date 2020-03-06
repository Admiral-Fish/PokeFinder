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

#include "GameCubeGenerator.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Enum/ShadowType.hpp>
#include <Core/Parents/Filters/FrameFilter.hpp>

GameCubeGenerator::GameCubeGenerator(u32 initialFrame, u32 maxResults, u16 tid, u16 sid, u8 genderRatio, Method method,
                                     const FrameFilter &filter) :
    Generator(initialFrame, maxResults, tid, sid, genderRatio, method, filter)
{
}

QVector<GameCubeFrame> GameCubeGenerator::generate(u32 seed) const
{
    switch (method)
    {
    case Method::XDColo:
        return generateXDColo(seed);
    case Method::XD:
    case Method::Colo:
        return generateXDColoShadow(seed);
    case Method::Channel:
        return generateChannel(seed);
    default:
        return QVector<GameCubeFrame>();
    }
}

void GameCubeGenerator::setShadowTeam(u8 index, u8 type)
{
    team = ShadowTeam::loadShadowTeams(method).at(index);
    this->type = type;
}

QVector<GameCubeFrame> GameCubeGenerator::generateXDColo(u32 seed) const
{
    QVector<GameCubeFrame> frames;

    XDRNG rng(seed);
    rng.advanceFrames(initialFrame - 1 + offset);

    // Method XD/Colo [SEED] [IVS] [IVS] [BLANK] [PID] [PID]

    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.nextUInt())
    {
        GameCubeFrame frame(initialFrame + cnt);
        XDRNG go(rng.getSeed());

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();
        go.nextUInt();
        u16 high = go.nextUShort();
        u16 low = go.nextUShort();

        frame.setPID(high, low);
        frame.setAbility(low & 1);
        frame.setGender(low & 255, genderRatio);
        frame.setNature(frame.getPID() % 25);
        frame.setShiny(tsv, high ^ low, 8);

        frame.setIVs(iv1, iv2);
        frame.calculateHiddenPower();

        if (filter.compareFrame(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}

QVector<GameCubeFrame> GameCubeGenerator::generateXDColoShadow(u32 seed) const
{
    QVector<GameCubeFrame> frames;

    XDRNG rng(seed);
    rng.advanceFrames(initialFrame - 1 + offset);

    for (u32 cnt = 0; cnt < maxResults; cnt++)
    {
        GameCubeFrame frame(initialFrame + cnt);

        XDRNG go(rng.nextUInt());
        go.advanceFrames(4);

        generateNonShadows(go);

        u16 iv1 = go.nextUShort();
        u16 iv2 = go.nextUShort();
        frame.setIVs(iv1, iv2);
        frame.calculateHiddenPower();

        go.nextUInt();

        u16 high = go.nextUShort();
        u16 low = go.nextUShort();
        if (method == Method::XD) // Shiny lock gales
        {
            while (tsv == ((high ^ low) >> 3))
            {
                high = go.nextUShort();
                low = go.nextUShort();
            }
            frame.setShiny(false);
        }
        else
        {
            frame.setShiny(tsv, high ^ low, 8);
        }

        frame.setPID(high, low);
        frame.setAbility(low & 1);
        frame.setGender(low & 255, genderRatio);
        frame.setNature(frame.getPID() % 25);

        if (filter.compareFrame(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}

QVector<GameCubeFrame> GameCubeGenerator::generateChannel(u32 seed) const
{
    QVector<GameCubeFrame> frames;

    XDRNG rng(seed);
    rng.advanceFrames(initialFrame - 1 + offset);

    // Method Channel [SEED] [SID] [PID] [PID] [BERRY] [GAME ORIGIN] [OT GENDER] [IV] [IV] [IV] [IV] [IV] [IV]

    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.nextUInt())
    {
        GameCubeFrame frame(initialFrame + cnt);
        XDRNG go(rng.getSeed());

        // u16 tid = 40122;
        u16 sid = go.nextUShort();
        u16 high = go.nextUShort();
        u16 low = go.nextUShort();
        go.advanceFrames(3);

        // u16 berry = go.nextUShort() >> 13; If >= 4 salac, else ganlon
        // u16 game = go.nextUShort() >> 12; If >= 8 ruby, else sapphire
        // u16 gender = go.nextUShort() >> 11; If >= 16 female, else male

        if ((low > 7 ? 0 : 1) != (high ^ 40122 ^ sid))
        {
            high ^= 0x8000;
        }

        frame.setPID(high, low);
        frame.setAbility(low & 1);
        frame.setGender(low & 255, genderRatio);
        frame.setNature(frame.getPID() % 25);
        frame.setShiny(40122 ^ sid, high ^ low, 8);

        u8 hp = go.nextUShort() >> 11;
        u8 atk = go.nextUShort() >> 11;
        u8 def = go.nextUShort() >> 11;
        u8 spe = go.nextUShort() >> 11;
        u8 spa = go.nextUShort() >> 11;
        u8 spd = go.nextUShort() >> 11;

        frame.setIVs(hp, atk, def, spa, spd, spe);
        frame.calculateHiddenPower();

        if (filter.compareFrame(frame))
        {
            frames.append(frame);
        }
    }

    return frames;
}

void GameCubeGenerator::generateNonShadows(XDRNG &rng) const
{
    for (auto i = team.getSize() - 1; i >= 0; i--)
    {
        rng.advanceFrames(3);
        u32 pid;
        do
        {
            u16 high = rng.nextUShort();
            u16 low = rng.nextUShort();
            pid = (high << 16) | low;
        } while (!team.getLock(i).compare(pid));
    }

    switch (team.getType())
    {
    case ShadowType::SingleLock:
    case ShadowType::FirstShadow:
        rng.advanceFrames(2);
        break;
    case ShadowType::SecondShadow:
    case ShadowType::Salamence:
        switch (type)
        {
        case 0: // Set
            rng.advanceFrames(7);
            break;
        case 1: // Unset
            rng.advanceFrames(9);
            break;
        case 2: // Shinyskip
            rng.advanceFrames(5);
            u16 psv = (rng.nextUShort() ^ rng.nextUShort()) >> 3;
            u16 psvTemp = (rng.nextUShort() ^ rng.nextUShort()) >> 3;
            while (psv == psvTemp)
            {
                psvTemp = psv;
                psv = (rng.nextUShort() ^ rng.nextUShort()) >> 3;
            }
            rng.advanceFrames(2);
            break;
        }
        break;
    case ShadowType::EReader:
        // Unconsume calls for IVs/PID for shadow
        XDRNGR backward(rng.getSeed());
        backward.advanceFrames(5);

        rng.setSeed(backward.getSeed());
        break;
    }
}
