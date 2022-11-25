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

#include "Profile5.hpp"
#include <Core/Enum/DSType.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Language.hpp>

Profile5::Profile5(const std::string &name, Game version, u16 tid, u16 sid, u64 mac, const std::array<bool, 4> &keypresses, u8 vcount,
                   u8 gxstat, u8 vframe, bool skipLR, u16 timer0Min, u16 timer0Max, bool softReset, bool memoryLink, bool shinyCharm,
                   DSType dsType, Language language) :
    Profile(name, version, tid, sid),
    memoryLink(memoryLink),
    shinyCharm(shinyCharm),
    skipLR(skipLR),
    softReset(softReset),
    dsType(dsType),
    language(language),
    gxstat(gxstat),
    vcount(vcount),
    vframe(vframe),
    timer0Max(timer0Max),
    timer0Min(timer0Min),
    mac(mac),
    keypresses(keypresses)
{
}

DSType Profile5::getDSType() const
{
    return dsType;
}

std::string Profile5::getDSTypeString() const
{
    switch (dsType)
    {
    case DSType::DS:
        return "DS Lite";
    case DSType::DSi:
        return "DSi";
    case DSType::DS3:
        return "3DS";
    }
    return "-";
}

u8 Profile5::getGxStat() const
{
    return gxstat;
}

std::array<bool, 4> Profile5::getKeypresses() const
{
    return keypresses;
}

std::string Profile5::getKeypressesString() const
{
    std::string keys;
    for (size_t i = 0; i < keypresses.size(); i++)
    {
        if (i == 0 && keypresses[i])
        {
            keys += "None";
        }
        else if (keypresses[i])
        {
            if (!keys.empty() && i != keypresses.size())
            {
                keys += ", ";
            }
            keys += std::to_string(i);
        }
    }
    return keys;
}

Language Profile5::getLanguage() const
{
    return language;
}

std::string Profile5::getLanguageString() const
{
    switch (language)
    {
    case Language::English:
        return "ENG";
    case Language::French:
        return "FRE";
    case Language::German:
        return "DEU";
    case Language::Italian:
        return "ITA";
    case Language::Japanese:
        return "JPN";
    case Language::Korean:
        return "KOR";
    case Language::Spanish:
        return "SPA";
    default:
        return "-";
    }
}

u64 Profile5::getMac() const
{
    return mac;
}

bool Profile5::getMemoryLink() const
{
    return memoryLink;
}

bool Profile5::getShinyCharm() const
{
    return shinyCharm;
}

bool Profile5::getSkipLR() const
{
    return skipLR;
}

bool Profile5::getSoftReset() const
{
    return softReset;
}

u16 Profile5::getTimer0Max() const
{
    return timer0Max;
}

u16 Profile5::getTimer0Min() const
{
    return timer0Min;
}

u8 Profile5::getVCount() const
{
    return vcount;
}

u8 Profile5::getVFrame() const
{
    return vframe;
}

bool Profile5::operator==(const Profile5 &other) const
{
    return Profile::operator==(other) && mac == other.mac && keypresses == other.keypresses && vcount == other.vcount
        && gxstat == other.gxstat && vframe == other.vframe && skipLR == other.skipLR && timer0Min == other.timer0Min
        && timer0Max == other.timer0Max && softReset == other.softReset && memoryLink == other.memoryLink && shinyCharm == other.shinyCharm
        && dsType == other.dsType && language == other.language;
}

bool Profile5::operator!=(const Profile5 &other) const
{
    return !(operator==(other));
}
