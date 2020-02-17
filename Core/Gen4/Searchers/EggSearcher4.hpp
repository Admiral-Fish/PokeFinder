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

#ifndef EGGSEARCHER4_HPP
#define EGGSEARCHER4_HPP

#include <Core/Gen4/Generators/EggGenerator4.hpp>
#include <Core/Parents/Searchers/Searcher.hpp>
#include <mutex>

class EggSearcher4 : public Searcher
{
public:
    EggSearcher4() = default;
    EggSearcher4(u16 tid, u16 sid, u8 genderRatio, Method method, const FrameFilter &filter);
    void setGenerators(const EggGenerator4 &generatorIV, const EggGenerator4 &generatorPID);
    void setType(int type);
    void startSearch(u32 minDelay, u32 maxDelay);
    void cancelSearch();
    QVector<EggFrame4> getResults();
    int getProgress() const;

private:
    EggGenerator4 generatorIV;
    EggGenerator4 generatorPID;
    int type;

    bool searching;
    int progress;
    QVector<EggFrame4> results;
    std::mutex mutex;
};

#endif // EGGSEARCHER4_HPP
