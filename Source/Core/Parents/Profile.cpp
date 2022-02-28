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

#include "Profile.hpp"
#include <Core/Enum/Game.hpp>

Profile::Profile() : name("-"), version(Game::Emerald), tid(12345), sid(54321)
{
}

Profile::Profile(const std::string &name, Game version, u16 tid, u16 sid) : name(name), version(version), tid(tid), sid(sid)
{
}

std::string Profile::getVersionString() const
{
    switch (version)
    {
    case Game::Ruby:
        return "Ruby";
    case Game::Sapphire:
        return "Sapphire";
    case Game::FireRed:
        return "Fire Red";
    case Game::LeafGreen:
        return "Leaf Green";
    case Game::Emerald:
        return "Emerald";
    case Game::Gales:
        return "Gales";
    case Game::Colosseum:
        return "Colosseum";
    case Game::Diamond:
        return "Diamond";
    case Game::Pearl:
        return "Pearl";
    case Game::Platinum:
        return "Platinum";
    case Game::HeartGold:
        return "Heart Gold";
    case Game::SoulSilver:
        return "Soul Silver";
    case Game::Black:
        return "Black";
    case Game::White:
        return "White";
    case Game::Black2:
        return "Black 2";
    case Game::White2:
        return "White 2";
    case Game::Sword:
        return "Sword";
    case Game::Shield:
        return "Shield";
    case Game::BD:
        return "Brilliant Diamond";
    case Game::SP:
        return "Shining Pearl";
    case Game::PLA:
        return "Legends Arceus";
    default:
        return "-";
    }
}

bool Profile::operator==(const Profile &other) const
{
    return name == other.name && version == other.version && tid == other.tid && sid == other.sid;
}

bool Profile::operator!=(const Profile &other) const
{
    return !(operator==(other));
}
