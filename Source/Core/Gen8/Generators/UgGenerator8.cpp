#include "UgGenerator8.hpp"
#include <Core/RNG/Xorshift.hpp>
#include <Core/Resources/Encounters.hpp>
#include <algorithm>
#include <cmath>

UgGenerator8::UgGenerator8(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, const StateFilter &filter, bool bonus) :
    Generator(initialAdvances, maxAdvances, tid, sid, 0, Method::None, filter), bonus(bonus)
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
};

std::vector<UgState> UgGenerator8::generate(u64 seed0, u64 seed1, u8 randMarkID, u8 storyFlag, const Profile8 &profile) const
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
            if (profile.getVersion() == Game::BD)
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

    std::vector<const u8 *> enabledPokemon;

    const u8 *ugEncountData = ug_encount.data();
    size_t ugEncountSize = ug_encount.size();

    u8 version;
    if (profile.getVersion() == Game::BD)
    {
        version = 2;
    }
    else
    {
        version = 3;
    }

    for (size_t offset = 0; offset < ugEncountSize; offset += 5)
    {
        const u8 *entry = ugEncountData + offset;
        if (entry[0] == randMarkInfo[0] && (entry[3] == 1 || entry[3] == version) && entry[4] <= storyFlag)
        {
            enabledPokemon.emplace_back(entry);
        }
    }

    std::vector<TypeAndSize> monsDataIndexs;

    const u8 *ugPokemonData = ug_pokemon_data.data();
    size_t ugPokemonSize = ug_pokemon_data.size();

    for (auto &pokemon : enabledPokemon)
    {
        TypeAndSize ts;
        u16 monsno = *reinterpret_cast<const u16 *>(pokemon + 1);
        for (size_t offset = 0; offset < ugPokemonSize; offset += 12)
        {
            const u8 *entry = offset + ugPokemonData;
            if (monsno == *reinterpret_cast<const u16 *>(entry))
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
                    ts.value = std::pow(10.0, entry[4]) + entry[3];
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

    std::vector<u8> sizes;
    u8 smax = randMarkInfo[4];
    u8 mmax = randMarkInfo[5];
    u8 lmax = randMarkInfo[6];
    u8 llmax = randMarkInfo[7];

    while (smax > 0)
    {
        sizes.emplace_back(0);
        smax--;
    }

    while (mmax > 0)
    {
        sizes.emplace_back(1);
        mmax--;
    }

    while (lmax > 0)
    {
        sizes.emplace_back(2);
        lmax--;
    }

    while (llmax > 0)
    {
        sizes.emplace_back(3);
        llmax--;
    }

    std::vector<UgState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next())
    {
        UgState state(initialAdvances + cnt);
        u8 spawnCount = randMarkInfo[2];
        Xorshift gen(rng);
        u32 rareRand = gen.next<0, 100>();
        u16 rareMonsNo = 0;
        if (rareRand < 50)
        {
            float rareESRand = gen.nextf(0.0, specialRatesSum);
            for (auto &p : specialPokemonRates)
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
            for (auto &tr : typeRates)
            {
                if (typeRand < tr.rate)
                {
                    type = tr.type;
                    break;
                }
                typeRand -= tr.rate;
            }

            std::vector<u8> existSizeList;

            for (auto &ts : monsDataIndexs)
            {
                if (ts.type == type && std::count(existSizeList.begin(), existSizeList.end(), ts.size) == 0)
                {
                    existSizeList.emplace_back(ts.size);
                }
            }

            if (std::all_of(sizes.begin(), sizes.end(),
                            [&](u8 i) { return std::find(existSizeList.begin(), existSizeList.end(), i) != existSizeList.end(); }))
            {
                sizes.erase(std::remove_if(sizes.begin(), sizes.end(), [&](u8 i) {
                    return std::find(existSizeList.begin(), existSizeList.end(), i) == existSizeList.end();
                }));
            }
            else
            {
                sizes.erase(std::remove_if(sizes.begin(), sizes.end(), [&](u8 i) {
                    return std::find(existSizeList.begin(), existSizeList.end(), i) != existSizeList.end();
                }));
            }

            u8 size = 0;
            if (sizes.size() != 0)
            {
                u32 sizeRand = gen.next(0, sizes.size());
                size = sizes[sizeRand];
                sizes.erase(std::next(sizes.begin(), sizeRand));
            }
            else
            {
                u32 sizeRand = gen.next(0, existSizeList.size());
                size = existSizeList[sizeRand];
            }

            TypeAndSize ts;
            ts.type = type;
            ts.size = size;
            ts.value = std::pow(10.0, size) + type;

            pokeSlots.emplace_back(ts);
        }

        for (auto &ts : pokeSlots)
        {
            std::vector<PokeRate> pokeRates;
            u16 pokeRatesSum = 0;
            for (auto &p : enabledPokemon)
            {
                const u8 *pokemonData = ugPokemonData + (*reinterpret_cast<const u16 *>(p + 1)) * 12;
                if (std::any_of(monsDataIndexs.begin(), monsDataIndexs.end(), [&](TypeAndSize ts2) {
                        return ts2.value == ts.value && (ts.type == pokemonData[2] || ts.type == pokemonData[3])
                            && ts.size == pokemonData[4];
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
            for (auto &pr : pokeRates)
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
            u32 sidtid = gen.next();
            u32 pid;
            u8 pidRolls = bonus ? 2 : 1;
            for (auto i = 0; i < pidRolls; i++)
            {
                u32 tmpPID = gen.next();

                u16 psv = (tmpPID >> 16) ^ (tmpPID & 0xffff);
                u16 fakeXor = (sidtid >> 16) ^ (sidtid & 0xffff) ^ psv;

                if (fakeXor < 16) // Force shiny
                {
                    u8 fakeShinyType = fakeXor == 0 ? 2 : 1;

                    u16 realXor = psv ^ tsv;
                    u8 realShinyType = realXor == 0 ? 2 : realXor < 16 ? 1 : 0;

                    state.setShiny(fakeShinyType);
                    if (realShinyType != fakeShinyType)
                    {
                        u16 high = (tmpPID & 0xFFFF) ^ tsv ^ (2 - fakeShinyType);
                        pid = (high << 16) | (tmpPID & 0xFFFF);
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

            PersonalInfo info = PersonalLoader::getPersonal(profile.getVersion(), species);

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

            states.emplace_back(state);
        }

        if (rareRand < 50)
        {
            gen.next(); // Level
            gen.next(); // EC
            u32 sidtid = gen.next();
            u32 pid;
            u8 pidRolls = bonus ? 2 : 1;
            for (auto i = 0; i < pidRolls; i++)
            {
                u32 tmpPID = gen.next();

                u16 psv = (tmpPID >> 16) ^ (tmpPID & 0xffff);
                u16 fakeXor = (sidtid >> 16) ^ (sidtid & 0xffff) ^ psv;

                if (fakeXor < 16) // Force shiny
                {
                    u8 fakeShinyType = fakeXor == 0 ? 2 : 1;

                    u16 realXor = psv ^ tsv;
                    u8 realShinyType = realXor == 0 ? 2 : realXor < 16 ? 1 : 0;

                    state.setShiny(fakeShinyType);
                    if (realShinyType != fakeShinyType)
                    {
                        u16 high = (tmpPID & 0xFFFF) ^ tsv ^ (2 - fakeShinyType);
                        pid = (high << 16) | (tmpPID & 0xFFFF);
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

            PersonalInfo info = PersonalLoader::getPersonal(profile.getVersion(), rareMonsNo);

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

            states.emplace_back(state);
        }
    }

    return states;
}
