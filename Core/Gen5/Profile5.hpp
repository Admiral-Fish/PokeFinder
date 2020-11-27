/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2020 by Admiral_Fish, bumba, and EzPzStreamz
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

#ifndef PROFILE5_HPP
#define PROFILE5_HPP

#include <Core/Enum/DSType.hpp>
#include <Core/Enum/Language.hpp>
#include <Core/Parents/Profile.hpp>
#include <vector>

class Profile5 : public Profile
{

public:
    Profile5();
    Profile5(const std::string &name, Game version, u16 tid, u16 sid, u64 mac, const std::vector<bool> &keypresses, u8 vcount, u8 gxstat,
             u8 vframe, bool skipLR, u16 timer0Min, u16 timer0Max, bool softReset, bool memoryLink, bool shinyCharm, DSType dsType,
             Language language = Language::English);
    u64 getMac() const;
    std::vector<bool> getKeypresses() const;
    std::string getKeypressesString() const;
    u8 getVCount() const;
    u8 getGxStat() const;
    u8 getVFrame() const;
    bool getSkipLR() const;
    u16 getTimer0Min() const;
    u16 getTimer0Max() const;
    bool getSoftReset() const;
    bool getMemoryLink() const;
    bool getShinyCharm() const;
    DSType getDSType() const;
    std::string getDSTypeString() const;
    Language getLanguage() const;
    std::string getLanguageString() const;

private:
    u64 mac;
    std::vector<bool> keypresses; // 4 index; 0: None, 1: One, 2: Two, 3: Three
    u8 vcount;
    u8 gxstat;
    u8 vframe;
    bool skipLR;
    u16 timer0Min;
    u16 timer0Max;
    bool softReset;
    bool memoryLink;
    bool shinyCharm;
    DSType dsType;
    Language language;
};

bool operator==(const Profile5 &left, const Profile5 &right);
bool operator!=(const Profile5 &left, const Profile5 &right);

#endif // PROFILE5_HPP
