/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef WILDSTATE5_HPP
#define WILDSTATE5_HPP

#include <Core/Parents/States/WildState.hpp>

/**
 * @brief State class for Gen5 static generator encounters
 */
class WildState5 : public WildGeneratorState
{
public:
    /**
     * @brief Construct a new State5 object
     *
     * @param prng PRNG call to determine Chatot pitch
     * @param advances Advances of the state
     * @param ivAdvances IV advances of the state
     * @param pid Pokemon PID
     * @param ivs Pokemon IVs
     * @param ability Pokemon ability
     * @param gender Pokemon gender
     * @param level Pokemon level
     * @param nature Pokemon nature
     * @param shiny Pokemon shininess
     * @param info Pokemon information
     */
    WildState5(u16 prng, u32 advances, u32 ivAdvances, u32 pid, const std::array<u8, 6> &ivs, u8 ability, u8 gender, u8 level, u8 nature,
               u8 shiny, u8 encounterSlot, u16 item, u16 specie, u8 form, const PersonalInfo *info) :
        WildGeneratorState(advances, pid, ivs, ability, gender, level, nature, shiny, encounterSlot, item, specie, form, info),
        ivAdvances(ivAdvances),
        chatot(prng / 82)
    {
    }

    /**
     * @brief Returns the chatot pitch
     *
     * @return Chatot pitch
     */
    u8 getChatot() const
    {
        return chatot;
    }

    /**
     * @brief Returns the IV advances of the state
     *
     * @return State IV advances
     */
    u32 getIVAdvances() const
    {
        return ivAdvances;
    }

private:
    u32 ivAdvances;
    u8 chatot;
};

#endif // WILDSTATE5_HPP
