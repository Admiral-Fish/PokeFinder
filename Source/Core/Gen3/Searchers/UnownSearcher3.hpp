/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2021 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef UNOWNSEARCHER3_HPP
#define UNOWNSEARCHER3_HPP

#include <Core/Gen3/EncounterArea3.hpp>
#include <Core/Parents/Searchers/UnownSearcher.hpp>
#include <Core/Parents/States/UnownState.hpp>
#include <Core/RNG/RNGCache.hpp>
#include <mutex>

class UnownSearcher3 : public UnownSearcher
{
public:
    std::array<std::string,28> letters = { "A", "B", "C", "D", "E",
                                           "F", "G", "H", "I", "J",
                                           "K", "L", "M", "N", "O",
                                           "P", "Q", "R", "S", "T",
                                           "U", "V", "W", "X", "Y",
                                                "Z", "!", "?" };
    UnownSearcher3() = default;
    UnownSearcher3(u16 tid, u16 sid, u8 genderRatio, Method method, const StateFilter &filter);
    void setEncounterArea(const EncounterArea3 &encounterArea);
    void startSearch(const std::array<u8, 6> &min, const std::array<u8, 6> &max);
    void cancelSearch();
    std::string getLetter(u32 pid) const;
    std::string getTargetLetter(u8 location, u8 slot) const;
    u32 getLocation(std::string letter) const;
    std::array<std::string,7> getLetterSlots(u32 slot) const;
    std::vector<UnownState> getResults();
    int getProgress() const;

private:
    RNGCache cache;
    EncounterArea3 encounterArea;

    bool searching;
    int progress;
    std::vector<UnownState> results;
    std::mutex mutex;

    std::vector<UnownState> search(u8 hp, u8 atk, u8 def, u8 spa, u8 spd, u8 spe) const;
};

#endif // UNOWNSEARCHER3_HPP
