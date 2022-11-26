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

#ifndef STATE5_HPP
#define STATE5_HPP

#include <Core/Parents/States/State.hpp>

class PersonalInfo;

/**
 * @brief State class for Gen5 static generator encounters
 */
class State5 : public GeneratorState
{
public:
    /**
     * @brief Construct a new State5 object
     *
     * @param prng PRNG call to determine Chatot pitch
     * @param advances State advances
     * @param pid PID value
     * @param iv1 First IV call
     * @param iv2 Second IV call
     * @param tsv Trainer shiny value
     * @param level Pokemon level
     * @param info Pokemon personal information
     */
    State5(u16 prng, u32 advances, const std::array<u8, 6> &ivs, u32 pid, u8 ability, u8 nature, u16 tsv, u8 level,
           const PersonalInfo *info);

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
     * @brief Returns the hidden power strength
     *
     * @return Hidden power strength
     */
    u8 getHiddenPowerStrength() const
    {
        return hiddenPowerStrength;
    }

private:
    u8 chatot;
    u8 hiddenPowerStrength;
};

#endif // STATE5_HPP
