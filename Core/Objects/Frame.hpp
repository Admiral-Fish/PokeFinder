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

#ifndef FRAME_HPP
#define FRAME_HPP
#include <Core/Objects/Encounter.hpp>
#include <Core/Objects/Lead.hpp>
#include <Core/Objects/Method.hpp>
#include <Core/Objects/Nature.hpp>
#include <Core/Objects/Power.hpp>
#include <QString>
#include <cstdint>
#include <vector>

class Frame
{
protected:
    std::vector<QString> natures = Nature::GetNatures();

    std::vector<QString> powers = Power::GetPowers();

public:

    uint32_t ability;
    uint32_t atk;
    uint32_t def;
    uint32_t dv1;
    uint32_t dv2;
    uint32_t encounterSlot;
    Encounter encounterType;
    uint32_t frame;
    uint32_t gender;
    uint32_t hidden;
    uint32_t hp;
    Lead leadType;
    Method methodType;
    uint32_t nature;
    uint32_t pid;
    uint32_t power;
    uint32_t psv;
    uint32_t seed;
    bool shiny;
    uint32_t sid;
    uint32_t spa;
    uint32_t spd;
    uint32_t spe;
    bool synchable;
    uint32_t tid;

    QString GetFemale125();

    QString GetFemale25();

    QString GetFemale50();

    QString GetFemale75();

    QString GetNature();

    QString GetPower();

    QString GetShiny();

    void SetIVs(uint32_t iv1, uint32_t iv2);

    void SetPID(uint32_t pid1, uint32_t pid2);

    void SetPID(uint32_t pid, uint32_t pid1, uint32_t pid2);

};

#endif // FRAME_HPP
