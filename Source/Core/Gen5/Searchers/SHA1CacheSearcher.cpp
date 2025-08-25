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

#include "SHA1CacheSearcher.hpp"
#include <Core/Enum/Buttons.hpp>
#include <Core/Enum/DSType.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Language.hpp>
#include <Core/Gen5/IVSeedCache.hpp>
#include <Core/RNG/SHA1.hpp>
#include <Core/Util/DateTime.hpp>
#include <fstream>

template <typename Type>
static void write(std::ofstream &file, Type val)
{
    file.write(reinterpret_cast<char *>(&val), sizeof(val));
}

SHA1CacheSearcher::SHA1CacheSearcher(const Profile5 &profile, const Date &start, const Date &end) :
    SearcherBase<SHA1Seed>(),
    profile(profile),
    keypresses(Keypresses::getKeypresses(profile)),
    entralinkSeeds(IVSeedCache::getSeeds(profile.getVersion(), CacheType::Entralink)),
    normalSeeds(IVSeedCache::getSeeds(profile.getVersion(), CacheType::Normal)),
    roamerSeeds(IVSeedCache::getSeeds(profile.getVersion(), CacheType::Roamer)),
    end(end),
    start(start)
{
}

void SHA1CacheSearcher::startSearch(int threads)
{
    this->searching = true;

    auto days = start.daysTo(end) + 1;
    if (days < threads)
    {
        threads = days;
    }

    auto *threadContainer = new std::thread[threads];

    auto daysSplit = days / threads;
    Date day = start;
    for (int i = 0; i < threads; i++, day += daysSplit)
    {
        if (i == threads - 1)
        {
            threadContainer[i] = std::thread([=] { search(day, end); });
        }
        else
        {
            Date mid = day + (daysSplit - 1);
            threadContainer[i] = std::thread([=] { search(day, mid); });
        }
    }

    for (int i = 0; i < threads; i++)
    {
        threadContainer[i].join();
    }

    delete[] threadContainer;
}

void SHA1CacheSearcher::writeResults(const std::string &file)
{
    auto sort = [](const SHA1Seed &first, const SHA1Seed &second) { return first.seed < second.seed; };

    std::ofstream stream(file.data(), std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
    if (stream.is_open())
    {
        write(stream, profile.getMac());
        write(stream, end);
        write(stream, start);
        write(stream, profile.getVersion());
        write<u32>(stream, results.size());
        write<u32>(stream, normalResults.size());
        write<u32>(stream, roamerResults.size());
        write(stream, profile.getTimer0Max());
        write(stream, profile.getTimer0Min());
        write(stream, profile.getSoftReset());
        write(stream, profile.getDSType());
        write(stream, profile.getLanguage());
        write(stream, profile.getKeypresses());
        write(stream, profile.getGxStat());
        write(stream, profile.getVCount());
        write(stream, profile.getVFrame());

        std::sort(results.begin(), results.end(), sort);
        std::sort(normalResults.begin(), normalResults.end(), sort);
        std::sort(roamerResults.begin(), roamerResults.end(), sort);

        stream.write(reinterpret_cast<char *>(results.data()), results.size() * sizeof(SHA1Seed));
        stream.write(reinterpret_cast<char *>(normalResults.data()), normalResults.size() * sizeof(SHA1Seed));
        stream.write(reinterpret_cast<char *>(roamerResults.data()), roamerResults.size() * sizeof(SHA1Seed));
    }
}

void SHA1CacheSearcher::search(const Date &start, const Date &end)
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
                            if (std::binary_search(entralinkSeeds.begin(), entralinkSeeds.end(), seeds[i] >> 32))
                            {
                                std::lock_guard<std::mutex> lock(this->mutex);
                                this->results.emplace_back(toInt(keypress.button), time + i, date.getJD() - Date().getJD(), timer0,
                                                           seeds[i]);
                            }

                            if (std::binary_search(normalSeeds.begin(), normalSeeds.end(), seeds[i] >> 32))
                            {
                                std::lock_guard<std::mutex> lock(this->mutex);
                                this->normalResults.emplace_back(toInt(keypress.button), time + i, date.getJD() - Date().getJD(), timer0,
                                                                 seeds[i]);
                            }

                            if (std::binary_search(roamerSeeds.begin(), roamerSeeds.end(), seeds[i] >> 32))
                            {
                                std::lock_guard<std::mutex> lock(this->mutex);
                                this->roamerResults.emplace_back(toInt(keypress.button), time + i, date.getJD() - Date().getJD(), timer0,
                                                                 seeds[i]);
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
                            if (std::binary_search(entralinkSeeds.begin(), entralinkSeeds.end(), seeds[i] >> 32))
                            {
                                std::lock_guard<std::mutex> lock(this->mutex);
                                this->results.emplace_back(toInt(keypress.button), time + i, date.getJD() - Date().getJD(), timer0,
                                                           seeds[i]);
                            }

                            if (std::binary_search(normalSeeds.begin(), normalSeeds.end(), seeds[i] >> 32))
                            {
                                std::lock_guard<std::mutex> lock(this->mutex);
                                this->normalResults.emplace_back(toInt(keypress.button), time + i, date.getJD() - Date().getJD(), timer0,
                                                                 seeds[i]);
                            }

                            if (std::binary_search(roamerSeeds.begin(), roamerSeeds.end(), seeds[i] >> 32))
                            {
                                std::lock_guard<std::mutex> lock(this->mutex);
                                this->roamerResults.emplace_back(toInt(keypress.button), time + i, date.getJD() - Date().getJD(), timer0,
                                                                 seeds[i]);
                            }
                        }
                    }
                    this->progress++;
                }
            }
        }
    }
}
