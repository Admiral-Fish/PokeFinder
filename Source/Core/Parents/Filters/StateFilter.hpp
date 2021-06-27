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

#ifndef STATEFILTER_HPP
#define STATEFILTER_HPP

#include <Core/Util/Global.hpp>
#include <array>
#include <vector>

class State;
class WildState;
class UnownState;

class StateFilter
{
public:
    StateFilter() = default;
    StateFilter(u8 gender, u8 ability, u8 shiny, bool skip, const std::array<u8, 6> &min, const std::array<u8, 6> &max,
                const std::vector<bool> &natures, const std::vector<bool> &powers, const std::vector<bool> &encounters);
    bool compareState(const State &state) const;
    bool comparePID(const State &state) const;
    bool compareIVs(const State &state) const;
    bool compareAbility(const State &state) const;
    bool compareGender(const State &state) const;
    bool compareNature(const State &state) const;
    bool compareShiny(const State &state) const;
    bool compareIV(const State &state) const;
    bool compareHiddenPower(const State &state) const;
    bool compareEncounterSlot(const WildState &state) const;
    bool compareLetter(const UnownState &state) const;

private:
    std::array<u8, 6> min;
    std::array<u8, 6> max;
    u8 gender;
    u8 ability;
    std::vector<bool> natures;
    std::vector<bool> powers;
    std::vector<bool> encounters;
    u8 shiny;
    bool skip;
};

#endif // STATEFILTER_HPP
