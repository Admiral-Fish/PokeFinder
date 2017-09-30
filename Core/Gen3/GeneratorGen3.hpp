/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish and bumba
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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

#ifndef GENERATORGEN3_HPP
#define GENERATORGEN3_HPP
#include <Core/Gen3/FrameGen3.hpp>
#include <Core/Objects/Generator.hpp>
#include <Core/Objects/FrameCompare.hpp>

class GeneratorGen3: public Generator
{

private:
    int iv1;
    int iv2;
    LCRNG rng = PokeRNG(0);

    std::vector<FrameGen3> generateMethodChannel(FrameCompare compare);

    std::vector<FrameGen3> generateMethodH124(FrameCompare compare);

    std::vector<FrameGen3> generateMethodH124Synch(FrameCompare compare);

    std::vector<FrameGen3> generateMethodH124CuteCharm(FrameCompare compare);

    std::vector<FrameGen3> generateMethodXDColo(FrameCompare compare);

    std::vector<FrameGen3> generateMethod124(FrameCompare compare);

public:

    GeneratorGen3();

    GeneratorGen3(uint32_t maxResults, uint32_t initialFrame, uint32_t initialSeed, uint32_t tid, uint32_t sid);

    std::vector<FrameGen3> Generate(FrameCompare compare);

};

#endif // GENERATORGEN3_HPP
