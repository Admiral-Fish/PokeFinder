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

#ifndef EGGGENERATOR4_HPP
#define EGGGENERATOR4_HPP

#include <Core/Gen4/Frames/EggFrame4.hpp>
#include <Core/Parents/Generators/EggGenerator.hpp>

class EggGenerator4 : public EggGenerator
{
public:
    EggGenerator4() = default;
    EggGenerator4(u32 initialFrame, u32 maxResults, u16 tid, u16 sid, u8 genderRatio, Method method, const FrameFilter &filter);
    QVector<EggFrame4> generate(u32 seed) const;

private:
    QVector<EggFrame4> generateNormal(u32 seed) const;
    QVector<EggFrame4> generateMasuada(u32 seed) const;
    QVector<EggFrame4> generateDPPtIVs(u32 seed) const;
    QVector<EggFrame4> generateHGSSIVs(u32 seed) const;
    void setInheritance(EggFrame4 &frame, const u16 *inh, const u16 *par, bool broken) const;
};

#endif // EGGGENERATOR4_HPP
