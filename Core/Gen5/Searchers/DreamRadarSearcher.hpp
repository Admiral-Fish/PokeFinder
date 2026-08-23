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

#ifndef DREAMRADARSEARCHER_HPP
#define DREAMRADARSEARCHER_HPP

#include <Core/Gen5/Generators/DreamRadarGenerator.hpp>
#include <Core/Gen5/Searchers/Searcher5.hpp>
#include <Core/Gen5/States/SearcherState5.hpp>
#include <Core/Gen5/States/DreamRadarState.hpp>

using DreamRadarSearcher = Searcher5<DreamRadarGenerator, DreamRadarState>;

#endif // DREAMRADARSEARCHER_HPP
