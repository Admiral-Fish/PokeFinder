/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Lead.hpp>
#include <Core/Enum/Method.hpp>
#include <Core/Enum/ShadowType.hpp>
#include <Core/Gen3/ShadowLock.hpp>
#include <Core/Gen3/ShadowTemplate.hpp>
#include <Core/Gen3/StaticTemplate3.hpp>
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Parents/States/State.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <Core/RNG/LCRNGReverse.hpp>
#include <Core/Util/Utilities.hpp>

static bool isShiny(u16 high, u16 low, u16 tsv)
{
    return (high ^ low ^ tsv) < 8;
}

/**
 * @brief Calculates whether the \p seed passes the Channel menu pattern
 *
 * @param rng PRNG state
 *
 * @return true Seed passes the menu pattern
 * @return false Seed does not pass the menu pattern
 */
static bool validateMenu(XDRNGR &rng)
{
    u8 target = rng.getSeed() >> 30;

    if (target == 0)
    {
        return false;
    }

    for (u8 mask = 1 << target; mask < 14;)
    {
        u8 num = rng.nextUShort() >> 14;
        if (num == target)
        {
            return false;
        }

        mask |= 1 << num;
    }

    // Keep advancing until we hit the target again
    // This will get the seed with the most possibilities for the target
    while ((rng.nextUShort() >> 14) != target)
    {
    }

    return true;
}

/**
 * @brief Calculates whether the \p seed passes the Channel Jirachi pattern
 *
 * @param seed PRNG state
 *
 * @return true Seed passes the Jirachi pattern
 * @return false Seed does not pass the Jirachi pattern
 */
static bool validateJirachi(u32 &seed)
{
    XDRNGR rng(seed);

    u16 num1 = rng.nextUShort();
    u16 num2 = rng.nextUShort();
    u16 num3 = rng.nextUShort();

    if (num3 > 0x4000 && num2 > 0x547a) // 8 advances
    {
        XDRNGR test(rng);
        test.advance(5);
        if (validateMenu(test))
        {
            seed = test.getSeed();
            return true;
        }
    }

    if (num2 > 0x4000 && num1 <= 0x547a) // 7 advances
    {
        XDRNGR test(rng);
        test.advance(4);
        if (validateMenu(test))
        {
            seed = test.getSeed();
            return true;
        }
    }

    if (num1 <= 0x4000) // 6 advances
    {
        XDRNGR test(rng);
        test.advance(3);
        if (validateMenu(test))
        {
            seed = test.getSeed();
            return true;
        }
    }

    return false;
}

GameCubeSearcher::GameCubeSearcher(Method method, bool unset, const Profile3 &profile, const StateFilter &filter) :
    StaticSearcher(method, Lead::None, profile, filter), unset(unset)
{
}

