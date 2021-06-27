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

#ifndef STATIONARYSEARCHER5_HPP
#define STATIONARYSEARCHER5_HPP

#include <Core/Gen5/Generators/StationaryGenerator5.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/States/StationaryState5.hpp>
#include <atomic>
#include <mutex>
#include <unordered_map>

class StationarySearcher5
{
public:
    StationarySearcher5() = default;
    explicit StationarySearcher5(const StationaryGenerator5 &ivGenerator, const StationaryGenerator5 &pidGenerator, const Profile5 &profile,
                                 const std::vector<std::unordered_map<u32, u32>> &ivMap, bool includePID);
    void startSearch(int threads, Date start, const Date &end);
    void cancelSearch();
    std::vector<StationaryState5> getResults();
    int getProgress() const;

private:
    // TODO: pass these to startSearch
    StationaryGenerator5 ivGenerator;
    StationaryGenerator5 pidGenerator;
    Profile5 profile;
    std::vector<std::unordered_map<u32, u32>> ivMap;
    bool includePID;
    bool fastSearch;

    bool searching;
    std::atomic<int> progress;
    std::vector<StationaryState5> results;
    std::mutex mutex;

    void search(const Date &start, const Date &end);
};

#endif // STATIONARYSEARCHER5_HPP
