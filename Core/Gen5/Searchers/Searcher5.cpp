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

#include "Searcher5.hpp"
#include <Core/RNG/SHA1.hpp>

template <class Generator, class State>
Searcher5<Generator, State>::Searcher5(const Generator &generator, const Profile5 &profile) :
    SearcherBase5<Generator, State>(generator, profile)
{
}

template <class Generator, class State>
void Searcher5<Generator, State>::search(const Date &start, const Date &end)
{
    SHA1SSE sha(this->profile);
    for (u16 timer0 = this->profile.getTimer0Min(); timer0 <= this->profile.getTimer0Max(); timer0++)
    {
        sha.setTimer0(timer0, this->profile.getVCount());
        for (Date date = start; date <= end; ++date)
        {
            sha.setDate(date);
            auto alpha = sha.precompute();
            for (const auto &keypress : this->keypresses)
            {
                sha.setButton(keypress.value);

                for (u32 time = 0; time < 86400; time += 4)
                {
                    if (!this->searching)
                    {
                        return;
                    }

                    sha.setTime(time, this->profile.getDSType());
                    auto seeds = sha.hashSeed(alpha);

                    for (u32 i = 0; i < seeds.size(); i++)
                    {
                        auto states = this->generator.generate(seeds[i]);
                        if (!states.empty())
                        {
                            DateTime dt(date, time + i);

                            std::lock_guard<std::mutex> lock(this->mutex);
                            this->results.reserve(this->results.capacity() + states.size());
                            for (const auto &state : states)
                            {
                                this->results.emplace_back(dt, seeds[i], keypress.button, timer0, state);
                            }
                        }
                    }
                }
                this->progress++;
            }
        }
    }
}

#include <Core/Gen5/Generators/DreamRadarGenerator.hpp>
#include <Core/Gen5/Generators/EggGenerator5.hpp>
#include <Core/Gen5/Generators/EventGenerator5.hpp>
#include <Core/Gen5/Generators/HiddenGrottoGenerator.hpp>
#include <Core/Gen5/Generators/IDGenerator5.hpp>
#include <Core/Parents/States/IDState.hpp>

template class Searcher5<DreamRadarGenerator, DreamRadarState>;
template class Searcher5<EventGenerator5, EventState5>;
template class Searcher5<EggGenerator5, EggState5>;
template class Searcher5<HiddenGrottoSlotGenerator, HiddenGrottoState>;
template class Searcher5<IDGenerator5, IDState>;
