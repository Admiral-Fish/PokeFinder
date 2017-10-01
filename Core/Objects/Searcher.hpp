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

#ifndef SEARCHER_HPP
#define SEARCHER_HPP
#include <Core/Objects/Encounter.hpp>
#include <Core/Objects/EncounterSlot.hpp>
#include <Core/Objects/FrameCompare.hpp>
#include <Core/Objects/Lead.hpp>
#include <Core/Objects/Method.hpp>
#include <Core/RNG/LCRNG.hpp>
#include <cstdint>
#include <vector>

class Searcher
{

protected:
    uint32_t psv;
    uint32_t sid;
    uint32_t tid;

public:
    uint32_t cuteCharm;
    Encounter encounterType = Stationary;
    Method frameType = Method1;
    Lead leadType = None;
    uint32_t synchNature;

};

#endif // SEARCHER_HPP
