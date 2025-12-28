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

#ifndef EGGSTATE4_HPP
#define EGGSTATE4_HPP

#include <Core/Parents/States/EggState.hpp>

/**
 * @brief State class for Gen4 egg generator encounters
 */
class EggGeneratorState4 : public EggGeneratorState
{
public:
    /**
     * @brief Construct a new EggGeneratorState4 object
     *
     * @param prng PRNG call to determine Elm/Irwin call and Chatot pitch
     * @param advances Advances of the state
     * @param pid Pokemon PID
     * @param gender Pokemon gender
     * @param shiny Pokemon shininess
     * @param info Pokemon information
     */
    EggGeneratorState4(u32 advances, u32 pid, u8 gender, u8 shiny, const PersonalInfo *info) :
        EggGeneratorState(advances, pid, { 0, 0, 0, 0, 0, 0 }, pid & 1, gender, 1, pid % 25, shiny, { 0, 0, 0, 0, 0, 0 }, info)
    {
    }

    /**
     * @brief Returns the advances of the state
     *
     * @return State advances
     */
    u32 getPickupAdvances() const
    {
        return pickupAdvances;
    }

    /**
     * @brief Updates egg with things that are calculated on pickup
     *
     * @param advances Advances of the state
     * @param ivs Pokemon IVs
     * @param inheritance Pokemon IV inheritance
     * @param info Pokemon information
     */
    void update(u16 prng, u32 advances, const std::array<u8, 6> &ivs, const std::array<u8, 6> &inheritance, const PersonalInfo *info)
    {
        call = prng % 3;
        chatot = ((prng % 8192) * 100) >> 13;
        pickupAdvances = advances;
        this->ivs = ivs;
        this->inheritance = inheritance;
        updateStats(info);
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

private:
    u32 pickupAdvances;
    u8 call;
    u8 chatot;
};

/**
 * @brief State class for Gen4 egg searcher encounters
 */
class EggSearcherState4
{
public:
    /**
     * @brief Construct a new EggSearcherState4 object
     *
     * @param seed Initial seed
     * @param state Generator state information
     */
    EggSearcherState4(u32 seed, const EggGeneratorState4 &state) : state(state), seed(seed)
    {
    }

    /**
     * @brief Returns the initial seed
     *
     * @return Initial seed
     */
    u32 getSeed() const
    {
        return seed;
    }

    /**
     * @brief Returns the generator state
     *
     * @return Generator state
     */
    EggGeneratorState4 getState() const
    {
        return state;
    }

private:
    EggGeneratorState4 state;
    u32 seed;
};

#endif // EGGSTATE4_HPP
