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

#ifndef IDGENERATOR3_HPP
#define IDGENERATOR3_HPP

#include <Core/Gen3/Frames/IDFrame3.hpp>
#include <Core/Parents/Generators/IDGenerator.hpp>
#include <QVector>

class IDGenerator3 : public IDGenerator
{
public:
    IDGenerator3() = default;
    IDGenerator3(u32 initialFrame, u32 maxResults, const IDFilter &filter);
    void setStaticTID(u16 staticTID);
    QVector<IDFrame3> generateXDColo(u32 seed);
    QVector<IDFrame3> generateFRLGE(u32 seed);
    QVector<IDFrame3> generateRS(u32 seed);

private:
    u16 staticTID;
};

#endif // IDGENERATOR3_HPP
