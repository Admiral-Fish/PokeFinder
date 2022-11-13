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

#include "UndergroundGenerator.hpp"
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Gen8/States/UndergroundState.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/RNG/Xorshift.hpp>
#include <Core/Resources/Encounters.hpp>
#include <Core/Util/Utilities.hpp>
#include <algorithm>
#include <cmath>

static u32 rand(u32 prng)
{
    return (prng % 0xffffffff) + 0x80000000;
}

static float rand(u32 prng, float max)
{
    float t = (prng & 0x7fffff) / 8388607.0;
    return (1.0 - t) * max;
}

static u16 getItem(u8 rand, const PersonalInfo *info)
{
    if (rand >= 60)
    {
        if (rand >= 80)
        {
            return info->getItem(2);
        }
        else
        {
            return info->getItem(1);
        }
    }
    else
    {
        return info->getItem(0);
    }
}

struct PokeRate
{
    u16 monsNo;
    u16 rate;

    bool operator>(const PokeRate &a) const
    {
        return rate > a.rate;
    }
};

struct TypeAndSize
{
    u16 value;
    u8 size;
    u8 type;

    bool operator==(size_t type) const
    {
        return this->type == type;
    }
};

struct TypeRate
{
    u16 rate;
    u8 type;

    bool operator>(const TypeRate &a) const
    {
        return rate > a.rate;
    }
};

UndergroundGenerator::UndergroundGenerator(u32 initialAdvances, u32 maxAdvances, u32 offset, u16 tid, u16 sid, Game version, Lead lead,
                                           u8 randMarkId, u8 storyFlag, bool bonus, const UndergroundStateFilter &filter) :
    StaticGenerator<UndergroundStateFilter>(initialAdvances, maxAdvances, offset, tid, sid, version, Method::None, lead, filter),
    bonus(bonus),
    randMarkId(randMarkId),
    storyFlag(storyFlag)
{
}

