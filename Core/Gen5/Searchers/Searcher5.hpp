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

#ifndef SEARCHER5_HPP
#define SEARCHER5_HPP

#include <Core/Gen5/Searchers/SearcherBase5.hpp>

/**
 * @brief Parent searcher class for most Gen 5 generators
 *
 * @tparam Generator Generator class to use
 * @tparam State State class to use
 */
template <class Generator, class State>
class Searcher5 : public SearcherBase5<Generator, State>
{
public:
    /**
     * @brief Construct a new Searcher5 object
     *
     * @param generator State generator
     * @param profile Profile information
     */
    Searcher5(const Generator &generator, const Profile5 &profile);

private:
    /**
     * @brief Searches between the \p start and \p end dates
     *
     * @param start Start date
     * @param end End date
     */
    void search(const Date &start, const Date &end) override;
};

#endif // SEARCHER5_HPP
