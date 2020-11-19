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

#include "ProfileLoader5.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>

namespace
{
    std::string path;
}

namespace ProfileLoader5
{
    namespace
    {
        Profile5 getProfile(const nlohmann::json &j)
        {
            std::string name = j["name"].get<std::string>();
            Game version = j["version"].get<Game>();
            u16 tid = j["tid"].get<u16>();
            u16 sid = j["sid"].get<u16>();
            u64 mac = std::stoull(j["mac"].get<std::string>(), nullptr, 16);
            std::vector<bool> keypresses = j["keypresses"].get<std::vector<bool>>();
            u8 vcount = j["vcount"].get<u8>();
            u8 gxstat = j["gxstat"].get<u8>();
            u8 vframe = j["vframe"].get<u8>();
            bool skipLR = j["skipLR"].get<bool>();
            u16 timer0Min = j["timer0Min"].get<u16>();
            u16 timer0Max = j["timer0Max"].get<u16>();
            bool softReset = j["softReset"].get<bool>();
            bool memoryLink = j["memoryLink"].get<bool>();
            bool shinyCharm = j["shinyCharm"].get<bool>();
            DSType dsType = j["dsType"].get<DSType>();
            Language language = j["language"].get<Language>();
            return Profile5(name, version, tid, sid, mac, keypresses, vcount, gxstat, vframe, skipLR, timer0Min, timer0Max, softReset,
                            memoryLink, shinyCharm, dsType, language);
        }

        nlohmann::json getJson(const Profile5 &profile)
        {
            nlohmann::json j;
            j["name"] = profile.getName();
            j["version"] = profile.getVersion();
            j["tid"] = profile.getTID();
            j["sid"] = profile.getSID();
            std::stringstream stream;
            stream << std::hex << profile.getMac();
            j["mac"] = stream.str();
            j["keypresses"] = profile.getKeypresses();
            j["vcount"] = profile.getVCount();
            j["gxstat"] = profile.getGxStat();
            j["vframe"] = profile.getVFrame();
            j["skipLR"] = profile.getSkipLR();
            j["timer0Min"] = profile.getTimer0Min();
            j["timer0Max"] = profile.getTimer0Max();
            j["softReset"] = profile.getSoftReset();
            j["memoryLink"] = profile.getMemoryLink();
            j["shinyCharm"] = profile.getShinyCharm();
            j["dsType"] = static_cast<int>(profile.getDSType());
            j["language"] = static_cast<int>(profile.getLanguage());
            return j;
        }
    }

    void init(const std::string &location)
    {
        path = location;
    }

    std::vector<Profile5> getProfiles()
    {
        std::vector<Profile5> profiles;

        std::ifstream read(path);
        if (read.is_open())
        {
            nlohmann::json j;
            read >> j;
            read.close();

            const auto &gen5 = j["gen5"];
            std::transform(gen5.begin(), gen5.end(), std::back_inserter(profiles), [](const nlohmann::json &j) { return getProfile(j); });
        }

        return profiles;
    }

    void addProfile(const Profile5 &profile)
    {
        std::ifstream read(path);
        if (read.is_open())
        {
            nlohmann::json j;
            read >> j;

            auto &gen5 = j["gen5"];
            gen5.emplace_back(getJson(profile));

            std::ofstream write(path);
            write << j;
            write.close();
        }
    }

    void removeProfile(const Profile5 &remove)
    {
        std::ifstream read(path);
        if (read.is_open())
        {
            nlohmann::json j;
            read >> j;
            read.close();

            auto &gen5 = j["gen5"];
            for (size_t i = 0; i < gen5.size(); i++)
            {
                Profile5 profile = getProfile(gen5.at(i));

                if (profile == remove)
                {
                    gen5.erase(gen5.begin() + i);

                    std::ofstream write(path);
                    write << j;
                    write.close();
                    break;
                }
            }
        }
    }

    void updateProfile(const Profile5 &update, const Profile5 &original)
    {
        std::ifstream read(path);
        if (read.is_open())
        {
            nlohmann::json j;
            read >> j;
            read.close();

            auto &gen5 = j["gen5"];
            for (size_t i = 0; i < gen5.size(); i++)
            {
                Profile5 profile = getProfile(gen5.at(i));

                if (original == profile && original != update)
                {
                    gen5[i] = getJson(update);

                    std::ofstream write(path);
                    write << j;
                    write.close();
                    break;
                }
            }
        }
    }
}
