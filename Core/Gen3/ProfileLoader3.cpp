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

#include "ProfileLoader3.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

namespace
{
    std::string path;
}

namespace ProfileLoader3
{
    namespace
    {
        Profile3 getProfile(const nlohmann::json &j)
        {
            std::string name = j["name"].get<std::string>();
            Game version = j["version"].get<Game>();
            u16 tid = j["tid"].get<u16>();
            u16 sid = j["sid"].get<u16>();
            bool deadBattery = j["battery"].get<bool>();

            return Profile3(name, version, tid, sid, deadBattery);
        }

        nlohmann::json getJson(const Profile3 &profile)
        {
            nlohmann::json j;
            j["name"] = profile.getName();
            j["version"] = profile.getVersion();
            j["tid"] = profile.getTID();
            j["sid"] = profile.getSID();
            j["battery"] = profile.getDeadBattery();
            return j;
        }
    }

    void init(const std::string &location)
    {
        path = location;
    }

    std::vector<Profile3> getProfiles()
    {
        std::vector<Profile3> profiles;

        std::ifstream read(path);
        if (read.is_open())
        {
            nlohmann::json j;
            read >> j;
            read.close();

            const auto &gen3 = j["gen3"];
            std::transform(gen3.begin(), gen3.end(), std::back_inserter(profiles), [](const nlohmann::json &j) { return getProfile(j); });
        }

        return profiles;
    }

    void addProfile(const Profile3 &profile)
    {
        std::ifstream read(path);
        if (read.is_open())
        {
            nlohmann::json j;
            read >> j;
            read.close();

            auto &gen3 = j["gen3"];
            gen3.emplace_back(getJson(profile));

            std::ofstream write(path);
            write << j.dump(4);
            write.close();
        }
    }

    void removeProfile(const Profile3 &remove)
    {
        std::ifstream read(path);
        if (read.is_open())
        {
            nlohmann::json j;
            read >> j;
            read.close();

            auto &gen3 = j["gen3"];
            for (size_t i = 0; i < gen3.size(); i++)
            {
                Profile3 profile = getProfile(gen3.at(i));

                if (profile == remove)
                {
                    gen3.erase(gen3.begin() + i);

                    std::ofstream write(path);
                    write << j.dump(4);
                    write.close();
                    break;
                }
            }
        }
    }

    void updateProfile(const Profile3 &update, const Profile3 &original)
    {
        std::ifstream read(path);
        if (read.is_open())
        {
            nlohmann::json j;
            read >> j;
            read.close();

            auto &gen3 = j["gen3"];
            for (size_t i = 0; i < gen3.size(); i++)
            {
                Profile3 profile = getProfile(gen3.at(i));

                if (original == profile && original != update)
                {
                    gen3[i] = getJson(update);

                    std::ofstream write(path);
                    write << j.dump(4);
                    write.close();
                    break;
                }
            }
        }
    }
}
