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

#include "IVChecker.hpp"
#include <Core/Util/Nature.hpp>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <set>

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
        std::vector<u8> minIVs(6, 31);
        std::vector<u8> maxIVs(6, 0);

        for (u8 i = 0; i < 6; i++)
        {
            for (u8 iv = 0; iv < 32; iv++)
            {
                u16 stat = ((2 * baseStats[i] + iv) * level) / 100;
                if (i == 0)
                {
                    stat += level + 10;
                }
                else
                {
                    stat = Nature::computeStat(stat + 5, nature, i);
                }

                if (stat == stats[i])
                {
                    minIVs[i] = std::min(iv, minIVs[i]);
                    maxIVs[i] = std::max(iv, maxIVs[i]);
                }
            }
        }

        std::array<std::vector<u8>, 6> possible;
        std::vector<u8> indexes = { 0, 1, 2, 3, 4, 5 };

        // Determine the max IV based on characteristic if provided
        u8 characteristicHigh = 31;
        if (characteristic != 255)
        {
            // Determine which stat is controlled by this charateristic and the % value
            u8 stat = ivOrder[characteristic / 5];
            u8 result = characteristic % 5;

            // Remove from indexes to handle below
            indexes.erase(std::find(indexes.begin(), indexes.end(), stat));

            for (u8 i = minIVs[stat]; i <= maxIVs[stat]; i++)
            {
                // IV is only possible if (iv % 5) matches the characteristic
                // Keep the highest value to filter with below
                if ((i % 5) == result)
                {
                    possible[stat].emplace_back(i);
                    characteristicHigh = i;
                }
            }
        }

        for (u8 i : indexes)
        {
            for (u8 iv = minIVs[i]; iv <= maxIVs[i]; iv++)
            {
                // No IV can be higher then the highest stat determined by the characteristic
                if (iv > characteristicHigh)
                {
                    break;
                }
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
            std::array<std::vector<u8>, 6> temp;
            for (size_t j = 0; j < 6; j++)
            {
                std::set_intersection(ivs[j].begin(), ivs[j].end(), current[j].begin(), current[j].end(), std::back_inserter(temp[j]));
            }
            ivs = temp;
        }
    }

    if (hiddenPower != 255)
    {
        std::vector<std::set<u8>> temp(6);
        for (u8 hp : ivs[0])
        {
            for (u8 atk : ivs[1])
            {
                for (u8 def : ivs[2])
                {
                    for (u8 spa : ivs[3])
                    {
                        for (u8 spd : ivs[4])
                        {
                            for (u8 spe : ivs[5])
                            {
                                u8 hpType
                                    = ((((hp & 1) + 2 * (atk & 1) + 4 * (def & 1) + 8 * (spe & 1) + 16 * (spa & 1) + 32 * (spd & 1)) * 15)
                                       / 63);
                                if (hpType == hiddenPower)
                                {
                                    temp[0].insert(hp);
                                    temp[1].insert(atk);
                                    temp[2].insert(def);
                                    temp[3].insert(spa);
                                    temp[4].insert(spd);
                                    temp[5].insert(spe);
                                }
                            }
                        }
                    }
                }
            }
        }

        for (size_t i = 0; i < 6; i++)
        {
            ivs[i].assign(temp[i].begin(), temp[i].end());
        }
    }

    return ivs;
}
