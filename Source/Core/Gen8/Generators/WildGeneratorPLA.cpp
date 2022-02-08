#include "WildGeneratorPLA.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Parents/States/State.hpp>
#include <Core/RNG/RNGList.hpp>
#include <Core/RNG/Xoroshiro.hpp>

WildGeneratorPLA::WildGeneratorPLA(u32 initialAdvances, u32 maxAdvances, u16 tid, u16 sid, u8 genderRatio, const StateFilter &filter,
                                   u8 rolls, bool isAlpha) :
    Generator(initialAdvances, maxAdvances, tid, sid, genderRatio, Method::None, filter), rolls(rolls), isAlpha(isAlpha)
{
}

std::vector<State> WildGeneratorPLA::generate(u64 seed) const
{
    std::vector<State> states;

    seed -= 0x82A2B175229D6A5B;

    for (u32 advance = 0; advance <= maxAdvances; advance++)
    {
        State result(initialAdvances + advance);
        Xoroshiro mainRng(seed);
        u64 spawnerSeed = mainRng.next();
        mainRng.next(); // unused (?) second spawner seed

        Xoroshiro groupRng(spawnerSeed);
        groupRng.next(); // level
        u64 fixedSeed = groupRng.next();

        Xoroshiro fixedRng(fixedSeed);

        u32 ec = fixedRng.nextUInt<0xffffffff>();

        result.setShiny(0);

        u32 sidtid = fixedRng.nextUInt<0xffffffff>();
        u32 pid = 0;
        for (u8 i = 0; i < rolls; i++)
        {
            pid = fixedRng.nextUInt<0xffffffff>();
            u16 psv = (pid >> 16) ^ (pid & 0xffff);

            u16 fakeXor = (sidtid >> 16) ^ (sidtid & 0xffff) ^ psv;

            if (fakeXor < 16) // Force shiny
            {
                u8 fakeShinyType = fakeXor == 0 ? 2 : 1;

                u16 realXor = psv ^ tsv;
                u8 realShinyType = realXor == 0 ? 2 : realXor < 16 ? 1 : 0;

                result.setShiny(fakeShinyType);
                if (realShinyType != fakeShinyType)
                {
                    u16 high = (pid & 0xFFFF) ^ tsv ^ (2 - fakeShinyType);
                    pid = (high << 16) | (pid & 0xFFFF);
                }
                break;
            }
            else // Force non shiny
            {
                if ((psv ^ tsv) < 16)
                {
                    pid ^= 0x10000000;
                }
            }
        }
        result.setPID(pid);

        if (!filter.compareShiny(result))
        {
            continue;
        }

        for (u8 i = 0; i < 6; i++)
        {
            result.setIV(i, 255);
        }

        if (isAlpha)
        {
            for (u8 i = 0; i < 3;)
            {
                u8 index = fixedRng.nextUInt<6>();
                if (result.getIV(i) == 255)
                {
                    result.setIV(i, 31);
                    i++;
                }
            }
        }

        for (u8 i = 0; i < 6; i++)
        {
            if (result.getIV(i) == 255)
            {
                result.setIV(i, fixedRng.nextUInt<32>());
            }
        }

        result.setAbility(fixedRng.nextUInt<2>());

        if (genderRatio == 255)
        {
            result.setGender(2);
        }
        else if (genderRatio == 254)
        {
            result.setGender(1);
        }
        else if (genderRatio == 0)
        {
            result.setGender(0);
        }
        else
        {
            result.setGender((fixedRng.nextUInt<253>() + 1) < genderRatio);
        }

        // Height (2 calls)
        // Weight (2 calls)

        if (filter.comparePID(result) && filter.compareIV(result))
        {
            states.emplace_back(result);
        }

        seed = mainRng.next();
    }

    return states;
}
