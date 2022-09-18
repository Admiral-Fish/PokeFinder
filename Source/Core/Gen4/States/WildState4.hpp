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

#ifndef WILDSTATE4_HPP
#define WILDSTATE4_HPP

#include <Core/Parents/States/WildState.hpp>

class PersonalInfo;

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
     * @param advances State advances
     * @param occidentary State advances after battle
     * @param pid PID value
     * @param nature Pokemon nature
     * @param iv1 First IV call
     * @param iv2 Second IV call
     * @param tsv Trainer shiny value
     * @param level Pokemon level
     * @param encounterSlot Pokemon encounter slot
     * @param item Pokemon item
     * @param specie Pokemon specie
     * @param info Pokemon personal information
     */
    WildGeneratorState4(u16 prng, u32 advances, u32 occidentary, u32 pid, u8 nature, u16 iv1, u16 iv2, u16 tsv, u8 level, u8 encounterSlot,
                        u16 item, u16 specie, const PersonalInfo *info);

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
     * @brief Returns the hidden power strength
     *
     * @return Hidden power strength
     */
    u8 getHiddenPowerStrength() const
    {
        return hiddenPowerStrength;
    }

    /**
     * @brief Returns the occidentary
     *
     * @return Occidentary
     */
    u32 getOccidentary() const
    {
        return occidentary;
    }

private:
    u32 occidentary;
    u8 call;
    u8 chatot;
    u8 hiddenPowerStrength;
};

/**
 * @brief State class for Gen4 wild searcher encounters
 */
class WildSearcherState4 : public WildSearcherState<u32>
{
public:
    /**
     * @brief Construct a new WildSearcherState4 object
     *
     * @param seed State seed
     * @param pid PID value
     * @param nature Pokemon nature
     * @param ivs Pokemon IVs
     * @param tsv Trainer shiny value
     * @param level Pokemon level
     * @param encounterSlot Pokemon encounter slot
     * @param item Pokemon item
     * @param specie Pokemon specie
     * @param info Pokemon personal information
     */
    WildSearcherState4(u32 seed, u32 pid, u8 nature, std::array<u8, 6> ivs, u16 tsv, u8 level, u8 encounterSlot, u16 item, u16 specie,
                       const PersonalInfo *info);

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
     * @brief Returns the hidden power strength
     *
     * @return Hidden power strength
     */
    u8 getHiddenPowerStrength() const
    {
        return hiddenPowerStrength;
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
    u8 hiddenPowerStrength;
};

#endif // WILDSTATE4_HPP
