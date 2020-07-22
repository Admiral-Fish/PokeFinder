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

#ifndef IDGENERATOR5_HPP
#define IDGENERATOR5_HPP

#include <Core/Gen5/States/IDState5.hpp>
#include <Core/Parents/Generators/IDGenerator.hpp>

class IDGenerator5 : public IDGenerator
{
public:
    IDGenerator5() = default;
    IDGenerator5(u32 initialAdvances, u32 maxAdvances, const IDFilter &filter);
    QVector<IDState5> generate(u64 seed, u32 pid = 0, bool checkPID = false);
    void setInitialAdvances(u32 initialAdvances);
};

#endif // IDGENERATOR5_HPP
