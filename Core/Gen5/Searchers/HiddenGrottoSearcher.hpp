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

#ifndef HIDDENGROTTOSEARCHER_HPP
#define HIDDENGROTTOSEARCHER_HPP

#include <Core/Gen5/Generators/HiddenGrottoGenerator.hpp>
#include <Core/Gen5/Searchers/IVSearcher5.hpp>
#include <Core/Gen5/Searchers/Searcher5.hpp>
#include <Core/Gen5/States/HiddenGrottoState.hpp>
#include <Core/Gen5/States/SearcherState5.hpp>
#include <Core/Gen5/States/State5.hpp>

using HiddenGrottoSlotSearcher = Searcher5<HiddenGrottoSlotGenerator, HiddenGrottoState>;
using HiddenGrottoIVSearcher = IVSearcher5<HiddenGrottoGenerator, State5>;
using HiddenGrottoIVSearcherFast = IVSearcher5Fast<HiddenGrottoGenerator, State5>;
using HiddenGrottoIVSearcherCacheFast = IVSearcher5CacheFast<HiddenGrottoGenerator, State5>;

#endif // HIDDENGROTTOSEARCHER_HPP
