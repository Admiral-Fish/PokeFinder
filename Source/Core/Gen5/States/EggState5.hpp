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

#ifndef EGGSTATE5_HPP
#define EGGSTATE5_HPP

#include <Core/Parents/States/EggState.hpp>

/**
 * @brief State class for Gen5 egg generator encounters
 */
class EggState5 : public EggGeneratorState
{
public:
    /**
     * @brief Construct a new EggState5 object
     *
     * @param prng PRNG call to determine Chatot pitch
     * @param advances Advances of the state
     * @param pid Pokemon PID
     * @param ivs Pokemon IVs
     * @param ability Pokemon ability
     * @param gender Pokemon gender
     * @param level Pokemon level
     * @param nature Pokemon nature
     * @param shiny Pokemon shininess
     * @param inheritance Pokemon IV inheritance
     * @param info Pokemon information
     */
    EggState5(u16 prng, u32 advances, u32 pid, const std::array<u8, 6> &ivs, u8 ability, u8 gender, u8 level, u8 nature, u8 shiny,
              const std::array<u8, 6> &inheritance, const PersonalInfo *info) :
        EggGeneratorState(advances, pid, ivs, ability, gender, level, nature, shiny, inheritance, info), chatot(prng / 82)
    {
    }

    /**
     * @brief Construct a new EggState5 object
     *
     * @param ivs Pokemon IVs
     * @param ability Pokemon ability
     * @param level Pokemon level
     * @param nature Pokemon nature
     * @param shiny Pokemon shininess
     * @param inheritance Pokemon IV inheritance
     * @param info Pokemon information
     */
    EggState5(const std::array<u8, 6> &ivs, u8 ability, u8 level, u8 nature, const std::array<u8, 6> &inheritance,
              const PersonalInfo *info) :
        EggGeneratorState(0, 0, ivs, ability, 0, level, nature, 0, inheritance, info)
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
     * @brief Updates egg with things that are calculated later in BW2
     *
     * @param prng PRNG call to determine Chatot pitch
     * @param advances Advances of the state
     * @param pid Pokemon PID
     * @param gender Pokemon gender
     * @param shiny Pokemon shininess
     */
    void update(u16 prng, u32 advances, u32 pid, u8 gender, u8 shiny)
    {
        chatot = prng / 82;
        this->advances = advances;
        this->pid = pid;
        this->gender = gender;
        this->shiny = shiny;
    }

private:
    u8 chatot;
};

#endif // EGGSTATE5_HPP