void GameCubeSearcher::startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max, const ShadowTemplate *shadowTemplate)
{
    searching = true;

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

                            std::vector<SearcherState> states;
                            if ((profile.getVersion() & Game::Colosseum) != Game::None)
                            {
                                states = searchColoShadow(hp, atk, def, spa, spd, spe, shadowTemplate);
                            }
                            else
                            {
                                states = searchGalesShadow(hp, atk, def, spa, spd, spe, shadowTemplate);
                            }

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

void GameCubeSearcher::startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max, const StaticTemplate3 *staticTemplate)
{
    searching = true;

    if (method == Method::Channel)
    {
        searchChannel(min[4], max[4], staticTemplate);
        return;
    }

    // Ageto Pikachu/Celebi
    if (staticTemplate->getSpecie() == 25 || staticTemplate->getSpecie() == 251)
    {
        tsv = 31121; // TID: 31121 SID: 0
    }
    // Mattle Ho-Oh
    else if (staticTemplate->getSpecie() == 250)
    {
        tsv = 10048; // TID: 10048 SID: 0
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

                            auto states = searchNonLock(hp, atk, def, spa, spd, spe, staticTemplate);

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

void GameCubeSearcher::searchChannel(u8 minSpd, u8 maxSpd, const StaticTemplate3 *staticTemplate)
{
    const PersonalInfo *info = staticTemplate->getInfo();

    for (u8 spd = minSpd; spd <= maxSpd; spd++)
    {
        u32 lower = spd << 27;
        u32 upper = lower | 0x7ffffff;

        for (u64 seed = lower; seed <= upper; seed++, progress++)
        {
            if (!searching)
            {
                return;
            }

            XDRNGR rng(seed);

            std::array<u8, 6> ivs;

            ivs[4] = spd;
            ivs[3] = rng.nextUShort() >> 11;
            ivs[5] = rng.nextUShort() >> 11;
            ivs[2] = rng.nextUShort() >> 11;
            ivs[1] = rng.nextUShort() >> 11;
            ivs[0] = rng.nextUShort() >> 11;

            if (!filter.compareIV(ivs))
            {
                continue;
            }

            rng.advance(3);
            u16 low = rng.nextUShort();
            u16 high = rng.nextUShort();
            u16 sid = rng.nextUShort();
            constexpr u16 tid = 40122;

            // Failed non-shiny check due to operator precedence
            if (tid ^ sid ^ high ^ low < 8)
            {
                high ^= 0x8000;
            }

            u32 pid = (high << 16) | low;
            u8 nature = pid % 25;
            if (!filter.compareNature(nature))
            {
                continue;
            }

            u32 origin = rng.next();
            if (!validateJirachi(origin))
            {
                continue;
            }

            SearcherState state(origin, pid, ivs, pid & 1, 2, staticTemplate->getLevel(), nature, Utilities::getShiny<true>(pid, tid ^ sid),
                                info);
            if (filter.compareState(static_cast<const SearcherState &>(state)))
            {
                std::lock_guard<std::mutex> guard(mutex);
                results.emplace_back(state);
            }
        }
    }
}

std::vector<SearcherState> GameCubeSearcher::searchColoShadow(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe,
                                                              const ShadowTemplate *shadowTemplate)
{
    std::vector<SearcherState> states;
    const PersonalInfo *info = shadowTemplate->getInfo();

    std::array<u8, 6> ivs;
    if (shadowTemplate->getType() == ShadowType::EReader)
    {
        ivs.fill(0);
    }
    else
    {
        ivs = { hp, atk, def, spa, spd, spe };
    }

    u32 seeds[6];
    int size = LCRNGReverse::recoverXDRNGIV(hp, atk, def, spa, spd, spe, seeds);
    for (int i = 0; i < size; i++)
    {
        XDRNG rng(seeds[i]);

        rng.advance(1);
        u8 ability = rng.nextUShort(2) & (info->getAbility(0) != info->getAbility(1));
        u32 pid = rng.nextUShort() << 16;
        pid |= rng.nextUShort();

        u8 nature = pid % 25;
        if (!filter.compareNature(nature))
        {
            continue;
        }

        bool valid = false;
        u32 seed = XDRNGR(seeds[i]).next();
        switch (shadowTemplate->getType())
        {
        case ShadowType::FirstShadow:
            valid = ShadowLock::coloShadow(seed, shadowTemplate);
            break;
        case ShadowType::EReader:
            valid = ShadowLock::ereader(seed, pid, shadowTemplate);
            break;
        default:
            break;
        }

        if (valid)
        {
            // If this spread passes and is the 'normal' spread we can skip the XORed spread
            if ((i & 2) == 0)
            {
                i++;
            }

            SearcherState state(seed, pid, ivs, ability, Utilities::getGender(pid, info), shadowTemplate->getLevel(), nature,
                                Utilities::getShiny<true>(pid, tsv), info);
            if (filter.compareState(static_cast<const SearcherState &>(state)))
            {
                states.emplace_back(state);
            }
        }
    }
    return states;
}

std::vector<SearcherState> GameCubeSearcher::searchGalesShadow(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe,
                                                               const ShadowTemplate *shadowTemplate)
{
    std::vector<SearcherState> states;
    const PersonalInfo *info = shadowTemplate->getInfo();
    std::array<u8, 6> ivs = { hp, atk, def, spa, spd, spe };

    u32 seeds[6];
    int size = LCRNGReverse::recoverXDRNGIV(hp, atk, def, spa, spd, spe, seeds);
    for (int i = 0; i < size; i++)
    {
        XDRNG rng(seeds[i]);

        rng.advance(1);
        u8 ability = rng.nextUShort(2) & (info->getAbility(0) != info->getAbility(1));
        u16 high = rng.nextUShort();
        u16 low = rng.nextUShort();
        while (isShiny(high, low, tsv))
        {
            high = rng.nextUShort();
            low = rng.nextUShort();
        }

        u32 pid = (high << 16) | low;
        u8 nature = pid % 25;
        if (!filter.compareNature(nature))
        {
            continue;
        }

        bool valid = false;
        u32 seed = XDRNGR(seeds[i]).next();
        switch (shadowTemplate->getType())
        {
        case ShadowType::SingleLock:
            valid = ShadowLock::singleNL(seed, tsv, shadowTemplate);
            break;
        case ShadowType::FirstShadow:
            valid = ShadowLock::firstShadowNormal(seed, tsv, shadowTemplate);
            break;
        case ShadowType::SecondShadow:
            if (unset)
            {
                valid = ShadowLock::firstShadowUnset(seed, tsv, shadowTemplate);
            }
            else
            {
                valid = ShadowLock::firstShadowSet(seed, tsv, shadowTemplate);
            }
            break;
        case ShadowType::Salamence:
            if (unset)
            {
                valid = ShadowLock::salamenceUnset(seed, tsv, shadowTemplate);
            }
            else
            {
                valid = ShadowLock::salamenceSet(seed, tsv, shadowTemplate);
            }
            break;
        default:
            break;
        }

        if (valid)
        {
            // If this spread passes and is the 'normal' spread we can skip the XORed spread
            if ((i & 2) == 0)
            {
                i++;
            }

            SearcherState state(seed, pid, ivs, ability, Utilities::getGender(pid, info), shadowTemplate->getLevel(), nature, 0, info);
            if (filter.compareState(static_cast<const SearcherState &>(state)))
            {
                states.emplace_back(state);
            }
        }
    }
    return states;
}

std::vector<SearcherState> GameCubeSearcher::searchNonLock(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe,
                                                           const StaticTemplate3 *staticTemplate)
{
    std::vector<SearcherState> states;
    const PersonalInfo *info = staticTemplate->getInfo();
    std::array<u8, 6> ivs = { hp, atk, def, spa, spd, spe };

    u32 seeds[6];
    int size = LCRNGReverse::recoverXDRNGIV(hp, atk, def, spa, spd, spe, seeds);
    for (int i = 0; i < size; i++)
    {
        XDRNG rng(seeds[i]);

        u32 seed = XDRNGR(seeds[i]).next();
        u8 ability;
        u16 high;
        u16 low;

        // Gales eevee
        if (staticTemplate->getSpecie() == 133)
        {
            XDRNGR temp(rng);
            temp.advance(2);
            tsv = temp.nextUShort() ^ temp.nextUShort();
            seed = temp.next();
        }

        // Colo Umbreon
        if (staticTemplate->getSpecie() == 197)
        {
            XDRNGR temp(rng);
            temp.advance(2);
            tsv = temp.nextUShort() ^ temp.nextUShort();
            seed = temp.next();

            rng.advance(1);
            ability = rng.nextUShort(2);
            high = rng.nextUShort();
            low = rng.nextUShort();

            // Colo starters are shiny locked and male locked
            while (isShiny(high, low, tsv) || (low & 255) < info->getGender())
            {
                high = rng.nextUShort();
                low = rng.nextUShort();
            }
        }
        // Colo Espeon
        else if (staticTemplate->getSpecie() == 196)
        {
            XDRNGR temp(rng);
            temp.advance(2);

            u32 originalUmbreonPID = temp.nextUShort();
            originalUmbreonPID |= temp.nextUShort() << 16;

            // Umbreon can't be male
            if ((originalUmbreonPID & 255) < info->getGender())
            {
                continue;
            }

            // Roll back RNG until we hit a female gender, we will assume shiny doesn't impact
            while (temp.nextUShort(256) < info->getGender())
            {
                temp.advance(1);
            }

            XDRNG test(temp);

            temp.advance(2);
            tsv = temp.nextUShort() ^ temp.nextUShort();
            seed = temp.advance(3);

            // This TSV better end up on the original Umbreon PID we found
            // The only reason this wouldn't be true is if we get some extra rerolls from a PID being shiny
            u16 umbreonHigh = test.nextUShort();
            u16 umbreonLow = test.nextUShort();
            while (isShiny(umbreonHigh, umbreonLow, tsv) || (umbreonLow & 255) < info->getGender())
            {
                umbreonHigh = test.nextUShort();
                umbreonLow = test.nextUShort();
            }
            u32 umbreonPID = (umbreonHigh << 16) | umbreonLow;
            if (originalUmbreonPID != umbreonPID)
            {
                continue;
            }

            rng.advance(1);
            ability = rng.nextUShort(2);
            high = rng.nextUShort();
            low = rng.nextUShort();

            // Colo starters are shiny locked and male locked
            while (isShiny(high, low, tsv) || (low & 255) < info->getGender())
            {
                high = rng.nextUShort();
                low = rng.nextUShort();
            }
        }
        else
        {
            rng.advance(1);
            ability = rng.nextUShort(2);
            high = rng.nextUShort();
            low = rng.nextUShort();

            if (staticTemplate->getShiny() == Shiny::Never)
            {
                while (isShiny(high, low, tsv))
                {
                    high = rng.nextUShort();
                    low = rng.nextUShort();
                }
            }
        }

        u32 pid = (high << 16) | low;
        u8 nature = pid % 25;
        if (!filter.compareNature(nature))
        {
            continue;
        }

        ability &= info->getAbility(0) != info->getAbility(1);

        SearcherState state(seed, pid, ivs, ability, Utilities::getGender(pid, info), staticTemplate->getLevel(), nature,
                            Utilities::getShiny<true>(pid, tsv), info);
        if (filter.compareState(static_cast<const SearcherState &>(state)))
        {
            states.emplace_back(state);
        }
    }
    return states;
}
