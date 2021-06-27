/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef GAMECUBESEARCHER_HPP
#define GAMECUBESEARCHER_HPP

#include <Core/Gen3/ShadowLock.hpp>
#include <Core/Gen3/States/GameCubeState.hpp>
#include <Core/Parents/Searchers/Searcher.hpp>
#include <mutex>

class GameCubeSearcher : public Searcher
{
public:
    GameCubeSearcher() = default;
    GameCubeSearcher(u16 tid, u16 sid, u8 genderRatio, Method method, const StateFilter &filter);
    void startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max);
    void cancelSearch();
    std::vector<GameCubeState> getResults();
    int getProgress() const;
    void setupNatureLock(u8 num);

private:
    ShadowLock lock;
    ShadowType type;

    bool searching;
    int progress;
    std::vector<GameCubeState> results;
    std::mutex mutex;

    std::vector<GameCubeState> search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe);
    std::vector<GameCubeState> searchXDColo(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe);
    std::vector<GameCubeState> searchAgeto(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe);
    std::vector<GameCubeState> searchXDShadow(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe);
    std::vector<GameCubeState> searchColoShadow(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe);
    void searchChannel(u8 minSpD, u8 maxSpD);
    bool validateJirachi(u32 seed);
    bool validateMenu(u32 seed);
};

#endif // GAMECUBESEARCHER_HPP
