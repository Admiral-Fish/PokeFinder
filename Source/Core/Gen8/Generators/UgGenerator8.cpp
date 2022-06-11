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

std::vector<UgState> UgGenerator8::generate(u64 seed0, u64 seed1, u8 randMarkID, u8 storyFlag, const PersonalInfo *info,
                                            const Profile8 &profile) const
{
    Xorshift rng(seed0, seed1);
    rng.advance(initialAdvances + offset);

    std::vector<PokeRate> specialPokemonRates;

    const u8 *specialPokemonData = ug_special_pokemon.data();
    size_t specialPokemonSize = ug_special_pokemon.size();

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

    for (size_t offset = 0; offset < 18; offset++)
    {
        u16 rate = (randMarkInfo + 8)[offset];
        if (std::count(monsDataIndexs.begin(), monsDataIndexs.end(), offset))
        {
            TypeRate tr;
            tr.rate = rate;
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
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, rng.next()) { }

    return states;
}
