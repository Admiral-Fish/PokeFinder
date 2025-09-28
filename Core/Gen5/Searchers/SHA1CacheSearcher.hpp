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

#ifndef SHA1CACHESEARCHER_HPP
#define SHA1CACHESEARCHER_HPP

#include <Core/Gen5/Keypresses.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen5/SHA1Cache.hpp>
#include <Core/Parents/Searchers/SearcherBase.hpp>
#include <Core/Util/DateTime.hpp>

enum class DSType : u8;
enum class Game : u32;
enum class Language : u8;

/**
 * @brief Searcher class for SHA1 cache
 */
class SHA1CacheSearcher final : public SearcherBase<SHA1Seed>
{
public:
    /**
     * @brief Construct a new SHA1CacheSearcher object
     *
     * @param profile Profile information
     * @param start Start date
     * @param end End date
     */
    SHA1CacheSearcher(const Profile5 &profile, const Date &start, const Date &end);

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
    Profile5 profile;
    std::vector<Keypress> keypresses;
    std::vector<u32> entralinkSeeds;
    std::vector<u32> normalSeeds;
    std::vector<SHA1Seed> normalResults;
    std::vector<u32> roamerSeeds;
    std::vector<SHA1Seed> roamerResults;
    Date end;
    Date start;

    /**
     * @brief Searches between the \p start and \p end dates
     *
     * @param start Start date
     * @param end End date
     */
    void search(const Date &start, const Date &end);
};

#endif // SHA1CACHESEARCHER_HPP
