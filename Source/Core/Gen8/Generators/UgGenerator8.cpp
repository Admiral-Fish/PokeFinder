#include "UgGenerator8.hpp"
#include <Core/RNG/Xorshift.hpp>
#include <Core/Resources/Encounters.hpp>
#include <algorithm>
#include <cmath>

UgGenerator8::UgGenerator8(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 randMarkID, u8 storyFlag, Game version, u16 species,
                           const StateFilter &filter, bool bonus) :
    Generator(initialAdvances, maxAdvances, tid, sid, 0, Method::None, filter),
    randMarkID(randMarkID),
    storyFlag(storyFlag),
    version(version),
    species(species),
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

std::vector<UgState> UgGenerator8::generate(u64 seed0, u64 seed1) const
{
    Xorshift rng(seed0, seed1);
    rng.advance(initialAdvances + offset);

    std::vector<PokeRate> specialPokemonRates;

    const u8 *specialPokemonData = ug_special_pokemon.data();
    size_t specialPokemonSize = ug_special_pokemon.size();
    u32 specialRatesSum = 0;

    for (size_t offset = 0; offset < specialPokemonSize; offset += 8)
    {
        const u8 *entry = specialPokemonData + offset;
        if (entry[0] == randMarkID)
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

    const u8 *randMarkData = ug_rand_mark.data();
    size_t randMarkSize = ug_rand_mark.size();

    for (size_t offset = 0; offset < randMarkSize; offset += 26)
    {
        const u8 *entry = randMarkData + offset;
        if (entry[0] == randMarkID)
        {
            randMarkInfo = entry;
            break;
        }
    }

    std::vector<u16> enabledPokemon;

    const u8 *ugEncountData = ug_encount.data();

    u8 romCode = version == Game::BD ? 2 : 3;

    const u8 *entry = ugEncountData;
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

    const u8 *ugPokemonData = ug_pokemon_data.data();
    size_t ugPokemonSize = ug_pokemon_data.size();

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

    std::vector<UgState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        u8 spawnCount = randMarkInfo[2];
        Xorshift gen(rng);
        u32 rareRand = gen.next<0, 100>();
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

        u32 minMaxRand = gen.next<0, 100>();
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
            UgState state(initialAdvances + cnt);
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

            state.setSpecies(species);

            gen.next(); // Level
            gen.next(); // EC
            u32 sidtid = gen.next();
            u32 pid = 0;
            u8 pidRolls = bonus ? 2 : 1;
            for (auto i = 0; i < pidRolls; i++)
            {
                pid = gen.next();

                u16 psv = (pid >> 16) ^ (pid & 0xffff);
                u16 fakeXor = (sidtid >> 16) ^ (sidtid & 0xffff) ^ psv;

                if (fakeXor < 16) // Force shiny
                {
                    u8 fakeShinyType = fakeXor == 0 ? 2 : 1;

                    u16 realXor = psv ^ tsv;
                    u8 realShinyType = realXor == 0 ? 2 : realXor < 16 ? 1 : 0;

                    state.setShiny(fakeShinyType);
                    if (realShinyType != fakeShinyType)
                    {
                        u16 high = (pid & 0xFFFF) ^ tsv ^ (2 - fakeShinyType);
                        pid = (high << 16) | (pid & 0xFFFF);
                    }
                    break;
                }
                else // Force non
                {
                    state.setShiny(0);
                    if ((psv ^ tsv) < 16)
                    {
                        pid ^= 0x10000000;
                    }
                }
            }

            state.setPID(pid);

            for (u8 i = 0; i < 6; i++)
            {
                state.setIV(i, gen.next() % 32);
            }

            state.setAbility(gen.next() % 2);

            PersonalInfo info = PersonalLoader::getPersonal(version, species);

            u8 genderRatio = info.getGender();

            if (genderRatio == 255)
            {
                state.setGender(2);
            }
            else if (genderRatio == 254)
            {
                state.setGender(1);
            }
            else if (genderRatio == 0)
            {
                state.setGender(0);
            }
            else
            {
                u8 gender = (gen.next() % 253) + 1 < genderRatio;
                state.setGender(gender);
            }

            state.setNature(gen.next() % 25);

            gen.advance(4); // 2 calls height, 2 calls weight

            state.setItem(gen.next<0, 100>());

            u16 hatchSpecies = info.getHatchSpecies();

            const u8 *tamagoWazaData = tamago_waza_table.data();
            size_t tamagoWazaSize = tamago_waza_table.size();

            const u8 *tamagoWazaIgnoreData = tamago_waza_ignore_table.data();
            size_t tamagoWazaIgnoreSize = tamago_waza_ignore_table.size();

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
                        gen.next(); // Egg Move
                    }
                    break;
                }
                offset += wazaCount * 2;
            }

            if (this->species != 0 && this->species != species)
            {
                continue;
            }

            if (filter.comparePID(state) && filter.compareIV(state))
            {
                states.emplace_back(state);
            }
        }

        if (rareRand < 50)
        {
            UgState state(initialAdvances + cnt);
            state.setSpecies(rareMonsNo);
            gen.next(); // Level
            gen.next(); // EC
            u32 sidtid = gen.next();
            u32 pid = 0;
            u8 pidRolls = bonus ? 2 : 1;
            for (auto i = 0; i < pidRolls; i++)
            {
                pid = gen.next();

                u16 psv = (pid >> 16) ^ (pid & 0xffff);
                u16 fakeXor = (sidtid >> 16) ^ (sidtid & 0xffff) ^ psv;

                if (fakeXor < 16) // Force shiny
                {
                    u8 fakeShinyType = fakeXor == 0 ? 2 : 1;

                    u16 realXor = psv ^ tsv;
                    u8 realShinyType = realXor == 0 ? 2 : realXor < 16 ? 1 : 0;

                    state.setShiny(fakeShinyType);
                    if (realShinyType != fakeShinyType)
                    {
                        u16 high = (pid & 0xFFFF) ^ tsv ^ (2 - fakeShinyType);
                        pid = (high << 16) | (pid & 0xFFFF);
                    }
                    break;
                }
                else // Force non
                {
                    state.setShiny(0);
                    if ((psv ^ tsv) < 16)
                    {
                        pid ^= 0x10000000;
                    }
                }
            }

            state.setPID(pid);

            for (u8 i = 0; i < 6; i++)
            {
                state.setIV(i, gen.next() % 32);
            }

            state.setAbility(gen.next() % 2);

            PersonalInfo info = PersonalLoader::getPersonal(version, rareMonsNo);

            u8 genderRatio = info.getGender();

            if (genderRatio == 255)
            {
                state.setGender(2);
            }
            else if (genderRatio == 254)
            {
                state.setGender(1);
            }
            else if (genderRatio == 0)
            {
                state.setGender(0);
            }
            else
            {
                u8 gender = (gen.next() % 253) + 1 < genderRatio;
                state.setGender(gender);
            }

            state.setNature(gen.next() % 25);

            gen.advance(4); // 2 calls height, 2 calls weight

            state.setItem(gen.next<0, 100>());

            if (this->species != 0 && this->species != rareMonsNo)
            {
                continue;
            }

            if (filter.comparePID(state) && filter.compareIV(state))
            {
                states.emplace_back(state);
            }
        }
    }

    return states;
}
