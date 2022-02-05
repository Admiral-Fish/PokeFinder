/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef EGGGENERATOR3_HPP
#define EGGGENERATOR3_HPP

#include <Core/Parents/Generators/EggGenerator.hpp>

class EggState3;

class EggGenerator3 : public EggGenerator
{
public:
    EggGenerator3() = default;
    EggGenerator3(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 genderRatio, Method method, const StateFilter &filter,
                  const Daycare &daycare);
    std::vector<EggState3> generate(u32 seed = 0, u32 seed2 = 0) const;
    void setInitialAdvancesPickup(u32 value);
    void setMaxAdvancesPickup(u32 value);
    void setCalibration(u8 value);
    void setMinRedraw(u8 value);
    void setMaxRedraw(u8 value);
    void setCompatability(u8 value);

private:
    u32 initialAdvancesPickup;
    u32 maxAdvancesPickup;
    u8 calibration;
    u8 minRedraw;
    u8 maxRedraw;
    u8 compatability;
    u8 iv1, iv2, inh;

    std::vector<EggState3> generateEmeraldPID() const;
    std::vector<EggState3> generateEmeraldIVs() const;
    std::vector<std::pair<u32, u16>> generateLower(u32 seed) const;
    std::vector<EggState3> generateUpper(u32 seed, const std::vector<std::pair<u32, u16>> &lower) const;
};

#endif // EGGGENERATOR3_HPP
