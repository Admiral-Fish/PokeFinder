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

#include <Core/Enum/Lead.hpp>
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
               u8 shiny, u8 encounterSlot, u16 item, u16 specie, u8 form, const PersonalInfo *info, Lead lead = Lead::None) :
        WildGeneratorState(advances, pid, ivs, ability, gender, level, nature, shiny, encounterSlot, item, specie, form, info),
        ivAdvances(ivAdvances),
        chatot(prng / 82),
        lead(lead),
        leadFlags(getLeadFlag(lead)),
        variableNature(false)
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

    /**
     * @brief Returns the lead that produced the state
     *
     * @return Encounter lead
     */
    Lead getLead() const
    {
        return lead;
    }

    /**
     * @brief Returns grouped lead flags that produced the state
     *
     * @return Encounter lead flags
     */
    u8 getLeadFlags() const
    {
        return leadFlags;
    }

    /**
     * @brief Adds a lead that produced the same state
     *
     * @param newLead Encounter lead
     */
    void addLead(Lead newLead)
    {
        if (newLead == Lead::None)
        {
            lead = Lead::None;
            leadFlags = 0;
        }
        else if (lead != Lead::None)
        {
            leadFlags |= getLeadFlag(newLead);
        }
    }

    /**
     * @brief Sets whether nature depends on selected synchronize lead
     *
     * @param flag Whether the nature should be displayed as variable
     */
    void setVariableNature(bool flag)
    {
        variableNature = flag;
    }

    /**
     * @brief Determines if nature depends on selected synchronize lead
     *
     * @return true Nature should be displayed as variable
     * @return false Nature is fixed
     */
    bool getVariableNature() const
    {
        return variableNature;
    }

private:
    static constexpr u8 getLeadFlag(Lead lead)
    {
        if (lead <= Lead::SynchronizeEnd)
        {
            return 1 << 0;
        }

        switch (lead)
        {
        case Lead::CuteCharmM:
            return 1 << 1;
        case Lead::CuteCharmF:
            return 1 << 2;
        case Lead::MagnetPull:
            return 1 << 3;
        case Lead::Static:
            return 1 << 4;
        case Lead::Pressure:
            return 1 << 5;
        case Lead::CompoundEyes:
            return 1 << 6;
        default:
            return 0;
        }
    }

    u32 ivAdvances;
    u8 chatot;
    Lead lead;
    u8 leadFlags;
    bool variableNature;
};

#endif // WILDSTATE5_HPP
