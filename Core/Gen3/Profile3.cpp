/*
 * This file is part of PokéFinder
 * Copyright (C) 2017-2019 by Admiral_Fish, bumba, and EzPzStreamz
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
#include <QApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

namespace PokeFinderCore
{
    Profile3::Profile3()
    {
        version = Game::Emerald;
        deadBattery = false;
    }

    Profile3::Profile3(const QString &profileName, Game version, u16 tid, u16 sid, Language language, bool deadBattery)
        : Profile(profileName, version, tid, sid, language)
    {
        this->deadBattery = deadBattery;
    }

    Profile3::Profile3(QJsonObject data)
        : Profile(data["name"].toString(), static_cast<Game>(data["version"].toInt()), data["tid"].toInt(),
            data["sid"].toInt(), static_cast<Language>(data["language"].toInt()))
    {
        deadBattery = data["battery"].toBool();
    }

    bool Profile3::getDeadBattery() const { return deadBattery; }

    QJsonObject Profile3::getJson() const
    {
        QJsonObject data;
        data["name"] = profileName;
        data["version"] = static_cast<int>(version);
        data["language"] = static_cast<int>(language);
        data["tid"] = tid;
        data["sid"] = sid;
        data["battery"] = deadBattery;
        return data;
    }

    QVector<Profile3> Profile3::loadProfileList()
    {
        QVector<Profile3> profileList;

        QSettings setting;
        QByteArray data = setting.value("profiles").toByteArray();

        QJsonObject profiles(QJsonDocument::fromJson(data).object());
        QJsonArray gen3 = profiles["gen3"].toArray();

        for (const auto &&i : gen3)
        {
            profileList.append(Profile3(i.toObject()));
        }

        return profileList;
    }

    void Profile3::saveProfile() const
    {
        QSettings setting;
        QByteArray data = setting.value("profiles").toByteArray();

        QJsonObject profiles(QJsonDocument::fromJson(data).object());
        QJsonArray gen3 = profiles["gen3"].toArray();

        gen3.append(getJson());
        profiles["gen3"] = gen3;

        setting.setValue("profiles", QJsonDocument(profiles).toJson());
    }

    void Profile3::deleteProfile() const
    {
        QSettings setting;
        QByteArray data = setting.value("profiles").toByteArray();

        QJsonObject profiles(QJsonDocument::fromJson(data).object());
        QJsonArray gen3 = profiles["gen3"].toArray();

        for (int i = 0; i < gen3.size(); i++)
        {
            Profile3 profile(gen3[i].toObject());

            if (profile == *this)
            {
                gen3.removeAt(i);
                profiles["gen3"] = gen3;

                setting.setValue("profiles", QJsonDocument(profiles).toJson());
                break;
            }
        }
    }

    void Profile3::updateProfile(const Profile3 &original) const
    {
        QSettings setting;
        QByteArray data = setting.value("profiles").toByteArray();

        QJsonObject profiles(QJsonDocument::fromJson(data).object());
        QJsonArray gen3 = profiles["gen3"].toArray();

        for (auto &&i : gen3)
        {
            Profile3 profile(i.toObject());

            if (original == profile && original != *this)
            {
                i = getJson();
                profiles["gen3"] = gen3;

                setting.setValue("profiles", QJsonDocument(profiles).toJson());
                break;
            }
        }
    }

    bool operator==(const Profile3 &left, const Profile3 &right)
    {
        return left.profileName == right.profileName && left.version == right.version && left.language == right.language
            && left.tid == right.tid && left.sid == right.sid && left.deadBattery == right.deadBattery;
    }

    bool operator!=(const Profile3 &left, const Profile3 &right) { return !(left == right); }
}
