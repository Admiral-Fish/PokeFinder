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

#include "ProfileLoader4.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

namespace
{
    std::string path;
}

namespace ProfileLoader4
{
    namespace
    {
        Profile4 getProfile(const nlohmann::json &j)
        {
            std::string name = j["name"].get<std::string>();
            Game version = j["version"].get<Game>();
            u16 tid = j["tid"].get<u16>();
            u16 sid = j["sid"].get<u16>();
            Game dual = j["dual"].get<Game>();
            int radio = j["radio"].get<int>();
            bool radar = j["radar"].get<bool>();
            bool swarm = j["swarm"].get<bool>();

            return Profile4(name, version, tid, sid, dual, radio, radar, swarm);
        }

        nlohmann::json getJson(const Profile4 &profile)
        {
            nlohmann::json j;
            j["name"] = profile.getName();
            j["version"] = profile.getVersion();
            j["tid"] = profile.getTID();
            j["sid"] = profile.getSID();
            j["dual"] = profile.getDualSlot();
            j["radio"] = profile.getRadio();
            j["radar"] = profile.getRadar();
            j["swarm"] = profile.getSwarm();
            return j;
        }
    }

    void init(const std::string &location)
    {
        path = location;
    }

    std::vector<Profile4> getProfiles()
    {
        std::vector<Profile4> profiles;

        std::ifstream read(path);
        if (read.is_open())
        {
            nlohmann::json j;
            read >> j;
            read.close();

            const auto &gen4 = j["gen4"];
            std::transform(gen4.begin(), gen4.end(), std::back_inserter(profiles), [](const nlohmann::json &j) { return getProfile(j); });
        }

        return profiles;
    }

    void addProfile(const Profile4 &profile)
    {
        std::ifstream read(path);
        if (read.is_open())
        {
            nlohmann::json j;
            read >> j;
            read.close();

            auto &gen4 = j["gen4"];
            gen4.emplace_back(getJson(profile));

            std::ofstream write(path);
            write << j.dump(4);
            write.close();
        }
    }

    void removeProfile(const Profile4 &remove)
    {
        std::ifstream read(path);
        if (read.is_open())
        {
            nlohmann::json j;
            read >> j;
            read.close();

            auto &gen4 = j["gen4"];
            for (size_t i = 0; i < gen4.size(); i++)
            {
                Profile4 profile = getProfile(gen4.at(i));

                if (profile == remove)
                {
                    gen4.erase(gen4.begin() + i);

                    std::ofstream write(path);
                    write << j.dump(4);
                    write.close();
                    break;
                }
            }
        }
    }

    void updateProfile(const Profile4 &update, const Profile4 &original)
    {
        std::ifstream read(path);
        if (read.is_open())
        {
            nlohmann::json j;
            read >> j;
            read.close();

            auto &gen4 = j["gen4"];
            for (size_t i = 0; i < gen4.size(); i++)
            {
                Profile4 profile = getProfile(gen4.at(i));

                if (original == profile && original != update)
                {
                    gen4[i] = getJson(update);

                    std::ofstream write(path);
                    write << j.dump(4);
                    write.close();
                    break;
                }
            }
        }
    }
}
