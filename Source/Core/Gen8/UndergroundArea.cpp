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

#include "UndergroundArea.hpp"
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/RNG/Xorshift.hpp>
#include <Core/Util/Translator.hpp>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iterator>

static float randF(u32 prng)
{
    float t = (prng & 0x7fffff) / 8388607.0;
    return 1.0 - t;
}

struct PokeRate
{
    const PersonalInfo *info;
    u16 rate;
    u16 specie;
};

UndergroundArea::UndergroundArea(u8 location, u8 min, u8 max, const std::vector<Pokemon> &pokemon,
                                 const std::vector<SpecialPokemon> &specialPokemon, const std::array<u8, 18> &typeRates,
                                 const std::vector<TypeSize> &typeSizes) :
    pokemon(pokemon), specialPokemon(specialPokemon), typeSizes(typeSizes), location(location), max(max), min(min)
{
    for (size_t i = 1; i < this->specialPokemon.size(); i++)
    {
        this->specialPokemon[i].rate += this->specialPokemon[i - 1].rate;
    }
    specialSum = this->specialPokemon.back().rate;

    typeSum = 0;
    for (u8 i = 0; i < typeRates.size(); i++)
    {
        u16 rate = typeRates[i];
        if (std::find_if(this->typeSizes.begin(), this->typeSizes.end(), [i](const TypeSize &typeSize) { return i == typeSize.type; })
            != this->typeSizes.end())
        {
            typeSum += rate;
            TypeRate typeRate = { rate, i };
            this->typeRates.emplace_back(typeRate);
        }
    }
    std::sort(this->typeRates.begin(), this->typeRates.end(),
              [](const TypeRate &left, const TypeRate &right) { return left.rate > right.rate; });

    for (size_t i = 1; i < this->typeRates.size(); i++)
    {
        this->typeRates[i].rate += this->typeRates[i - 1].rate;
    }
}

u8 UndergroundArea::getLocation() const
{
    return location;
}

u8 UndergroundArea::getMax() const
{
    return max;
}

u8 UndergroundArea::getMin() const
{
    return min;
}

std::pair<const PersonalInfo *, u16> UndergroundArea::getPokemon(RNGList<u32, Xorshift, 256> &rngList, const TypeSize &type) const
{
    u8 tempCount = 0;
    std::array<TypeSize, 23> temp;
    for (auto const &typeSize : typeSizes)
    {
        if (type.value == typeSize.value)
        {
            temp[tempCount++] = typeSize;
        }
    }
    assert(tempCount <= temp.size());

    u8 filteredCount = 0;
    u16 sum = 0;
    std::array<Pokemon, 23> filtered;
    for (const Pokemon &mon : pokemon)
    {
        if (std::find_if(temp.begin(), temp.end(),
                         [mon](const TypeSize &t) {
                             return t.size == mon.size && (t.type == mon.info->getType(0) || t.type == mon.info->getType(1));
                         })
            != temp.end())
        {
            sum += mon.rate;
            filtered[filteredCount++] = mon;
        }
    }
    assert(filteredCount <= filtered.size());
    assert(tempCount == filteredCount);
    std::sort(filtered.begin(), filtered.begin() + filteredCount,
              [](const Pokemon &left, const Pokemon &right) { return left.rate > right.rate; });

    float rand = rngList.next(randF) * sum;
    for (u8 i = 0; i < filteredCount; i++)
    {
        const auto &filter = filtered[i];
        if (rand < filter.rate)
        {
            return std::make_pair(filter.info, filter.specie);
        }
        rand -= filter.rate;
    }

    return std::make_pair(nullptr, 0);
}

std::array<TypeSize, 10> UndergroundArea::getSlots(RNGList<u32, Xorshift, 256> &rngList, u8 count) const
{
    std::array<TypeSize, 10> slots;
    for (u8 i = 0; i < count; i++)
    {
        u8 type = 0;
        float rand = rngList.next(randF) * typeSum;
        auto it = std::find_if(typeRates.begin(), typeRates.end(), [rand](const TypeRate &rate) { return rand < rate.rate; });
        if (it != typeRates.end())
        {
            type = it->type;
        }

        std::array<u8, 3> sizes = { 255, 255, 255 };
        u8 sizeCount = 0;
        for (const auto &t : typeSizes)
        {
            if (t.type == type && std::find(sizes.begin(), sizes.begin() + sizeCount, t.size) == sizes.begin() + sizeCount)
            {
                sizes[sizeCount++] = t.size;
            }
        }

        u8 size = sizes[rngList.next() % sizeCount];
        u16 value = std::pow(10, size) + type;

        TypeSize slot = { value, size, type };
        slots[i] = slot;
    }
    return slots;
}

std::pair<const PersonalInfo *, u16> UndergroundArea::getSpecialPokemon(RNGList<u32, Xorshift, 256> &rngList) const
{
    if ((rngList.next() % 100) < 50)
    {
        float rand = rngList.next(randF) * specialSum;
        auto it = std::find_if(specialPokemon.begin(), specialPokemon.end(), [rand](const SpecialPokemon &mon) { return rand < mon.rate; });
        if (it != specialPokemon.end())
        {
            return std::pair(it->info, it->specie);
        }
    }
    return std::make_pair(nullptr, 0);
}

std::vector<u16> UndergroundArea::getSpecies() const
{
    std::vector<u16> nums;
    std::transform(pokemon.begin(), pokemon.end(), std::back_inserter(nums), [](const Pokemon &mon) { return mon.specie; });
    std::transform(specialPokemon.begin(), specialPokemon.end(), std::back_inserter(nums),
                   [](const SpecialPokemon &mon) { return mon.specie; });
    std::sort(nums.begin(), nums.end());
    return nums;
}

std::vector<std::string> UndergroundArea::getSpecieNames() const
{
    return Translator::getSpecies(getSpecies());
}
