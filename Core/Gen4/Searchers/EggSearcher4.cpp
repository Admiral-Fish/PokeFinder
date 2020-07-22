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

#include "EggSearcher4.hpp"

EggSearcher4::EggSearcher4(u16 tid, u16 sid, u8 genderRatio, Method method, const StateFilter &filter) :
    Searcher(tid, sid, genderRatio, method, filter), searching(false), progress(0)
{
}

void EggSearcher4::setGenerators(const EggGenerator4 &generatorIV, const EggGenerator4 &generatorPID)
{
    this->generatorIV = generatorIV;
    this->generatorPID = generatorPID;
}

void EggSearcher4::setType(int type)
{
    this->type = type;
}

void EggSearcher4::startSearch(u32 minDelay, u32 maxDelay)
{
    searching = true;
    u16 total = 0;

    for (u16 ab = 0; ab < 256; ab++)
    {
        for (u8 cd = 0; cd < 24; cd++)
        {
            for (u32 efgh = minDelay; efgh <= maxDelay; efgh++)
            {
                if (!searching)
                {
                    return;
                }

                if (total > 10000)
                {
                    progress = static_cast<int>(256 * 24 * (maxDelay - minDelay + 1));
                    return;
                }

                QVector<EggState4> states;
                u32 seed = static_cast<u32>((ab << 24) | (cd << 16)) + efgh;

                if (type == 0)
                {
                    states = generatorIV.generate(seed);
                }
                else if (type == 1)
                {
                    states = generatorPID.generate(seed);
                }
                else
                {
                    auto statesIV = generatorIV.generate(seed);
                    auto statesPID = generatorPID.generate(seed);

                    if (!statesIV.isEmpty() && !statesPID.isEmpty())
                    {
                        for (auto currentStatePID : statesPID)
                        {
                            for (const auto &currentStateIV : statesIV)
                            {
                                currentStatePID.setIVs(currentStateIV.getIV(0), currentStateIV.getIV(1), currentStateIV.getIV(2),
                                                       currentStateIV.getIV(3), currentStateIV.getIV(4), currentStateIV.getIV(5));
                                for (u8 i = 0; i < 6; i++)
                                {
                                    currentStatePID.setInheritance(i, currentStateIV.getInheritance(i));
                                }
                                currentStatePID.setSecondaryAdvance(currentStateIV.getAdvance());

                                states.append(currentStatePID);
                            }
                        }
                    }
                }

                for (EggState4 &currentState : states)
                {
                    currentState.setInitialSeed(seed);
                }

                total += states.size();

                std::lock_guard<std::mutex> guard(mutex);
                results.append(states);
                progress++;
            }
        }
    }
}

void EggSearcher4::cancelSearch()
{
    searching = false;
}

QVector<EggState4> EggSearcher4::getResults()
{
    std::lock_guard<std::mutex> guard(mutex);
    auto data(results);
    results.clear();
    return data;
}

int EggSearcher4::getProgress() const
{
    return progress;
}
