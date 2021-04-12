/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "GameCubeSearcher.hpp"
#include <Core/Enum/Method.hpp>
#include <Core/Enum/ShadowType.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/RNG/RNGEuclidean.hpp>

GameCubeSearcher::GameCubeSearcher(u16 tid, u16 sid, u8 genderRatio, Method method, const StateFilter &filter) :
    Searcher(tid, sid, genderRatio, method, filter), searching(false), progress(0)
{
}

void GameCubeSearcher::startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max)
{
    searching = true;

    if (method == Method::Channel)
    {
        searchChannel(min[4], max[4]);
        return;
    }

    for (u8 hp = min[0]; hp <= max[0]; hp++)
    {
        for (u8 atk = min[1]; atk <= max[1]; atk++)
        {
            for (u8 def = min[2]; def <= max[2]; def++)
            {
                for (u8 spa = min[3]; spa <= max[3]; spa++)
                {
                    for (u8 spd = min[4]; spd <= max[4]; spd++)
                    {
                        for (u8 spe = min[5]; spe <= max[5]; spe++)
                        {
                            if (!searching)
                            {
                                return;
                            }

                            std::vector<GameCubeState> states = search(hp, atk, def, spa, spd, spe);

                            std::lock_guard<std::mutex> guard(mutex);
                            results.insert(results.end(), states.begin(), states.end());
                            progress++;
                        }
                    }
                }
            }
        }
    }
}

void GameCubeSearcher::cancelSearch()
{
    searching = false;
}

std::vector<GameCubeState> GameCubeSearcher::getResults()
{
    std::lock_guard<std::mutex> guard(mutex);
    auto data = std::move(results);
    return data;
}

int GameCubeSearcher::getProgress() const
{
    return progress;
}

void GameCubeSearcher::setupNatureLock(u8 num)
{
    lock = ShadowLock(num, method);
    type = lock.getType();
}

std::vector<GameCubeState> GameCubeSearcher::search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe)
{
    switch (method)
    {
    case Method::XDColo:
        return searchXDColo(hp, atk, def, spa, spd, spe);
    case Method::XD:
        return searchXDShadow(hp, atk, def, spa, spd, spe);
    case Method::Colo:
        return searchColoShadow(hp, atk, def, spa, spd, spe);
    default:
        return std::vector<GameCubeState>();
    }
}

std::vector<GameCubeState> GameCubeSearcher::searchXDColo(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe)
{
    std::vector<GameCubeState> states;

    GameCubeState state;
    state.setIVs(hp, atk, def, spa, spd, spe);
    state.calculateHiddenPower();
    if (!filter.compareHiddenPower(state))
    {
        return states;
    }

    auto seeds = RNGEuclidean::recoverLower16BitsIV(hp, atk, def, spa, spd, spe);
    for (const auto &pair : seeds)
    {
        // Setup normal state
        XDRNG rng(pair.second);

        u8 ability = rng.nextUShort() & 1;
        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();

        state.setSeed(pair.first * 0xB9B33155 + 0xA170F641);
        state.setPID(high, low);
        state.setAbility(ability);
        state.setGender(low & 255, genderRatio);
        state.setNature(state.getPID() % 25);
        state.setShiny(tsv, high ^ low, 8);

        if (filter.comparePID(state))
        {
            states.emplace_back(state);
        }

        // Setup XORed state
        state.setPID(state.getPID() ^ 0x80008000);
        state.setNature(state.getPID() % 25);
        if (filter.comparePID(state))
        {
            state.setSeed(state.getSeed() ^ 0x80000000);
            states.emplace_back(state);
        }
    }
    return states;
}

