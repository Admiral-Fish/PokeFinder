/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef ADJACENTSEEDCALCULATOR_HPP
#define ADJACENTSEEDCALCULATOR_HPP

#include <Core/Enum/Buttons.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Global.hpp>
#include <Core/Util/DateTime.hpp>
#include <array>
#include <vector>

enum class AdjacentSeedMethod : u8
{
    Standard,
    Roamer
};

class AdjacentSeedState
{
public:
    AdjacentSeedState(u64 seed, const DateTime &dateTime, Buttons buttons, u16 timer0, u32 ivAdvance, const std::array<u8, 6> &ivs,
                      u32 pidAdvance, bool target);

    Buttons getButtons() const;
    const DateTime &getDateTime() const;
    u8 getIV(u8 index) const;
    u32 getIVAdvance() const;
    u32 getPIDAdvance() const;
    u64 getSeed() const;
    u16 getTimer0() const;
    bool isTarget() const;

private:
    u64 seed;
    DateTime dateTime;
    Buttons buttons;
    u16 timer0;
    u32 ivAdvance;
    std::array<u8, 6> ivs;
    u32 pidAdvance;
    bool target;
};

struct AdjacentSeedSettings
{
    Profile5 profile;
    DateTime dateTime;
    Buttons buttons;
    u32 seconds;
    u32 minIVAdvance;
    u32 maxIVAdvance;
    AdjacentSeedMethod method;
};

namespace AdjacentSeedCalculator
{
    std::vector<AdjacentSeedState> generate(const AdjacentSeedSettings &settings);
    std::vector<u32> previewPRNG(u64 seed, const Profile5 &profile, u32 pidAdvance, u32 count);
}

#endif // ADJACENTSEEDCALCULATOR_HPP
