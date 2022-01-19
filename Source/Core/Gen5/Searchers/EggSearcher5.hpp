/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2022 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef EGGSEARCHER5_HPP
#define EGGSEARCHER5_HPP

#include <Core/Gen5/Profile5.hpp>
#include <Core/Util/Global.hpp>
#include <atomic>
#include <mutex>

class Date;
class EggGenerator5;
class EggState;
template <class StateType>
class SearcherState5;

class EggSearcher5
{
public:
    EggSearcher5() = default;
    explicit EggSearcher5(const Profile5 &profile);
    void startSearch(const EggGenerator5 &generator, int threads, const Date &start, const Date &end);
    void cancelSearch();
    std::vector<SearcherState5<EggState>> getResults();
    int getProgress() const;

private:
    Profile5 profile;

    bool searching;
    std::atomic<int> progress;
    std::vector<SearcherState5<EggState>> results;
    std::mutex mutex;

    void search(EggGenerator5 generator, const Date &start, const Date &end);
};

#endif // EGGSEARCHER5_HPP