std::vector<GameCubeState> GameCubeSearcher::searchXDShadow(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe)
{
    std::vector<GameCubeState> states;

    GameCubeState state;
    state.setIVs(hp, atk, def, spa, spd, spe);
    state.calculateHiddenPower();
    if (!filter.compareHiddenPower(state))
    {
        return states;
    }

    auto seeds = RNGEuclidean::recoverLower16BitsIV(hp, atk, def, spa, spd, spe);
    for (const auto &pair : seeds)
    {
        // Setup normal state
        XDRNG rng(pair.second);

        u8 ability = rng.nextUShort() & 1;
        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();

        while ((tsv ^ high ^ low) < 8)
        {
            high = rng.nextUShort();
            low = rng.nextUShort();
        }

        state.setSeed(pair.first * 0xB9B33155 + 0xA170F641);
        state.setPID(high, low);
        state.setAbility(ability);
        state.setGender(low & 255, genderRatio);
        state.setNature(state.getPID() % 25);
        state.setShiny(0);

        if (filter.comparePID(state))
        {
            switch (type)
            {
            case ShadowType::SingleLock:
                if (lock.singleNL(state.getSeed(), tsv))
                {
                    state.setInfo(0);
                    states.emplace_back(state); // If this seed passes it is impossible for the sister spread to generate
                    continue;
                }
                break;
            case ShadowType::FirstShadow:
                if (lock.firstShadowNormal(state.getSeed(), tsv))
                {
                    state.setInfo(0);
                    states.emplace_back(state); // If this seed passes it is impossible for the sister spread to generate
                    continue;
                }
                break;
            case ShadowType::SecondShadow:
                if (lock.firstShadowUnset(state.getSeed(), tsv))
                {
                    state.setInfo(1); // Also unlikely for the other methods of encounter to pass
                    states.emplace_back(state); // If this seed passes it is impossible for the sister spread to generate
                    continue;
                }
                if (lock.firstShadowSet(state.getSeed(), tsv))
                {
                    state.setInfo(2); // Also unlikely for the other methods of encounter to pass
                    states.emplace_back(state); // If this seed passes it is impossible for the sister spread to generate
                    continue;
                }
                break;
            case ShadowType::Salamence:
                if (lock.salamenceUnset(state.getSeed(), tsv))
                {
                    state.setInfo(1); // Also unlikely for the other methods of encounter to pass
                    states.emplace_back(state); // If this seed passes it is impossible for the sister spread to generate
                    continue;
                }
                if (lock.salamenceSet(state.getSeed(), tsv))
                {
                    state.setInfo(2); // Also unlikely for the other methods of encounter to pass
                    states.emplace_back(state); // If this seed passes it is impossible for the sister spread to generate
                    continue;
                }
                break;
            default:
                break;
            }
        }

        // Setup XORed state
        state.setSeed(state.getSeed() ^ 0x80000000);
        state.setPID(state.getPID() ^ 0x80008000);
        state.setNature(state.getPID() % 25);

        if (filter.comparePID(state))
        {
            switch (type)
            {
            case ShadowType::SingleLock:
                if (lock.singleNL(state.getSeed(), tsv))
                {
                    state.setInfo(0);
                    states.emplace_back(state);
                }
                break;
            case ShadowType::FirstShadow:
                if (lock.firstShadowNormal(state.getSeed(), tsv))
                {
                    state.setInfo(0);
                    states.emplace_back(state);
                }
                break;
            case ShadowType::SecondShadow:
                if (lock.firstShadowUnset(state.getSeed(), tsv))
                {
                    state.setInfo(1);
                    states.emplace_back(state);
                }
                else if (lock.firstShadowSet(state.getSeed(), tsv))
                {
                    state.setInfo(2);
                    states.emplace_back(state);
                }
                break;
            case ShadowType::Salamence:
                if (lock.salamenceUnset(state.getSeed(), tsv))
                {
                    state.setInfo(1);
                    states.emplace_back(state);
                }
                else if (lock.salamenceSet(state.getSeed(), tsv))
                {
                    state.setInfo(2);
                    states.emplace_back(state);
                }
                break;
            default:
                break;
            }
        }
    }
    return states;
}