std::vector<UndergroundState> UndergroundGenerator::generate(u64 seed0, u64 seed1, const EncounterArea8 &area) const
{
    u32 specialEncountSize;
    u8 *specialEncountData = Utilities::decompress(ug_special_pokemon.data(), ug_special_pokemon.size(), specialEncountSize);

    u32 specialRatesSum = 0;
    std::vector<PokeRate> specialPokemonRates;
    for (size_t offset = 0; offset < specialEncountSize; offset += 8)
    {
        const u8 *entry = specialEncountData + offset;
        if (entry[0] == randMarkId)
        {
            PokeRate rate;
            rate.monsNo = *reinterpret_cast<const u16 *>(entry + 1);
            if (version == Game::BD)
            {
                rate.rate = *reinterpret_cast<const u16 *>(entry + 4);
            }
            else
            {
                rate.rate = *reinterpret_cast<const u16 *>(entry + 6);
            }
            specialPokemonRates.emplace_back(rate);
            specialRatesSum += rate.rate;
        }
    }

    std::sort(specialPokemonRates.begin(), specialPokemonRates.end(), std::greater<PokeRate>());

    u32 randMarkSize;
    u8 *randMarkData = Utilities::decompress(ug_rand_mark.data(), ug_rand_mark.size(), randMarkSize);

    const u8 *randMarkInfo;
    for (size_t offset = 0; offset < randMarkSize; offset += 26)
    {
        const u8 *entry = randMarkData + offset;
        if (entry[0] == randMarkId)
        {
            randMarkInfo = entry;
            break;
        }
    }

    u32 encountSize;
    u8 *encountData = Utilities::decompress(ug_encount.data(), ug_encount.size(), encountSize);
    u8 romCode = version == Game::BD ? 2 : 3;

    const u8 *entry = encountData;
    u8 encount_id = randMarkInfo[1];
    for (u8 i = 2; i < encount_id; i += 1)
    {
        entry += entry[0] * 4 + 1;
    }

    u8 ugEncountSize = entry[0];
    entry += 1;

    std::vector<u16> enabledPokemon;
    for (size_t offset = 0; offset < ugEncountSize; offset += 1)
    {
        if ((entry[2] == 1 || entry[2] == romCode) && entry[3] <= storyFlag)
        {
            u16 species = *reinterpret_cast<const u16 *>(entry);
            enabledPokemon.emplace_back(species);
        }
        entry += 4;
    }

    u32 ugPokemonSize;
    u8 *ugPokemonData = Utilities::decompress(ug_pokemon_data.data(), ug_pokemon_data.size(), ugPokemonSize);

    u32 tamagoWazaSize;
    u8 *tamagoWazaData = Utilities::decompress(tamago_waza_table.data(), tamago_waza_table.size(), tamagoWazaSize);

    const u8 *compressedTamagoWazaIgnoreData = tamago_waza_ignore_table.data();
    size_t compressedTamagoWazaIgnoreSize = tamago_waza_ignore_table.size();

    u32 tamagoWazaIgnoreSize;
    u8 *tamagoWazaIgnoreData
        = Utilities::decompress(tamago_waza_ignore_table.data(), tamago_waza_ignore_table.size(), tamagoWazaIgnoreSize);

    std::vector<TypeAndSize> monsDataIndexs;
    for (size_t offset = 0; offset < ugPokemonSize; offset += 12)
    {
        for (u16 species : enabledPokemon)
        {
            const u8 *entry = offset + ugPokemonData;
            if (species == *reinterpret_cast<const u16 *>(entry))
            {
                TypeAndSize ts;
                ts.size = entry[4];
                ts.type = entry[2];
                ts.value = std::pow(10.0, entry[4]) + entry[2];
                monsDataIndexs.emplace_back(ts);
                if (entry[3] != 18)
                {
                    TypeAndSize ts2;
                    ts2.size = entry[4];
                    ts2.type = entry[3];
                    ts2.value = std::pow(10.0, entry[4]) + entry[3];
                    monsDataIndexs.emplace_back(ts2);
                }
            }
        }
    }

    u16 typeRatesSum = 0;
    std::vector<TypeRate> typeRates;
    for (size_t offset = 0; offset < 18; offset++)
    {
        u16 rate = (randMarkInfo + 8)[offset];
        if (std::count(monsDataIndexs.begin(), monsDataIndexs.end(), offset))
        {
            TypeRate tr;
            tr.rate = rate;
            typeRatesSum += rate;
            tr.type = offset;
            typeRates.emplace_back(tr);
        }
    }

    std::sort(typeRates.begin(), typeRates.end(), std::greater<TypeRate>());

    RNGList<u32, Xorshift, 256> rngList(seed0, seed1, initialAdvances + offset);
    std::vector<UndergroundState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rngList.advanceState())
    {
        u8 spawnCount = randMarkInfo[2];

        u32 rareRand = rngList.next() % 100;
        u16 rareMonsNo = 0;
        if (rareRand < 50)
        {
            float rareESRand = rand(rngList.next(), specialRatesSum);
            for (auto p : specialPokemonRates)
            {
                if (rareESRand < p.rate)
                {
                    rareMonsNo = p.monsNo;
                    break;
                }
                rareESRand -= p.rate;
            }
        }

        u32 minMaxRand = rngList.next() % 100;
        if (50 <= minMaxRand)
        {
            spawnCount = randMarkInfo[3];
        }

        if (rareRand < 50)
        {
            spawnCount -= 1;
        }

        std::vector<TypeAndSize> pokeSlots;
        for (auto i = 0; i < spawnCount; i++)
        {
            u8 type = 0;
            float typeRand = rand(rngList.next(), typeRatesSum);
            for (auto tr : typeRates)
            {
                if (typeRand < tr.rate)
                {
                    type = tr.type;
                    break;
                }
                typeRand -= tr.rate;
            }

            std::vector<u8> existSizeList;
            for (auto ts : monsDataIndexs)
            {
                if (ts.type == type && std::count(existSizeList.begin(), existSizeList.end(), ts.size) == 0)
                {
                    existSizeList.emplace_back(ts.size);
                }
            }

            u32 sizeRand = rngList.next() % existSizeList.size();
            u8 size = existSizeList[sizeRand];

            TypeAndSize ts;
            ts.type = type;
            ts.size = size;
            ts.value = std::pow(10.0, size) + type;

            pokeSlots.emplace_back(ts);
        }

        for (auto ts : pokeSlots)
        {
            std::vector<PokeRate> pokeRates;
            std::vector<TypeAndSize> tmpList;

            for (auto ts2 : monsDataIndexs)
            {
                if (ts2.value == ts.value)
                {
                    tmpList.emplace_back(ts2);
                }
            }

            u16 pokeRatesSum = 0;
            for (auto p : enabledPokemon)
            {
                const u8 *pokemonData = ugPokemonData + ((p - 1) * 12);
                if (std::any_of(tmpList.begin(), tmpList.end(), [&](TypeAndSize ts2) {
                        return (ts2.type == pokemonData[2] || ts2.type == pokemonData[3]) && ts2.size == pokemonData[4];
                    }))
                {
                    PokeRate pr;
                    pr.monsNo = *reinterpret_cast<const u16 *>(pokemonData);
                    if (bonus)
                    {
                        pr.rate = (pokemonData + 5)[storyFlag - 1] * pokemonData[11];
                    }
                    else
                    {
                        pr.rate = (pokemonData + 5)[storyFlag - 1];
                    }
                    pokeRatesSum += pr.rate;
                    pokeRates.emplace_back(pr);
                }
            }

            std::sort(pokeRates.begin(), pokeRates.end(), std::greater<PokeRate>());

            u16 species = 0;
            float slotRand = rand(rngList.next(), pokeRatesSum);
            for (auto pr : pokeRates)
            {
                if (slotRand < pr.rate)
                {
                    species = pr.monsNo;
                    break;
                }
                slotRand -= pr.rate;
            }

            rngList.advance(1); // Level
            rngList.advance(1); // EC
            u32 sidtid = rngList.next(rand);
            u32 pid = 0;
            u8 shiny = 0;
            u8 pidRolls = bonus ? 2 : 1;
            for (auto i = 0; i < pidRolls; i++)
            {
                pid = rngList.next(rand);

                u16 psv = (pid >> 16) ^ (pid & 0xffff);
                u16 fakeXor = (sidtid >> 16) ^ (sidtid & 0xffff) ^ psv;

                if (fakeXor < 16) // Force shiny
                {
                    shiny = fakeXor == 0 ? 2 : 1;

                    u16 realXor = psv ^ tsv;
                    u8 realShiny = realXor == 0 ? 2 : realXor < 16 ? 1 : 0;

                    if (realShiny != shiny)
                    {
                        u16 high = (pid & 0xFFFF) ^ tsv ^ (2 - shiny);
                        pid = (high << 16) | (pid & 0xFFFF);
                    }
                    break;
                }
                else // Force non
                {
                    shiny = 0;
                    if ((psv ^ tsv) < 16)
                    {
                        pid ^= 0x10000000;
                    }
                }
            }

            std::array<u8, 6> ivs;
            for (u8 &iv : ivs)
            {
                iv = rngList.next(rand) % 32;
            }

            u8 ability = rngList.next(rand) % 2;

            const PersonalInfo *info = PersonalLoader::getPersonal(version, species);

            u8 gender;
            switch (info->getGender())
            {
            case 255:
                gender = 2;
                break;
            case 254:
                gender = 1;
                break;
            case 0:
                gender = 0;
                break;
            default:
                gender = (rngList.next(rand) % 253) + 1 < info->getGender();
                break;
            }

            u8 nature = rngList.next(rand) % 25;

            rngList.advance(4); // 2 calls height, 2 calls weight

            u16 item = getItem(rngList.next() % 100, info);

            u16 hatchSpecies = info->getHatchSpecie();

            u8 ignoreCount = 0;

            for (size_t offset = 0; offset < tamagoWazaIgnoreSize; offset += 10)
            {
                const u8 *entry = tamagoWazaIgnoreData + offset;
                u16 species = *reinterpret_cast<const u16 *>(entry);
                if (species == hatchSpecies)
                {
                    const u8 *waza = entry + 2;
                    for (size_t offset2 = 0; offset2 < 8; offset2 += 2)
                    {
                        u16 wazaNo = *reinterpret_cast<const u16 *>(waza + offset2);
                        if (wazaNo != 0)
                        {
                            ignoreCount += 1;
                        }
                        else
                        {
                            break;
                        }
                    }
                    break;
                }
            }

            for (size_t offset = 0; offset < tamagoWazaSize; offset += 4)
            {
                const u8 *entry = tamagoWazaData + offset;
                u16 species = *reinterpret_cast<const u16 *>(entry);
                u8 wazaCount = entry[3];

                if (species == hatchSpecies)
                {
                    if (wazaCount - ignoreCount > 0)
                    {
                        rngList.next(rand); // Egg Move
                    }
                    break;
                }
                offset += wazaCount * 2;
            }

            UndergroundState state(initialAdvances + cnt, species, pid, shiny, ivs, ability, gender, nature, item, info);
            if (filter.compareState(state))
            {
                states.emplace_back(state);
            }
        }

        if (rareRand < 50)
        {
            rngList.advance(1); // Level
            rngList.advance(1); // EC
            u32 sidtid = rngList.next(rand);
            u32 pid = 0;
            u8 shiny = 0;
            u8 pidRolls = bonus ? 2 : 1;
            for (auto i = 0; i < pidRolls; i++)
            {
                pid = rngList.next(rand);

                u16 psv = (pid >> 16) ^ (pid & 0xffff);
                u16 fakeXor = (sidtid >> 16) ^ (sidtid & 0xffff) ^ psv;

                if (fakeXor < 16) // Force shiny
                {
                    shiny = fakeXor == 0 ? 2 : 1;

                    u16 realXor = psv ^ tsv;
                    u8 realShiny = realXor == 0 ? 2 : realXor < 16 ? 1 : 0;

                    if (realShiny != shiny)
                    {
                        u16 high = (pid & 0xFFFF) ^ tsv ^ (2 - shiny);
                        pid = (high << 16) | (pid & 0xFFFF);
                    }
                    break;
                }
                else // Force non
                {
                    shiny = 0;
                    if ((psv ^ tsv) < 16)
                    {
                        pid ^= 0x10000000;
                    }
                }
            }

            std::array<u8, 6> ivs;
            for (u8 &iv : ivs)
            {
                iv = rngList.next(rand) % 32;
            }

            u8 ability = rngList.next(rand) % 2;

            const PersonalInfo *info = PersonalLoader::getPersonal(version, rareMonsNo);

            u8 gender;
            switch (info->getGender())
            {
            case 255:
                gender = 2;
                break;
            case 254:
                gender = 1;
                break;
            case 0:
                gender = 0;
                break;
            default:
                gender = (rngList.next(rand) % 253) + 1 < info->getGender();
                break;
            }

            u8 nature = rngList.next(rand) % 25;

            rngList.advance(4); // 2 calls height, 2 calls weight

            u16 item = getItem(rngList.next() % 100, info);

            UndergroundState state(initialAdvances + cnt, rareMonsNo, pid, shiny, ivs, ability, gender, nature, item, info);
            if (filter.compareState(state))
            {
                states.emplace_back(state);
            }
        }
    }

    delete[] randMarkData;
    delete[] encountData;
    delete[] specialEncountData;
    delete[] ugPokemonData;
    delete[] tamagoWazaData;
    delete[] tamagoWazaIgnoreData;

    return states;
}
