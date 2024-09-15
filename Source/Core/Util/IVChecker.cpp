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

#include "IVChecker.hpp"
#include <Core/Util/Nature.hpp>
#include <algorithm>
#include <iterator>

constexpr u8 ivOrder[6] = { 0, 1, 2, 5, 3, 4 };

namespace
{
    /**
     * @brief Calculates the IV range for the given \p stats.
     *
     * @param baseStats Pokemon base stats
     * @param stats Pokemon stats
     * @param level Pokemon level
     * @param nature Pokemon nature
     * @param characteristic Pokemon characteristic
     *
     * @return Array of IV ranges for each stat
     */
    std::array<std::vector<u8>, 6> calculateIVs(const std::array<u8, 6> &baseStats, const std::array<u16, 6> &stats, u8 level, u8 nature,
                                                u8 characteristic)
    {
        std::array<u8, 6> minIVs = { 31, 31, 31, 31, 31, 31 };
        std::array<u8, 6> maxIVs = { 0, 0, 0, 0, 0, 0 };

        for (u8 i = 0; i < 6; i++)
        {
            for (u8 iv = 0; iv < 32; iv++)
            {
                u16 stat;
                if (nature != 255)
                {
                    stat = Nature::computeStat(baseStats[i], iv, nature, level, i);
                    if (stat == stats[i])
                    {
                        minIVs[i] = std::min(iv, minIVs[i]);
                        maxIVs[i] = std::max(iv, maxIVs[i]);
                    }
                }
                else
                {
                    // Hard pass nature as Hardy to not modify the stat
                    stat = Nature::computeStat(baseStats[i], iv, 0, level, i);
                    if (stat == stats[i]
                        || (i != 0 && (static_cast<u16>(stat * 0.9f) == stats[i] || static_cast<u16>(stat * 1.1f) == stats[i])))
                    {
                        minIVs[i] = std::min(iv, minIVs[i]);
                        maxIVs[i] = std::max(iv, maxIVs[i]);
                    }
                }
            }
        }

        std::array<std::vector<u8>, 6> possible;

        // Determine the max IV based on characteristic if provided
        u8 characteristicHigh = 31;
        u8 charIndex = -1;
        if (characteristic != 255)
        {
            // Determine which stat is controlled by this charateristic and the % value
            charIndex = ivOrder[characteristic / 5];
            u8 result = characteristic % 5;

            for (u8 i = minIVs[charIndex]; i <= maxIVs[charIndex]; i++)
            {
                // IV is only possible if (iv % 5) matches the characteristic
                // Keep the highest value to filter with below
                if ((i % 5) == result)
                {
                    possible[charIndex].emplace_back(i);
                    characteristicHigh = i;
                }
            }
        }

        for (u8 i = 0; i < 6; i++)
        {
            if (i == charIndex)
            {
                continue;
            }

            for (u8 iv = minIVs[i]; iv <= maxIVs[i] && iv <= characteristicHigh; iv++)
            {
                possible[i].emplace_back(iv);
            }
        }

        return possible;
    }
}

