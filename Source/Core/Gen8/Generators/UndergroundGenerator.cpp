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

struct EggMoveList
{
    u16 move[16];
    u16 specie;
    u8 count;
};

constexpr EggMoveList eggMoveList[141] = { { { 130, 80, 174, 275, 267, 204, 345, 133, 437, 438, 124, 580 }, 1, 12 },
                                           { { 187, 246, 44, 407, 232, 68, 17, 525, 200, 251, 349, 242, 314 }, 4, 13 },
                                           { { 243, 114, 54, 175, 281, 252, 392, 453, 323, 791, 330, 396 }, 7, 12 },
                                           { { 103, 172, 154, 68, 179, 253, 387, 279, 515 }, 19, 9 },
                                           { { 21, 180, 251, 305, 184, 342, 50, 415, 389 }, 23, 9 },
                                           { { 68, 175, 189, 400, 232, 468, 306, 341 }, 27, 8 },
                                           { { 68, 50, 342, 130, 48, 36, 305, 116, 204, 251, 599 }, 29, 11 },
                                           { { 93, 68, 50, 457, 342, 389, 48, 36, 32, 37, 133, 251, 599 }, 32, 13 },
                                           { { 95, 175, 336, 262, 608, 488, 257, 394, 384, 506 }, 37, 10 },
                                           { { 174, 16, 95, 98, 18, 17, 428, 413, 599 }, 41, 9 },
                                           { { 75, 175, 235, 275, 321, 298, 267, 495, 668, 73, 204 }, 43, 11 },
                                           { { 103, 68, 60, 175, 230, 232, 450, 440, 97, 73, 469, 565, 580 }, 46, 13 },
                                           { { 226, 103, 97, 234, 390, 450, 476 }, 48, 7 },
                                           { { 499, 109, 238, 95, 60, 493, 281, 248, 227 }, 54, 9 },
                                           { { 68, 179, 251, 279, 227, 400 }, 56, 6 },
                                           { { 37, 38, 234, 343, 24, 34, 83, 257, 370, 682 }, 58, 10 },
                                           { { 283, 114, 54, 170, 150, 227 }, 60, 6 },
                                           { { 93, 470, 277, 227, 282, 7, 9, 8, 379, 385, 375, 502 }, 63, 12 },
                                           { { 68, 418, 321, 501, 66, 227, 370, 7, 9, 8, 379, 484 }, 66, 12 },
                                           { { 227, 235, 141, 275, 345, 388, 321, 311, 499, 438, 491, 562, 668 }, 69, 13 },
                                           { { 367, 392, 62, 109, 114, 282, 243, 229, 321, 330 }, 72, 10 },
                                           { { 5, 335, 359, 175, 174, 475, 469 }, 74, 7 },
                                           { { 37, 24, 95, 38, 32, 234, 204, 67, 502, 667 }, 77, 10 },
                                           { { 562, 187, 335, 23, 248, 173, 472 }, 79, 7 },
                                           { { 16, 98, 175, 297, 174, 343, 400, 493, 515, 364, 143, 189, 279 }, 83, 13 },
                                           { { 48, 114, 175, 413, 372 }, 84, 5 },
                                           { { 122, 195, 50, 32, 21, 252, 333, 254, 256, 255, 562, 494 }, 86, 12 },
                                           { { 114, 212, 122, 286, 174, 325, 425, 254, 256, 255, 184, 491, 612 }, 88, 13 },
                                           { { 61, 392, 791, 36, 229, 103, 333, 341, 350 }, 90, 9 },
                                           { { 195, 114, 310, 288, 50, 499, 123, 513, 7, 8, 9, 184 }, 92, 12 },
                                           { { 175, 335, 111, 469, 205, 525, 457, 350, 484 }, 95, 9 },
                                           { { 272, 7, 9, 8, 260, 427, 385, 471 }, 96, 8 },
                                           { { 282, 246, 359, 163, 400, 114, 133, 321, 97, 502 }, 98, 10 },
                                           { { 246, 187, 197, 24, 195, 130, 174, 43, 103, 442 }, 104, 10 },
                                           { { 562, 174, 359, 37, 34, 173, 133, 330, 428 }, 108, 9 },
                                           { { 60, 220, 288, 180, 174, 254, 256, 255, 103, 390, 599 }, 109, 11 },
                                           { { 68, 174, 407, 130, 368, 470, 242, 179, 306, 423, 424, 422 }, 111, 12 },
                                           { { 93, 283, 175, 73, 267, 476, 133, 437, 384 }, 114, 9 },
                                           { { 62, 499, 50, 175, 190, 150, 330, 200 }, 116, 8 },
                                           { { 95, 252, 109, 471, 321, 248, 271, 478, 196 }, 122, 9 },
                                           { { 68, 400, 364, 501, 226, 179 }, 123, 6 },
                                           { { 364, 175, 37, 31, 98, 370 }, 127, 6 },
                                           { { 453, 225, 245, 54, 48, 114 }, 147, 6 },
                                           { { 22, 73, 68, 246, 175, 275, 437, 505, 580 }, 152, 9 },
                                           { { 154, 179, 37, 343, 336, 306, 24, 394, 326, 267 }, 155, 10 },
                                           { { 246, 8, 232, 349, 453, 313, 335, 260 }, 158, 8 },
                                           { { 38, 163, 116, 271, 387, 204, 343, 608 }, 161, 8 },
                                           { { 48, 17, 18, 297, 101, 143, 97, 212, 542 }, 163, 9 },
                                           { { 60, 103, 227, 282, 450, 366, 68 }, 165, 7 },
                                           { { 60, 50, 226, 390, 476, 400, 224, 679 }, 167, 8 },
                                           { { 60, 54, 487, 103, 133, 250, 97 }, 170, 7 },
                                           { { 217, 64, 375, 326, 234, 248, 500 }, 175, 7 },
                                           { { 65, 98, 297, 389, 493, 114, 428, 502 }, 177, 8 },
                                           { { 34, 103, 260, 28, 495, 97, 598, 604 }, 179, 8 },
                                           { { 93, 227, 38, 133, 270, 312, 538, 402, 580, 668 }, 187, 10 },
                                           { { 68, 180, 21, 251, 252, 343, 340, 279, 415, 501 }, 190, 10 },
                                           { { 227, 267, 174, 270, 230, 234, 580 }, 191, 7 },
                                           { { 246, 174, 254, 256, 255, 68, 24, 105, 495, 491, 612, 34, 227, 385, 598 }, 194, 15 },
                                           { { 18, 65, 143, 109, 297, 195, 375, 103, 413, 260 }, 198, 10 },
                                           { { 103, 194, 286, 262, 389, 425, 174, 472 }, 200, 8 },
                                           { { 36, 133, 248, 251, 273, 277, 24, 243, 212 }, 203, 9 },
                                           { { 42, 175, 129, 68, 328, 279, 390, 379 }, 204, 8 },
                                           { { 232, 17, 68, 328, 97, 226, 38, 364, 400, 440, 379, 342 }, 207, 12 },
                                           { { 118, 217, 215, 173, 370, 38, 102, 313 }, 209, 8 },
                                           { { 175, 114, 61, 48, 310, 453, 491 }, 211, 7 },
                                           { { 230, 282, 367, 51, 515, 111, 611, 343, 270, 328, 189, 350 }, 213, 12 },
                                           { { 68, 180, 44, 252, 458, 420, 364, 556, 306, 8, 675 }, 215, 11 },
                                           { { 242, 36, 69, 68, 232, 281, 238, 38, 370, 400, 187, 583 }, 216, 12 },
                                           { { 151, 257, 174, 108, 262, 254, 255, 256, 205, 517, 385 }, 218, 11 },
                                           { { 44, 246, 38, 90, 174, 556, 573, 34, 341, 333, 341 }, 220, 11 },
                                           { { 54, 109, 267, 174, 457, 103, 133, 275, 333, 710 }, 222, 10 },
                                           { { 190, 48, 114, 175, 323, 491, 103, 350, 173, 341, 129, 494 }, 223, 12 },
                                           { { 62, 98, 150, 229, 420, 252, 573, 194, 68, 694, 262, 248, 8, 196, 191 }, 225, 15 },
                                           { { 83, 68, 180, 179, 422, 364, 389, 194 }, 228, 8 },
                                           { { 116, 34, 246, 173, 68, 90, 283, 420, 457, 189, 484, 583, 667 }, 231, 13 },
                                           { { 229, 136, 183, 170, 68, 410, 418, 364 }, 236, 8 },
                                           { { 252, 273, 272 }, 238, 3 },
                                           { { 238, 223, 359, 364, 7, 8 }, 239, 6 },
                                           { { 562, 223, 183, 238, 5, 103, 9, 394, 187, 116, 384 }, 240, 11 },
                                           { { 562, 174, 38, 359, 217, 69, 179, 270 }, 241, 8 },
                                           { { 23, 246, 174, 200, 116, 349, 334, 372, 442 }, 246, 9 },
                                           { { 71, 163, 400, 24, 225, 73, 388, 235, 242, 306, 345, 580 }, 252, 12 },
                                           { { 64, 306, 174, 68, 364, 387, 400, 226, 97, 67 }, 255, 10 },
                                           { { 189, 44, 246, 68, 174, 38, 243, 124, 23, 469, 281, 253, 250 }, 258, 13 },
                                           { { 310, 305, 343, 43, 423, 424, 422 }, 261, 7 },
                                           { { 321, 493, 245, 204, 271, 270, 189 }, 263, 7 },
                                           { { 235, 75, 230, 73, 321, 68, 298 }, 270, 7 },
                                           { { 73, 98, 36, 388, 400, 133, 384, 492, 251, 580 }, 273, 10 },
                                           { { 239, 16, 392, 282, 487, 469, 314 }, 278, 7 },
                                           { { 50, 212, 262, 194, 288, 425, 109, 282, 227, 581, 502 }, 280, 11 },
                                           { { 341, 60, 56, 170, 450, 565, 471, 679 }, 283, 8 },
                                           { { 16, 400, 450, 515, 175 }, 290, 5 },
                                           { { 270, 253, 313, 273, 226, 321, 387, 428, 389, 493, 322 }, 300, 11 },
                                           { { 105, 260, 364, 389, 368, 236, 271, 286 }, 302, 8 },
                                           { { 246, 321, 21, 69, 612, 305, 423, 424, 422, 385, 368, 581 }, 303, 12 },
                                           { { 174, 407, 283, 457, 23, 189, 34, 103, 276, 179 }, 304, 10 },
                                           { { 7, 9, 8, 252, 226, 223, 384, 385, 427, 418, 501 }, 307, 11 },
                                           { { 226, 271, 69, 679 }, 313, 4 },
                                           { { 226, 74, 313, 109, 312 }, 314, 5 },
                                           { { 178, 79, 75, 326, 791, 191, 42, 170, 437, 402, 438 }, 315, 11 },
                                           { { 246, 194, 38, 37, 56, 129 }, 318, 6 },
                                           { { 336, 184, 34, 205, 111, 23, 246, 257, 254, 256, 255, 442, 74, 484 }, 322, 14 },
                                           { { 281, 90, 130, 175, 246, 276 }, 324, 6 },
                                           { { 98, 16, 175, 210, 450, 364, 116, 341 }, 328, 8 },
                                           { { 51, 298, 223, 68, 67, 345, 402, 50, 335, 388, 415, 565, 562, 612 }, 331, 14 },
                                           { { 407, 297, 114, 366, 310, 97, 384, 304, 583 }, 333, 9 },
                                           { { 175, 24, 68, 174, 154, 400, 232, 458, 50, 515, 364, 501, 187 }, 335, 13 },
                                           { { 254, 256, 255, 34, 184, 372, 415, 515 }, 336, 8 },
                                           { { 37, 209, 175, 36, 250, 56, 349, 414, 341 }, 339, 9 },
                                           { { 246, 232, 38, 453, 415, 163, 34, 276, 349 }, 341, 9 },
                                           { { 277, 271, 252, 105, 612 }, 352, 5 },
                                           { { 50, 194, 310, 286, 109, 441 }, 353, 6 },
                                           { { 220, 288, 262, 114, 286, 194 }, 355, 6 },
                                           { { 29, 21, 73, 267, 174, 348 }, 357, 6 },
                                           { { 174, 38, 277, 212, 44, 364, 226, 428, 372, 224, 506, 583 }, 359, 12 },
                                           { { 335, 50, 415, 205, 556, 191, 311, 506, 313 }, 361, 9 },
                                           { { 174, 90, 254, 256, 255, 281, 392, 187 }, 363, 8 },
                                           { { 34, 48, 109, 392, 330 }, 366, 5 },
                                           { { 111, 407, 37, 239, 56, 349, 424 }, 371, 7 },
                                           { { 388, 321, 34, 38, 328, 402, 37, 133, 276, 254, 256, 255, 414, 469, 580, 484 }, 387, 16 },
                                           { { 7, 9, 24, 227, 257, 116, 270, 252, 299, 68, 501, 66 }, 390, 12 },
                                           { { 458, 48, 281, 189, 173, 175, 97, 392, 297, 196 }, 393, 10 },
                                           { { 98, 38, 154, 401, 130 }, 399, 5 },
                                           { { 608, 36, 24, 336, 400, 98, 423, 424, 422, 129, 270, 598, 313 }, 403, 13 },
                                           { { 235, 178, 79, 75, 326, 791, 191, 42, 170, 437, 402 }, 406, 11 },
                                           { { 343, 44, 313, 111, 205, 260, 175, 39, 266, 268 }, 417, 10 },
                                           { { 189, 29, 154, 163, 210, 226, 392, 415, 487, 270 }, 418, 10 },
                                           { { 75, 230, 321, 267, 312, 505, 361, 111, 579, 205, 311, 402, 580 }, 420, 13 },
                                           { { 68, 243, 254, 256, 255, 281, 174, 124, 499, 54, 151, 133, 90 }, 422, 13 },
                                           { { 509, 383, 458, 252, 175, 186, 415, 298, 313, 227, 67, 9, 8, 7, 322, 612 }, 427, 16 },
                                           { { 44, 39, 98, 28, 313, 372, 175, 387 }, 431, 8 },
                                           { { 50, 174, 95, 273, 248, 105, 500, 322 }, 433, 8 },
                                           { { 43, 123, 38, 310, 114, 163, 242, 184, 492, 583 }, 434, 10 },
                                           { { 37, 232, 38, 239, 200, 184, 34, 442, 341 }, 443, 9 },
                                           { { 38, 174, 90, 68, 562, 204, 18, 428, 495, 120 }, 446, 10 },
                                           { { 254, 256, 255, 174, 18, 34, 279 }, 449, 7 },
                                           { { 28, 163, 109, 18, 103, 97, 342 }, 451, 7 },
                                           { { 364, 223, 29, 410, 252, 238, 418, 68, 501, 367 }, 453, 10 },
                                           { { 239, 114, 21, 243, 150, 366, 109, 133 }, 458, 8 },
                                           { { 73, 74, 38, 23, 130, 345, 402 }, 459, 7 },
                                           { { 68, 243, 254, 256, 255, 281, 174, 124, 499, 54, 151, 133, 90 }, 422, 13 } };

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

static u32 rand(u32 prng)
{
    return (prng % 0xffffffff) + 0x80000000;
}

static float randF(u32 prng)
{
    float t = (prng & 0x7fffff) / 8388607.0;
    return 1.0 - t;
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
            float rareESRand = rngList.next(randF) * specialRatesSum;
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
            float typeRand = rngList.next(randF) * typeRatesSum;
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
            float slotRand = rngList.next(randF) * pokeRatesSum;
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

            const auto *eggMoves = std::lower_bound(std::begin(eggMoveList), std::end(eggMoveList), info->getHatchSpecie(),
                                                    [](const EggMoveList &eggMove, u16 specie) { return eggMove.specie < specie; });
            if (eggMoves != std::end(eggMoveList))
            {
                rngList.next(rand); // Egg Move
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

    return states;
}
