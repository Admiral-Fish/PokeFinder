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

#include "GameCubeRTCState.hpp"

GameCubeRTCState::GameCubeRTCState(const QDateTime &dateTime, u32 seed, u32 advances) : dateTime(dateTime), seed(seed), advances(advances)
{
}

QString GameCubeRTCState::getDateTime() const
{
    return dateTime.toString();
}

u32 GameCubeRTCState::getSeed() const
{
    return seed;
}

u32 GameCubeRTCState::getAdvances() const
{
    return advances;
}
