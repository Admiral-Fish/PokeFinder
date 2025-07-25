/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2024 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "ProfileLoader.hpp"
#include <Core/Enum/DSType.hpp>
#include <Core/Enum/Game.hpp>
#include <Core/Enum/Language.hpp>
#include <Core/Gen3/Profile3.hpp>
#include <Core/Gen4/Profile4.hpp>
#include <Core/Gen5/Profile5.hpp>
#include <Core/Gen8/Profile8.hpp>
#include <filesystem>
#include <fstream>
#include <json.hpp>

using json = nlohmann::json;

static std::filesystem::path path;

/**
 * @brief Reads provided profiles file
 *
 * @return json object with profile information
 */
static json readJson()
{
    json j;

    std::ifstream read(path);
    if (read.is_open())
    {
        j = json::parse(read, nullptr, false);
    }

    return j.is_discarded() ? json() : j;
}

/**
 * @brief Writes json object with profile information to profiles file
 *
 * @param j json object with profile information
 */
static void writeJson(const json &j)
{
    std::ofstream write(path);
    write << j.dump();
    write.close();
}

namespace ProfileLoader
{
    bool init(const std::wstring &location)
    {
        path = std::filesystem::path(location);

        bool exists = std::filesystem::exists(path);
        if (!exists)
        {
            std::ofstream json(path);
            json << "{}";
            json.close();
        }

        return exists;
    }
}

namespace ProfileLoader3
{
    namespace
    {
        /**
         * @brief Converts profile to JSON
         *
         * @param profile Profile to convert
         *
         * @return Profile JSON representation
         */
        json getJson(const Profile3 &profile)
        {
            json j;
            j["name"] = profile.getName();
            j["version"] = profile.getVersion();
            j["tid"] = profile.getTID();
            j["sid"] = profile.getSID();
            j["battery"] = profile.getDeadBattery();
            return j;
        }

        /**
         * @brief Converts JSON data to profile
         *
         * @param j JSON to convert
         *
         * @return Converted profile
         */
        Profile3 getProfile(const json &j)
        {
            std::string name = j.value("name", "-");
            Game version = j.value("version", Game::Emerald);
            u16 tid = j.value("tid", 0);
            u16 sid = j.value("sid", 0);
            u16 deadBattery = j.value("battery", false);
            return Profile3(name, version, tid, sid, deadBattery);
        }
    }

    void addProfile(const Profile3 &profile)
    {
        json j = readJson();
        auto &gen3 = j["gen3"];
        gen3.emplace_back(getJson(profile));
        writeJson(j);
    }

    std::vector<Profile3> getProfiles()
    {
        std::vector<Profile3> profiles;
        json j = readJson();
        const auto &gen3 = j["gen3"];
        std::transform(gen3.begin(), gen3.end(), std::back_inserter(profiles), [](const json &j) { return getProfile(j); });
        return profiles;
    }

    void removeProfile(const Profile3 &remove)
    {
        json j = readJson();
        auto &gen3 = j["gen3"];
        for (size_t i = 0; i < gen3.size(); i++)
        {
            Profile3 profile = getProfile(gen3[i]);
            if (profile == remove)
            {
                gen3.erase(i);
                writeJson(j);
                break;
            }
        }
    }

    void updateProfile(const Profile3 &update, const Profile3 &original)
    {
        if (update != original)
        {
            json j = readJson();
            auto &gen3 = j["gen3"];
            for (auto &i : gen3)
            {
                Profile3 profile = getProfile(i);
                if (profile == original)
                {
                    i = getJson(update);
                    writeJson(j);
                    break;
                }
            }
        }
    }
}

namespace ProfileLoader4
{
    namespace
    {
        /**
         * @brief Converts profile to JSON
         *
         * @param profile Profile to convert
         *
         * @return Profile JSON representation
         */
        json getJson(const Profile4 &profile)
        {
            json j;
            j["name"] = profile.getName();
            j["version"] = profile.getVersion();
            j["tid"] = profile.getTID();
            j["sid"] = profile.getSID();
            j["dex"] = profile.getNationalDex();
            j["unownDiscovered"] = profile.getUnownDiscovered();
            j["unownPuzzle"] = profile.getUnownPuzzle();
            return j;
        }

