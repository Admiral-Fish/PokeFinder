/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef CHANNELSEEDSEARCHER_HPP
#define CHANNELSEEDSEARCHER_HPP

#include <Core/Gen3/Searchers/SeedSearcher.hpp>
#include <Core/RNG/LCRNG.hpp>

class ChannelSeedSearcher : public SeedSearcher
{
public:
    explicit ChannelSeedSearcher(const std::vector<u32> &criteria);
    void startSearch(int threads);
    int getProgress() const override;

private:
    void search(u32 start, u32 end);
    bool searchSeed(XDRNG &rng);
};

#endif // CHANNELSEEDSEARCHER_HPP
