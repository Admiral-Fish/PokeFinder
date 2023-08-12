/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef WILDSTATE4_HPP
#define WILDSTATE4_HPP

#include <Core/Parents/States/WildState.hpp>

/**
 * @brief State class for Gen4 wild generator encounters
 */
class WildGeneratorState4 : public WildGeneratorState
{
public:
    /**
     * @brief Construct a new WildGeneratorState4 object
     *
     * @param prng PRNG call to determine Elm/Irwin call and Chatot pitch
     * @param battleAdvances State advances after battle
     * @param advances Advances of the state
     * @param pid Pokemon PID
     * @param ivs Pokemon IVs
     * @param ability Pokemon ability
     * @param gender Pokemon gender
     * @param level Pokemon level
     * @param nature Pokemon nature
     * @param shiny Pokemon shininess
     * @param encounterSlot Pokemon encounter slot
     * @param item Pokemon item
     * @param specie Pokemon specie
     * @param form Pokemon form
     * @param info Pokemon information
     */
    WildGeneratorState4(u16 prng, u32 battleAdvances, u32 advances, u32 pid, const std::array<u8, 6> &ivs, u8 ability, u8 gender, u8 level,
                        u8 nature, u8 shiny, u8 encounterSlot, u16 item, u16 specie, u8 form, const PersonalInfo *info) :
        WildGeneratorState(advances, pid, ivs, ability, gender, level, nature, shiny, encounterSlot, item, specie, form, info),
        battleAdvances(battleAdvances),
        call(prng % 3),
        chatot(((prng % 8192) * 100) >> 13)
    {
    }

    /**
     * @brief Returns the Elm/Irwin call
     *
     * @return Elm/Irwin call
     */
    u8 getCall() const
    {
        return call;
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
     * @brief Returns the battle advances
     *
     * @return Battle advances
     */
    u32 getBattleAdvances() const
    {
        return battleAdvances;
    }

private:
    u32 battleAdvances;
    u8 call;
    u8 chatot;
};

/**
 * @brief State class for Gen4 wild searcher encounters
 */
class WildSearcherState4 : public WildSearcherState
{
public:
    /**
     * @brief Construct a new WildSearcherState4 object
     *
     * @param seed Seed of the state
     * @param pid Pokemon PID
     * @param ivs Pokemon IVs
     * @param ability Pokemon ability
     * @param gender Pokemon gender
     * @param level Pokemon level
     * @param nature Pokemon nature
     * @param shiny Pokemon shininess
     * @param encounterSlot Pokemon encounter slot
     * @param item Pokemon item
     * @param specie Pokemon specie
     * @param form Pokemon form
     * @param info Pokemon information
     */
    WildSearcherState4(u32 seed, u32 pid, const std::array<u8, 6> &ivs, u8 ability, u8 gender, u8 level, u8 nature, u8 shiny,
                       u8 encounterSlot, u16 item, u16 specie, u8 form, const PersonalInfo *info) :
        WildSearcherState(seed, pid, ivs, ability, gender, level, nature, shiny, encounterSlot, item, specie, form, info)
    {
    }

    /**
     * @brief Returns the advances of the state
     *
     * @return State advances
     */
    u32 getAdvances() const
    {
        return advances;
    }

    /**
     * @brief Sets the advances of the state
     *
     * @param advances State advances
     */
    void setAdvances(u32 advances)
    {
        this->advances = advances;
    }

    /**
     * @brief Sets the seed of the state
     *
     * @param seed State seed
     */
    void setSeed(u32 seed)
    {
        this->seed = seed;
    }

private:
    u32 advances;
};

#endif // WILDSTATE4_HPP
