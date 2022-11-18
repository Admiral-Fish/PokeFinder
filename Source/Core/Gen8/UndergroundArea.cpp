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
        if (std::find_if(this->typeSizes.begin(), this->typeSizes.end(), [i](const TypeSize &typeSize) { return typeSize.type == i; })
            != this->typeSizes.end())
        {
            typeSum += rate;
            TypeRate typeRate = { rate, i };
            this->typeRates.emplace_back(typeRate);
        }
    }
    std::sort(this->typeRates.begin(), this->typeRates.end(), [](const TypeRate &left, const TypeRate &right) { return left.rate > right.rate; });
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

std::pair<const PersonalInfo *, u16> UndergroundArea::getPokemon(RNGList<u32, Xorshift, 256> &rngList, const TypeSize &type, u8 story,
                                                                 bool diglett) const
{
    std::vector<TypeSize> temp;
    std::copy_if(typeSizes.begin(), typeSizes.end(), std::back_inserter(temp),
                 [&type](const TypeSize &t) { return t.value == type.value; });

    u16 pokeRateSum = 0;
    std::vector<Pokemon> filtered;
    for (const Pokemon &mon : pokemon)
    {
        if (std::find_if(temp.begin(), temp.end(),
                         [mon](const TypeSize &t) {
                             return t.size == mon.size && (t.type == mon.info->getType(0) || t.type == mon.info->getType(1));
                         })
            != temp.end())
        {
            pokeRateSum += mon.rate;
            filtered.emplace_back(mon);
        }
    }
    std::sort(filtered.begin(), filtered.end(), [](const Pokemon &left, const Pokemon &right) { return left.rate > right.rate; });

    float rand = rngList.next(randF) * pokeRateSum;
    for (auto const &filter : filtered)
    {
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
        for (const auto &rate : typeRates)
        {
            if (rand < rate.rate)
            {
                type = rate.type;
                break;
            }
            rand -= rate.rate;
        }

        std::vector<u8> sizesFiltered;
        for (const auto &t : typeSizes)
        {
            if (t.type == type && std::find(sizesFiltered.begin(), sizesFiltered.end(), t.size) == sizesFiltered.end())
            {
                sizesFiltered.emplace_back(t.size);
            }
        }

        u8 size = sizesFiltered[rngList.next() % sizesFiltered.size()];
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
        float rate = rngList.next(randF) * specialSum;
        for (const auto &pokemon : specialPokemon)
        {
            if (rate < pokemon.rate)
            {
                return std::pair<const PersonalInfo *, u16>(pokemon.info, pokemon.specie);
            }
        }
    }
    return std::make_pair(nullptr, 0);
}

std::vector<u16> UndergroundArea::getSpecies() const
{
    std::vector<u16> nums;
    for (const auto &mon : pokemon)
    {
        nums.emplace_back(mon.specie);
    }
    for (const auto &mon : specialPokemon)
    {
        nums.emplace_back(mon.specie);
    }
    return nums;
}

std::vector<std::string> UndergroundArea::getSpecieNames() const
{
    return Translator::getSpecies(getSpecies());
}
