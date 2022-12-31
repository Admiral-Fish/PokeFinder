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

#ifndef CHAINEDSIDCALC_HPP
#define CHAINEDSIDCALC_HPP

#include <Core/Global.hpp>
#include <vector>

class PersonalInfo;

/**
 * @brief Computes SID from several rounds of a chained shiny Pokemon
 */
class ChainedSIDCalc
{
public:
    /**
     * @brief Construct a new ChainedSIDCalc object
     *
     * @param tid Trainer ID
     */
    ChainedSIDCalc(u16 tid);

    /**
     * @brief Filters down potentional SIDs from another chained shiny encounter of the same Pokemon
     *
     * @param hp HP IV
     * @param atk Atk IV
     * @param def Def IV
     * @param spa SpA IV
     * @param spd SpD IV
     * @param spe Spe IV
     * @param ability Pokemon ability to compare against
     * @param gender Pokemon gender to compare against
     * @param nature Pokemon nature to compare against
     * @param info Pokemon information
     */
    void addEntry(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe, u16 ability, u8 gender, u8 nature, const PersonalInfo *info);

    /**
     * @brief Returns computed SIDs
     *
     * @return Vector of SIDs
     */
    std::vector<u16> getSIDs() const;

private:
    std::vector<u16> sids;
    u16 tid;
};

#endif // CHAINEDSIDCALC_HPP
