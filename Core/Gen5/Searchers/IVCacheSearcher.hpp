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

#ifndef IVCACHESEARCHER_HPP
#define IVCACHESEARCHER_HPP

#include <Core/Parents/Searchers/SearcherBase.hpp>
#include <string>

/**
 * @brief Searcher class for SHA1 cache
 */
class IVCacheSearcher final : public SearcherBase<std::vector<u32>>
{
public:
    /**
     * @brief Construct a new IVCacheSearcher object
     *
     * @param initialAdvances
     * @param maxAdvances Maximum number of advances
     */
    IVCacheSearcher(u32 initialAdvances, u32 maxAdvances);

    /**
     * @brief Starts the search
     *
     * @param threads Numbers of threads to search with
     * @param start Start date
     * @param end End date
     */
    void startSearch(int threads);

    /**
     * @brief Writes cache results to file
     *
     * @param file Path to write files too
     */
    void writeResults(const std::string &file);

private:
    std::vector<std::vector<u32>> entralink;
    std::vector<std::vector<u32>> roamer;
    u32 initialAdvances;
    u32 maxAdvances;

    /**
     * @brief Searches between the \p start and \p end seeds
     *
     * @param start Start seed
     * @param end End seed
     */
    void search(u32 start, u32 end);
};

#endif // IVCACHESEARCHER_HPP
