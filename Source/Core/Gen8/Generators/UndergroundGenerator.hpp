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

#ifndef UNDERGROUNDGENERATOR_HPP
#define UNDERGROUNDGENERATOR_HPP

#include <Core/Gen8/Filters/StateFilter8.hpp>
#include <Core/Parents/Generators/StaticGenerator.hpp>

class EncounterArea8;
enum class Lead : u8;

class UndergroundGenerator : public StaticGenerator<UndergroundStateFilter>
{
public:
    UndergroundGenerator(u32 initialAdvances, u32 maxAdvances, u32 offset, u16 tid, u16 sid, Game version, Lead lead, u8 randMarkId,
                         u8 storyFlag, bool bonus, const UndergroundStateFilter &filter);

    std::vector<UndergroundState> generate(u64 seed0, u64 seed1, const EncounterArea8 &area) const;

protected:
    bool bonus;
    u8 randMarkId;
    u8 storyFlag;
};

#endif // UNDERGROUNDGENERATOR_HPP
