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

#ifndef GAMECUBEGENERATOR_HPP
#define GAMECUBEGENERATOR_HPP

#include <Core/Gen3/LockInfo.hpp>
#include <Core/Gen3/ShadowTeam.hpp>
#include <Core/Gen3/States/GameCubeState.hpp>
#include <Core/Parents/Generators/Generator.hpp>

class GameCubeGenerator : public Generator
{
public:
    GameCubeGenerator() = default;
    GameCubeGenerator(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 genderRatio, Method method, const StateFilter &filter);
    std::vector<GameCubeState> generate(u32 seed) const;
    void setShadowTeam(u8 index, u8 type);

private:
    ShadowTeam team;
    u8 type;

    std::vector<GameCubeState> generateXDColo(u32 seed) const;
    std::vector<GameCubeState> generateAgeto(u32 seed) const;
    std::vector<GameCubeState> generateAgeto0Difference(u32 seed) const;
    std::vector<GameCubeState> generateXDShadow(u32 seed) const;
    std::vector<GameCubeState> generateColoShadow(u32 seed) const;
    std::vector<GameCubeState> generateChannel(u32 seed) const;
    std::vector<GameCubeState> generatePossibleChannel(u32 seed) const;
    bool validateJirachi(u32 seed) const;
    bool validateMenu(u32 seed) const;
};

#endif // GAMECUBEGENERATOR_HPP
