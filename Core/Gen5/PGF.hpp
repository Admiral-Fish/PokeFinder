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

#ifndef PGF_HPP
#define PGF_HPP

#include <Core/Util/Global.hpp>
#include <QByteArray>

class PGF
{
public:
    explicit PGF(const QByteArray &data);
    u16 getTID() const;
    u16 getSID() const;
    u16 getSpecies() const;
    u8 getForm() const;
    u8 getNature() const;
    u8 getGender() const;
    u8 getAbilityType() const;
    u8 getPIDType() const;
    u8 getIV(u8 index) const;
    bool isEgg() const;
    u8 getAdvances() const;

private:
    u16 tid;
    u16 sid;
    u16 species;
    u8 form;
    u8 nature; // 0xff -> unset
    u8 gender; // 0: male, 1: female, 2: random
    u8 abilityType; // 0: 0, 1: 1, 2: H, 3: 1/2, 4: 1/2/H
    u8 pidType; // 0: no shiny, 1: allow shiny, 2: force shiny
    u8 hp, atk, def, spa, spd, spe; // 0xff -> unset
    bool egg;
};

#endif // PGF_HPP
