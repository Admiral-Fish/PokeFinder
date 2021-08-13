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

#include "PokeWalkerSearcher.hpp"
#include <Core/Gen4/Generators/PokeWalkerGenerator.hpp>

PokeWalkerSearcher::PokeWalkerSearcher(u16 tid, u16 sid, u8 genderRatio, Method method, const StateFilter &filter) :
    Searcher(tid, sid, genderRatio, method, filter), searching(false), progress(0)
{
}

void PokeWalkerSearcher::startSearch(u32 minDelay, u32 maxDelay, int type, const PokeWalkerGenerator &generatorIV, const PokeWalkerGenerator &generatorPID)
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

                std::vector<PokeWalkerState> states;
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

                    if (!statesIV.empty() && !statesPID.empty())
                    {
                        for (auto statePID : statesPID)
                        {
                            for (const auto &stateIV : statesIV)
                            {
                                statePID.setIVs(stateIV.getIV(0), stateIV.getIV(1), stateIV.getIV(2), stateIV.getIV(3), stateIV.getIV(4),
                                                stateIV.getIV(5));
                                statePID.calculateHiddenPower();
                                statePID.setSecondaryAdvance(stateIV.getAdvances());

                                states.emplace_back(statePID);
                            }
                        }
                    }
                }

                for (PokeWalkerState &state : states)
                {
                    state.setInitialSeed(seed);
                }

                total += states.size();

                std::lock_guard<std::mutex> guard(mutex);
                results.insert(results.end(), states.begin(), states.end());
                progress++;
            }
        }
    }
}

void PokeWalkerSearcher::cancelSearch()
{
    searching = false;
}

std::vector<PokeWalkerState> PokeWalkerSearcher::getResults()
{
    std::lock_guard<std::mutex> guard(mutex);
    auto data = std::move(results);
    return data;
}

int PokeWalkerSearcher::getProgress() const
{
    return progress;
}
