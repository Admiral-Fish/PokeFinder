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

static AdvanceSearcher::Range getChatotRange(AdvanceSearcher::ChatotToken token)
{
    using ChatotToken = AdvanceSearcher::ChatotToken;
    switch (token)
    {
    case ChatotToken::Any:
        return { 0, 100 };
    case ChatotToken::High:
        return { 80, 100 };
    case ChatotToken::MidHigh:
        return { 60, 80 };
    case ChatotToken::Mid:
        return { 40, 60 };
    case ChatotToken::MidLow:
        return { 20, 40 };
    case ChatotToken::Low:
        return { 0, 20 };
    case ChatotToken::HighMidHigh:
        return { 60, 100 };
    case ChatotToken::MidHighMid:
        return { 40, 80 };
    case ChatotToken::MidMidLow:
        return { 20, 60 };
    case ChatotToken::MidLowLow:
        return { 0, 40 };
    case ChatotToken::HighMidHighMid:
        return { 40, 100 };
    case ChatotToken::MidMidLowLow:
        return { 0, 60 };
    case ChatotToken::HigherThanLast:
    case ChatotToken::LowerThanLast:
        return { 0, 0 };
    }
    return { 0, 0 };
}

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
            const ChatotToken chatotToken = static_cast<ChatotToken>(token);
            if (chatotToken != ChatotToken::HigherThanLast && chatotToken != ChatotToken::LowerThanLast)
            {
                sequence.emplace_back(getChatotRange(chatotToken));
            }
        }

        return sequence;
    }

    std::vector<int> findChatotMatches(int rowCount, const std::vector<u8> &tokens, const ValueGetter &getter)
    {
        std::vector<int> matches;
        if (tokens.empty())
        {
            return matches;
        }

        const bool needsPrevious = static_cast<ChatotToken>(tokens.front()) == ChatotToken::HigherThanLast
            || static_cast<ChatotToken>(tokens.front()) == ChatotToken::LowerThanLast;
        for (int i = needsPrevious ? 1 : 0; i + tokens.size() <= rowCount; i++)
        {
            bool match = true;
            for (int j = 0; j < tokens.size() && match; j++)
            {
                const u8 value = getter(i + j);
                switch (static_cast<ChatotToken>(tokens[j]))
                {
                case ChatotToken::HigherThanLast:
                    match = value > getter(i + j - 1);
                    break;
                case ChatotToken::LowerThanLast:
                    match = value < getter(i + j - 1);
                    break;
                default:
                {
                    const Range range = getChatotRange(static_cast<ChatotToken>(tokens[j]));
                    match = value >= range.first && value < range.second;
                    break;
                }
                }
            }

            if (match)
            {
                matches.emplace_back(i + tokens.size() - 1);
            }
        }

        return matches;
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

    std::vector<int> findMatches(int rowCount, const Sequence &sequence, const ValueGetter &getter)
    {
        std::vector<int> matches;
        if (sequence.empty())
        {
            return matches;
        }

        for (int i = 0; i + sequence.size() <= rowCount; i++)
        {
            bool match = true;
            for (int j = 0; j < sequence.size() && match; j++)
            {
                u8 value = getter(i + j);
                match = value >= sequence[j].first && value < sequence[j].second;
            }

            if (match)
            {
                matches.emplace_back(i + sequence.size() - 1);
            }
        }

        return matches;
    }
}
