/*
 * This file is part of PokéFinder
 * Copyright (C) 2017 by Admiral_Fish, bumba, and EzPzStreamz
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

#include "Profile3.hpp"

Profile3::Profile3(const QString &profileName, Game version, u16 tid, u16 sid, int language, bool deadBattery)
    : Profile(profileName, version, tid, sid, language)
{
    this->deadBattery = deadBattery;
}

Profile3::Profile3(QJsonObject data)
    : Profile(data["name"].toString(), static_cast<Game>(data["version"].toInt()), data["tid"].toInt(), data["sid"].toInt(), data["language"].toInt())
{
    deadBattery = data["battery"].toBool();
}

Profile3::Profile3()
{
    deadBattery = false;
    version = Game::Emerald;
}

bool Profile3::getDeadBattery() const
{
    return deadBattery;
}

QJsonObject Profile3::getJson()
{
    QJsonObject data;
    data["name"] = profileName;
    data["version"] = version;
    data["language"] = language;
    data["tid"] = tid;
    data["sid"] = sid;
    data["battery"] = deadBattery;
    return data;
}

QVector<Profile3> Profile3::loadProfileList()
{
    QVector<Profile3> profileList;

    QFile file(QApplication::applicationDirPath() + "/profiles.json");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QJsonDocument profiles(QJsonDocument::fromJson(file.readAll()));
        QJsonArray gen3 = profiles["gen3"].toArray();

        for (const auto &&i : gen3)
        {
            profileList.append(Profile3(i.toObject()));
        }
        file.close();
    }

    return profileList;
}

void Profile3::saveProfile()
{
    QFile file(QApplication::applicationDirPath() + "/profiles.json");
    if (file.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QJsonObject profiles(QJsonDocument::fromJson(file.readAll()).object());
        QJsonArray gen3 = profiles["gen3"].toArray();

        gen3.append(getJson());
        profiles["gen3"] = gen3;

        file.resize(0);
        file.write(QJsonDocument(profiles).toJson());
        file.close();
    }
}

void Profile3::deleteProfile()
{
    QFile file(QApplication::applicationDirPath() + "/profiles.json");
    if (file.open(QIODevice::ReadWrite | QFile::Text))
    {
        QJsonObject profiles(QJsonDocument::fromJson(file.readAll()).object());
        QJsonArray gen3 = profiles["gen3"].toArray();

        for (int i = 0; i < gen3.size(); i++)
        {
            Profile3 profile(gen3[i].toObject());

            if (profile.profileName == profileName && profile.version == version && profile.language == language &&
                    profile.tid == tid && profile.sid == sid && profile.deadBattery == deadBattery)
            {
                gen3.removeAt(i);
                profiles["gen3"] = gen3;

                file.resize(0);
                file.write(QJsonDocument(profiles).toJson());
                break;
            }
        }

        file.close();
    }
}

void Profile3::updateProfile(Profile3 original)
{
    QFile file(QApplication::applicationDirPath() + "/profiles.json");
    if (file.open(QIODevice::ReadWrite | QFile::Text))
    {
        QJsonObject profiles(QJsonDocument::fromJson(file.readAll()).object());
        QJsonArray gen3 = profiles["gen3"].toArray();

        for (auto &&i : gen3)
        {
            Profile3 profile(i.toObject());

            if (original.profileName == profile.profileName && original.version == profile.version && original.language == profile.language &&
                    original.tid == profile.tid && original.sid == profile.sid && original.deadBattery == profile.deadBattery)
            {
                i = getJson();
                profiles["gen3"] = gen3;

                file.resize(0);
                file.write(QJsonDocument(profiles).toJson());
                break;
            }
        }
        file.close();
    }
}
