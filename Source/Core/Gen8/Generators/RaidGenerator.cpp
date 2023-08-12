/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "RaidGenerator.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Gen8/Raid.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/States/State.hpp>
#include <Core/RNG/Xoroshiro.hpp>

constexpr u8 toxtricityAmpedNatures[] = { 3, 4, 2, 8, 9, 19, 22, 11, 13, 14, 0, 6, 24 };
constexpr u8 toxtricityLowKeyNatures[] = { 1, 5, 7, 10, 12, 15, 16, 17, 18, 20, 21, 23 };

RaidGenerator::RaidGenerator(u32 initialAdvances, u32 maxAdvances, u32 delay, const Profile8 &profile, const StateFilter &filter) :
    Generator(initialAdvances, maxAdvances, delay, Method::None, profile, filter)
{
}

std::vector<GeneratorState> RaidGenerator::generate(u64 seed, u8 level, const Raid &raid) const
{
    const PersonalInfo *info = raid.getInfo();
    seed += 0x82A2B175229D6A5B * (initialAdvances + delay);

    std::vector<GeneratorState> states;
    for (u32 cnt = 0; cnt <= maxAdvances; cnt++, seed += 0x82A2B175229D6A5B)
    {
        Xoroshiro rng(seed);

        u32 ec = rng.nextUInt<0xffffffff>();
        u32 sidtid = rng.nextUInt<0xffffffff>();
        u32 pid = rng.nextUInt<0xffffffff>();
        u16 psv = (pid >> 16) ^ (pid & 0xffff);
        u8 shiny;
        if (raid.getShiny() == Shiny::Random) // Random shiny chance
        {
            // Game uses a fake TID/SID to determine shiny or not
            // PID is later modified using the actual TID/SID of trainer if necessary
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
            }
            else // Force non shiny
            {
                shiny = 0;
                if ((psv ^ tsv) < 16)
                {
                    pid ^= 0x10000000;
                }
            }
        }
        else if (raid.getShiny() == Shiny::Never) // Force non-shiny
        {
            shiny = 0;
            if ((psv ^ tsv) < 16)
            {
                pid ^= 0x10000000;
            }
        }
        else // Force shiny
        {
            shiny = 2;
            u16 realXor = psv ^ tsv;
            if (realXor != 0) // Check if PID is not normally square shiny
            {
                // Force shiny (makes it square)
                u16 high = (pid & 0xffff) ^ tsv;
                pid = (high << 16) | (pid & 0xffff);
            }
        }

        // Set IVs that will be 31s
        std::array<u8, 6> ivs = { 255, 255, 255, 255, 255, 255 };
        for (u8 i = 0; i < raid.getIVCount();)
        {
            u8 index = rng.nextUInt<6>();
            if (ivs[index] == 255)
            {
                ivs[index] = 31;
                i++;
            }
        }

        // Fill rest of IVs with rand calls
        for (u8 &iv : ivs)
        {
            if (iv == 255)
            {
                iv = rng.nextUInt<32>();
            }
        }

        u8 ability;
        if (raid.getAbility() == 4) // Allow hidden ability
        {
            ability = rng.nextUInt<3>();
        }
        else if (raid.getAbility() == 3) // No hidden ability
        {
            ability = rng.nextUInt<2>();
        }
        else // Locked ability
        {
            ability = raid.getAbility();
        }

        // Altform, doesn't seem to have a rand call for raids

        u8 gender;
        if (raid.getGender() == 0) // Random
        {
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
                gender = (rng.nextUInt<253>() + 1) < info->getGender();
                break;
            }
        }
        else if (raid.getGender() == 1) // Male
        {
            gender = 0;
        }
        else if (raid.getGender() == 2) // Female
        {
            gender = 1;
        }
        else if (raid.getGender() == 3) // Genderless
        {
            gender = 2;
        }

        u8 nature;
        if (raid.getSpecie() != 849)
        {
            nature = rng.nextUInt<25>();
        }
        else
        {
            if (raid.getForm() == 0)
            {
                nature = toxtricityAmpedNatures[rng.nextUInt<13>()];
            }
            else
            {
                nature = toxtricityLowKeyNatures[rng.nextUInt<12>()];
            }
        }

        // Height (2 calls)
        // Weight (2 calls)

        GeneratorState state(initialAdvances + cnt, ec, pid, ivs, ability, gender, level, nature, shiny, info);
        if (filter.compareState(static_cast<const State &>(state)))
        {
            states.emplace_back(state);
        }
    }

    return states;
}
