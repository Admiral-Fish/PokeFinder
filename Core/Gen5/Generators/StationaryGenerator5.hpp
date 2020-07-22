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

#ifndef GENERATOR5_HPP
#define GENERATOR5_HPP

#include <Core/Parents/Generators/StationaryGenerator.hpp>
#include <Core/Parents/States/StationaryState.hpp>

enum Encounter : u8;

class StationaryGenerator5 : public StationaryGenerator
{
public:
    StationaryGenerator5() = default;
    StationaryGenerator5(u32 initialAdvances, u32 maxResults, u16 tid, u16 sid, u8 genderRatio, Method method, Encounter encounter,
                         const StateFilter &filter);
    QVector<StationaryState> generate(u64 seed) const;

private:
    u8 idBit;
    Encounter encounter;

    QVector<StationaryState> generateRoamerIVs(u64 seed) const;
    QVector<StationaryState> generateIVs(u64 seed) const;
    QVector<StationaryState> generateRoamerCGear(u64 seed) const;
    QVector<StationaryState> generateCGear(u64 seed) const;
    QVector<StationaryState> generateStationary(u64 seed) const;
    QVector<StationaryState> generateRoamer(u64 seed);
    QVector<StationaryState> generateGift(u64 seed);
    QVector<StationaryState> generateEntraLink(u64 seed);
    QVector<StationaryState> generateLarvestaEgg(u64 seed);
    QVector<StationaryState> generateHiddenGrotto(u64 seed);
};

#endif // GENERATOR5_HPP
