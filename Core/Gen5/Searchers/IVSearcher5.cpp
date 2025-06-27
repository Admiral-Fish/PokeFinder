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

#include "IVSearcher5.hpp"
#include <Core/Enum/Buttons.hpp>
#include <Core/Gen5/SHA1Cache.hpp>
#include <Core/RNG/SHA1.hpp>

template <class Generator, class State>
IVSearcher5<Generator, State>::IVSearcher5(u32 initialAdvances, u32 maxAdvances, const Generator &generator, const Profile5 &profile) :
    SearcherBase5<Generator, State>(generator, profile), initialAdvances(initialAdvances), maxAdvances(maxAdvances)
{
}

template <class Generator, class State>
void IVSearcher5<Generator, State>::search(const Date &start, const Date &end)
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
                        auto states = this->generator.generate(seeds[i], initialAdvances, maxAdvances);
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

template <class Generator, class State>
IVSearcher5Fast<Generator, State>::IVSearcher5Fast(u32 initialAdvances, u32 maxAdvances,
                                                   const fph::MetaFphMap<u64, std::array<u8, 6>> &ivCache, const Generator &generator,
                                                   const Profile5 &profile) :
    SearcherBase5<Generator, State>(generator, profile), ivCache(ivCache), initialAdvances(initialAdvances), maxAdvances(maxAdvances)
{
}

template <class Generator, class State>
void IVSearcher5Fast<Generator, State>::search(const Date &start, const Date &end)
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
                        for (u64 j = initialAdvances; j <= (initialAdvances + maxAdvances); j++)
                        {
                            const auto entry = ivCache.find((j << 32) | (seeds[i] >> 32));
                            if (entry == ivCache.end())
                            {
                                continue;
                            }

                            auto states = this->generator.generate(seeds[i], { { j, entry->second } });
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
                }
                this->progress++;
            }
        }
    }
}

template <class Generator, class State>
IVSearcher5CacheFast<Generator, State>::IVSearcher5CacheFast(u32 initialAdvances, u32 maxAdvances,
                                                             const fph::MetaFphMap<u64, u64> &sha1Cache,
                                                             const fph::MetaFphMap<u64, std::array<u8, 6>> &ivCache,
                                                             const Generator &generator, const Profile5 &profile) :
    SearcherBase5<Generator, State>(generator, profile),
    sha1Cache(sha1Cache),
    ivCache(ivCache),
    initialAdvances(initialAdvances),
    maxAdvances(maxAdvances)
{
}

template <class Generator, class State>
void IVSearcher5CacheFast<Generator, State>::search(const Date &start, const Date &end)
{
    SHA1Key key;
    for (u16 timer0 = this->profile.getTimer0Min(); timer0 <= this->profile.getTimer0Max(); timer0++)
    {
        key.timer0 = timer0;
        for (Date date = start; date <= end; ++date)
        {
            key.date = date.getJD() - Date().getJD();
            for (const auto &keypress : this->keypresses)
            {
                key.button = toInt(keypress.button);
                for (u32 time = 0; time < 86400; time++)
                {
                    if (!this->searching)
                    {
                        return;
                    }

                    key.time = time;

                    const auto sha1Entry = sha1Cache.find(key.key);
                    if (sha1Entry == sha1Cache.end())
                    {
                        continue;
                    }

                    u64 seed = sha1Entry->second;
                    for (u64 j = initialAdvances; j <= (initialAdvances + maxAdvances); j++)
                    {
                        const auto ivEntry = ivCache.find((j << 32) | (seed >> 32));
                        if (ivEntry == ivCache.end())
                        {
                            continue;
                        }

                        auto states = this->generator.generate(seed, { { j, ivEntry->second } });
                        if (!states.empty())
                        {
                            DateTime dt(date, time);

                            std::lock_guard<std::mutex> lock(this->mutex);
                            this->results.reserve(this->results.capacity() + states.size());
                            for (const auto &state : states)
                            {
                                this->results.emplace_back(dt, seed, keypress.button, timer0, state);
                            }
                        }
                    }
                }
                this->progress++;
            }
        }
    }
}

#include <Core/Gen5/Generators/HiddenGrottoGenerator.hpp>
#include <Core/Gen5/Generators/StaticGenerator5.hpp>
#include <Core/Gen5/Generators/WildGenerator5.hpp>

template class IVSearcher5<HiddenGrottoGenerator, State5>;
template class IVSearcher5Fast<HiddenGrottoGenerator, State5>;
template class IVSearcher5CacheFast<HiddenGrottoGenerator, State5>;

template class IVSearcher5<StaticGenerator5, State5>;
template class IVSearcher5Fast<StaticGenerator5, State5>;
template class IVSearcher5CacheFast<StaticGenerator5, State5>;

template class IVSearcher5<WildGenerator5, WildState5>;
template class IVSearcher5Fast<WildGenerator5, WildState5>;
template class IVSearcher5CacheFast<WildGenerator5, WildState5>;
