/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2019 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "ChainedSIDCalc.hpp"

ChainedSIDCalc::ChainedSIDCalc(u16 tid)
{
    this->tid = tid;

    for (u32 i = 0; i <= 0xFFFF; i += 8)
    {
        sids.append(i);
    }
}

void ChainedSIDCalc::addEntry(const QVector<u8> &ivs, u8 nature, u8 ability, u8 gender)
{
    QVector<QPair<u32, u32>> pids;

    u32 iv1 = ivs[0] | (ivs[1] << 5) | (ivs[2] << 10);
    u32 iv2 = ivs[5] | (ivs[3] << 5) | (ivs[4] << 10);

    RNGCache cache(Method::Method1);
    auto seeds = cache.recoverLower16BitsIV(iv1 << 16, iv2 << 16);

    for (const auto seed : seeds)
    {
        u32 adjust = 0;
        PokeRNGR rng = PokeRNGR(seed);

        for (u8 i = 0; i < 13; i++)
        {
            adjust |= ((rng.nextUShort() & 1) << (15 - i));
        }

        u16 pid2 = rng.nextUShort();
        u16 pid1 = rng.nextUShort();

        u32 adjustLow = adjust | (pid1 & 7);

        u8 abilityNum = adjustLow & 1;
        u8 genderNum = adjustLow & 0xFF;

        if ((ability == 0 || (abilityNum == 0 && ability == 1) || (abilityNum == 1 && ability == 2)) && matchGender(gender, genderNum))
        {
            pids.append(qMakePair(adjustLow, pid2));
        }
    }

    QVector<u16> newSids;
    for (const auto sid : sids)
    {
        for (const auto pair : pids)
        {
            u32 adjustHigh = pair.first ^ tid ^ sid;
            adjustHigh &= 0xFFF8;
            adjustHigh += (pair.second & 7);

            u32 pid = (adjustHigh << 16) | pair.first;
            if ((pid % 25) == nature)
            {
                newSids.append(sid);
            }
        }
    }
    sids = newSids;
}

QVector<u16> ChainedSIDCalc::getSIDs() const
{
    return sids;
}

bool ChainedSIDCalc::matchGender(u8 gender, u8 val) const
{
    switch (gender)
    {
        case 0: // Genderless
            return true;
        case 1: // Male (50 M / 50 F)
            return val >= 127 && val <= 255;
        case 2: // Female (50 M / 50 F)
            return val >= 0 && val <= 126;
        case 3: // Male (25 M / 75 F)
            return val >= 191 && val <= 255;
        case 4: // Female (25 M / 75 F)
            return val >= 0 && val <= 190;
        case 5: // Male (75 M / 25 F)
            return val >= 64 && val <= 255;
        case 6: // Female (75 M / 25 F)
            return val >= 0 && val <= 63;
        case 7: // Male (87.5 M / 12.5 F)
            return val >= 31 && val <= 255;
        case 8: // Female (87.5 < / 12.5 F)
            return val >= 0 && val <= 30;
        case 9: // Male only
            return true;
        case 10: // Female only
            return true;
    }
}
