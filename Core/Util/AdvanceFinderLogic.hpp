/*
 * This file is part of PokeFinder
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

#ifndef ADVANCEFINDERLOGIC_HPP
#define ADVANCEFINDERLOGIC_HPP

#include <Core/Global.hpp>
#include <functional>
#include <utility>
#include <vector>

namespace AdvanceFinderLogic
{
    using Range = std::pair<u8, u8>;
    using Sequence = std::vector<Range>;
    using ValueGetter = std::function<u8(size_t)>;

    enum class ChatotToken : u8
    {
        Any,
        High,
        MidHigh,
        Mid,
        MidLow,
        Low,
        HighMidHigh,
        MidHighMid,
        MidMidLow,
        MidLowLow
    };

    enum class NeedleToken : u8
    {
        Needle0,
        Needle1,
        Needle2,
        Needle3,
        Needle4,
        Needle5,
        Needle6,
        Needle7,
        Any
    };

    enum class CallToken : u8
    {
        E,
        K,
        P
    };

    /**
     * @brief Converts call tokens into allowed call values
     *
     * @param tokens Search tokens
     *
     * @return Sequence of valid values for each token
     */
    Sequence getCallSequence(const std::vector<u8> &tokens);

    /**
     * @brief Converts Chatot pitch tokens into allowed pitch values
     *
     * @param tokens Search tokens
     *
     * @return Sequence of valid values for each token
     */
    Sequence getChatotSequence(const std::vector<u8> &tokens);

    /**
     * @brief Converts save needle tokens into allowed needle values
     *
     * @param tokens Search tokens
     *
     * @return Sequence of valid values for each token
     */
    Sequence getNeedleSequence(const std::vector<u8> &tokens);

    /**
     * @brief Searches a row sequence for matching advances
     *
     * @param rowCount Number of rows to search
     * @param sequence Allowed values for each search position
     * @param getter Retrieves the value for a given row
     *
     * @return Matching starting rows
     */
    std::vector<size_t> findMatches(size_t rowCount, const Sequence &sequence, const ValueGetter &getter);
}

#endif // ADVANCEFINDERLOGIC_HPP
