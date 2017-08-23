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

#include "Utilities.hpp"

// Returns initial seed for Ruby/Sapphire live battery given date and time
uint32_t Utilities::calcGen3Seed(boost::gregorian::date time, uint32_t h, uint32_t m)
{
    boost::gregorian::date start = boost::gregorian::date(1999, 12, 31);
    boost::gregorian::date_duration span = time - start;
    uint32_t d = span.days();
    
    uint32_t seed = 1440 * d + 960 * (h / 10) + 60 * (h % 10) + 16 * (m / 10) + m % 10;
    return (seed >> 16) ^ (seed & 0xFFFF);
}