        /**
         * @brief Converts JSON data to profile
         *
         * @param j JSON to convert
         *
         * @return Converted profile
         */
        Profile4 getProfile(const json &j)
        {
            std::string name = j.value("name", "-");
            Game version = j.value("version", Game::Diamond);
            u16 tid = j.value("tid", 0);
            u16 sid = j.value("sid", 0);
            bool dex = j.value("dex", false);
            std::array<bool, 26> unownDiscovered
                = j.value("unownDiscovered", std::array<bool, 26> { false, false, false, false, false, false, false, false, false,
                                                                    false, false, false, false, false, false, false, false, false,
                                                                    false, false, false, false, false, false, false, false });
            std::array<bool, 4> unownPuzzle = j.value("unownPuzzle", std::array<bool, 4> { false, false, false, false });
            return Profile4(name, version, tid, sid, dex, unownDiscovered, unownPuzzle);
        }
    }

    std::vector<Profile4> getProfiles()
    {
        std::vector<Profile4> profiles;
        json j = readJson();
        const auto &gen4 = j["gen4"];
        std::transform(gen4.begin(), gen4.end(), std::back_inserter(profiles), [](const json &j) { return getProfile(j); });
        return profiles;
    }

    void addProfile(const Profile4 &profile)
    {
        json j = readJson();
        auto &gen4 = j["gen4"];
        gen4.emplace_back(getJson(profile));
        writeJson(j);
    }

    void removeProfile(const Profile4 &remove)
    {
        json j = readJson();
        auto &gen4 = j["gen4"];
        for (size_t i = 0; i < gen4.size(); i++)
        {
            Profile4 profile = getProfile(gen4[i]);
            if (profile == remove)
            {
                gen4.erase(i);
                writeJson(j);
                break;
            }
        }
    }

    void updateProfile(const Profile4 &update, const Profile4 &original)
    {
        if (update != original)
        {
            json j = readJson();
            auto &gen4 = j["gen4"];
            for (auto &i : gen4)
            {
                Profile4 profile = getProfile(i);
                if (profile == original)
                {
                    i = getJson(update);
                    writeJson(j);
                    break;
                }
            }
        }
    }
}

namespace ProfileLoader5
{
    namespace
    {
        /**
         * @brief Converts profile to JSON
         *
         * @param profile Profile to convert
         *
         * @return Profile JSON representation
         */
        json getJson(const Profile5 &profile)
        {
            json j;
            j["name"] = profile.getName();
            j["version"] = profile.getVersion();
            j["tid"] = profile.getTID();
            j["sid"] = profile.getSID();
            j["mac"] = profile.getMac();
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
            j["dsType"] = profile.getDSType();
            j["language"] = profile.getLanguage();
            return j;
        }

        /**
         * @brief Converts JSON data to profile
         *
         * @param j JSON to convert
         *
         * @return Converted profile
         */
        Profile5 getProfile(const json &j)
        {
            std::string name = j.value("name", "-");
            Game version = j.value("version", Game::Black);
            u16 tid = j.value("tid", 0);
            u16 sid = j.value("sid", 0);
            u64 mac;
            try
            {
                mac = j.value("mac", 0ULL);
            } catch (...)
            {
                mac = std::stoull(j.value("mac", "000000000000"), nullptr, 16);
            }
            std::array<bool, 9> keypresses;
            try
            {
                keypresses = j.value("keypresses", std::array<bool, 9> { true, false, false, false, false, false, false, false, false });
            } catch (...)
            {
                keypresses = { true, false, false, false, false, false, false, false, false };
            }
            u8 vcount = j.value("vcount", 0);
            u8 gxstat = j.value("gxstat", 0);
            u8 vframe = j.value("vframe", 0);
            bool skipLR = j.value("skipLR", false);
            u16 timer0Min = j.value("timer0Min", 0);
            u16 timer0Max = j.value("timer0Max", 0);
            bool softReset = j.value("softReset", false);
            bool memoryLink = j.value("memoryLink", false);
            bool shinyCharm = j.value("shinyCharm", false);
            DSType dsType = j.value("dsType", DSType::DS);
            Language language = j.value("language", Language::English);
            return Profile5(name, version, tid, sid, mac, keypresses, vcount, gxstat, vframe, skipLR, timer0Min, timer0Max, softReset,
                            memoryLink, shinyCharm, dsType, language);
        }

    }

