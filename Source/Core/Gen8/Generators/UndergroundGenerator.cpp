#include "UndergroundGenerator.hpp"

#include <Core/Enum/Method.hpp>
#include <Core/Gen8/States/UndergroundState.hpp>
#include <Core/Parents/PersonalLoader.hpp>
#include <Core/RNG/Xorshift.hpp>
#include <Core/Resources/Encounters.hpp>
#include <algorithm>
#include <bzlib.h>
#include <cmath>

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

UndergroundGenerator::UndergroundGenerator(u32 initialAdvances, u32 maxAdvances, u32 offset, u16 tid, u16 sid, Game version, Lead lead,
                                           u8 randMarkId, u8 storyFlag, bool bonus, const UndergroundStateFilter &filter) :
    Generator<UndergroundStateFilter>(initialAdvances, maxAdvances, offset, tid, sid, version, Method::None, filter),
    lead(lead),
    randMarkId(randMarkId),
    storyFlag(storyFlag),
    bonus(bonus)
{
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
    u8 type;
    u8 size;
    u16 value;

    bool operator==(size_t type) const
    {
        return this->type == type;
    }
};

struct TypeRate
{
    u8 type;
    u16 rate;

    bool operator>(const TypeRate &a) const
    {
        return rate > a.rate;
    }
};

