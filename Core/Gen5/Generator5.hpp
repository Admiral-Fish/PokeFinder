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

#ifndef GENERATOR5_HPP
#define GENERATOR5_HPP

#include <Core/Gen5/Frame5.hpp>
#include <Core/Parents/Generator.hpp>
#include <Core/RNG/LCRNG64.hpp>
#include <Core/RNG/MTRNG.hpp>

class Generator5 : public Generator
{

public:
    Generator5();
    Generator5(u32 maxResults, u32 initialFrame, u32 initialSeed, u16 tid, u16 sid, u32 offset, Method type);
    //void setEncounter(const EncounterArea5 &value);
    QVector<Frame5> generate(const FrameCompare &compare) const;

private:
    //EncounterArea5 encounter;

    QVector<Frame5> generateRoamerIVs(const FrameCompare &compare) const;
    QVector<Frame5> generateIVs(const FrameCompare &compare) const;
    QVector<Frame5> generateRoamerCGear(const FrameCompare &compare) const;
    QVector<Frame5> generateCGear(const FrameCompare &compare) const;

    QVector<Frame5> generateStationary(const FrameCompare &compare) const;

};

#endif // GENERATOR5_HPP
