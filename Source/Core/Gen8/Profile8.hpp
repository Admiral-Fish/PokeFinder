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

#ifndef PROFILE8_HPP
#define PROFILE8_HPP

#include <Core/Enum/Game.hpp>
#include <Core/Parents/Profile.hpp>

class Profile8 : public Profile
{
public:
    Profile8(Game version = Game::Sword) :
        Profile("-", version, 12345, 54321), shinyCharm(false), ovalCharm(false), radar(false), swarm(false), storyFlag(1)
    {
    }

    Profile8(const std::string &name, Game version, u16 tid, u16 sid, bool shinyCharm, bool ovalCharm, bool radar, bool swarm,
             u8 storyFlag) :
        Profile(name, version, tid, sid), shinyCharm(shinyCharm), ovalCharm(ovalCharm), radar(radar), swarm(swarm), storyFlag(storyFlag)
    {
    }

    bool getShinyCharm() const
    {
        return shinyCharm;
    }

    bool getOvalCharm() const
    {
        return ovalCharm;
    }

    bool getRadar() const
    {
        return radar;
    }

    bool getSwarm() const
    {
        return swarm;
    }

    u8 getStoryFlag() const
    {
        return storyFlag;
    }

    bool operator==(const Profile8 &other) const
    {
        return Profile::operator==(other) && shinyCharm == other.shinyCharm && ovalCharm == other.ovalCharm && radar == other.radar
            && swarm == other.swarm && storyFlag == other.storyFlag;
    }

    bool operator!=(const Profile8 &other) const
    {
        return !(operator==(other));
    }

    std::string getStoryFlagString() const
    {
        switch (storyFlag)
        {
        case 1:
            return "Underground Unlocked";
        case 2:
            return "Strength Obtained";
        case 3:
            return "Defog Obtained";
        case 4:
            return "7 Badges";
        case 5:
            return "Waterfall Obtained";
        case 6:
            return "National Dex";
        default:
            return "-";
        }
    }

private:
    bool shinyCharm;
    bool ovalCharm;
    bool radar;
    bool swarm;
    u8 storyFlag;
};

#endif // PROFILE8_HPP
