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

#ifndef STATICGENERATOR_HPP
#define STATICGENERATOR_HPP

#include <Core/Enum/Lead.hpp>
#include <Core/Parents/Generators/Generator.hpp>

/**
 * @brief Parent generator class for static encounters
 *
 * @tparam Filter Filter class that is used by the generator
 */
template <class Filter>
class StaticGenerator : public Generator<Filter>
{
public:
    /**
     * @brief Construct a new StaticGenerator object
     *
     * @param initialAdvances Initial number of advances
     * @param maxAdvances Maximum number of advances
     * @param offset Number of advances to offset
     * @param tid Trainer ID
     * @param sid Secret ID
     * @param version Game version
     * @param method Encounter method
     * @param filter State filter
     */
    StaticGenerator(u32 initialAdvances, u32 maxAdvances, u32 offset, u16 tid, u16 sid, Game version, Method method, const Filter &filter) :
        Generator<Filter>(initialAdvances, maxAdvances, offset, tid, sid, version, method, filter), lead(Lead::None)
    {
    }

    /**
     * @brief Sets the lead type used by the generator
     *
     * @param lead Lead type
     */
    void setLead(Lead lead)
    {
        this->lead = lead;
    }

    /**
     * @brief Sets the synchronize nature used by the generator
     *
     * @param synchNature Synchronize nature
     */
    void setSynchNature(u8 synchNature)
    {
        this->synchNature = synchNature;
    }

protected:
    Lead lead;
    u8 synchNature;
};

#endif // STATICGENERATOR_HPP
