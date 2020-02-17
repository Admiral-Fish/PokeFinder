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

#ifndef IDGENERATOR4_HPP
#define IDGENERATOR4_HPP

#include <Core/Gen4/Frames/IDFrame4.hpp>
#include <Core/Parents/Generators/IDGenerator.hpp>
#include <QVector>

class IDGenerator4 : public IDGenerator
{
public:
    IDGenerator4(u32 minDelay, u32 maxDelay, u16 year, u8 month, u8 day, u8 hour, u8 minute);
    QVector<IDFrame4> generate(const IDFilter &filter);

private:
    u32 minDelay;
    u32 maxDelay;
    u16 year;
    u8 month;
    u8 day;
    u8 hour;
    u8 minute;
};

#endif // IDGENERATOR4_HPP
