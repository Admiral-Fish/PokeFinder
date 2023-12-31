/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2023 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <Core/Enum/Language.hpp>

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
