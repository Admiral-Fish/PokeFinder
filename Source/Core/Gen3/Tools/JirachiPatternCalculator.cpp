/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "JirachiPatternCalculator.hpp"
#include <Core/RNG/LCRNG.hpp>
#include <iterator>
#include <sstream>

namespace JirachiPatternCalculator
{
    namespace
    {
        u8 getTarget(u8 index, const std::vector<u16> &data)
        {
            /*
             *  thresh = [ .25,.33 ]
             *  thresh = [ 0x4000, 0x547a ]
             *  rng.advance(4)
             *  flag = false
             *  for (u8 i = 0; i < 2; i++)
             *      rand = (double)rng.nextUShort()/65536.0
             *      if rand <= thresh[i]
             *          flag = true
             *          break
             *  rng.advance(flag ? 1 : 2)
             */

            switch (index)
            {
            case 0: // 6 advances total
                if (data[1] <= 0x4000)
                {
                    return 6;
                }
                break;
            case 1: // 7 advances total
                if (data[2] > 0x4000 && data[1] <= 0x547a)
                {
                    return 7;
                }
                break;
            case 2: // 8 advances total
                if (data[3] > 0x4000 && data[2] > 0x547a)
                {
                    return 8;
                }
                break;
            }
            return 0;
        }
    }

    std::vector<std::string> getPatterns(u32 seed)
    {
        std::vector<std::string> patterns;

        XDRNGR rng(seed);
        std::vector<u16> data;
        data.emplace_back(seed >> 16);

        for (u8 i = 0; i < 30; i++)
        {
            data.emplace_back(rng.nextUShort());
        }

        // Loop through 3 possible pattern cases
        for (u8 i = 0; i < 3; i++)
        {
            u8 index = getTarget(i, data);
            if (index != 0)
            {
                // Menu advances can't stop on 0; skip
                u8 target = data[index] >> 14;
                if (target != 0)
                {
                    // From start, game advances until(prng >> 30) gives a 1, 2, and 3
                    // (prng >> 30) being 0 just acts as a filler
                    u8 mask = static_cast<u8>(1 << target);

                    // Determine if spread is possible
                    // Need to work backwards to see if going forward with 1, 2, and 3 lands on our target
                    for (size_t x = index + 1; x < data.size(); x++)
                    {
                        u8 temp = data[x] >> 14;

                        // Spread impossible
                        if (temp == target)
                        {
                            break;
                        }

                        mask |= 1 << temp;

                        // Spread valid
                        if ((mask & 14) == 14)
                        {
                            std::vector<std::string> pattern;
                            for (size_t j = data.size() - 1; j > 0; j--)
                            {
                                pattern.emplace_back(std::to_string(data[j] >> 14));
                            }
                            pattern[pattern.size() - index] = "T:" + pattern[pattern.size() - index];

                            std::stringstream stream;
                            std::copy(pattern.begin(), pattern.end(), std::ostream_iterator<std::string>(stream, " | "));
                            patterns.emplace_back(stream.str());

                            break;
                        }
                    }
                }
            }
        }

        return patterns;
    }
}