std::array<std::vector<u8>, 6> IVChecker::calculateIVRange(const std::array<u8, 6> &baseStats, const std::vector<std::array<u16, 6>> &stats,
                                                           const std::vector<u8> &level, u8 nature, u8 characteristic, u8 hiddenPower)
{
    std::array<std::vector<u8>, 6> ivs;
    for (size_t i = 0; i < stats.size(); i++)
    {
        auto current = calculateIVs(baseStats, stats[i], level[i], nature, characteristic);

        if (i == 0)
        {
            ivs = current;
        }
        else
        {
            for (size_t j = 0; j < 6; j++)
            {
                std::vector<u8> temp;
                std::set_intersection(ivs[j].begin(), ivs[j].end(), current[j].begin(), current[j].end(), std::back_inserter(temp));
                ivs[j] = temp;
            }
        }
    }

    if (hiddenPower != 255)
    {
        std::array<std::vector<u8>, 6> possible;
        for (int i = 0; i < 6; i++)
        {
            if (std::find_if(ivs[i].begin(), ivs[i].end(), [](u8 iv) { return (iv % 2) == 0; }) != ivs[i].end())
            {
                possible[i].emplace_back(0);
            }
            if (std::find_if(ivs[i].begin(), ivs[i].end(), [](u8 iv) { return (iv % 2) == 1; }) != ivs[i].end())
            {
                possible[i].emplace_back(1);
            }
        }

        std::array<std::vector<u8>, 6> temp;
        for (u8 hp : possible[0])
        {
            u8 hpVal = hp;
            for (u8 atk : possible[1])
            {
                u8 atkVal = hpVal + 2 * (atk);
                for (u8 def : possible[2])
                {
                    u8 defVal = atkVal + 4 * (def);
                    for (u8 spa : possible[3])
                    {
                        u8 spaVal = defVal + 16 * (spa);
                        for (u8 spd : possible[4])
                        {
                            u8 spdVal = spaVal + 32 * (spd);
                            for (u8 spe : possible[5])
                            {
                                u8 type = ((spdVal + 8 * (spe)) * 15) / 63;
                                if (type == hiddenPower)
                                {
                                    std::copy_if(ivs[0].begin(), ivs[0].end(), std::back_inserter(temp[0]),
                                                 [hp](u8 iv) { return (iv % 2) == hp; });
                                    std::copy_if(ivs[1].begin(), ivs[1].end(), std::back_inserter(temp[1]),
                                                 [atk](u8 iv) { return (iv % 2) == atk; });
                                    std::copy_if(ivs[2].begin(), ivs[2].end(), std::back_inserter(temp[2]),
                                                 [def](u8 iv) { return (iv % 2) == def; });
                                    std::copy_if(ivs[3].begin(), ivs[3].end(), std::back_inserter(temp[3]),
                                                 [spa](u8 iv) { return (iv % 2) == spa; });
                                    std::copy_if(ivs[4].begin(), ivs[4].end(), std::back_inserter(temp[4]),
                                                 [spd](u8 iv) { return (iv % 2) == spd; });
                                    std::copy_if(ivs[5].begin(), ivs[5].end(), std::back_inserter(temp[5]),
                                                 [spe](u8 iv) { return (iv % 2) == spe; });
                                }
                            }
                        }
                    }
                }
            }
        }

        for (size_t i = 0; i < 6; i++)
        {
            std::sort(temp[i].begin(), temp[i].end());
            temp[i].erase(std::unique(temp[i].begin(), temp[i].end()), temp[i].end());
            ivs[i] = temp[i];
        }
    }

    return ivs;
}

std::array<u8, 6> IVChecker::nextLevel(const std::array<u8, 6> &baseStats, const std::array<std::vector<u8>, 6> &ivs, u8 level, u8 nature)
{
    std::array<u8, 6> levels;
    levels.fill(level);

    for (int i = 0; i < 6; i++)
    {
        const std::vector<u8> &statIVs = ivs[i];
        if (statIVs.size() < 2)
        {
            continue;
        }

        for (u8 l = level + 1; levels[i] == level && l <= 100; l++)
        {
            for (size_t j = 1; j < statIVs.size(); j++)
            {
                u16 previous;
                u16 current;

                if (nature != 255)
                {
                    previous = Nature::computeStat(baseStats[i], statIVs[j - 1], nature, l, i);
                    current = Nature::computeStat(baseStats[i], statIVs[j], nature, l, i);
                }
                else
                {
                    // Hard pass nature as Hardy to not modify the stat
                    previous = Nature::computeStat(baseStats[i], statIVs[j - 1], 0, l, i);
                    current = Nature::computeStat(baseStats[i], statIVs[j], 0, l, i);
                }

                if (previous < current)
                {
                    levels[i] = l;
                    break;
                }
            }
        }
    }

    return levels;
}
