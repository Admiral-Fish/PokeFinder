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

#ifndef SEARCHER_HPP
#define SEARCHER_HPP

#include <Core/Global.hpp>
#include <vector>

enum class Game : u32;
enum class Method : u8;

/**
 * @brief Parent generator class that stores common attributes
 *
 * @tparam Filter Filter class used by the searcher
 */
template <class Filter>
class Searcher
{
public:
    /**
     * @brief Construct a new Searcher object
     *
     * @param tid Trainer ID
     * @param sid Secret ID
     * @param version Game version
     * @param method Encounter method
     * @param filter State filter
     */
    Searcher(u16 tid, u16 sid, Game version, Method method, const Filter &filter) :
        version(version), sid(sid), tid(tid), tsv(tid ^ sid), method(method), filter(filter)
    {
    }

protected:
    Game version;
    u16 sid;
    u16 tid;
    u16 tsv;
    Method method;
    Filter filter;
};

#endif // SEARCHER_HPP
