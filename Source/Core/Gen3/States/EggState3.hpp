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

#ifndef EGGSTATE3_HPP
#define EGGSTATE3_HPP

#include <Core/Parents/States/EggState.hpp>

/**
 * @brief State class for Gen3 egg generator encounters
 */
class EggState3 : public EggGeneratorState
{
public:
    /**
     * @brief Construct a new EggState3 object
     *
     * @param advances Advances of the state
     * @param redraws Redraws of the state
     * @param pid Pokemon PID
     * @param gender Pokemon gender
     * @param shiny Pokemon shininess
     * @param info Pokemon information
     */
    EggState3(u32 advances, u8 redraws, u32 pid, u8 gender, u8 shiny, const PersonalInfo *info) :
        EggGeneratorState(advances, pid, { 0, 0, 0, 0, 0, 0 }, pid & 1, gender, 5, pid % 25, shiny, { 0, 0, 0, 0, 0, 0 }, info),
        redraws(redraws)
    {
    }

    /**
     * @brief Construct a new EggState3 object
     *
     * @param advances Advances of the state
     * @param low Pokemon PID low
     * @param gender Pokemon gender
     * @param shiny Pokemon shininess
     * @param info Pokemon information
     */
    EggState3(u32 advances, u16 low, u8 gender, const PersonalInfo *info) :
        EggGeneratorState(advances, low, { 0, 0, 0, 0, 0, 0 }, low & 1, gender, 5, 0, 0, { 0, 0, 0, 0, 0, 0 }, info)
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
     * @brief Returns the redraws of the state
     *
     * @return State redraws
     */
    u8 getRedraws() const
    {
        return redraws;
    }

    /**
     * @brief Updates egg with things that are calculated on pickup in Emerald
     *
     * @param advances Advances of the state
     * @param ivs Pokemon IVs
     * @param inheritance Pokemon IV inheritance
     * @param info Pokemon information
     */
    void update(u32 advances, const std::array<u8, 6> &ivs, const std::array<u8, 6> &inheritance, const PersonalInfo *info)
    {
        pickupAdvances = advances;
        this->ivs = ivs;
        this->inheritance = inheritance;
        updateStats(info);
    }

    /**
     * @brief Updates egg with things that are calculated on pickup in RS/FRLG
     *
     * @param advances Advances of the state
     * @param pid Pokemon PID
     * @param shiny Pokemon shininess
     * @param ivs Pokemon IVs
     * @param inheritance Pokemon IV inheritance
     * @param info Pokemon information
     */
    void update(u32 advances, u32 pid, u8 shiny, const std::array<u8, 6> &ivs, const std::array<u8, 6> &inheritance,
                const PersonalInfo *info)
    {
        pickupAdvances = advances;
        this->pid = pid;
        nature = pid % 25;
        this->shiny = shiny;
        this->ivs = ivs;
        this->inheritance = inheritance;
        updateStats(info);
    }

private:
    u32 pickupAdvances;
    u8 redraws;
};

#endif // EGGSTATE3_HPP
