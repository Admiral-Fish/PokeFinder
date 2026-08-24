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

#include "SearcherBase5.hpp"
#include <Core/Util/DateTime.hpp>
#include <thread>

template <class Generator, class State>
SearcherBase5<Generator, State>::SearcherBase5(const Generator &generator, const Profile5 &profile) :
    SearcherBase<SearcherState5<State>>(), generator(generator), profile(profile), keypresses(Keypresses::getKeypresses(profile))
{
}

template <class Generator, class State>
u64 SearcherBase5<Generator, State>::getMaxProgress(const Date &start, const Date &end) const
{
    return keypresses.size() * (start.daysTo(end) + 1) * (profile.getTimer0Max() - profile.getTimer0Min() + 1);
}

template <class Generator, class State>
void SearcherBase5<Generator, State>::startSearch(int threads, const Date &start, const Date &end)
{
    this->searching = true;

    auto days = start.daysTo(end) + 1;
    if (days < threads)
    {
        threads = days;
    }

    auto *threadContainer = new std::thread[threads];

    auto daysSplit = days / threads;
    Date day = start;
    for (int i = 0; i < threads; i++, day += daysSplit)
    {
        if (i == threads - 1)
        {
            threadContainer[i] = std::thread([this, day, end] { search(day, end); });
        }
        else
        {
            Date mid = day + (daysSplit - 1);
            threadContainer[i] = std::thread([this, day, mid] { search(day, mid); });
        }
    }

    for (int i = 0; i < threads; i++)
    {
        threadContainer[i].join();
    }

    delete[] threadContainer;
}

#include <Core/Gen5/Generators/DreamRadarGenerator.hpp>
#include <Core/Gen5/Generators/EggGenerator5.hpp>
#include <Core/Gen5/Generators/EventGenerator5.hpp>
#include <Core/Gen5/Generators/HiddenGrottoGenerator.hpp>
#include <Core/Gen5/Generators/IDGenerator5.hpp>
#include <Core/Gen5/Generators/StaticGenerator5.hpp>
#include <Core/Gen5/Generators/WildGenerator5.hpp>

template class SearcherBase5<DreamRadarGenerator, DreamRadarState>;
template class SearcherBase5<EggGenerator5, EggState5>;
template class SearcherBase5<EventGenerator5, EventState5>;
template class SearcherBase5<HiddenGrottoGenerator, State5>;
template class SearcherBase5<HiddenGrottoSlotGenerator, HiddenGrottoState>;
template class SearcherBase5<IDGenerator5, IDState>;
template class SearcherBase5<StaticGenerator5, State5>;
template class SearcherBase5<WildGenerator5, WildState5>;
