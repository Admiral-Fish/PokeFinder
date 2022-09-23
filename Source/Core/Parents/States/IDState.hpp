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

#ifndef IDSTATE_HPP
#define IDSTATE_HPP

#include <Core/Global.hpp>

#ifdef TEST
#include <nlohmann/json.hpp>
#endif

/**
 * @brief Parent state that contains all the common information for TID/SID across each game
 */
class IDState
{
public:
#ifdef TEST
    IDState() = default;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(IDState, advances, sid, tid, tsv);
#endif

    /**
     * @brief Construct a new IDState object
     *
     * @param tid Trainer ID
     * @param sid Secret ID
     * @param tsv Trainer shiny value
     */
    IDState(u16 tid, u16 sid, u16 tsv) : sid(sid), tid(tid), tsv(tsv)
    {
    }

    /**
     * @brief Construct a new IDState object
     *
     * @param advances
     * @param tid Trainer ID
     * @param sid Secret ID
     * @param tsv Trainer shiny value
     */
    IDState(u32 advances, u16 tid, u16 sid, u16 tsv) : advances(advances), sid(sid), tid(tid), tsv(tsv)
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
     * @brief Returns the SID of the state
     *
     * @return State SID
     */
    u16 getSID() const
    {
        return sid;
    }

    /**
     * @brief Returns the TID of the state
     *
     * @return State TID
     */
    u16 getTID() const
    {
        return tid;
    }

    /**
     * @brief Returns the TSV of the state
     *
     * @return State TSV
     */
    u16 getTSV() const
    {
        return tsv;
    }

protected:
    u32 advances;
    u16 sid;
    u16 tid;
    u16 tsv;
};

#endif // IDSTATE_HPP
