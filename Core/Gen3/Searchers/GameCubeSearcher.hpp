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

#ifndef GAMECUBESEARCHER_HPP
#define GAMECUBESEARCHER_HPP

#include <Core/Gen3/Frames/GameCubeFrame.hpp>
#include <Core/Gen3/ShadowLock.hpp>
#include <Core/Parents/Searchers/Searcher.hpp>
#include <Core/RNG/RNGEuclidean.hpp>
#include <mutex>

class GameCubeSearcher : public Searcher
{
public:
    GameCubeSearcher() = default;
    GameCubeSearcher(u16 tid, u16 sid, u8 genderRatio, Method method, const FrameFilter &filter);
    void startSearch(const QVector<u8> &min, const QVector<u8> &max);
    void cancelSearch();
    QVector<GameCubeFrame> getResults();
    int getProgress() const;
    void setupNatureLock(u8 num);

private:
    RNGEuclidean euclidean;
    ShadowLock lock;
    ShadowType type;

    bool searching;
    int progress;
    QVector<GameCubeFrame> results;
    std::mutex mutex;

    QVector<GameCubeFrame> search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe);
    QVector<GameCubeFrame> searchXDColo(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe);
    QVector<GameCubeFrame> searchXDColoShadow(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe);
    void searchChannel(u8 minSpD, u8 maxSpD);
    bool validateJirachi(u32 seed);
    bool validateMenu(u32 seed);
};

#endif // GAMECUBESEARCHER_HPP
