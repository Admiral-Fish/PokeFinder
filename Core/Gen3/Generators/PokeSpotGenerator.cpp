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

#include "PokeSpotGenerator.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Parents/Filters/FrameFilter.hpp>
#include <Core/RNG/LCRNG.hpp>

PokeSpotGenerator::PokeSpotGenerator(u32 initialFrame, u32 maxResults, u16 tid, u16 sid, u8 genderRatio, const FrameFilter &filter) :
    Generator(initialFrame, maxResults, tid, sid, genderRatio, Method::XDColo, filter)
{
}

void PokeSpotGenerator::setSpots(const QVector<bool> &spots)
{
    this->spots = spots;
}

QVector<GameCubeFrame> PokeSpotGenerator::generate(u32 seed) const
{
    QVector<GameCubeFrame> frames;

    XDRNG rng(seed);
    rng.advanceFrames(initialFrame - 1);

    for (u32 cnt = 0; cnt < maxResults; cnt++, rng.nextUInt())
    {
        XDRNG go(rng.getSeed());

        // Check if frame is a valid pokespot call
        if ((go.nextUShort() % 3) == 0)
        {
            // Munchlax provides a frame skip
            if ((go.nextUShort() % 100) >= 10)
            {
                GameCubeFrame frame(initialFrame + cnt);

                // Check what type the pokespot is
                u8 call = go.nextUShort() % 100;
                if (call < 50)
                {
                    if (!spots.at(0))
                    {
                        continue;
                    }
                    frame.setInfo(0);
                }
                else if (call > 49 && call < 85)
                {
                    if (!spots.at(1))
                    {
                        continue;
                    }
                    frame.setInfo(1);
                }
                else
                {
                    if (!spots.at(2))
                    {
                        continue;
                    }
                    frame.setInfo(2);
                }

                u16 high = go.nextUShort();
                u16 low = go.nextUShort();

                frame.setPID(high, low);
                frame.setAbility(low & 1);
                frame.setGender(low & 255, genderRatio);
                frame.setNature(frame.getPID() % 25);
                frame.setShiny(tsv, high ^ low, 8);

                if (filter.comparePID(frame))
                {
                    frames.append(frame);
                }
            }
        }
    }

    return frames;
}
