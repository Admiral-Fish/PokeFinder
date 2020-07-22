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

#ifndef HIDDENGROTTOGENERATOR_HPP
#define HIDDENGROTTOGENERATOR_HPP

#include <Core/Gen5/Filters/HiddenGrottoFilter.hpp>
#include <Core/Gen5/States/HiddenGrottoState.hpp>
#include <QVector>

class HiddenGrottoGenerator
{
public:
    HiddenGrottoGenerator(u32 initialAdvances, u32 maxResults, u8 genderRatio, const HiddenGrottoFilter &filter);
    QVector<HiddenGrottoState> generate(u64 seed);

private:
    u32 initialAdvances;
    u32 maxResults;
    u8 genderRatio;
    HiddenGrottoFilter filter;
};

#endif // HIDDENGROTTOGENERATOR_HPP
