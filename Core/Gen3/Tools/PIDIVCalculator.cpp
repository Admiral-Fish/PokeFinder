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

#include "PIDIVCalculator.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/RNG/RNGCache.hpp>
#include <Core/RNG/RNGEuclidean.hpp>

namespace PIDIVCalculator
{
    namespace
    {
        void setIVs(PIDIVFrame &frame, u16 iv1, u16 iv2)
        {
            frame.setIV(0, iv1 & 0x1f);
            frame.setIV(1, (iv1 >> 5) & 0x1f);
            frame.setIV(2, (iv1 >> 10) & 0x1f);
            frame.setIV(3, (iv2 >> 5) & 0x1f);
            frame.setIV(4, (iv2 >> 10) & 0x1f);
            frame.setIV(5, iv2 & 0x1f);
        }

        QVector<PIDIVFrame> calcMethod124(u32 pid)
        {
            QVector<PIDIVFrame> frames;

            RNGCache cache(Method::Method1);

            auto seeds = cache.recoverLower16BitsPID(pid);
            for (const u32 seed : seeds)
            {
                PokeRNGR backward(seed);
                u32 originSeed = backward.nextUInt();

                PokeRNG forward(seed);
                forward.advanceFrames(1);

                u16 iv1 = forward.nextUShort(); // Method 1/4
                u16 iv2 = forward.nextUShort(); // Method 1/2
                u16 iv3 = forward.nextUShort(); // Method 2/4

                PIDIVFrame frame1(originSeed, Method::Method1);
                PIDIVFrame frame2(originSeed, Method::Method2);
                PIDIVFrame frame3(originSeed, Method::Method4);

                setIVs(frame1, iv1, iv2);
                setIVs(frame2, iv2, iv3);
                setIVs(frame3, iv1, iv3);

                frames.append({ frame1, frame2, frame3 });
            }

            return frames;
        }

        QVector<PIDIVFrame> calcMethodXDColo(u32 pid)
        {
            QVector<PIDIVFrame> frames;

            RNGEuclidean euclidean(Method::XDColo);

            QVector<QPair<u32, u32>> seeds = euclidean.recoverLower16BitsPID(pid);
            for (const auto &pair : seeds)
            {
                XDRNGR backward(pair.first);
                backward.advanceFrames(1);

                u16 iv2 = backward.nextUShort();
                u16 iv1 = backward.nextUShort();
                u32 seed = backward.nextUInt();

                PIDIVFrame frame(seed, Method::XDColo);

                setIVs(frame, iv1, iv2);

                frames.append(frame);
            }

            return frames;
        }

        QVector<PIDIVFrame> calcMethodChannel(u32 pid)
        {
            QVector<PIDIVFrame> frames;

            RNGEuclidean euclidean(Method::XDColo);

            // Whether PID is xored or unxored is determined by SID which we don't know by only providing a PID
            // So we have to check both xored and unxored and recalculate the PID to see if we have a match

            QVector<QPair<u32, u32>> seeds = euclidean.recoverLower16BitsPID(pid);
            for (const auto &pair : seeds)
            {
                XDRNGR backward(pair.first);
                u16 sid = backward.nextUShort();
                u32 seed = backward.nextUInt();

                XDRNG forward(seed);
                forward.advanceFrames(1);

                u16 high = forward.nextUShort();
                u16 low = forward.nextUShort();

                if ((low > 7 ? 0 : 1) != (high ^ 40122 ^ sid))
                {
                    high ^= 0x8000;
                }

                u32 val = static_cast<u32>((high << 16) | low);
                if (val == pid) // PID matches based on SID
                {
                    forward.advanceFrames(3);

                    PIDIVFrame frame(seed, Method::Channel);

                    for (u8 i : { 0, 1, 2, 5, 3, 4 })
                    {
                        frame.setIV(i, forward.nextUInt() >> 27);
                    }

                    frames.append(frame);
                }
            }

            QVector<QPair<u32, u32>> seedsXOR = euclidean.recoverLower16BitsPID(pid ^ 0x80000000);
            for (const auto &pair : seedsXOR)
            {
                XDRNGR backward(pair.first);
                u16 sid = backward.nextUShort();
                u32 seed = backward.nextUInt();

                XDRNG forward(seed);
                forward.advanceFrames(1);

                u16 high = forward.nextUShort();
                u16 low = forward.nextUShort();

                if ((low > 7 ? 0 : 1) != (high ^ 40122 ^ sid))
                {
                    high ^= 0x8000;
                }

                u32 val = static_cast<u32>((high << 16) | low);
                if (val == pid) // PID matches based on SID
                {
                    forward.advanceFrames(3);

                    PIDIVFrame frame(seed, Method::Channel);

                    for (u8 i : { 0, 1, 2, 5, 3, 4 })
                    {
                        frame.setIV(i, forward.nextUInt() >> 27);
                    }

                    frames.append(frame);
                }
            }

            return frames;
        }
    }

    QVector<PIDIVFrame> calculateIVs(u32 pid)
    {
        QVector<PIDIVFrame> frames;

        frames.append(calcMethod124(pid));
        frames.append(calcMethodXDColo(pid));
        frames.append(calcMethodChannel(pid));

        return frames;
    }
}
