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

#ifndef IDGENERATOR_HPP
#define IDGENERATOR_HPP

#include <Core/Parents/Filters/IDFilter.hpp>
#include <Core/Util/Global.hpp>

class IDGenerator
{
public:
    IDGenerator() = default;
    IDGenerator(u32 initialFrame, u32 maxResults, const IDFilter &filter);

protected:
    u32 initialFrame;
    u32 maxResults;
    IDFilter filter;
};

#endif // IDGENERATOR_HPP
