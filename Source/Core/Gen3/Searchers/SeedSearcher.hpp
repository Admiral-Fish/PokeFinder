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

#ifndef SEEDSEARCHER_HPP
#define SEEDSEARCHER_HPP

#include <Core/Util/Global.hpp>
#include <atomic>
#include <mutex>
#include <vector>

class SeedSearcher
{
public:
    explicit SeedSearcher(const std::vector<u32> &criteria);
    std::vector<u32> getInitialSeeds();
    void cancelSearch();
    std::vector<u32> getResults() const;
    virtual int getProgress() const;

protected:
    std::vector<u32> results;
    std::vector<u32> criteria;
    bool searching;
    std::atomic<u32> progress;
    std::mutex mutex;
};

#endif // SEEDSEARCHER_HPP
