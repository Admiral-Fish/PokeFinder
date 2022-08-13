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

#ifndef STATEFILTER_HPP
#define STATEFILTER_HPP

#include <Core/Global.hpp>
#include <array>

class StateFilter
{
public:
    StateFilter(u8 gender, u8 ability, u8 shiny, bool skip, const std::array<u8, 6> &min, const std::array<u8, 6> &max,
                const std::array<bool, 25> &natures, const std::array<bool, 16> &powers);
    bool compareAbility(u8 ability) const;
    bool compareGender(u8 gender) const;
    bool compareNature(u8 nature) const;
    bool compareShiny(u8 shiny) const;
    bool compareIV(std::array<u8, 6> ivs) const;

private:
    std::array<bool, 25> natures;
    std::array<bool, 16> powers;
    std::array<u8, 6> max;
    std::array<u8, 6> min;
    bool skip;
    u8 ability;
    u8 gender;
    u8 shiny;
};

#endif // STATEFILTER_HPP
