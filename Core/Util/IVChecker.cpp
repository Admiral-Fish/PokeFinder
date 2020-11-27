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

#include "IVChecker.hpp"
#include <Core/Parents/PersonalInfo.hpp>
#include <Core/Util/Nature.hpp>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <set>

constexpr u8 ivOrder[6] = { 0, 1, 2, 5, 3, 4 };

namespace
{
    std::vector<std::vector<u8>> calculateIVs(const std::vector<u8> &baseStats, const std::vector<u16> &stats, u8 level, u8 nature,
                                              u8 characteristic)
    {
        std::vector<u8> minIVs(6, 31);
        std::vector<u8> maxIVs(6, 0);

        for (u8 i = 0; i < 6; i++)
        {
            for (u8 iv = 0; iv < 32; iv++)
            {
                double stat;
                if (i == 0)
                {
                    stat = std::floor(((2 * baseStats[i] + iv) * level) / 100.0) + level + 10;
                }
                else
                {
                    stat = (std::floor(((2 * baseStats[i] + iv) * level) / 100.0) + 5) * Nature::getNatureModifier(nature, i);
                }

                if (static_cast<u16>(stat) == stats[i])
                {
                    minIVs[i] = std::min(iv, minIVs[i]);
                    maxIVs[i] = std::max(iv, maxIVs[i]);
                }
            }
        }

        std::vector<std::vector<u8>> possible(6);
        std::vector<u8> indexes = { 0, 1, 2, 3, 4, 5 };

        u8 characteristicHigh = 31;
        if (characteristic != 255)
        {
            u8 stat = ivOrder[characteristic / 5];
            u8 result = characteristic % 5;

            indexes.erase(std::find(indexes.begin(), indexes.end(), stat));

            for (u8 i = minIVs[stat]; i <= maxIVs[stat]; i++)
            {
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

std::vector<std::vector<u8>> IVChecker::calculateIVRange(const std::vector<u8> &baseStats, const std::vector<std::vector<u16>> &stats,
                                                         const std::vector<u8> &level, u8 nature, u8 characteristic, u8 hiddenPower)
{
    std::vector<std::vector<u8>> ivs(6);
    for (size_t i = 0; i < stats.size(); i++)
    {
        auto current = calculateIVs(baseStats, stats[i], level[i], nature, characteristic);

        if (i == 0)
        {
            ivs = current;
        }
        else
        {
            std::vector<std::vector<u8>> temp(6);
            for (size_t j = 0; j < 6; j++)
            {
                std::set_intersection(ivs[j].begin(), ivs[j].end(), current[j].begin(), current[j].end(), std::back_inserter(temp[i]));
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
