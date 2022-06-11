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

#ifndef UGSTATE_HPP
#define UGSTATE_HPP

#include <Core/Parents/States/State.hpp>

class UgState : public State
{
public:
    UgState() = default;

    explicit UgState(u32 advance) : State(advance)
    {
    }

    u16 getSpecies() const
    {
        return species;
    }

    void setSpecies(u16 species)
    {
        this->species = species;
    }

    u8 getItem() const
    {
        return item;
    }

    void setItem(u8 item)
    {
        this->item = item;
    }

    u16 getEggMove() const
    {
        return eggMove;
    }

    void setEggMove(u16 eggMove)
    {
        this->eggMove = eggMove;
    }

private:
    u16 species;
    u8 item;
    u16 eggMove;
};

#endif // IDSTATE8_HPP