    void addProfile(const Profile5 &profile)
    {
        json j = readJson();
        auto &gen5 = j["gen5"];
        gen5.emplace_back(getJson(profile));
        writeJson(j);
    }

    std::vector<Profile5> getProfiles()
    {
        std::vector<Profile5> profiles;
        json j = readJson();
        const auto &gen5 = j["gen5"];
        std::transform(gen5.begin(), gen5.end(), std::back_inserter(profiles), [](const json &j) { return getProfile(j); });
        return profiles;
    }

    void removeProfile(const Profile5 &remove)
    {
        json j = readJson();
        auto &gen5 = j["gen5"];
        for (size_t i = 0; i < gen5.size(); i++)
        {
            Profile5 profile = getProfile(gen5[i]);
            if (profile == remove)
            {
                gen5.erase(i);
                writeJson(j);
                break;
            }
        }
    }

    void updateProfile(const Profile5 &update, const Profile5 &original)
    {
        if (update != original)
        {
            json j = readJson();
            auto &gen5 = j["gen5"];
            for (auto &i : gen5)
            {
                Profile5 profile = getProfile(i);
                if (profile == original)
                {
                    i = getJson(update);
                    writeJson(j);
                    break;
                }
            }
        }
    }
}

namespace ProfileLoader8
{
    namespace
    {
        /**
         * @brief Converts profile to JSON
         *
         * @param profile Profile to convert
         *
         * @return Profile JSON representation
         */
        json getJson(const Profile8 &profile)
        {
            json j;
            j["name"] = profile.getName();
            j["version"] = profile.getVersion();
            j["tid"] = profile.getTID();
            j["sid"] = profile.getSID();
            j["dex"] = profile.getNationalDex();
            j["shinyCharm"] = profile.getShinyCharm();
            j["ovalCharm"] = profile.getOvalCharm();
            return j;
        }

        /**
         * @brief Converts JSON data to profile
         *
         * @param j JSON to convert
         *
         * @return Converted profile
         */
        Profile8 getProfile(const json &j)
        {
            std::string name = j.value("name", "-");
            Game version = j.value("version", Game::BD);
            u16 tid = j.value("tid", 0);
            u16 sid = j.value("sid", 0);
            bool dex = j.value("dex", false);
            bool shinyCharm = j.value("shinyCharm", false);
            bool ovalCharm = j.value("ovalCharm", false);
            return Profile8(name, version, tid, sid, dex, shinyCharm, ovalCharm);
        }
    }

    void addProfile(const Profile8 &profile)
    {
        json j = readJson();
        auto &gen8 = j["gen8"];
        gen8.emplace_back(getJson(profile));
        writeJson(j);
    }

    std::vector<Profile8> getProfiles()
    {
        std::vector<Profile8> profiles;
        json j = readJson();
        const auto &gen8 = j["gen8"];
        std::transform(gen8.begin(), gen8.end(), std::back_inserter(profiles), [](const json &j) { return getProfile(j); });
        return profiles;
    }

    void removeProfile(const Profile8 &remove)
    {
        json j = readJson();
        auto &gen8 = j["gen8"];
        for (size_t i = 0; i < gen8.size(); i++)
        {
            Profile8 profile = getProfile(gen8[i]);
            if (profile == remove)
            {
                gen8.erase(i);
                writeJson(j);
                break;
            }
        }
    }

    void updateProfile(const Profile8 &update, const Profile8 &original)
    {
        if (update != original)
        {
            json j = readJson();
            auto &gen8 = j["gen8"];
            for (auto &i : gen8)
            {
                Profile8 profile = getProfile(i);
                if (profile == original)
                {
                    i = getJson(update);
                    writeJson(j);
                    break;
                }
            }
        }
    }
}