std::vector<UndergroundState> UndergroundGenerator::generate(u64 seed0, u64 seed1) const
{
    Xorshift rng(seed0, seed1);
    rng.advance(initialAdvances + offset);

    std::vector<PokeRate> specialPokemonRates;

    const u8 *compressedSpecialEncountData = ug_special_pokemon.data();
    size_t compressedSpecialEncountSize = ug_special_pokemon.size();

    u32 specialEncountSize = *reinterpret_cast<const u16 *>(compressedSpecialEncountData);
    u8 *specialEncountData = new u8[specialEncountSize];

    BZ2_bzBuffToBuffDecompress(reinterpret_cast<char *>(specialEncountData), &specialEncountSize,
                               reinterpret_cast<char *>(const_cast<u8 *>(compressedSpecialEncountData + sizeof(u16))),
                               compressedSpecialEncountSize, 0, 0);
    u32 specialRatesSum = 0;

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

    const u8 *randMarkInfo;

    const u8 *compressedRandMarkData = ug_rand_mark.data();
    size_t compressedRandMarkDataSize = ug_rand_mark.size();

    u32 randMarkSize = *reinterpret_cast<const u16 *>(compressedRandMarkData);
    u8 *randMarkData = new u8[randMarkSize];

    BZ2_bzBuffToBuffDecompress(reinterpret_cast<char *>(randMarkData), &randMarkSize,
                               reinterpret_cast<char *>(const_cast<u8 *>(compressedRandMarkData + sizeof(u16))), compressedRandMarkDataSize,
                               0, 0);

    for (size_t offset = 0; offset < randMarkSize; offset += 26)
    {
        const u8 *entry = randMarkData + offset;
        if (entry[0] == randMarkId)
        {
            randMarkInfo = entry;
            break;
        }
    }

    std::vector<u16> enabledPokemon;

    const u8 *compressedEncountData = ug_encount.data();
    size_t compressedEncountSize = ug_encount.size();

    u32 encountSize = *reinterpret_cast<const u16 *>(compressedEncountData);
    u8 *encountData = new u8[encountSize];

    BZ2_bzBuffToBuffDecompress(reinterpret_cast<char *>(encountData), &encountSize,
                               reinterpret_cast<char *>(const_cast<u8 *>(compressedEncountData + sizeof(u16))), compressedEncountSize, 0,
                               0);

    u8 romCode = version == Game::BD ? 2 : 3;

    const u8 *entry = encountData;
    u8 encount_id = randMarkInfo[1];
    for (u8 i = 2; i < encount_id; i += 1)
    {
        entry += entry[0] * 4 + 1;
    }

    u8 ugEncountSize = entry[0];
    entry += 1;

    for (size_t offset = 0; offset < ugEncountSize; offset += 1)
    {
        if ((entry[2] == 1 || entry[2] == romCode) && entry[3] <= storyFlag)
        {
            u16 species = *reinterpret_cast<const u16 *>(entry);
            enabledPokemon.emplace_back(species);
        }
        entry += 4;
    }

    std::vector<TypeAndSize> monsDataIndexs;

    const u8 *compressedUgPokemonData = ug_pokemon_data.data();
    size_t compressedUgPokemonSize = ug_pokemon_data.size();

    u32 ugPokemonSize = *reinterpret_cast<const u16 *>(compressedUgPokemonData);
    u8 *ugPokemonData = new u8[ugPokemonSize];

    BZ2_bzBuffToBuffDecompress(reinterpret_cast<char *>(ugPokemonData), &ugPokemonSize,
                               reinterpret_cast<char *>(const_cast<u8 *>(compressedUgPokemonData + sizeof(u16))), compressedUgPokemonSize,
                               0, 0);

    const u8 *compressedTamagoWazaData = tamago_waza_table.data();
    size_t compressedTamagoWazaSize = tamago_waza_table.size();

    u32 tamagoWazaSize = *reinterpret_cast<const u16 *>(compressedTamagoWazaData);
    u8 *tamagoWazaData = new u8[tamagoWazaSize];

    BZ2_bzBuffToBuffDecompress(reinterpret_cast<char *>(tamagoWazaData), &tamagoWazaSize,
                               reinterpret_cast<char *>(const_cast<u8 *>(compressedTamagoWazaData + sizeof(u16))), compressedTamagoWazaSize,
                               0, 0);

    const u8 *compressedTamagoWazaIgnoreData = tamago_waza_ignore_table.data();
    size_t compressedTamagoWazaIgnoreSize = tamago_waza_ignore_table.size();

    u32 tamagoWazaIgnoreSize = *reinterpret_cast<const u16 *>(compressedTamagoWazaIgnoreData);
    u8 *tamagoWazaIgnoreData = new u8[tamagoWazaIgnoreSize];

    BZ2_bzBuffToBuffDecompress(reinterpret_cast<char *>(tamagoWazaIgnoreData), &tamagoWazaIgnoreSize,
                               reinterpret_cast<char *>(const_cast<u8 *>(compressedTamagoWazaIgnoreData + sizeof(u16))),
                               compressedTamagoWazaIgnoreSize, 0, 0);

    for (u16 species : enabledPokemon)
    {
        TypeAndSize ts;
        for (size_t offset = 0; offset < ugPokemonSize; offset += 12)
        {
            const u8 *entry = offset + ugPokemonData;
            if (species == *reinterpret_cast<const u16 *>(entry))
            {
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

    std::vector<TypeRate> typeRates;

    u16 typeRatesSum = 0;
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

    std::vector<UndergroundState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        u8 spawnCount = randMarkInfo[2];
        Xorshift gen(rng);
        u32 rareRand = gen.next(0, 100);
        u16 rareMonsNo = 0;
        if (rareRand < 50)
        {
            float rareESRand = gen.nextf(0.0, specialRatesSum);
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

        u32 minMaxRand = gen.next(0, 100);
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
            float typeRand = gen.nextf(0.0, typeRatesSum);
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

            u32 sizeRand = gen.next(0, existSizeList.size());
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
            float slotRand = gen.nextf(0.0, pokeRatesSum);
            for (auto pr : pokeRates)
            {
                if (slotRand < pr.rate)
                {
                    species = pr.monsNo;
                    break;
                }
                slotRand -= pr.rate;
            }

            gen.next(); // Level
            gen.next(); // EC
            u32 sidtid = gen.next(-0x7fffffff - 1, 0x7fffffff);
            u32 pid = 0;
            u8 shiny = 0;
            u8 pidRolls = bonus ? 2 : 1;
            for (auto i = 0; i < pidRolls; i++)
            {
                pid = gen.next(-0x7fffffff - 1, 0x7fffffff);

                u16 psv = (pid >> 16) ^ (pid & 0xffff);
                u16 fakeXor = (sidtid >> 16) ^ (sidtid & 0xffff) ^ psv;

                if (fakeXor < 16) // Force shiny
                {
                    shiny = fakeXor == 0 ? 2 : 1;

                    u16 realXor = psv ^ tsv;
                    u8 realShinyType = realXor == 0 ? 2 : realXor < 16 ? 1 : 0;

                    if (realShinyType != shiny)
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
                iv = gen.next(-0x7fffffff - 1, 0x7fffffff) % 32;
            }

            u8 ability = gen.next(-0x7fffffff - 1, 0x7fffffff) % 2;

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
                gender = (gen.next(-0x7fffffff - 1, 0x7fffffff) % 253) + 1 < info->getGender();

                break;
            }

            u8 nature = gen.next(-0x7fffffff - 1, 0x7fffffff) % 25;

            gen.advance(4); // 2 calls height, 2 calls weight

            u16 item = getItem(gen.next(0, 100), info);

            u16 hatchSpecies = info->getHatchSpecies();

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
                        gen.next(-0x7fffffff - 1, 0x7fffffff); // Egg Move
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
            gen.next(); // Level
            gen.next(); // EC
            u32 sidtid = gen.next(-0x7fffffff - 1, 0x7fffffff);
            u32 pid = 0;
            u8 shiny = 0;
            u8 pidRolls = bonus ? 2 : 1;
            for (auto i = 0; i < pidRolls; i++)
            {
                pid = gen.next(-0x7fffffff - 1, 0x7fffffff);

                u16 psv = (pid >> 16) ^ (pid & 0xffff);
                u16 fakeXor = (sidtid >> 16) ^ (sidtid & 0xffff) ^ psv;

                if (fakeXor < 16) // Force shiny
                {
                    shiny = fakeXor == 0 ? 2 : 1;

                    u16 realXor = psv ^ tsv;
                    u8 realShinyType = realXor == 0 ? 2 : realXor < 16 ? 1 : 0;

                    if (realShinyType != shiny)
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
                iv = gen.next(-0x7fffffff - 1, 0x7fffffff) % 32;
            }

            u8 ability = gen.next(-0x7fffffff - 1, 0x7fffffff) % 2;

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
                gender = (gen.next(-0x7fffffff - 1, 0x7fffffff) % 253) + 1 < info->getGender();

                break;
            }

            u8 nature = gen.next(-0x7fffffff - 1, 0x7fffffff) % 25;

            gen.advance(4); // 2 calls height, 2 calls weight

            u16 item = getItem(gen.next(0, 100), info);

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
