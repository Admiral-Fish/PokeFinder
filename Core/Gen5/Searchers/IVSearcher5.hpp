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

#ifndef IVSEARCHER5_HPP
#define IVSEARCHER5_HPP

#include <Core/Enum/Buttons.hpp>
#include <Core/Gen5/SHA1Cache.hpp>
#include <Core/Gen5/Searchers/SearcherBase5.hpp>
#include <fph/meta_fph_table.h>

/**
 * @brief Parent searcher class for Static/Wild Gen 5 generators
 *
 * @tparam Generator Generator class to use
 * @tparam State State class to use
 */
template <class Generator, class State>
class IVSearcher5 final : public SearcherBase5<Generator, State>
{
public:
    /**
     * @brief Construct a new IVSearcher5 object
     *
     * @param initialAdvances Minimum IV advances
     * @param maxAdvances Maximum IV advances
     * @param generator State generator
     * @param profile Profile information
     */
    IVSearcher5(u32 initialAdvances, u32 maxAdvances, const Generator &generator, const Profile5 &profile) :
        SearcherBase5<Generator, State>(generator, profile), initialAdvances(initialAdvances), maxAdvances(maxAdvances)
    {
    }

private:
    u32 initialAdvances;
    u32 maxAdvances;

    /**
     * @brief Searches between the \p start and \p end dates
     *
     * @param start Start date
     * @param end End date
     */
    void search(const Date &start, const Date &end) override
    {
#ifdef SIMD_X86
        if (hasAVX2())
        {
            SHA1AVX2 sha(this->profile);
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
                        for (u32 time = 0; time < 86400; time += 8)
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
        else
#endif
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
    }
};

/**
 * @brief Parent searcher class for Static/Wild Gen 5 generators
 *
 * @tparam Generator Generator class to use
 * @tparam State State class to use
 */
template <class Generator, class State>
class IVSearcher5Fast final : public SearcherBase5<Generator, State>
{
public:
    /**
     * @brief Construct a new IVSearcher5Fast object
     *
     * @param initialAdvances Minimum IV advances
     * @param maxAdvances Maximum IV advances
     * @param ivCache Fast search IV cache
     * @param generator State generator
     * @param profile Profile information
     */
    IVSearcher5Fast(u32 initialAdvances, u32 maxAdvances, const fph::MetaFphMap<u64, std::array<u8, 6>> &ivCache,
                    const Generator &generator, const Profile5 &profile) :
        SearcherBase5<Generator, State>(generator, profile), ivCache(ivCache), initialAdvances(initialAdvances), maxAdvances(maxAdvances)
    {
    }

private:
    fph::MetaFphMap<u64, std::array<u8, 6>> ivCache;
    u32 initialAdvances;
    u32 maxAdvances;

    /**
     * @brief Searches between the \p start and \p end dates
     *
     * @param start Start date
     * @param end End date
     */
    void search(const Date &start, const Date &end) override
    {
#ifdef SIMD_X86
        if (hasAVX2())
        {
            SHA1AVX2 sha(this->profile);
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
                        for (u32 time = 0; time < 86400; time += 8)
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
        else
#endif
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
    }
};

/**
 * @brief Parent searcher class for Static/Wild Gen 5 generators
 *
 * @tparam Generator Generator class to use
 * @tparam State State class to use
 */
template <class Generator, class State>
class IVSearcher5CacheFast final : public SearcherBase5<Generator, State>
{
public:
    /**
     * @brief Construct a new IVSearcher5CacheFast object
     *
     * @param initialAdvances Minimum IV advances
     * @param maxAdvances Maximum IV advances
     * @param sha1Cache Fast search SHA1 cache
     * @param ivCache Fast search IV cache
     * @param generator State generator
     * @param profile Profile information
     */
    IVSearcher5CacheFast(u32 initialAdvances, u32 maxAdvances, const fph::MetaFphMap<u64, u64> &sha1Cache,
                         const fph::MetaFphMap<u64, std::array<u8, 6>> &ivCache, const Generator &generator, const Profile5 &profile) :
        SearcherBase5<Generator, State>(generator, profile),
        sha1Cache(sha1Cache),
        ivCache(ivCache),
        initialAdvances(initialAdvances),
        maxAdvances(maxAdvances)
    {
    }

private:
    fph::MetaFphMap<u64, u64> sha1Cache;
    fph::MetaFphMap<u64, std::array<u8, 6>> ivCache;
    u32 initialAdvances;
    u32 maxAdvances;

    /**
     * @brief Searches between the \p start and \p end dates
     *
     * @param start Start date
     * @param end End date
     */
    void search(const Date &start, const Date &end) override
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
};

#endif // IVSEARCHER5_HPP