std::vector<GameCubeState> GameCubeSearcher::searchColoShadow(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe)
{
    std::vector<GameCubeState> states;

    GameCubeState state;
    state.setIVs(hp, atk, def, spa, spd, spe);
    state.calculateHiddenPower();
    if (!filter.compareHiddenPower(state))
    {
        return states;
    }

    auto seeds = RNGEuclidean::recoverLower16BitsIV(hp, atk, def, spa, spd, spe);
    for (const auto &pair : seeds)
    {
        // Setup normal state
        XDRNG rng(pair.second);

        u8 ability = rng.nextUShort() & 1;
        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();

        state.setSeed(pair.first * 0xB9B33155 + 0xA170F641);
        state.setPID(high, low);
        state.setAbility(ability);
        state.setGender(low & 255, genderRatio);
        state.setNature(state.getPID() % 25);
        state.setShiny(tsv, high ^ low, 8);

        if (filter.comparePID(state))
        {
            switch (type)
            {
            case ShadowType::FirstShadow:
                if (lock.coloShadow(state.getSeed()))
                {
                    state.setInfo(0);
                    states.emplace_back(state); // If this seed passes it is impossible for the sister spread to generate
                    continue;
                }
                break;
            case ShadowType::EReader:
                if (lock.ereader(state.getSeed(), state.getPID()))
                {
                    state.setInfo(0);
                    states.emplace_back(state); // If this seed passes it is impossible for the sister spread to generate
                    continue;
                }
                break;
            default:
                break;
            }
        }

        // Setup XORed state
        state.setSeed(state.getSeed() ^ 0x80000000);
        state.setPID(state.getPID() ^ 0x80008000);
        state.setNature(state.getPID() % 25);

        if (filter.comparePID(state))
        {
            switch (type)
            {
            case ShadowType::FirstShadow:
                if (lock.coloShadow(state.getSeed()))
                {
                    state.setInfo(0);
                    states.emplace_back(state);
                }
                break;
            case ShadowType::EReader:
                if (lock.ereader(state.getSeed(), state.getPID()))
                {
                    state.setInfo(0);
                    states.emplace_back(state); // If this seed passes it is impossible for the sister spread to generate
                    continue;
                }
                break;
            default:
                break;
            }
        }
    }
    return states;
}

void GameCubeSearcher::searchChannel(u8 minSpD, u8 maxSpD)
{
    for (u8 spd = minSpD; spd <= maxSpD; spd++)
    {
        u32 lower = static_cast<u32>(spd << 27);
        u32 upper = lower | 0x7ffffff;

        for (u64 seed = lower; seed <= upper; seed++, progress++)
        {
            if (!searching)
            {
                return;
            }

            XDRNGR rng(static_cast<u32>(seed));

            u8 spa = rng.nextUShort() >> 11;
            u8 spe = rng.nextUShort() >> 11;
            u8 def = rng.nextUShort() >> 11;
            u8 atk = rng.nextUShort() >> 11;
            u8 hp = rng.nextUShort() >> 11;

            GameCubeState state;
            state.setIVs(hp, atk, def, spa, spd, spe);
            state.calculateHiddenPower();
            if (!filter.compareIVs(state))
            {
                continue;
            }

            rng.advance(3);

            // Calculate PID
            u16 low = rng.nextUShort();
            u16 high = rng.nextUShort();
            u16 sid = rng.nextUShort();

            // Determine if PID needs to be XORed
            if ((low > 7 ? 0 : 1) != (high ^ sid ^ 40122))
            {
                high ^= 0x8000;
            }

            state.setPID(high, low);
            state.setAbility(low & 1);
            state.setGender(low & 255, genderRatio);
            state.setNature(state.getPID() % 25);
            state.setShiny(40122 ^ sid, high ^ low, 8);

            u32 originSeed = rng.next();
            if (filter.comparePID(state) && validateJirachi(originSeed))
            {
                state.setSeed(originSeed);

                std::lock_guard<std::mutex> lock(mutex);
                results.emplace_back(state);
            }
        }
    }
}

bool GameCubeSearcher::validateJirachi(u32 seed)
{
    XDRNGR rng(seed);

    u16 num1 = rng.nextUShort();
    u16 num2 = rng.nextUShort();
    u16 num3 = rng.nextUShort();

    rng.advance(3);
    if (num1 <= 0x4000) // 6 advances
    {
        if (validateMenu(rng.getSeed()))
        {
            return true;
        }
    }

    rng.advance(1);
    if (num2 > 0x4000 && num1 <= 0x547a) // 7 advances
    {
        if (validateMenu(rng.getSeed()))
        {
            return true;
        }
    }

    rng.advance(1);
    if (num3 > 0x4000 && num2 > 0x547a) // 8 advances
    {
        if (validateMenu(rng.getSeed()))
        {
            return true;
        }
    }

    return false;
}

bool GameCubeSearcher::validateMenu(u32 seed)
{
    u8 target = seed >> 30;

    if (target == 0)
    {
        return false;
    }

    XDRNGR rng(seed);
    u8 mask = static_cast<u8>(1 << target);
    while ((mask & 14) != 14)
    {
        u8 num = rng.next() >> 30;
        if (num == target)
        {
            return false;
        }

        mask |= 1 << num;
    }

    return true;
}
