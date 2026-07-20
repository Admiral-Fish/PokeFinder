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

#include "AdvanceSearcher.hpp"

namespace AdvanceSearcher
{
    Sequence getCallSequence(const std::vector<u8> &tokens)
    {
        Sequence sequence;

        for (u8 token : tokens)
        {
            switch (static_cast<CallToken>(token))
            {
            case CallToken::E:
                sequence.emplace_back(0, 1);
                break;
            case CallToken::K:
                sequence.emplace_back(1, 2);
                break;
            case CallToken::P:
                sequence.emplace_back(2, 3);
                break;
            }
        }

        return sequence;
    }

    Sequence getChatotSequence(const std::vector<u8> &tokens)
    {
        Sequence sequence;

        for (u8 token : tokens)
        {
            switch (static_cast<ChatotToken>(token))
            {
            case ChatotToken::Any:
                sequence.emplace_back(0, 100);
                break;
            case ChatotToken::High:
                sequence.emplace_back(80, 100);
                break;
            case ChatotToken::MidHigh:
                sequence.emplace_back(60, 80);
                break;
            case ChatotToken::Mid:
                sequence.emplace_back(40, 60);
                break;
            case ChatotToken::MidLow:
                sequence.emplace_back(20, 40);
                break;
            case ChatotToken::Low:
                sequence.emplace_back(0, 20);
                break;
            case ChatotToken::HighMidHigh:
                sequence.emplace_back(60, 100);
                break;
            case ChatotToken::MidHighMid:
                sequence.emplace_back(40, 80);
                break;
            case ChatotToken::MidMidLow:
                sequence.emplace_back(20, 60);
                break;
            case ChatotToken::MidLowLow:
                sequence.emplace_back(0, 40);
                break;
            }
        }

        return sequence;
    }

    Sequence getNeedleSequence(const std::vector<u8> &tokens)
    {
        Sequence sequence;

        for (u8 token : tokens)
        {
            if (static_cast<NeedleToken>(token) == NeedleToken::Any)
            {
                sequence.emplace_back(0, 8);
            }
            else
            {
                u8 needle = static_cast<u8>(token);
                sequence.emplace_back(needle, needle + 1);
            }
        }

        return sequence;
    }

    std::vector<size_t> findMatches(size_t rowCount, const Sequence &sequence, const ValueGetter &getter)
    {
        std::vector<size_t> matches;
        if (sequence.empty())
        {
            return matches;
        }

        for (size_t i = 0; i + sequence.size() <= rowCount; i++)
        {
            bool match = true;
            for (size_t j = 0; j < sequence.size() && match; j++)
            {
                u8 value = getter(i + j);
                match = value >= sequence[j].first && value < sequence[j].second;
            }

            if (match)
            {
                matches.emplace_back(i);
            }
        }

        return matches;
    }
}
